
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
  *  COPYDOWN LAMOGUI ALL RIGHTS RESERVED
  *
  * FILE         : bass.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 
  * DEPENDENCIES : config.h
  *                bass.hpp
  */

  
#include <libtools/public/config.h>
#include <libtools/drivers/bass.hpp>
#include <iostream>

#if defined(LIBTOOLS_WINDOWS)

#endif

#include <cstdio>

#if defined(LIBTOOLS_WINDOWS) || defined(BASS_H)

string_t BassDecoder::handleBassInitError()
{
  #if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)
  if (!BASS_ErrorGetCode)
    return "BASS not loaded";
  #endif
  switch (BASS_ErrorGetCode())
  {
    case BASS_ERROR_DX:	return "DirectX (or ALSA on Linux or OpenSL ES on Android) is not installed.";
    case BASS_ERROR_DEVICE:	return "device is invalid.";
    case BASS_ERROR_ALREADY:	return "The device has already been initialized. BASS_Free must be called before it can be initialized again.";
    case BASS_ERROR_DRIVER:	return "There is no available device driver. The device may already be in use.";
    case BASS_ERROR_FORMAT:	return "The specified format is not supported by the device. Try changing the freq and flags parameters.";
    case BASS_ERROR_MEM:	return "There is insufficient memory.";
    case BASS_ERROR_NO3D:	return "Could not initialize 3D support.";
    case BASS_ERROR_UNKNOWN:	return "Some other mystery problem!";
    default: return "";
  }
}

string_t BassDecoder::handleBassStreamCreateError()
{
  #if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)
  if (!BASS_ErrorGetCode)
    return "BASS not loaded";
  #endif
  switch (BASS_ErrorGetCode())
  {
    case BASS_ERROR_INIT:	return "BASS_Init has not been successfully called.";
    case BASS_ERROR_NOTAVAIL:	return "Only decoding channels (BASS_STREAM_DECODE) are allowed when using the 'no sound' device. The BASS_STREAM_AUTOFREE flag is also unavailable to decoding channels.";
    case BASS_ERROR_FORMAT:	return "The sample format is not supported by the device/drivers. If the stream is more than stereo or the BASS_SAMPLE_FLOAT flag is used, it could be that they are not supported.";
    case BASS_ERROR_SPEAKER:	return "The specified SPEAKER flags are invalid. The device/drivers do not support them, they are attempting to assign a stereo stream to a mono speaker or 3D functionality is enabled.";
    case BASS_ERROR_MEM:	return "There is insufficient memory.";
    case BASS_ERROR_NO3D:	return "Could not initialize 3D support.";
    case BASS_ERROR_UNKNOWN:	return "Some other mystery problem!";
    default: return "";
  }
}

string_t BassDecoder::handleBassChannelPlayError()
{
  #if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)
  if (!BASS_ErrorGetCode)
    return "BASS not loaded";
  #endif
  switch (BASS_ErrorGetCode())
  {
    case BASS_ERROR_HANDLE:	return "handle is not a valid channel.";
    case BASS_ERROR_START:	return "The output is paused/stopped, use BASS_Start to start it.";
    case BASS_ERROR_DECODE:	return "The channel is not playable; it is a 'decoding channel'.";
    case BASS_ERROR_BUFLOST:	return "Should not happen... check that a valid window handle was used with BASS_Init.";
    case BASS_ERROR_NOHW:	return "No hardware voices are available (HCHANNEL only). This only occurs if the sample was loaded/created with the BASS_SAMPLE_VAM flag and BASS_VAM_HARDWARE is set in the sample's VAM mode, and there are no hardware voices available to play it.";
    default: return "";
  }
}

BassDecoder::BassDecoder():
_music(0),
_mod(false),
_ended(true),
_bytesFrame(0),
_samplesForSignals(0)
{

}

BassDecoder::~BassDecoder()
{
  reset();

  if (
#if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)
       BASS_MusicFree && 
       BASS_StreamFree &&
#endif
       _music)
  {
	  if (_mod) BASS_MusicFree((HMUSIC) _music);
	  else BASS_StreamFree((HSTREAM) _music);
  }
  if (_samplesForSignals)
    free(_samplesForSignals);
}

void BassDecoder::_reset()
{
  if (
#if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)
       BASS_MusicFree && 
       BASS_StreamFree &&
#endif
      _music) {
    if (_mod) BASS_MusicFree((HMUSIC) _music);
	  else BASS_StreamFree((HSTREAM) _music);
  }
  _music = 0;
  _ended=true;
}

