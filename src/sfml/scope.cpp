/*

THIS FILE IS SUBJECT TO STRICT RULES OF BDE NE666 COPYDOWN. 
NOBODY IS PERMITTED TO COPY AND DISTRIBUTE VERBATIM OR MODIFIED COPIES OF
THIS LICENSE HEADER. A SECURITY LEVEL IS ASSIGNED TO THE FILE AND A VISIBILITY.
THIS FILE MUST NOT BE COPIED OR REDISTRIBUTED IF IT'S VISIBILITY IS NOT PUBLIC. 
VISIBILITY HAS 3 POSSIBLE STATES:
- PRIVATE: LIMITED TO PROJECTS WITH THE SAME SECURITY LEVEL WITHIN THE BDE NE666. (DEFAULT VISIBILITY)
- PROTECTED: LIMITED TO PROJECTS USING THE SAME SECURITY RULES WITHIN THE BDE NE666. 
- PUBLIC: USABLE FOR FREE IN PUBLIC PROJECTS UNDER THE FOLLOWING LICENSES: BSD, MIT OR DWTFYWT 
WITHOUT OTHER CONDITIONS THAN THE CONSERVATION OF THIS HEADER INCLUDING: 
RULES, ORIGINAL AUTHORS (WITH EMAIL), ORIGINAL FILENAME, AND VERSION, INSIDE THE FILE
WHICH CONTAINS PART (OR ENTIRE) CODE FROM THIS FILE. USABLE IN OTHER PROJECTS WITH CONDITIONS 
AND RENUMERATIONS, FIXED BY ORIGINAL AUTHORS (CONTACT THEM).

*/

/**
  * THIS FILE IS PART OF LIBTOOLS 
  * SECURITY LEVEL : 8 (CRITICAL)  
  * VISIBILITY     : PRIVATE
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : ScopeA.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 
  * DEPENDENCIES : ScopeA.hpp
  *                settings.hpp
  * ROLE         : Définie une interface de visualisation des signaux 
  *                configurable en oscilloScopeA ou analyseur de spectre
  */
  
#include <libtools/sfml/scope.hpp>
#include <libtools/core/settings.hpp>
#include <libtools/sfml/glslrender.hpp>

#include <iostream>
#include <sstream>

#ifdef _MSC_VER
#pragma warning( disable : 4244 )
#endif


ScopeA::ScopeA(const sf::Vector2f& size,const sf::Font& font, bool spectrum, Signal* s) :
  Interface(sf::Vector2u(Signal::size,100),size),
  _signal(s),
  _fft(0),
  _pixels(0),
  _color(255,255,255,255),
  _texture(),
  _back(sf::Vector2f(Signal::size,100)),
  _sprite(),
  _y_zoom(1),
  _update_time(0),
  _time(0),
  _spectrum(spectrum),
  _autoscaley(0),
  _modeButton(sf::Vector2f(80.f,18.f), "FFT", font ),
  _autoYScaleButton(sf::Vector2f(80.f,18.f), "Auto Scale",font )
{
  _back.setFillColor(sf::Color(42,42,42,128));
  _modeButton.setCallback(this,&ScopeA::setSpectrum, &ScopeA::isSpectrum);
  _autoYScaleButton.linkTo(&_autoscaley);
  addMouseCatcher(&_modeButton);
  addMouseCatcher(&_autoYScaleButton);
  addDrawable(&_sprite);
  addDrawable(&_back);
  _allocate();
}

ScopeA::~ScopeA()
{
  if (_pixels) free(_pixels);
  if (_fft) delete(_fft);
}

void ScopeA::setYZoom(float z)
{
  if (z) _y_zoom=z;
}
void ScopeA::setSignal(Signal* s)
{
  _signal=s;
  _allocate();
}

