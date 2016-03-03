
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
  * FILE         : audiodriver.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2
  * DEPENDENCIES : config.h
  *                signal.hpp
  *                audiostream.hpp
  * ROLE         : Defini une interface vers un driver audio en sortie steréo
  */

#ifndef LIBTOOLS_AUDIODRIVER_HPP
#define LIBTOOLS_AUDIODRIVER_HPP

#include <libtools/public/config.h>
#include <libtools/core/signal.hpp>
#include <libtools/core/audiostream.hpp>
//An audio driver should have a single instance

class LIBTOOLS_PUBLIC AudioDriver : public noncopyable_c
{
  public:
    AudioDriver() {}
    virtual ~AudioDriver() {}
    
    //(Re)Start the driver thread
    //It should now play buffered samples 
    virtual bool start()=0;
    
    //stop the audio thread
    //Garantie : No Output than the last driver buffer
    //           No use of Signals Objects !!!
    virtual void stop()=0;
    
    //Driver, API currently working in output mode with a device
    virtual bool working()=0;
    
    
    //Push a signal block into driver buffers
    virtual bool pushStereoSignal(const Signal& left,const Signal& right)=0; 
    
    //Return not yet played samples (at least an estimation > than the reality)
    //It is not related to the number of channel
    virtual unsigned int getBufferedSamplesCount()=0;
    
    
   
};

#endif
