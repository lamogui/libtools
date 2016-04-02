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
  * VISIBILITY     : PROTECTED
  * © COPYDOWN LAMOGUI ALL RIGHTS RESERVED
  *
  * FILE         : ncwordart.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  */
  
#ifndef NOISECONTROLLERS_NCWORDART_HPP
#define NOISECONTROLLERS_NCWORDART_HPP

#include <libtools/public/config.h>
#include <SFML/Graphics.hpp>

class NCWordArt : public sf::Drawable, public sf::Transformable
{
  public:
    NCWordArt(const sf::Font& font);
    ~NCWordArt();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
    inline void setCharacterSize(unsigned int size)
    {
      _text.setCharacterSize(size);
    }

    static const std::string fadeFragmentShader;
    static const std::string cosVertexShader;
    static const std::string staticVertexShader;
    
    void start(
      const string_t str,
      float length=3.f,
      sf::Shader* shader=NULL,
      const sf::Color& color=sf::Color(255,255,255,255));
    
    inline const sf::Vector2f& size() const 
    {
      return _size;
    }

    inline float time () const
    {
      return _timer.getElapsedTime().asSeconds();
    }
    
  
  private:
    sf::Vector2f _size;
    sf::Text _text;
    sf::Clock _timer;
    sf::Shader* _shader;
    float _animationTime;
    unsigned int _len;
  
};

#endif //NOISECONTROLLERS_NCWORDART_HPP
