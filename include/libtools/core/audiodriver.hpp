
/*

THIS FILE IS SUBJECT TO STRICT RULES OF BDE NE666 COPYDOWN. 
NOBODY IS PERMITTED TO COPY AND DISTRIBUTE VERBATIM OR MODIFIED COPIES OF
THIS LICENSE HEADER. A SECURITY LEVEL IS ASSIGNED TO THE FILE AND A VISIBILITY.
THIS FILE MUST NOT BE COPIED OR REDISTRIBUTED IF IT'S VISIBILITY IS NOT PUBLIC. 
VISIBILITY HAS 3 POSSIBLE STATES:
- PRIVATE: LIMITED TO PROJECTS WITH THE SAME SECURITY LEVEL WITHIN THE BDE NE666. (DEFAULT VISIBILITY)
- PROTECTED: LIMITED TO PROJECTS USING THE SAME SECURITY RULES WITHIN THE BDE NE666. 
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
    virtual unsigned int getBufferedSamplesCount()=0;
    
    
   
};

#endif
