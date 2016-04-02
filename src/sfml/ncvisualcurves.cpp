/*

THIS FILE IS SUBJECT TO STRICT RULES OF BDE NE777 COPYDOWN. 
NOBODY IS PERMITTED TO COPY AND DISTRIBUTE VERBATIM OR MODIFIED COPIES OF
THIS LICENSE HEADER. A SECURITY LEVEL IS ASSIGNED TO THE FILE AND A VISIBILITY.
THIS FILE MUST NOT BE COPIED OR REDISTRIBUTED IF IT'S VISIBILITY IS NOT PUBLIC. 
VISIBILITY HAS 3 POSSIBLE STATES:
- PRIVATE: LIMITED TO PROJECTS WITH THE SAME SECURITY LEVEL WITHIN THE BDE NE777. (DEFAULT VISIBILITY)
- PROTECTED: LIMITED TO PROJECTS USING THE SAME SECURITY RULES WITHIN THE BDE NE777. 
- PUBLIC: USABLE FOR FREE IN PUBLIC PROJECTS UNDER THE FOLLOWING LICENSES: BSD, MIT OR DWTFYWT 
WITHOUT OTHER CONDITIONS THAN THE CONSERVATION OF THIS HEADER INCLUDING: 
RULES, ORIGINAL AUTHORS (WITH EMAIL), ORIGINAL FILENAME, AND VERSION, INSIDE THE FILE
WHICH CONTAINS PART (OR ENTIRE) CODE FROM THIS FILE. USABLE IN OTHER PROJECTS WITH CONDITIONS 
AND REMUMERATIONS, FIXED BY ORIGINAL AUTHORS (CONTACT THEM).

*/

/**
  * THIS FILE IS PART OF NOISECONTROLLERS 
  * SECURITY LEVEL : 8 (CRITICAL)  
  * VISIBILITY     : PRIVATE
  * © COPYDOWN LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : ncvisualcurves.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  */

#include <libtools/sfml/ncvisualcurves.hpp>
#include <sstream>

NCVisualCurves::NCVisualCurves(
  const sf::Vector2f& size, 
  const sf::Font& font,
  const string_t& title) :
Interface(sf::Vector2u(600,100),size),
_title(title),
_maxwidth(0.f),
_text("",font,11)
{
  _text.setPosition(0,10);
  addDrawable(&_text);
}

NCVisualCurves::~NCVisualCurves()
{
  
  
}

void NCVisualCurves::addTracker(const string_t name, tracker& t)
{
  t.bar.setRotation(180);
  t.bar.setFillColor(t.color);
  t.name=name;
  t.sampler.create(t.buffer->size(), t.buffer->data());
  t.sampler.setRepeated(true);
  t.render.loadFromMemory(GLSLRender::getVertexShaderCode(),classic_flux_fs_src);
  t.render.setParameter("flux",t.sampler);
  t.render.setParameter("thickness",2.f/100.f);
  t.render.setParameter("step",1.f/(float)t.buffer->size());
  t.render.setParameter("reversed",false); //Most recent = right
  t.render.setParameter("color",t.color);
  _trackers[name]=&t;
  if (t.buffer->size() > _maxwidth)
      _maxwidth = t.buffer->size();
  
  addDrawable(&(t.render));
  addDrawable(&(t.bar));
  _place();
}

void NCVisualCurves::_place()
{
  _computeText();
  std::map<string_t, tracker*>::iterator it;
  float x=160;
  //float x=_text.getGlobalBounds().left+_text.getGlobalBounds().width + 60;
  float y=_text.getGlobalBounds().top+_text.getGlobalBounds().height;
  for (it=_trackers.begin(); it != _trackers.end() ;it++)
  {
    it->second->bar.setPosition(x,y);
    x += 30;
  }
  for (it=_trackers.begin(); it != _trackers.end() ;it++)
  {
    it->second->render.setPosition(sf::Vector2f(x + _maxwidth*0.5f, 10 + (y-10)*0.5f));
    it->second->render.setScale(sf::Vector2f(_maxwidth, -(y-10)));
  }
  _internalZoneChanged(sf::Vector2u(x+_maxwidth, y+10));
}

void NCVisualCurves::_computeText()
{
  std::map<string_t, tracker*>::iterator it;
  
  std::stringstream text;
  text << string_t_to_std(_title) << std::endl;
  for (it=_trackers.begin(); it != _trackers.end() ;it++)
  {
    text << string_t_to_std(it->first) << ": " << it->second->value << std::endl;
  }
  text.flush();
  _text.setString(text.str().c_str());
}

