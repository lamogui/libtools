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
  * FILE         : envelope.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  *                Kwon-Young Choi (kwonyoungchoi93@gmail.com)
  * VERSION      : 1.7 olol
  * DEPENDENCIES : signal.hpp
  *                config.h
  */


#ifndef LIBTOOLS_ENVELOPE_HPP
#define LIBTOOLS_ENVELOPE_HPP

#include <libtools/public/config.h>
#include <libtools/core/signal.hpp>

class LIBTOOLS_PRIVATE Enveloppe : public AbstractSignalGenerator {
   public:
     Enveloppe() : AbstractSignalGenerator(), expatk(false), _releaseTime(0) {};
     virtual ~Enveloppe() {};
    
     virtual void step(Signal* output);

     inline bool hasEnded()
     {
       
       return _releaseTime && _time > release + _releaseTime;
     }
     
     virtual inline void resetTime()
     {
       _time=0;
       _releaseTime=0;
     }

     inline void beginRelease()
     {
      _releaseTime=_time;
     }
    
     unsigned int attack;
     unsigned int decay;
     float sustain;
     unsigned int release;
     
     bool expatk;
     
   protected:
     float _releaseLevel;
     unsigned int _releaseTime;
};


#endif
