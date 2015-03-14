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
  * VISIBILITY     : PUBLIC
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : config.h
  * AUTHOR       : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.7 olol
  * DEPENDENCIES : pstdint.h
  *
  */
  
#ifndef LIBTOOLS_CONFIGURED
 
//Dependencies
#ifdef __cplusplus
  #if (_MSC_VER >= 1700) || (__cplusplus > 199711L)
    #include <cstdint>
  #else
    #include <pstdint.h> //Search on google
  #endif
#else
  #include <pstdint.h> 
#endif

//Determine system 
#if defined(_WIN32) || defined(__WIN32__)
  // Windows
  #define LIBTOOLS_WINDOWS
#elif defined(linux) || defined(__linux)
  // Linux
  #define LIBTOOLS_LINUX
#elif defined(__APPLE__) || defined(MACOSX) || defined(macintosh) || defined(Macintosh)
  // MacOS
  #error Apple wont get libtools
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
  // FreeBSD
  #define LIBTOOLS_FREEBSD
#else
  // Unsupported system
  #error This operating system is not supported by Libtools
#endif

#if defined(LIBTOOLS_DLL)
  #if defined(LIBTOOLS_WINDOWS)
    #define LIBTOOLS_DLL_EXPORT_PRIVATE
    #define LIBTOOLS_DLL_EXPORT_PUBLIC __declspec(dllexport)
    #define LIBTOOLS_DLL_IMPORT __declspec(dllimport)
    #ifdef _MSC_VER
        #pragma warning(disable : 4251)
    #endif
  #else 
      #error No fucking support for "linux dll" now
    #if __GNUC__ >= 4
      #define LIBTOOLS_DLL_EXPORT_PRIVATE __attribute__ ((__visibility__ ("hidden")))
      #define LIBTOOLS_DLL_EXPORT_PUBLIC __attribute__ ((__visibility__ ("default")))
      #define LIBTOOLS_DLL_IMPORT __attribute__ ((__visibility__ ("default")))

    #else
      #error Your gcc is fucking too old
    #endif
    
    #if defined(LIBTOOLS_DLL_IMPLEMENT)
      #define LIBTOOLS_PUBLIC LIBTOOLS_DLL_EXPORT_PUBLIC
      #define LIBTOOLS_PRIVATE LIBTOOLS_DLL_EXPORT_PRIVATE
    #else
      #define LIBTOOLS_PRIVATE
      #define LIBTOOLS_PUBLIC LIBTOOLS_DLL_IMPORT 
    #endif
    
  #endif
#else
  #define LIBTOOLS_PRIVATE
  #define LIBTOOLS_PUBLIC 
#endif

#ifdef __cplusplus
///C++ API

#ifdef BUILD_LIBTOOLS_SFML
#include <sfml/system.hpp>
#include <sfml/window.hpp>
#include <sfml/graphics.hpp>
#include <sfml/audio.hpp>
#endif


//string_t 
#if defined(SFML_STRING_HPP) 
typedef sf::String string_t;
#define string_t_find_failed(find_ret) (find_ret==sf::String::InvalidPos)
#define string_t_find_success(find_ret) (find_ret!=sf::String::InvalidPos)
#define string_t_size(str) str.getSize()
#define string_t_to_std(str) str.toAnsiString()
#define string_t_to_stdw(str) str.toWideString()
#define string_t_empty(str) str.isEmpty()
#define string_t_w_available
#else
#include <string>
typedef std::string string_t;
#define string_t_find_failed(find_ret) (find_ret==std::string::npos)
#define string_t_find_success(find_ret) (find_ret!=std::string::npos)
#define string_t_size(str) str.size()
#define string_t_to_std(str) str
#define string_t_empty(str) str.empty()
#error No support for C++11 version yet
#endif 

//mutex_t 
#if defined(SFML_MUTEX_HPP) 
typedef sf::Mutex mutex_t;
#else
#include <mutex>
typedef std::mutex mutex_t;
#endif 

//noncopyable_t
#if defined(SFML_NONCOPYABLE_HPP) 
typedef sf::NonCopyable noncopyable_c;
#else
class NonCopyable
{
  protected :
    NonCopyable() {}
  private :
    NonCopyable(const NonCopyable&);
    NonCopyable& operator =(const NonCopyable&);
};
typedef NonCopyable noncopyable_c;
#endif

#endif


#define LIBTOOLS_CONFIGURED
#endif