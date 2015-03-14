
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
  * FILE         : libtools.h
  * AUTHOR       : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.7 olol
  * DEPENDENCIES : config.h
  *
  *
  * How to use it ?
  *  Simply include libtools dependencies you want use BEFORE libtools.h
  *  Then libtools.h will include all necessary 
  *  Current usable dependencies are : - SFML 2.X custom [https://github.com/lamogui/SFML]
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

//core (all primary interfaces)
#include <libtools/core/variant.hpp>
#include <libtools/core/file.hpp>
#include <libtools/core/signal.hpp>
#include <libtools/core/circularpurger.hpp>
#include <libtools/core/visualsignal.hpp>
#include <libtools/core/note.hpp>
#include <libtools/core/fft.hpp>
#include <libtools/core/tfdeloor.hpp>
#include <libtools/core/analogfilter.hpp>
#include <libtools/core/envelope.hpp>
#include <libtools/core/audiostream.hpp>
#include <libtools/core/audiodriver.hpp>
#include <libtools/core/decoder.hpp>
#include <libtools/core/production.hpp>
#include <libtools/core/peak.hpp>
#include <libtools/core/tempo.hpp>
#include <libtools/core/music.hpp>


//decoders (may use external lib)
#include <libtools/decoders/settings.hpp>

//drivers (use external library to implement interfaces)
#if defined(LIBTOOLS_WINDOWS)
  #include <libtools/drivers/winmm.hpp>
#endif
#if defined(BASS_H) || defined(LIBTOOLS_WINDOWS)
  #include <libtools/drivers/bass.hpp>
#endif

///the following are specifics tools to use with a framework
//sfml (graphical tools) not available on public dll
#if defined(SFML_GRAPHICS_HPP) && !defined(LIBTOOLS_DLL)
#include <libtools/sfml/glslrender.hpp>
#include <libtools/sfml/sampler1d.hpp>
#include <libtools/sfml/interface.hpp>
#include <libtools/sfml/window.hpp>
#include <libtools/sfml/debug.hpp>
#include <libtools/sfml/scope.hpp>
#endif

#if defined(QT_VERSION)
//qt stuff
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

//shaders
#include <libtools/shaders/shaders.h>

#ifdef __cplusplus
}
#endif

#define LIBTOOLS_LOADED
#endif