bool BassDecoder::_open(const string_t& filename)
{
  _mod=false;
  #if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)
  if (BASS_MusicLoad && 
      BASS_StreamCreateFile && 
      BASS_StreamCreateURL && 
      BASS_ChannelGetInfo)
  {
  #endif
  
  
  //std::wcerr << "Try loading: " << filename.toWideString() << std::endl;
  bass_unicode_string_t bass_unicode_str=string_t_to_bass_unicode_string_t(filename);
  void* unicode_filename=(void*)bass_unicode_str.c_str();
  
  if (_music=BASS_MusicLoad(FALSE, 
                            unicode_filename, 0,0,
                            BASS_MUSIC_PRESCAN | 
                            BASS_MUSIC_DECODE | 
                            BASS_SAMPLE_FLOAT |
                            BASS_UNICODE,
                            Signal::frequency))
	{
		_mod=true;
	}
	else if (_music=BASS_StreamCreateFile(FALSE, 
                                        unicode_filename, 0,0,
                                        BASS_STREAM_PRESCAN |
                                        BASS_STREAM_DECODE | 
                                        BASS_SAMPLE_FLOAT | 
                                        BASS_UNICODE));
	else if (
#if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)
           BASS_FLAC_StreamCreateFile && 
#endif
           (_music=BASS_FLAC_StreamCreateFile(FALSE, 
                                              unicode_filename, 0,0,
                                              BASS_STREAM_PRESCAN | 
                                              BASS_STREAM_DECODE | 
                                              BASS_SAMPLE_FLOAT | 
                                              BASS_UNICODE)));
	else if (_music=BASS_StreamCreateURL((const char *)unicode_filename,0,
                                        BASS_STREAM_DECODE | 
                                        BASS_SAMPLE_FLOAT | 
                                        BASS_UNICODE, 
                                        (DOWNLOADPROC*) 0,
                                        (void*)0));
	else if (
#if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)
           BASS_FLAC_StreamCreateURL && 
#endif
           (_music=BASS_FLAC_StreamCreateURL((const char *)unicode_filename,0,
                                                    BASS_STREAM_DECODE | 
                                                    BASS_SAMPLE_FLOAT | 
                                                    BASS_UNICODE, 
                                                    (DOWNLOADPROC*) 0,
                                                    (void*)0)));
  if (_music)
  {
    BASS_ChannelGetInfo((DWORD)_music,&_infos);
    _bytesFrame= sizeof(float)*Signal::size*_infos.chans;
    _samplesForSignals = (float*) realloc(_samplesForSignals,_bytesFrame);
    _sampleRate=_infos.freq;
    if (_sampleRate != Signal::frequency)
      std::cout << "warning : file rate (" << _sampleRate << "Hz) is different of " << Signal::frequency << "Hz" << std::endl;
    _ended=false;
    
    if (prepareDecode()) {
      decodeID3v1();
      decodeOGGTag();
    }
    
  }
  #if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)
  } else {
    std::cerr << "Error missing bass fonctions in DLL " << std::endl;
  }
  #endif
  return (_music);
}



bool BassDecoder::_load(const uint8_t* buffer, unsigned int size)
{
  _mod=false;
  #if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)
  if (BASS_MusicLoad &&
      BASS_StreamCreateFile &&
      BASS_ChannelGetInfo)
  {
  #endif

  if (_music=BASS_MusicLoad(TRUE,
                            (void*) buffer, 0,size,
                            BASS_MUSIC_PRESCAN |
                            BASS_MUSIC_DECODE |
                            BASS_SAMPLE_FLOAT,
                            Signal::frequency))
  {
    _mod=true;
  }
  else if (_music=BASS_StreamCreateFile(TRUE,
                                        (void*) buffer, 0, size,
                                        BASS_STREAM_PRESCAN |
                                        BASS_STREAM_DECODE |
                                        BASS_SAMPLE_FLOAT));
  if (_music)
  {
    BASS_ChannelGetInfo((DWORD)_music,&_infos);
    _bytesFrame= sizeof(float)*Signal::size*_infos.chans;
    _samplesForSignals = (float*) realloc(_samplesForSignals,_bytesFrame);
    _sampleRate=_infos.freq;
    if (_sampleRate != Signal::frequency)
      std::cout << "warning : file rate (" << _sampleRate << "Hz) is different of " << Signal::frequency << "Hz" << std::endl;
    _ended=false;

    if (prepareDecode()) {
      decodeID3v1();
      decodeOGGTag();
    }

  }
  #if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)
  } else {
    std::cerr << "Error missing bass fonctions in DLL " << std::endl;
  }
  #endif
  return (_music);
}

