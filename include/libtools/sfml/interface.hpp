﻿/*

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
  * FILE         : interface.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  * DEPENDENCIES : config.h
  * ROLE         : Définis des classes abstraites (ou très
  *                générique) permettant de créer des élements 
  *                d'interface compatible avec opengl
  */


#ifndef LIBTOOLS_INTERFACE_HPP
#define LIBTOOLS_INTERFACE_HPP


#include <libtools/public/config.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


class LIBTOOLS_PUBLIC MouseCatcher : public sf::Drawable
{
  public:
    //Coordinates must be view converted !
    virtual bool onMousePress(float x, float y)=0; //return true if the device catch the mouse
    virtual void onMouseMove(float x, float y)=0;
    virtual void onMouseRelease(float x, float y)=0;
    virtual void onMouseOver(float x, float y) {
      (void) x; (void) y;
    }
    //update something ?
    virtual void update()=0;
};

class LIBTOOLS_PRIVATE Knob : public MouseCatcher, public sf::Transformable
{
  public:
    Knob();
    Knob(const sf::Texture &texture, const sf::IntRect &backRect, const sf::IntRect &knobRect);
    virtual ~Knob();
    
    
    void setBackTexture(const sf::Texture &texture,const sf::IntRect &rect);
    void setKnobTexture(const sf::Texture &texture,const sf::IntRect &rect);
    inline sf::Color overColor(){return _overColor;}
    inline void setOverColor(const sf::Color& c){_overColor = c;}
    
    virtual bool onMousePress(float x, float y);
    virtual void onMouseMove(float x, float y);
    virtual void onMouseRelease(float x, float y);
    virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void update();
	
    
    
    
  protected:
    sf::Color _overColor;
    sf::Sprite _back_sprite;
    sf::Sprite _knob_sprite;
    //InstrumentParameter* _param;
    bool _catched;
    float _catch_x;
    float _catch_y;
    float _catch_angle;
};

namespace ButtonMode {
  typedef enum {
    toggle,
    increment,
    decrement,
    on,
    off,
    interrupt
  } Mode;
}


//Defini comment dessiner un bouton 
class LIBTOOLS_PRIVATE AbstractButton : public MouseCatcher, public sf::Transformable
{
  public:
    AbstractButton(const sf::Vector2f& size, 
                   const sf::String text,
                   const sf::Font& font);
    AbstractButton( const sf::Texture &texture, 
                    const sf::IntRect &idle, 
                    const sf::IntRect &clicked,
                    const sf::Font& font);
              
    virtual ~AbstractButton();      
      
      
    //Shape setters    
    inline void setOutlineThickness(float f) {_shape.setOutlineThickness(f);}
    inline void setOutlineColor(const sf::Color& c) {_shape.setOutlineColor(c); _text.setColor(c);}

    //Color setters
    inline void setColors(const sf::Color& i, const sf::Color& c) {setClickedColor(c);setIdleColor(i);}
    inline void setClickedColor(const sf::Color& c) {_clickedColor=c;}
    inline void setIdleColor(const sf::Color& i) {_idleColor=i; _shape.setFillColor(_idleColor);}
  
    //Text setter
    void setText(const sf::String& t);
    inline sf::Text& getText() { return _text; }
     
    //Texture setter (assume button is not catched)
    void setTexture(const sf::Texture &texture, const sf::IntRect &idle, const sf::IntRect &clicked);

	//Windows 98 Styke
	inline bool isWin98Looking() { return _win98; } 
	void setWin98Looking(bool e);
     
    virtual bool onMousePress(float x, float y);
    virtual void onMouseMove(float x, float y);
    virtual void onMouseRelease(float x, float y);
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void update();

    inline sf::Vector2f getSize() const { return _shape.getSize(); }
      
  protected:
    void released();
    void pressed();

    inline virtual bool allowed() { return true;}
    virtual void clicked()=0;   
   
    sf::Color _idleColor;
    sf::Color _clickedColor;
    sf::RectangleShape _shape;
    sf::RectangleShape _lightup;
    sf::RectangleShape _lightleft;
    sf::RectangleShape _shadowright;
    sf::RectangleShape _shadowdown;
    sf::Text _text;
    sf::IntRect _idleRect;
    sf::IntRect _clickedRect;
	bool _win98;
};

