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
