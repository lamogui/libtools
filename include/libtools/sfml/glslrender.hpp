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
  * FILE         : glslrender.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  * DEPENDENCIES : config.h
  */

#ifndef LIBTOOLS_SFMLGLSLRENDER_HPP
#define LIBTOOLS_SFMLGLSLRENDER_HPP

#include <libtools/public/config.h>
#include <SFML/Graphics.hpp>
#include <string>

class LIBTOOLS_PRIVATE GLSLRender : public sf::Drawable, public sf::Transformable, public sf::Shader
{
  public:
    GLSLRender();
    virtual ~GLSLRender();
    
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    static std::string getVertexShaderCode();

    bool shortcutLoadFSFromFile(const std::string& fragment_file);
    
  protected:
    sf::RectangleShape _rectangle;
  
};

#endif
