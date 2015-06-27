﻿
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
  * FILE         : bass.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 
  * DEPENDENCIES : config.h
  *                decoder.hpp
  *                audiodriver.hpp
  *                audiostream.hpp
  */
  
  
#ifndef LIBTOOLS_BASS_HPP
#define LIBTOOLS_BASS_HPP

#include <libtools/public/config.h>

#if defined(LIBTOOLS_WINDOWS)
#include <windows.h>
#include <libtools/drivers/bassproc.h>
#include <libtools/core/audiodriver.hpp>
#include <libtools/core/audiostream.hpp>

class LIBTOOLS_PRIVATE BassAsioDriver : public AudioDriver
{
  public:
    BassAsioDriver();
    virtual ~BassAsioDriver();
    
    bool init(unsigned int rate);
    void free();
    bool start(AudioStream* stream);
    void stop();
    
    static string_t handleBassAsioInitError();
    static string_t handleBassAsioStartError();
    
  protected:
    bool _initialized;
    bool _running;
    unsigned _right; //right channel
    unsigned _left;  //left channel
    
    static DWORD CALLBACK _AsioProc(BOOL input, 
                                    DWORD channel, 
                                    void *buffer, 
                                    DWORD length, 
                                    void *user);
};

#endif

#if defined(LIBTOOLS_WINDOWS) || defined(BASS_H)
#include <libtools/core/decoder.hpp>

class LIBTOOLS_PRIVATE BassDecoder : public MusicDecoder
{
  public:  
    BassDecoder();
    virtual ~BassDecoder();

    inline virtual bool ended() const {return _ended;}
    virtual void rewind();
    unsigned int frames();
    virtual double length() const;
    
    void reset();
    
    virtual unsigned int fetch(Signal& outleft, Signal& outright);

    static string_t handleBassInitError();
    static string_t handleBassStreamCreateError();
    static string_t handleBassChannelPlayError();
    
  protected:
    virtual bool _open(const string_t& filename);
    virtual bool _load(const uint8_t* buffer, unsigned int size);

    bool prepareDecode();
    bool decodeID3v2();
    bool decodeID3v1();
    bool decodeOGGTag();
  
  
    DWORD _music;
    bool _mod;
    bool _ended;
    BASS_CHANNELINFO _infos;
    unsigned _bytesFrame;
    float* _samplesForSignals;

};

#endif

//String spec
typedef std::basic_string<uint16_t> bass_unicode_string_t;
#if defined(LIBTOOLS_WINDOWS)
  #if defined(SFML_STRING_HPP)
    #define string_t_to_bass_unicode_string_t(str) (str.toUtf16())
  #elif defined(string_t_w_available)
    #define string_t_to_bass_unicode_string_t(str) (static_cast<bass_unicode_string_t>(string_t_to_stdw(str)))
  #else 
    #define string_t_to_bass_unicode_string_t(str) (static_cast<bass_unicode_string_t>(str))
  #endif
#elif defined(LIBTOOLS_LINUX)
  #if defined(SFML_STRING_HPP)
    #define string_t_to_bass_unicode_string_t(str) (str.toUtf16())
  #else 
    #define string_t_to_bass_unicode_string_t(str) (static_cast<bass_unicode_string_t>(str))
  #endif
#endif //String spec

#endif //Head
