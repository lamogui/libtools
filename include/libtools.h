
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
  * FILE         : libtools.h
  * AUTHOR       : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.7 olol
  * DEPENDENCIES : config.h
  *
  *
  * How to use it ?
  *  Simply include libtools dependencies you want use BEFORE libtools.h
  *  Then libtools.h will include all necessary 
  *  Current usable dependencies are : - SFML 2.X (Graphics)
  *                                    - Bass
  *                                    - Bassflac
  *                                    - Bassasio
  *  
  */
  

#ifndef LIBTOOLS_LOADED

//Dependencies
#include <libtools/public/config.h> //Search on google

#ifdef __cplusplus
///C++ API
#if !defined(LIBTOOLS_DLL) || defined(LIBTOOLS_DLL_IMPLEMENT) 

//core (no external dependencies)
#include <libtools/core/variant.hpp>
#include <libtools/core/file.hpp>
#include <libtools/core/settings.hpp>
#include <libtools/core/signal.hpp>
#include <libtools/core/visualsignal.hpp>
#include <libtools/core/note.hpp>
#include <libtools/core/fft.hpp>
#include <libtools/core/tfdeloor.hpp>
#include <libtools/core/analogfilter.hpp>
#include <libtools/core/envelope.hpp>
#include <libtools/core/audiostream.hpp>
#include <libtools/core/audiodriver.hpp>
#include <libtools/core/decoder.hpp>
#include <libtools/core/peak.hpp>
#include <libtools/core/tempo.hpp>
#include <libtools/core/music.hpp>


//drivers
#if defined(LIBTOOLS_WINDOWS)
  #include <libtools/drivers/winmm.hpp>
#endif
#if defined(BASS_H) || defined(LIBTOOLS_WINDOWS)
  #include <libtools/drivers/bass.hpp>
#endif
#if defined(SFML_AUDIO_HPP)
  #include <libtools/drivers/sfmlaudio.hpp>
#endif

//sfml (graphical tools) not available on public dll
#if defined(SFML_GRAPHICS_HPP) && !defined(LIBTOOLS_DLL)
#include <libtools/sfml/glslrender.hpp>
#include <libtools/sfml/interface.hpp>
#include <libtools/sfml/window.hpp>
#include <libtools/sfml/debug.hpp>
#include <libtools/sfml/scope.hpp>
#endif

#endif
//public (dll public symbols)
#include <libtools/public/singleinstance.hpp>

extern "C" {
#endif

///C API
//core
#include <libtools/core/file.h>

//drivers
#include <libtools/drivers/bassproc.h>

#ifdef __cplusplus
}
#endif

#define LIBTOOLS_LOADED
#endif