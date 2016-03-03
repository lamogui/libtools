
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
  * FILE         : visualsignal.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 
  * DEPENDENCIES : signal.hpp
  *                circularpurger.hpp
  *                config.h
  */

#ifndef LIBTOOLS_VISUALSIGNAL_HPP
#define LIBTOOLS_VISUALSIGNAL_HPP

#include <libtools/public/config.h>
#include <libtools/core/signal.hpp>

class LIBTOOLS_PRIVATE VisualSignal
{
  public:
    VisualSignal();
    virtual ~VisualSignal();

    void update(const Signal& signal);
    
    int16_t noise;
    int16_t clean;
    int16_t distord;
    int16_t linear;
    int16_t grain;
    int16_t triangles;

  protected:
    int16_t samples[1024]; //samples scaled from 0 to 100
    int16_t previous;
    int16_t previous2;
    int16_t dprevious;
    int16_t dprevious2;
};

#endif
