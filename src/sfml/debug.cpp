
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
  * FILE         : debug.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  * DEPENDENCIES : debug.hpp
  */
  
#include <libtools/sfml/debug.hpp>

#ifdef _MSC_VER
#pragma warning( disable : 4244 ) //"Possible perte de données"
#endif


//Terminal debug(sf::Vector2u(600,150));

Terminal::Terminal(const sf::Vector2u& zone,const sf::Font& font) :
Interface(sf::Vector2u(80*11,200),sf::Vector2f(zone.x,zone.y)),
_render("",font,11)
{
  addDrawable(&_render);
}

Terminal::~Terminal()
{
}

void Terminal::update()
{  
  _render.setString(str().c_str());
  str(std::string());
  _internalZoneChanged(sf::Vector2u(80*11,_render.getGlobalBounds().height));
}