unsigned int BassDecoder::fetch(Signal& left, Signal& right){
  if (_music)
  {
    DWORD readed=0;
    #if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)
    if (!BASS_ChannelGetData || ! BASS_ErrorGetCode) {
      std::cerr << "Error missing BASS_ChannelGetData or BASS_ErrorGetCode functions" << std::endl;
      return 0;
    }
    #endif
    
    
    readed=BASS_ChannelGetData((DWORD)_music, (void*)_samplesForSignals, _bytesFrame);

    if(readed==-1 && BASS_ErrorGetCode() == BASS_ERROR_ENDED) {
      left.reset();
      right.reset();
      _ended=true;
      return 0;
    }

    if (readed < _bytesFrame)
    {
      for (unsigned int i=readed; i< _bytesFrame ;i++)
        ((char*)_samplesForSignals)[i]=0;
    }
    const unsigned int signal_size=Signal::size;
    if (_infos.chans>=2)
    {
      unsigned int k=0;
		  for (unsigned int i=0; i < signal_size;)
		  {
			  left.samples[i]=_samplesForSignals[k++];
			  right.samples[i++]=_samplesForSignals[k++];
        k+=_infos.chans-2;
		  }
    }
    else if (_infos.chans==1) //mono
    {
      unsigned int k=0;
		  for (unsigned int i=0; i < signal_size;)
		  {
			  left.samples[i]=_samplesForSignals[k];
			  right.samples[i++]=_samplesForSignals[k++];
		  }
    }
    return readed/sizeof(sample);
  }
  return 0;
}


void BassDecoder::rewind()
{
  #if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)
  if (!BASS_ChannelSetPosition)
  {
    std::cerr << "Missing BASS_ChannelSetPosition function" << std::endl;
    return;
  }
  #endif 

  if (_music)
  {
    if (_mod)
    {
      BASS_ChannelSetPosition((DWORD) _music,0,BASS_MUSIC_POSRESETEX|BASS_MUSIC_POSRESET);
    }
    else 
      BASS_ChannelSetPosition((DWORD) _music,0,BASS_POS_BYTE);
    _ended=false;
  }
}


unsigned int BassDecoder::frames()
{
  #if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)
  if (!BASS_ChannelGetLength || 
      !BASS_ChannelBytes2Seconds || 
      !BASS_ErrorGetCode)
  {
    std::cerr << "Missing BASS_ChannelGetLength, BASS_ChannelBytes2Seconds or BASS_ErrorGetCode function" << std::endl;
    return -1;
  }
  #endif 

  if (_music)
  {
    QWORD ret = BASS_ChannelGetLength((DWORD)_music,BASS_POS_BYTE);
    if (ret==-1) return -1;
    double seconds=BASS_ChannelBytes2Seconds((DWORD)_music,ret);
    unsigned int frames = (unsigned int)(seconds*(double)Signal::refreshRate+0.5);
    return frames;
  }
  return 0;
}

double BassDecoder::length() const {
  #if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)
  if (!BASS_ChannelGetLength || 
      !BASS_ChannelBytes2Seconds || 
      !BASS_ErrorGetCode)
  {
    std::cerr << "Missing BASS_ChannelGetLength, BASS_ChannelBytes2Seconds or BASS_ErrorGetCode function" << std::endl;
    return -1;
  }
  #endif 

  if (_music)
  {
    QWORD ret = BASS_ChannelGetLength((DWORD)_music,BASS_POS_BYTE);
    if (ret==-1) return -1;
    return BASS_ChannelBytes2Seconds((DWORD)_music,ret);
  }
  return -1;
}


bool BassDecoder::prepareDecode()
{
  #if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)
  if (!BASS_ChannelGetTags)
  {
    std::cerr << "Missing BASS_ChannelGetTags function" << std::endl;
    return false;
  }
  #endif 
  if (!_music) return false;
  return true;
}

bool BassDecoder::decodeID3v2(){
  const char* tags=BASS_ChannelGetTags(_music, BASS_TAG_ID3V2);
  if (tags) {
    unsigned int offset=0;
    offset+= 3 + 2 ; //go to flags
    bool have_extended = (tags[offset] & (1 << 6));
  }
  return false;
}

bool BassDecoder::decodeID3v1()
{
  const char* tags=BASS_ChannelGetTags(_music, BASS_TAG_ID3);
  if (tags) {
    TAG_ID3 *id3=(TAG_ID3*)tags;
    char buf[31]={0};
    sprintf(buf,"%.30s",id3->title);
    setName(string_t(buf));
    sprintf(buf,"%.30s",id3->artist);
    setAuthor(string_t(buf));
    //setGenre(id3->genre);
    return true;
  }
  return false;
}


#include <libtools/decoders/oggvorbisfiledecoder.hpp>

bool BassDecoder::decodeOGGTag(){
  char* tags=const_cast<char*>(BASS_ChannelGetTags(_music, BASS_TAG_OGG));
  string_t field;
  string_t value;
  if (tags) {
    while (*tags) {
      unsigned int size=strlen(tags);
      if (OggVorbisFileDecoder::splitComment(tags,size,field,value))
      {
#ifdef STRING_T_IS_SF_STRING
        sf::String::Iterator it;
        for (it=field.begin(); it !=field.end(); it++)
        {
          *it=toupper(*it);
        }
#endif
        if (field==string_t("TITLE"))
        {
          setName(value);
          return true;
        }
        else if (field==string_t("ARTIST"))
        {
          setAuthor(value);
          return true;
        }
      }
      tags+=strlen(tags)+1;
    }
  }
  return false;
}

#endif
