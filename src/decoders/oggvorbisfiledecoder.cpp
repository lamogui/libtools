﻿/*

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
  * THIS FILE IS PART OF
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
  _current_bitstream(0),
  _opened(false),
  _ended(true)
{
  _buffer.resize(1024*2);
  _resetCallbacks();
}

OggVorbisFileDecoder::~OggVorbisFileDecoder()
{
  _resetCallbacks();
}

bool OggVorbisFileDecoder::_open(const string_t& filename)
{
   _file=fopen(string_t_to_std(filename).c_str(),"rb");
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
     _reset();
     return false;
   }

   _parseComments();
   _opened=true;
   return true;
}

void OggVorbisFileDecoder::_reset()
{
  if (_opened)
  {
    ov_clear(&_vf);
  }
  _file=NULL;
  _resetCallbacks();
  _opened=false;
  _ended=true;

}

void OggVorbisFileDecoder::_resetCallbacks()
{
  _callbacks.close_func=NULL;
  _callbacks.read_func=NULL;
  _callbacks.tell_func=NULL;
  _callbacks.seek_func=NULL;
}

bool OggVorbisFileDecoder::splitComment(
    const char* comment,
    unsigned int size,
    string_t& field,
    string_t& value)
{
#ifdef STRING_T_IS_SF_STRING
  sf::String str=sf::String::fromUtf8(comment, &(comment[size]));
  size_t split=str.find(sf::String("="));
  if (split==sf::String::InvalidPos){
    return false;
  } else {
    field=str.substring(0,split);
    value=str.substring(split+1);
    sf::String::Iterator it;
    for (it=field.begin(); it !=field.end(); it++)
    {
      *it=toupper(*it);
    }

    //std::cout << "field " << field.toAnsiString() << "=" << value.toAnsiString() << std::endl;
    return true;
  }
#else
  return false;
#endif
}

bool OggVorbisFileDecoder::parseComment(
    const string_t& field,
    const string_t& value)
{
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
  return false;
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
    if (infos->rate != Signal::frequency)
    {
      std::cout << "OggVorbisFileDecoder warning: sample rate (" <<
                   infos->rate << "Hz) different of Signal rate (" <<
                   Signal::frequency << ")" << std::endl;
    }
    _infos=*infos; //Make a copy
    return true;
  }
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
        return 0;
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

double OggVorbisFileDecoder::length()
{
  if (!_opened)
  {
    return 0;
  }
  return ov_time_total(&_vf,-1);
}

void OggVorbisFileDecoder::rewind()
{
  if (!_opened) return;
  if (ov_pcm_seek(&_vf,0))
  {
    std::cerr << "OggVorbisFileDecoder error: cannot seek" << std::endl;
  }
}
