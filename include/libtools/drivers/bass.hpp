
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
    
    
    virtual unsigned int fetch(Signal& outleft, Signal& outright);

    static string_t handleBassInitError();
    static string_t handleBassStreamCreateError();
    static string_t handleBassChannelPlayError();
    
  protected:
    virtual bool _open(const string_t& filename);
    virtual bool _load(const uint8_t* buffer, unsigned int size);
    virtual void _reset();

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