void NCVisualCurves::update()
{
  _computeText();
  std::map<string_t, tracker*>::iterator it;
  for (it=_trackers.begin(); it != _trackers.end() ;it++)
  {
    float value=it->second->buffer->last();
    unsigned int offset=it->second->buffer->offset();
    float size=(float)it->second->buffer->size();
    it->second->render.setParameter("offset",(float)offset/size);
    it->second->render.setParameter("color",it->second->color);
    it->second->bar.setSize(sf::Vector2f(10, value*(_zone.y-20)));
    it->second->sampler.update(&value,1,offset);
  }
}

NCPrecomputerValueCurves::NCPrecomputerValueCurves(
                         const sf::Vector2f& size,
                         const sf::Font& font,
                         const string_t& title):
  NCVisualCurves(size,font,title + string_t(" curves"))
{
  _createBuffers();
  _setupColors();
  _addTrackers(title);
}

void NCPrecomputerValueCurves::_setupColors()
{
  _brut.color=sf::Color(255,255,255,255);

/*  _l11.color=sf::Color(40,19,232,255);
  _m11.color=sf::Color(255,250,34,255);
  _h11.color=sf::Color(255,4,4,255);
  */
  _l11a11.color=sf::Color(40,19,232,255);
  _m11a11.color=sf::Color(255,250,34,255);
  _h11a11.color=sf::Color(255,4,4,255);

  _l11a128.color=sf::Color(40,19,232,255);
  _m11a128.color=sf::Color(255,250,34,255);
  _h11a128.color=sf::Color(255,4,4,255);

}

void NCPrecomputerValueCurves::_createBuffers()
{
  _brut.buffer=new circularpurger<float>(512);

 /* _l11.buffer=new circularpurger<float>(512);
  _m11.buffer=new circularpurger<float>(512);
  _h11.buffer=new circularpurger<float>(512);
 */
  _l11a11.buffer=new circularpurger<float>(512);
  _m11a11.buffer=new circularpurger<float>(512);
  _h11a11.buffer=new circularpurger<float>(512);

  _l11a128.buffer=new circularpurger<float>(512);
  _m11a128.buffer=new circularpurger<float>(512);
  _h11a128.buffer=new circularpurger<float>(512);
}

void NCPrecomputerValueCurves::_addTrackers(const string_t base)
{
  this->addTracker(base,_brut);
  this->addTracker(base+string_t("l11a11"),_l11a11);
  this->addTracker(base+string_t("m11a11"),_m11a11);
  this->addTracker(base+string_t("h11a11"),_h11a11);
  this->addTracker(base+string_t("l11a128"),_l11a128);
  this->addTracker(base+string_t("m11a128"),_m11a128);
  this->addTracker(base+string_t("h11a128"),_h11a128);
}

void NCPrecomputerValueCurves::feed(
    const ncprecomputervalueframef& frame,
    int16_t multiplier)
{

  _brut.value=frame.brut*multiplier;
  _l11a11.value=frame.l11a11*multiplier;
  _m11a11.value=frame.m11a11*multiplier;
  _h11a11.value=frame.h11a11*multiplier;
  _l11a128.value=frame.l11a128*multiplier;
  _m11a128.value=frame.m11a128*multiplier;
  _h11a128.value=frame.h11a128*multiplier;

  _brut.buffer->push(frame.brut);
  _l11a11.buffer->push(frame.l11a11);
  _m11a11.buffer->push(frame.m11a11);
  _h11a11.buffer->push(frame.h11a11);
  _l11a128.buffer->push(frame.l11a128);
  _m11a128.buffer->push(frame.m11a128);
  _h11a128.buffer->push(frame.h11a128);

  this->update();
}

void NCPrecomputerValueCurves::feed(
    const ncprecomputervalueframe16& frame,
    float diviser)
{
  _brut.value=frame.brut;
  _l11a11.value=frame.l11a11;
  _m11a11.value=frame.m11a11;
  _h11a11.value=frame.h11a11;
  _l11a128.value=frame.l11a128;
  _m11a128.value=frame.m11a128;
  _h11a128.value=frame.h11a128;

  _brut.buffer->push((float)frame.brut/diviser);
  _l11a11.buffer->push((float)frame.l11a11/diviser);
  _m11a11.buffer->push((float)frame.m11a11/diviser);
  _h11a11.buffer->push((float)frame.h11a11/diviser);
  _l11a128.buffer->push((float)frame.l11a128/diviser);
  _m11a128.buffer->push((float)frame.m11a128/diviser);
  _h11a128.buffer->push((float)frame.h11a128/diviser);

  this->update();
}