void ScopeA::_allocate()
{
  if (_pixels) free(_pixels);
  if (_fft) delete _fft;
  _fft=0;
  _pixels=0;
  if (_signal)
  {
    if (_spectrum) { 
      const unsigned samples = GetSettingsFor("FFT/Samples",4096);
      const unsigned size=samples < 4096 ? samples: 4096;
      _fft=new FFT(samples); 
      _internalZoneChanged(sf::Vector2u(size>>1,_zone.y));
    }
    else {
      _internalZoneChanged(sf::Vector2u(Signal::size,_zone.y));
    } 
    _texture.create(_zone.y, _zone.x);
    _back.setSize(sf::Vector2f(_zone.x,_zone.y));
    unsigned p = _texture.getSize().x* _texture.getSize().y*4;
    _pixels = (sf::Uint8*) malloc(p);
    setColor(_color);

    _sprite.setTexture(_texture,true);
    _sprite.setOrigin(_zone.y/2,_zone.x/2);
    if (_spectrum)  _sprite.setPosition(_zone.x/2,_zone.y/2-12.f);
    else _sprite.setPosition(_zone.x/2,_zone.y/2);
    _sprite.setRotation(90);
  }
  
}

void ScopeA::update()
{
  Interface::update();
  sf::Vector2i position=getPosition();
  _modeButton.setPosition(position.x+10,position.y+10);
  _autoYScaleButton.setPosition(position.x+10,position.y+30);
  
  if (_pixels && _time++ > _update_time && _signal)
  {
    _time=0;
    const unsigned int s = (_texture.getSize().x*_texture.getSize().y) << 2;
    for (unsigned int i=3; i < s;i+=4)
    {
      _pixels[i]=0;
    }

    if (!_spectrum) {
      const unsigned int l = Signal::size < _texture.getSize().y ? Signal::size : _texture.getSize().y;
      float smax = 0.0;
      for (unsigned int x=0; x < l;x++) {
        const float a=fabs(_signal->samples[x]);
        if (a > smax) smax=a;
      }
      const float _y_factor = _autoscaley && smax > 0.001 ?  
                  (_texture.getSize().x-20) *_y_zoom * 0.5 * 1/smax :
                  _texture.getSize().x * 0.5 * _y_zoom;
      for (unsigned int x=0; x < l;x++)
      {
        const int y = _signal->samples[x]*_y_factor;

        if (y-1 >= -(int)(_texture.getSize().x>>1) && y+1 < (int)(_texture.getSize().x>>1))
        {
		  const unsigned int offset = ((x*_texture.getSize().x + y + (_texture.getSize().x>>1))<<2) + 3;
          _pixels[offset] = 255;
          _pixels[offset+4] = 120;
          _pixels[offset-4] = 120;
		  /*
          _pixels[(x*_texture.getSize().x + y + _texture.getSize().x/2)*4 + 3] = 255;
          _pixels[(x*_texture.getSize().x + y + 1 + _texture.getSize().x/2)*4 + 3] = 120;
          _pixels[(x*_texture.getSize().x + y - 1 + _texture.getSize().x/2)*4 + 3] = 120;*/

        }
      }
    }
    else if (_fft) {
      _fft->pushSignal(*_signal);
      _fft->computeModule();
      const unsigned int l = _fft->size() < _texture.getSize().y ? _fft->size() : _texture.getSize().y;
      const unsigned int sy =_texture.getSize().x;
      float smax = 0.0;
      for (unsigned int x=0; x < l;x++) {
        const float a=2.0*_fft->getModule()[x];
        //if (a < 0) std::cerr << "Erreur dans la fft !! " << a << std::endl;
        if (sqrt(a) > smax) smax=sqrt(a);
      }
      //std::cout << " smax " << smax << std::endl;
      const float _y_factor = _autoscaley && smax > 0.001 ?  
                  (_texture.getSize().x-20) *_y_zoom * 1/smax :
                  _texture.getSize().x * _y_zoom;
      for (unsigned int x=0; x < l;x++)
      {
        const unsigned int fakey = sqrt(2.0*_fft->getModule()[x])*_y_factor ;
        const unsigned int y = fakey > sy ? sy : fakey;
        const unsigned delta_x = x*sy*4 + 3;
        
        for (unsigned int i=0; i<y; i++) {
          _pixels[delta_x + ((sy-i-1)<<2)] = 255;
        }
      }
    }
    else {
      return; //no need update
    }
    _texture.update(_pixels);
  }
}

void ScopeA::setColor(const sf::Color& color)
{
  _color = color;
  if (_pixels)
  {
    unsigned p = _texture.getSize().x* _texture.getSize().y*4;
    for (unsigned int i=0; i < p;)
    {
      _pixels[i++]=_color.r;
      _pixels[i++]=_color.g;
      _pixels[i++]=_color.b;
      _pixels[i++]=0;
    }
  }
}

