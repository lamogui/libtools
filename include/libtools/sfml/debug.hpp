
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
  * FILE         : debug.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  * DEPENDENCIES : config.h
  *                interface.hpp
  */

#ifndef LIBTOOLS_DEBUGINTERFACE_HPP
#define LIBTOOLS_DEBUGINTERFACE_HPP

#include <libtools/public/config.h>
#include <libtools/sfml/interface.hpp>
#include <sstream>
#include <SFML/Graphics.hpp>

#ifdef _MSC_VER
#pragma warning( disable : 4250 ) //"Machin hérite de bidule"
#endif

class Terminal : public Interface, public std::stringstream
{
  public:
    Terminal(const sf::Vector2u& zone, const sf::Font&);
    virtual ~Terminal();

    virtual void update();

  protected:
    sf::Text _render;
  
};

//extern Terminal debug;

#endif
