/*

    This file is part of libtools.

    Foobar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

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


class GLSLRender;

class VisualSignalInterface : public Interface
{
  public:
    VisualSignalInterface(const sf::Vector2f& size, const sf::Font&);
    virtual ~VisualSignalInterface();

    virtual void update();
    void setVisualSignal(const VisualSignal* s);
    void setColor(int rouge,int ver,int bleu, int jaune);
    void addRender(GLSLRender& render,float size);


    virtual void setViewSize(float x, float y);

  private:

    void _setupRendered();

    const VisualSignal* _visual;
    sf::Text _text;
    sf::RectangleShape _noise;
    sf::RectangleShape _clean;
    sf::RectangleShape _distord;
    sf::RectangleShape _linear;
    sf::RectangleShape _rouge;
    sf::RectangleShape _vert;
    sf::RectangleShape _bleu;
    sf::RectangleShape _jaune;

    std::vector<std::pair<GLSLRender*, float> > _renders;

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

    virtual void setViewSize(float x, float y);

  private:
    void setupGraphics();

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