void ScopeA::setSpectrum(bool spectrum) {
  _spectrum=spectrum;
  _allocate();
}

void ScopeA::setFadeColor(const sf::Color& colorInit, const sf::Color& colorEnd, bool axe)
{
  if (axe) {
    _color = colorInit;
    if (_pixels)
    {
      unsigned int sizeX=_texture.getSize().y;
      unsigned int sizeY=_texture.getSize().x;
      for (unsigned int i=0; i<sizeX ; i++) {
        float k=(float)i/(float)sizeX;
        for (unsigned int j=0; j<sizeY ; j++) {
          //unsigned p = _texture.getSize().x* _texture.getSize().y*4;
          unsigned int a=(i*sizeY+j)*4;
          _pixels[a]=_color.r*k+colorEnd.r*(1-k);
          _pixels[a+1]=_color.g*k+colorEnd.g*(1-k);
          _pixels[a+2]=_color.b*k+colorEnd.b*(1-k);
          _pixels[a+3]=0;
        }
      }
    }
  }
}



VisualSignalInterface::VisualSignalInterface(const sf::Vector2f& size,const sf::Font& font):
Interface(sf::Vector2u(600,100),size),
_visual(0),
_text("",font,11)
{
  _noise.setRotation(180);
  _clean.setRotation(180);
  _distord.setRotation(180);
  _linear.setRotation(180);
  _rouge.setRotation(180);
  _vert.setRotation(180);
  _bleu.setRotation(180);
  _jaune.setRotation(180);


  _noise.setPosition(120,90);
  _clean.setPosition(150,90);
  _distord.setPosition(180,90);
  _linear.setPosition(210,90);
  _text.setPosition(0,10);

  _rouge.setPosition(300,90);
  _vert.setPosition(330,90);
  _bleu.setPosition(360,90);
  _jaune.setPosition(390,90);

  _rouge.setFillColor(sf::Color(221,23,23,255));
  _vert.setFillColor(sf::Color(50,126,54,255));
  _bleu.setFillColor(sf::Color(4,133,170,255));
  _jaune.setFillColor(sf::Color(233,136,39,255));

  addDrawable(&_text);
  addDrawable(&_noise);
  addDrawable(&_clean);
  addDrawable(&_distord);
  addDrawable(&_linear);
 /* addDrawable(&_rouge);
  addDrawable(&_vert);
  addDrawable(&_bleu);
  addDrawable(&_jaune);*/
}

VisualSignalInterface::~VisualSignalInterface()
{
}

void VisualSignalInterface::setVisualSignal(VisualSignal* s) {
  _visual=s;
}

void VisualSignalInterface::setColor(int rouge,int vert,int bleu, int jaune) {
  _rouge.setSize(sf::Vector2f(10,rouge));
  _vert.setSize(sf::Vector2f(10,vert));
  _bleu.setSize(sf::Vector2f(10,bleu));
  _jaune.setSize(sf::Vector2f(10,jaune));
}
  
void VisualSignalInterface::update()
{
  if (_visual) {
  std::stringstream text;
  text << "Analyser" << std::endl;
  text << "noise: " << _visual->noise << std::endl;
  text << "clean: " << _visual->clean << std::endl;
  text << "distord: " << _visual->distord << std::endl;
  text << "linear: " << _visual->linear << std::endl;
  text << "grain: " << _visual->grain << std::endl;
  text.flush();

  _text.setString(text.str().c_str());

  _noise.setSize(sf::Vector2f(10, _visual->noise  / 200));
  _clean.setSize(sf::Vector2f(10, _visual->clean*80/6144));
  _distord.setSize(sf::Vector2f(10, _visual->distord / 250));
  _linear.setSize(sf::Vector2f(10, _visual->linear*80/3072));

  }
}

void VisualSignalInterface::addRender(GLSLRender& render,float size)
{
  render.setPosition(sf::Vector2f(256.f+size*0.5f,50));
  render.setScale(sf::Vector2f(size,-100.f));
  addDrawable(&render);
}


