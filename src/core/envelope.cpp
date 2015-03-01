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
