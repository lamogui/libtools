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
  * VISIBILITY     : PRIVATE
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : envelope.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  *                Kwon-Young Choi (kwonyoungchoi93@gmail.com)
  * VERSION      : 1.7 olol
  * DEPENDENCIES : envelope.hpp
  */

#include <libtools/core/envelope.hpp>
#include <cmath>


 void Enveloppe::step(Signal* output)
 {
    sample* samples = output->samples;

    if (_releaseTime)
    {
      //release
      for (unsigned int i=0;i < Signal::size;i++)
      {
        samples[i]=_releaseLevel*exp(-5.f*(_time-_releaseTime)/(float)release);
        _time++;
      }
    }
    else
    {
      if (_time < attack)
      {
        unsigned int l = Signal::size;
        if (attack < _time + l) l = attack-_time;
        
        if (expatk)
        {
          for (unsigned int i=0;i < l;i++)
          {
            samples[i]=exp(5.f*(float)(_time-attack)/(float)attack);
            _time++;
          }
        }
        else
        {
          for (unsigned int i=0;i < l;i++)
          {
            //samples[i]=1.f-exp(-5.f*_time/(float)attack);
            samples[i]=(float)_time/(float)attack;
            _time++;
          }
        }
        
        const float sustain_1 = (1.f - sustain);
        for (unsigned int i=l;i < Signal::size;i++)
        {
          samples[i]=sustain + sustain_1*exp(-5.f*(_time-attack)/(float)decay);
          _time++;
        }
      }
      else if (_time < attack + decay)
      {
        //decay
        const float sustain_1 = (1.f - sustain);
        for (unsigned int i=0;i < Signal::size;i++)
        {
          samples[i]=sustain + sustain_1*exp(-5.f*(_time-attack)/(float)decay);
          _time++;
        }
      }
      else
      {
        //sustain
        output->constant(sustain);
        _time += Signal::size >> 1; //size/2
      }
      
      //toujours pas de release time ! on met à jour la valeur à laquel il va commencer
      _releaseLevel = output->samples[Signal::size-1]; 
    }
    
 }
