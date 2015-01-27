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
  * VISIBILITY     : PROTECTED
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : scope.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 
  * DEPENDENCIES : config.h
  *                signal.hpp
  *                interface.hpp
  *                fft.hpp
  *                visualsignal.hpp
  *                tfdeloor.hpp
  *                peak.hpp
  * ROLE         : Définie une interface de visualisation des signaux 
  *                configurable en oscilloscope ou analyseur de spectre
  */

#ifndef LIBTOOLS_SCOPE_HPP
#define LIBTOOLS_SCOPE_HPP

#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <libtools/public/config.h>
#include <libtools/core/signal.hpp>
#include <libtools/core/visualsignal.hpp>
#include <libtools/sfml/interface.hpp>
#include <libtools/core/fft.hpp>
#include <libtools/core/tfdeloor.hpp>
#include <libtools/core/peak.hpp>

class ScopeA : public Interface
{
   public:
      //ScopeA(const sf::Vector2f& size,const sf::Font& font, bool spectrum=false);
      ScopeA(const sf::Vector2f& size,const sf::Font& font, bool spectrum=false, Signal* s=0);
      virtual ~ScopeA();
      
      void setYZoom(float z);
      void setSignal(Signal* s);

      inline bool isSpectrum() {
        return _spectrum;
      }

      void setSpectrum(bool spectrum);

      virtual void update();
      
      inline void setUpdateRate(unsigned int t) { _update_time=t;}
      
      void setColor(const sf::Color& color);
      
      void setFadeColor(const sf::Color& colorInit, const sf::Color& colorEnd, bool axe);

   protected:
      void _allocate();
   
      Signal* _signal;
      FFT* _fft;
      uint8_t* _pixels;
      sf::Color _color;
      sf::Texture _texture;
      sf::RectangleShape _back;
      sf::Sprite _sprite;
      float _y_zoom;
      unsigned int _update_time;
      unsigned int _time;
      bool _spectrum;
      int _autoscaley;
      ModulableCallbackButton<ScopeA> _modeButton;
      Button _autoYScaleButton;
};


class VisualSignalInterface : public Interface
{
  public:
    VisualSignalInterface(const sf::Vector2f& size, const sf::Font&);
    virtual ~VisualSignalInterface();

    virtual void update();
    void setVisualSignal(VisualSignal* s);
    void setCouleur(int rouge,int ver,int bleu, int jaune);


  private:
    VisualSignal* _visual;
    sf::Text _text;
    sf::RectangleShape _noise;
    sf::RectangleShape _clean;
    sf::RectangleShape _distord;
    sf::RectangleShape _linear;
    sf::RectangleShape _rouge;
    sf::RectangleShape _vert;
    sf::RectangleShape _bleu;
    sf::RectangleShape _jaune;

};

class DeLoorPiano  : public Interface
{
  public:
    DeLoorPiano(const sf::Vector2f& size, const sf::Font& );
    virtual ~DeLoorPiano();

    inline void setTFDeLoor(sample* table) {
      _table=table;
    }
    inline void setManager(TFDeLoorPeakManager* m) {
      _manager=m;
    }

    inline void setTFDeLoorPrecomputer(TFDeLoorPrecomputer* c)
    {
      _precomupter=c;
    }


    virtual void update();

    void setKeyboard(bool k);
    bool isKeyboard() {
      return _keyboard;
    }

  private:

    void _realupdate();

    TFDeLoorPeakManager* _manager;
    sample* _table;
    TFDeLoorPrecomputer* _precomupter;
    sf::RectangleShape _bars[NB_NOTES];
    sf::RectangleShape _keys[NB_NOTES];

    sf::Text _progress;
    ModulableCallbackButton<DeLoorPiano> _modeButton;
    bool _keyboard;
};


#endif
