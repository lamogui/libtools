
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
  * VISIBILITY     : PRIVATE
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : audiostream.cpp
  * AUTHOR       : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V1.2
  * DEPENDENCIES : audiostream.hpp
  * ROLE         : Permet de convertir les signaux en flux 
  *                tendu pour la carte son
  */

#include <libtools/core/audiostream.hpp>

//#define MULTIPLIER_16 (0.22*32768)
#define MULTIPLIER_16 0.99*32768

AudioStream::AudioStream(unsigned int buffer_length) :
_buffer(0),
_pwrite(0),
_pread(0), 
_length(buffer_length), 
_count(0)
{
  _buffer=(short*) malloc(_length*sizeof(short));
  _pread = _pwrite = _buffer;
  _end = _buffer + _length;
}

AudioStream::~AudioStream()
{
  free((void*)_buffer);
}

bool AudioStream::setBufferLength(unsigned int buffer_length)
{
  if (_count) return false;
  _length = buffer_length;
  _buffer=(short*) realloc((void*) _buffer,_length*sizeof(short));
  _pread = _pwrite = _buffer;
  _end = _buffer + _length;
  return true;
}

unsigned int AudioStream::read(short* buffer, unsigned int len)
{
  unsigned int readed=0;
  while (_count && _pread != _end && len > readed)
  {
    buffer[readed++] = *_pread++;
    _count--;
  }
  if (_pread == _end) _pread = _buffer;
  while (_count && len > readed)
  {
    buffer[readed++] = *_pread++;
    _count--;
  }
  return readed;
}

unsigned int AudioStream::write(const short* buffer, unsigned int len)
{
  unsigned int writed=0;
  while (_count < _length && _pwrite != _end && len > writed)
  {
    *_pwrite++ = buffer[writed++];
    _count++;
  }
  if (_pwrite == _end) _pwrite = _buffer;
  while (_count < _length && len > writed)
  {
    *_pwrite++ = buffer[writed++];
    _count++;
  }
  return writed;
}

bool AudioStream::writeSignal(const Signal& signal)
{
  if (getFreeSamplesCount() < Signal::size << 1) return false;
  unsigned int i=0;
  while (_pwrite != _end && Signal::size > i)
  {
    const short v = (short)(signal.samples[i++]*MULTIPLIER_16);
    *_pwrite++ = v;
    *_pwrite++ = v;
  }
  if (_pwrite == _end) _pwrite = _buffer;
  while (Signal::size > i)
  {
    const short v = (const short) (signal.samples[i++]*MULTIPLIER_16);
    *_pwrite++ = v;
    *_pwrite++ = v;
  }
  _count += Signal::size << 1;
  return true;
}

bool AudioStream::writeStereoSignal(const Signal& left,const Signal& right)
{
  if (getFreeSamplesCount() < Signal::size << 1) return false;
  unsigned int i=0;
  while (_pwrite != _end && Signal::size > i )
  {
    *_pwrite++ = (short)(left.samples[i]*MULTIPLIER_16);
    *_pwrite++ = (short)(right.samples[i++]*MULTIPLIER_16);
  }
  if (_pwrite == _end) _pwrite = _buffer;
  while (Signal::size > i)
  {
    *_pwrite++ = (short)(left.samples[i]*MULTIPLIER_16);
    *_pwrite++ = (short)(right.samples[i++]*MULTIPLIER_16);
  }
  _count += Signal::size << 1;
  return true;
}


/*
DirectOutputAudioStream::DirectOutputAudioStream()
{
}

DirectOutputAudioStream::~DirectOutputAudioStream()
{
}
*/