void DeLoorPiano::setKeyboard(bool k)
{
  _keyboard=k;
  _mouseCatcher.clear();
  _drawables.clear();
  addMouseCatcher(&_modeButton);
  addDrawable(&_progress);
  if (_keyboard) {
    for (unsigned int i=0; i < NB_NOTES; i++) {
      if (Note::isBlack(i)) {
        addDrawable(&(_keys[i]));
      }
    }
    for (unsigned int i=0; i < NB_NOTES; i++) {
      if (!Note::isBlack(i)) {
        addDrawable(&(_keys[i]));
      }
    }
  }
  else {
    for (unsigned int i=0; i < NB_NOTES; i++) {
      addDrawable(&(_bars[i]));
    }
  }

}

DeLoorPiano::DeLoorPiano(const sf::Vector2f& size, const sf::Font& font ) :
  Interface(sf::Vector2u(Signal::size,100),size),
  _manager(0),
  _table(0),
  _precomupter(0),
  _progress("",font,11),
  _modeButton(sf::Vector2f(60.f,18.f), "Keyboard",font ),
  _keyboard(false)
{
  _modeButton.setCallback(this,&DeLoorPiano::setKeyboard, &DeLoorPiano::isKeyboard);
  _modeButton.setPosition(Signal::size-65,5.f);
  addMouseCatcher(&_modeButton);

  for (unsigned int i=0; i < NB_NOTES; i++) {
     _bars[i].setRotation(180);
     _bars[i].setPosition((float)i*(float)Signal::size/(float)NB_NOTES,85);
  }
  
  const unsigned int dpos=Signal::size/52;
  unsigned int pos=dpos;
  for (unsigned int i=0; i < NB_NOTES; i++) {
     _keys[i].setRotation(180);
     if (Note::isBlack(i)) {
      _keys[i].setPosition(pos-dpos/2,55);
      _keys[i].setSize(sf::Vector2f(dpos-2,50));
      _keys[i].setFillColor(sf::Color(0,0,0,255));
     } else {
      _keys[i].setPosition(pos,85);
      _keys[i].setSize(sf::Vector2f(dpos-2,80));
      pos+=dpos;
     }
  }

  setKeyboard(_keyboard);
  _progress.setPosition(0,85);
}

DeLoorPiano::~DeLoorPiano() 
{

}


void DeLoorPiano::update()
{
  if (_precomupter)
  {
    if (_precomupter->ready()) {
      _table = _precomupter->getFrame(_precomupter->progress());
    }
    else {
      const unsigned int p = NB_NOTES*(_precomupter->progress()+1)/(_precomupter->frames()+1);
      std::stringstream s;
      s << _precomupter->progress() << "/" << _precomupter->frames() << std::endl;
      for (unsigned int i=0; i < p; i++) {

        float height = 15.f*cos(4.f*i/(float)NB_NOTES + _precomupter->progress()/500.f);
        height += 10.f*sin(16.f*i/(float)NB_NOTES + _precomupter->progress()/200.f) * cos(_precomupter->progress()/1500.f);
        _bars[i].setSize(sf::Vector2f(Signal::size/(float)NB_NOTES - 2.f,height + 60.f));
      }
      for (unsigned int i=p; i < NB_NOTES; i++) {
        _bars[i].setSize(sf::Vector2f(0,0));
      }
      s.flush();
      _progress.setString(s.str().c_str());
      return;
    }
  }

  if (_table) {
    if (_manager)
      _realupdate();
    for (unsigned int i=0; i < NB_NOTES; i++) {
      _bars[i].setSize(sf::Vector2f(Signal::size/(float)NB_NOTES - 2.f,_table[i]*80));
    }
  }
  else {
    for (unsigned int i=0; i < NB_NOTES; i++) {
      _bars[i].setSize(sf::Vector2f(0,0));
    }
  }

}