class LIBTOOLS_PRIVATE ModulableButton : public AbstractButton 
{
  public:
    ModulableButton( const sf::Texture &texture,
                     const sf::IntRect &idle,
                     const sf::IntRect &clicked,
                     const sf::Font& font,
                     ButtonMode::Mode mode=ButtonMode::toggle);
                     
    ModulableButton( const sf::Vector2f& size, 
                     const sf::String text,
                     const sf::Font& font,
                     ButtonMode::Mode mode=ButtonMode::toggle);
                     
    virtual ~ModulableButton();
    
    inline virtual void forceOn() {
      _shape.setTextureRect(_clickedRect);
      _shape.setFillColor(_clickedColor);
    }
    inline virtual void forceOff() {
      _shape.setTextureRect(_idleRect);
      _shape.setFillColor(_idleColor);
    }
    
    virtual bool onMousePress(float x, float y);
    virtual void onMouseRelease(float x, float y);
  protected:
    //return the state of the button in ToggleMode
    virtual bool toggleState()=0;
    ButtonMode::Mode _mode;
};


class LIBTOOLS_PRIVATE Button : public ModulableButton 
{
  public:
    Button(const sf::Vector2f& size, 
           const sf::String text,
           const sf::Font& font,
           ButtonMode::Mode mode=ButtonMode::toggle);
  
    Button(int* v, 
           const sf::Texture &texture,
           const sf::IntRect &idle,
           const sf::IntRect &clicked,
           const sf::Font& font,
           ButtonMode::Mode mode=ButtonMode::toggle);
           
    Button( int* v,  const sf::Vector2f& size, const sf::String text,
            const sf::Font& font, ButtonMode::Mode mode=ButtonMode::toggle);
    
    virtual ~Button();
    
    void linkTo(int* v,
                ButtonMode::Mode mode=ButtonMode::toggle);
    
    inline virtual void forceOn()
    {
      if (_val) *_val=1;
      ModulableButton::forceOn();
    }
    
    inline virtual void forceOff()
    {
      if (_val) *_val=0;
      ModulableButton::forceOff();
    }
    
  protected:
    virtual void clicked();
    inline virtual bool toggleState()
    {
      if (_val)
        return *_val ? true : false;
      return false;
    }
    int* _val; 
};


class LIBTOOLS_PRIVATE SingleProcessButton : public AbstractButton
{
  public:
    SingleProcessButton(const sf::Vector2f& size, 
                        const sf::String text,
                        const sf::Font& font);
    
    SingleProcessButton(const sf::Texture &texture,
                         const sf::IntRect &idle,
                         const sf::IntRect &clicked,
                         const sf::Font& font);
                   
    virtual ~SingleProcessButton();
  
    template<typename F > 
    void setProcess(F function) {
      if (_thread) delete _thread;
      _thread=new sf::Thread(function);
    }
  
    template<typename F , typename A >
    void setProcess(F function, A argument){
      if (_thread) delete _thread;
      _thread=new sf::Thread(function,argument);
    }
  
    template<typename C >
    void setProcess(void(C::*function)(), C *object) {
      if (_thread) delete _thread;
      _thread=new sf::Thread(function,object);
    }
  
  protected:
    inline virtual bool allowed() { 
      return _thread ? true : false;
    } 
    virtual void clicked();
    sf::Thread* _thread;
};


template<class C >
class LIBTOOLS_PUBLIC ModulableCallbackButton : public ModulableButton
{
  public:
    typedef void(C::*setter)(bool);
    typedef bool(C::*getter)(void);
  protected:
    C* _object;
    setter _setter;
    getter _getter;

  public:
    ModulableCallbackButton(const sf::Vector2f& size, 
                            const sf::String text,
                            const sf::Font& font,
                            ButtonMode::Mode mode=ButtonMode::toggle) :
    ModulableButton(size,text,font,mode),    
    _object(0),
    _setter(0),
    _getter(0) 
    {
    }
    
