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
  * FILE         : production.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V1.0
  * DEPENDENCIES : config.h
  *                signal.hpp
  */
  
#ifndef LIBTOOLS_PRODUCTION_HPP
#define LIBTOOLS_PRODUCTION_HPP

#include <libtools/public/config.h>
#include <libtools/core/signal.hpp>
#include <map>


class Production
{
  public:
    virtual ~Production() {}
    
    typedef int32_t RoleId;
    typedef int32_t GenreId;
    
    virtual string_t name()=0; //Title, name of the production
    virtual string_t author()=0; //Main author, alone or presented with '&' in the string
    //Return true if the production have a author list (= if implemented)
    inline virtual bool authors(std::multimap<RoleId,string_t>& list) {
      (void)list;
      return false;
    }
    inline virtual string_t group() { //Whatever (demo,music)group/label/producer
      return "";
    }
    virtual GenreId genre()=0;    
};


class MusicProduction : public Production, public AbstractStereoSignalSource
{
  public:
    virtual ~MusicProduction() {} 
    virtual string_t name()=0; //Title, name of the production
    virtual string_t author()=0; //Main author, alone or presented with '&' in the string
    virtual Production::GenreId genre()=0;    
    virtual unsigned int fetch(Signal& outleft, Signal& outright) = 0;
    
    virtual bool ended() const = 0; //can fetch ?
    virtual void rewind() = 0; //restart the production
    virtual double length() const = 0; //Length of the music in seconds
  
};

#endif
