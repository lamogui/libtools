
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
  * FILE         : decoder.hpp
  * AUTHOR       : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.0
  * DEPENDENCIES : signal.hpp
  *                config.h
  */



#ifndef LIBTOOLS_DECODER_HPP
#define LIBTOOLS_DECODER_HPP

#include <libtools/public/config.h>
#include <libtools/core/signal.hpp>

class LIBTOOLS_PRIVATE Decoder
{
  public:
    virtual ~Decoder() {
    }

    virtual bool open(const string_t& filename)=0;
    virtual bool ended() = 0;
    virtual void rewind() = 0; 
    virtual unsigned int frames() = 0; 
    inline virtual string_t name() { return string_t(); }
    inline virtual string_t artist() { return string_t(); }
    inline virtual string_t album() { return string_t(); }
    inline virtual string_t genre() { return string_t(); }
    virtual unsigned int nextFrame(Signal& left, Signal& right)=0;

};

#endif