    ModulableCallbackButton(const sf::Texture &texture,
                            const sf::IntRect &idle,
                            const sf::IntRect &clicked,
                            const sf::Font& font,
                            ButtonMode::Mode mode=ButtonMode::toggle):
    ModulableButton(texture,idle,clicked,font,mode),    
    _object(0),
    _setter(0),
    _getter(0) 
    {
    }
                   
    virtual ~ModulableCallbackButton() {
    }
    
    

    void setCallback(C *object,setter s,getter g) {
       _object=object;
       _setter=s;
       _getter=g;
    }
  
  protected:
    virtual void clicked() {
      if (_object && _setter) {
      switch (_mode)
      {
        case ButtonMode::toggle:
          (_object->*_setter)(!(_object->*_getter)()); 
          break;

        case ButtonMode::increment:
        case ButtonMode::interrupt:
        case ButtonMode::on:
          (_object->*_setter)(true);
          break;

        case ButtonMode::decrement: 
        case ButtonMode::off:
          (_object->*_setter)(false);
          break;
      }
    }
    }
    

    inline virtual bool allowed() { 
      return _object && _setter ? true : false;
    } 

    inline virtual bool toggleState() {
      return (_object->*_getter)();
    }
};


class LIBTOOLS_PRIVATE ScrollBar : public MouseCatcher
{
  public:
    ScrollBar(sf::View& view, unsigned int zone,bool horizontal=false);
    virtual ~ScrollBar();
    
    virtual bool onMousePress(float x, float y);
    virtual void onMouseMove(float x, float y);
    virtual void onMouseRelease(float x, float y);
    virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void update();

    void setZoneSize(unsigned int zone);
    inline int getPosition() {return _current_offset;}
    
  private:
    sf::View *_view;
    sf::RectangleShape _bar;
    sf::RectangleShape _decoration;
    float _catch;
    int _current_offset;
    unsigned int _zone_size;
    bool _horizontal;
};

class LIBTOOLS_PRIVATE Fader : public MouseCatcher {
  public:
    Fader(const sf::Texture &texture, const sf::IntRect &backRect, const sf::IntRect &faderRect);
    virtual ~Fader();
    
    virtual bool onMousePress(float x, float y);
    virtual void onMouseMove(float x, float y);
    virtual void onMouseRelease(float x, float y);
    virtual void draw (sf::RenderTarget &target, sf::RenderStates states) const;
    
  protected:
    sf::Sprite _back_sprite;
    sf::Sprite _fader_sprite;
    float _catch_x;
    float _catch_y;
    float _catch_translation;
};

class LIBTOOLS_PRIVATE Interface : public sf::Drawable
{
  public:
    Interface(const sf::Vector2u& zone,const sf::Vector2f &size);
    virtual ~Interface();

    //is the interface have an object to interact with
    //x and y in opengl coordinates
    MouseCatcher* onMousePress(float x, float y);

    
    //is the mouse on the interface now ?
    bool onIt(unsigned int x, unsigned int y, 
              unsigned int sx, unsigned int sy);

    inline void setViewport(const sf::FloatRect &viewport)
    {
      _view.setViewport(viewport);
    }
    inline sf::View& getView() 
    {
      return _view;
    }
    
    virtual inline sf::Vector2u getIdealSize() //pas forcement la "zone occupé"
    {
      return _zone;
    }
    
    virtual inline sf::Color getColor() //Couleur de l'interface
    {
      return sf::Color(255,255,255,0);
    }


    inline sf::Vector2i getPosition() {
      return sf::Vector2i(_horizontalBar.getPosition(),
                          _verticalBar.getPosition());
    }    

    virtual void setViewSize(float x, float y);
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void update();

    void addMouseCatcher(MouseCatcher* c);
    void addDrawable(sf::Drawable* c);

  protected:
    virtual void _internalZoneChanged(const sf::Vector2u& nz);
  
    std::vector<MouseCatcher*> _mouseCatcher;
    std::vector<sf::Drawable*> _drawables;
    sf::View _view;
    sf::Vector2u _zone;
    ScrollBar _verticalBar;
    ScrollBar _horizontalBar;
};

#endif
