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
  * SECURITY LEVEL : 8
  * VISIBILITY     : PRIVATE
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED
  *
  * FILE         : oggvorbisfiledecoder.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.0
  */

#include <libtools/decoders/oggvorbisfiledecoder.hpp>
#include <assert.h>
#include <iostream>

OggVorbisFileDecoder::OggVorbisFileDecoder():
  _file(NULL),
  _data(NULL),
  _dataindex(0),
  _datasize(0),
  _current_bitstream(0),
  _opened(false),
  _ended(true)
{
  _resetCallbacks();
}

OggVorbisFileDecoder::~OggVorbisFileDecoder()
{
  _reset();
}

bool OggVorbisFileDecoder::_open(const string_t& filename)
{
#if defined(LIBTOOLS_WINDOWS) && defined(string_t_w_available)
  _file=_wfopen(string_t_to_stdw(filename).c_str(),L"rb");
#else
  _file=fopen(string_t_to_std(filename).c_str(),"rb");
#endif
   if (!_file)
   {
     std::cerr << "OggVorbisFileDecoder cannot open file: " << string_t_to_std(filename) << std::endl;
     return false;
   }

   _callbacks=OV_CALLBACKS_DEFAULT;
   if (ov_test_callbacks((void*)_file,&_vf,NULL,0,_callbacks) ||
       ov_test_open(&_vf) ||
       !_checkInfos())
   {
     reset();
     return false;
   }

   _parseComments();
   _opened=true;
   _ended=false;
   return true;
}

void OggVorbisFileDecoder::_reset()
{
  if (_opened)
  {
    ov_clear(&_vf); //this call fclose(_file) if needed
  }

  _file=NULL;
  _resetCallbacks();
  _opened=false;
  _ended=true;
  _data=NULL;
  _datasize=0;
  _dataindex=0;

}

void OggVorbisFileDecoder::_resetCallbacks()
{
  _callbacks.close_func=NULL;
  _callbacks.read_func=NULL;
  _callbacks.tell_func=NULL;
  _callbacks.seek_func=NULL;
}

void OggVorbisFileDecoder::_parseComments()
{
  vorbis_comment* comments=ov_comment(&_vf,-1);
  if (comments)
  {
    string_t field, value;
    for (int i=0; i < comments->comments; i++)
    {
      splitComment(comments->user_comments[i],
                   comments->comment_lengths[i],
                   field,
                   value);
      parseComment(field,value);
    }
  }
}

bool OggVorbisFileDecoder::_checkInfos()
{
  vorbis_info* infos=ov_info(&_vf,-1);
  if (infos)
  {
    if (infos->channels > 2)
    {
      std::cout << "OggVorbisFileDecoder warning: Too many channels (" <<  infos->channels << ")" << std::endl;
    }
    _sampleRate=infos->rate;
    if (_sampleRate != Signal::frequency)
    {
      std::cout << "OggVorbisFileDecoder warning: sample rate (" <<
                   _sampleRate << "Hz) different of Signal rate (" <<
                   Signal::frequency << ")" << std::endl;
    }
    _infos=*infos; //Make a copy
    return true;
  }
  std::cerr << "OggVorbisFileDecoder error: cannot get stream infos from file" << std::endl;
  return false;
}

unsigned int OggVorbisFileDecoder::fetch(Signal& outleft, Signal& outright)
{
  outleft.reset();
  outright.reset();
  if (_opened)
  {
    _checkInfos();


    unsigned int request_size=Signal::size;
    long total_readed=0;
    unsigned int i=0;
    while (request_size)
    {
      long readed=0;
      float** samplesForSignals;
      readed=ov_read_float(&_vf,&samplesForSignals,request_size,&_current_bitstream);

      if(readed <=0 ) { //EOF or ERROR
        _ended=true;
        return total_readed;
      }
      total_readed+=readed;
      request_size-=readed;

      float* leftSignal=samplesForSignals[0];
      float* rightSignal=samplesForSignals[1];
      if (_infos.channels==1)
      {
        rightSignal=leftSignal;
      }

      unsigned int k=0;
      for (; k < readed;)
      {
        outleft.samples[i]=leftSignal[k];
        outright.samples[i++]=rightSignal[k++];
      }
    }

    return total_readed;
  }
  _ended=true;
  return 0;
}