void DeLoorPiano::_realupdate()
{
    unsigned int peaks=0;

    if (_keyboard)
    {
      for (unsigned int i=0; i < NB_NOTES; i++) {
        if (_manager->peaks[i].alive && _manager->peaks[i].frames > 1 /*((_manager->peaks[i].frames > 1 && _manager->peaks[i].oscillate) || !_manager->peaks[i].oscillate)*/) {
            peaks++;
            if ( _manager->peaks[i].oscillate && i < _manager->mountains_position){
              if (Note::isBlack(i)) _keys[i].setFillColor(sf::Color(0,0,0,255));
              else _keys[i].setFillColor(sf::Color(255,255,255,255));
              _keys[_manager->peaks[i].begin_note_id].setFillColor(sf::Color(255,127,39,255));
            }
            else if (Note::isBlack(i)) {
              _keys[i].setFillColor(sf::Color(144,0,0,255));
            }
            else {
              _keys[i].setFillColor(sf::Color(100,100,200,255));
            }
        }
        else {
            if (Note::isBlack(i)) _keys[i].setFillColor(sf::Color(0,0,0,255));
            else _keys[i].setFillColor(sf::Color(255,255,255,255));   
        }
      }
    }

    else 
    {
      for (unsigned int i=0; i < NB_NOTES; i++) {
        if (_manager->peaks[i].alive) {
            peaks++;
            if ( _manager->peaks[i].oscillate){
              _bars[i].setFillColor(sf::Color(255,127,39,255));
            }
            else if (Note::isBlack(i)) {
              _bars[i].setFillColor(sf::Color(144,0,0,255));
            }
            else {
              _bars[i].setFillColor(sf::Color(100,100,200,255));
            }
        }
        else {
            _bars[i].setFillColor(sf::Color(255,255,255,255));   
        }
      }
    }

    std::stringstream s;
    s << "peaks: " << peaks  << " mountains pos: " << _manager->mountains_position <<  std::endl; 
    s.flush();
    _progress.setString(s.str().c_str());
}
/*
    sample max=0;
    sample min = 2.f;
    sample cmoy=0;
    for (unsigned int i=0; i < NB_NOTES; i++) {
      cmoy+=table[i];
      if (table[i] > max)
        max=table[i];
      else if (table[i] < min)
        min=table[i];
    }
    unsigned int mountain=0;
    sample mountain_power=0;

    for (unsigned int i=0; i < FA_4; i++) {
      mountain_power+=table[i];
      if (mountain_power > cmoy/1.2f) {
        mountain=i;
        break;
      }
    }

    mountain = FA_4;
    if (mountain){
      max=0;
      min=2.f;
      cmoy=0;
      for (unsigned int i=mountain; i < NB_NOTES; i++) {
        cmoy+=table[i];
        if (table[i] > max)
          max=table[i];
        else if (table[i] < min)
          min=table[i];
      }
    }

    const sample hmoy = cmoy/(sample)(NB_NOTES-mountain);
    const sample ecart = max-min;
    const sample seuil = hmoy + ecart/5.f;
    unsigned int played =0;
    

    for (unsigned int i=mountain; i < NB_NOTES; i++) {
      if (table[i] > _moy[i] && table[i] > seuil) {
        _framecount[i]=1;
      }
      else if (_framecount[i])
      {
        _framecount[i]++;
        if (_framecount[i] > 200 || table[i] < seuil)
          _framecount[i]=0;
      }

      if (_framecount[i])
      {
        played++;
        /*if (Note::isBlack(i))
          _keys[i].setFillColor(sf::Color(144-(_framecount[i]>>2),0,0,255));
        else 
          _keys[i].setFillColor(sf::Color(100+(_framecount[i]>>2),100+(_framecount[i]>>6),200-(_framecount[i]>>2),255));*/
      /*  if (Note::isBlack(i))
          _keys[i].setFillColor(sf::Color(144,0,0,255));
        else 
          _keys[i].setFillColor(sf::Color(100,100,200,255));
      }
      else {
          if (Note::isBlack(i)) _keys[i].setFillColor(sf::Color(0,0,0,255));
          else _keys[i].setFillColor(sf::Color(255,255,255,255));   
      }
      _previous2[i]=_previous[i];
      _previous[i]=table[i];
      _moy[i]=table[i]+_previous[i]+_previous2[i];
    }
    

    std::stringstream s;
    s << "moutain: " << mountain << " played: " << played <<  " max: " << (int)(max*1000) << " min: " << (int) (min*1000)  << " ecart: " << (int)(ecart*1000)  << " hmoy: " << (int)(hmoy*1000)  << std::endl; 
    s.flush();
    _progress.setString(s.str().c_str());
    
    _pmax2=_pmax;
    _pmax=max;
}

*/