double OggVorbisFileDecoder::length() const
{
  if (!_opened)
  {
    return 0;
  }
  double t=ov_time_total(&_vf,-1);
  return t;
}

void OggVorbisFileDecoder::rewind()
{
  if (!_opened) return;
  if (ov_pcm_seek(&_vf,0))
  {
    std::cerr << "OggVorbisFileDecoder error: cannot seek" << std::endl;
  } else
  {
    _ended=false;
  }
}

bool OggVorbisFileDecoder::_load(const uint8_t* data, unsigned int size)
{
  _data=data;
  _datasize=size;
  if (!_data)
  {
#ifndef NDEBUG
    std::cerr << "OggVorbisFileDecoder error: unable to load from null ptr" << std::endl;
#endif
    return false;
  }
  _opened=true;

  _callbacks.close_func=NULL;
  _callbacks.read_func=&OggVorbisFileDecoder::_read_func;
  _callbacks.seek_func=&OggVorbisFileDecoder::_seek_func;
  _callbacks.tell_func=&OggVorbisFileDecoder::_tell_func;
  int ovtc_result=ov_test_callbacks((void*)this,&_vf,NULL,0,_callbacks);
  int ovto_result=ov_test_open(&_vf);
  if (ovtc_result ||
      ovto_result ||
      !_checkInfos())
  {
    reset();
#ifndef NDEBUG
    std::cerr << "OggVorbisFileDecoder error: ov_test_callbacks " << ovtc_result << " ov_test_open " << ovto_result << std::endl;
#endif
    return false;
  }

  _parseComments();
  _ended=false;
  return true;
}

size_t OggVorbisFileDecoder::_read_func(void* ptr, size_t size, size_t nmeb, void* datasource)
{
  OggVorbisFileDecoder* decoder=(OggVorbisFileDecoder*) datasource;
  size_t cpy_size=size*nmeb;
  if ( decoder->_dataindex >= decoder->_datasize)
  {
    return 0;
  }
  if (cpy_size > decoder->_datasize - decoder->_dataindex)
  {
    cpy_size=decoder->_datasize - decoder->_dataindex;
  }
  memcpy(ptr,(void*)&(decoder->_data[decoder->_dataindex]),cpy_size);

  decoder->_dataindex+=cpy_size;
  return cpy_size;
}

int OggVorbisFileDecoder::_seek_func(void *datasource, ogg_int64_t offset, int whence)
{
  OggVorbisFileDecoder* decoder=(OggVorbisFileDecoder*) datasource;
  switch (whence)
  {
    case SEEK_SET:
      if (offset >= 0 && offset < decoder->_datasize)
      {
        decoder->_dataindex=offset;
        return 0;
      }
      break;
    case SEEK_CUR:
      if ((ogg_int64_t)decoder->_dataindex + offset <= (ogg_int64_t)decoder->_datasize &&
          (ogg_int64_t)decoder->_dataindex + offset >= (ogg_int64_t)0)
      {
        decoder->_dataindex+=offset;
        return 0;
      }
      break;
    case SEEK_END:
      if ((ogg_int64_t)decoder->_datasize + offset <= (ogg_int64_t)decoder->_datasize &&
          (ogg_int64_t)decoder->_datasize + offset >= (ogg_int64_t)0)
      {
        decoder->_dataindex=decoder->_datasize + offset;
        return 0;
      }
      break;
    default:
      break;
  }
  return -1;
}

long OggVorbisFileDecoder::_tell_func(void *datasource)
{
  OggVorbisFileDecoder* decoder=(OggVorbisFileDecoder*) datasource;
  return decoder->_dataindex;
}
