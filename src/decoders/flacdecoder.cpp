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
  * FILE         : flacdecoder.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.0
  */

#include <libtools/decoders/flacdecoder.hpp>
#include <assert.h>
#include <iostream>
#include <libtools/decoders/oggvorbisfiledecoder.hpp>

FlacDecoder::FlacDecoder():
  _streamdecoder(NULL),
  _file(NULL),
  _data(NULL),
  _dataindex(0),
  _datasize(0),
  _bufferpos(0),
  _opened(false),
  _ended(true),
  _error(false)
{
  _streamdecoder=FLAC__stream_decoder_new();
  assert(_streamdecoder);
  FLAC__stream_decoder_set_metadata_respond(_streamdecoder,FLAC__METADATA_TYPE_STREAMINFO);
  FLAC__stream_decoder_set_metadata_respond(_streamdecoder,FLAC__METADATA_TYPE_VORBIS_COMMENT);
}

FlacDecoder::~FlacDecoder()
{
  if (_opened)
  {
    FLAC__stream_decoder_finish(_streamdecoder);
    if (_file)
    {
      fclose(_file);
    }
  }
  FLAC__stream_decoder_delete(_streamdecoder);
}

bool FlacDecoder::_open(const string_t& filename)
{
#if defined(LIBTOOLS_WINDOWS) && defined(string_t_w_available)
  _file=_wfopen(string_t_to_stdw(filename).c_str(),L"rb");
#else
  _file=fopen(string_t_to_std(filename).c_str(),"rb");
#endif
   if (!_file)
   {
     std::cerr << "FlacDecoder cannot open file: " << string_t_to_std(filename) << std::endl;
     return false;
   }
   _error=false;
   FLAC__StreamDecoderInitStatus init_status=
   FLAC__stream_decoder_init_FILE(_streamdecoder,_file,
                                  &FlacDecoder::_write_callback,
                                  &FlacDecoder::_metadata_callback,
                                  &FlacDecoder::_error_callback,
                                  (void*) this);

   assert(init_status==FLAC__STREAM_DECODER_INIT_STATUS_OK);
   if (!FLAC__stream_decoder_process_until_end_of_metadata(_streamdecoder) || _error)
   {
     _error=false;
     init_status=
     FLAC__stream_decoder_init_ogg_FILE(_streamdecoder,_file,
                                        &FlacDecoder::_write_callback,
                                        &FlacDecoder::_metadata_callback,
                                        &FlacDecoder::_error_callback,
                                        (void*) this);
     assert(init_status==FLAC__STREAM_DECODER_INIT_STATUS_OK);
     if (!FLAC__stream_decoder_process_until_end_of_metadata(_streamdecoder)||_error)
     {
       std::cerr << "FlacDecoder file: " << string_t_to_std(filename) << " is not a FLAC or OGG FLAC file" << std::endl;
       FLAC__stream_decoder_reset(_streamdecoder);
       fclose(_file);
       return false;
     }
   }

   uint64_t music_len=FLAC__stream_decoder_get_total_samples(_streamdecoder);
   _bufferl.reserve(music_len);
   _bufferr.reserve(music_len);
   _opened=true;
   _ended=false;
   return true;
}

void FlacDecoder::_reset()
{
  if (_opened)
  {
    FLAC__stream_decoder_finish(_streamdecoder);
    if (_file)
    {
      fclose(_file);
    }
  }
  _file=NULL;
  _opened=false;
  _ended=true;
  _error=false;
  _data=NULL;
  _datasize=0;
  _dataindex=0;
  _sampleRate=0;
  _bufferpos=0;
  _bufferl.clear();
  _bufferr.clear();
  FLAC__stream_decoder_set_metadata_respond(_streamdecoder,FLAC__METADATA_TYPE_STREAMINFO);
  FLAC__stream_decoder_set_metadata_respond(_streamdecoder,FLAC__METADATA_TYPE_VORBIS_COMMENT);
}


unsigned int FlacDecoder::fetch(Signal& outleft, Signal& outright)
{
  outleft.reset();
  outright.reset();
  if (_opened)
  {
    unsigned int request_size=Signal::size;
    while (request_size > _bufferl.size() - _bufferpos)
    {
      if (!FLAC__stream_decoder_process_single(_streamdecoder) ||
          (FLAC__stream_decoder_get_state(_streamdecoder) != FLAC__STREAM_DECODER_READ_FRAME &&
           FLAC__stream_decoder_get_state(_streamdecoder) != FLAC__STREAM_DECODER_SEARCH_FOR_FRAME_SYNC &&
           FLAC__stream_decoder_get_state(_streamdecoder) != FLAC__STREAM_DECODER_SEARCH_FOR_METADATA &&
           FLAC__stream_decoder_get_state(_streamdecoder) != FLAC__STREAM_DECODER_READ_METADATA ))
      {
        break;
      }
    }

    const unsigned int to_read=request_size >_bufferl.size() - _bufferpos ? _bufferl.size() - _bufferpos : request_size;
    for (unsigned int i=0; i < to_read; i++)
    {
      outleft.samples[i]=_bufferl.at(_bufferpos);
      outright.samples[i]=_bufferr.at(_bufferpos++);
    }
    if (to_read == 0)
    {
      _ended=true;
    }
    return to_read;
  }
  _ended=true;
  return 0;
}

double FlacDecoder::length() const
{
  if (!_opened)
  {
    return static_cast<double>(FLAC__stream_decoder_get_total_samples(_streamdecoder))/static_cast<double>(_sampleRate);
  }
  return 0.;
}

void FlacDecoder::rewind()
{
  if (!_opened) return;
  _bufferpos=0;
  _ended=false;
}

bool FlacDecoder::_load(const uint8_t* data, unsigned int size)
{
  _data=data;
  _datasize=size;
  if (!_data)
  {
#ifndef NDEBUG
    std::cerr << "FlacDecoder error: unable to load from null ptr" << std::endl;
#endif
    return false;
  }
  std::cerr << "FlacDecoder: load from memory not implemented yet" << std::endl;
  //_opened=true;

  return false;
}

FLAC__StreamDecoderWriteStatus FlacDecoder::_write_callback(
    const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame,
    const FLAC__int32 * const buffer[], void *client_data)
{
  assert(client_data);
  FlacDecoder* flac_decoder=(FlacDecoder*) client_data;
  flac_decoder->_sampleRate=frame->header.sample_rate;
  int64_t int_diviser=1<<(frame->header.bits_per_sample);
  const sample diviser=static_cast<float>(int_diviser);
  const unsigned int blocksize=frame->header.blocksize;


  flac_decoder->_bufferl.reserve(flac_decoder->_bufferl.size()+blocksize);
  flac_decoder->_bufferr.reserve(flac_decoder->_bufferr.size()+blocksize);

  if (frame->header.channels==1)
  {
    for (unsigned int i=0; i < blocksize; i++)
    {
      flac_decoder->_bufferl.push_back(static_cast<sample>(buffer[0][i])/diviser);
      flac_decoder->_bufferr.push_back(static_cast<sample>(buffer[0][i])/diviser);
    }
  }
  else if (frame->header.channels > 1)
  {
    for (unsigned int i=0; i < blocksize; i++)
    {
      flac_decoder->_bufferl.push_back(static_cast<sample>(buffer[0][i])/diviser);
      flac_decoder->_bufferr.push_back(static_cast<sample>(buffer[1][i])/diviser);
    }
  }

  return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;

}


void FlacDecoder::_metadata_callback(const FLAC__StreamDecoder* decoder,
                                     const FLAC__StreamMetadata* metadata,
                                     void* client_data)
{
  assert(client_data);
  FlacDecoder* flac_decoder=(FlacDecoder*) client_data;
  switch (metadata->type)
  {
    case FLAC__METADATA_TYPE_STREAMINFO:
      flac_decoder->_sampleRate=metadata->data.stream_info.sample_rate;
      if (metadata->data.stream_info.channels > 2)
      {
          std::cout << "FlacDecoder warning: Too many channels (" << metadata->data.stream_info.channels << " availables)" << std::endl;
      }
      break;
    case FLAC__METADATA_TYPE_VORBIS_COMMENT:
      for (unsigned int i=0; i < metadata->data.vorbis_comment.num_comments; i++)
      {
        string_t field;
        string_t value;
        MusicDecoder::splitComment((const char*)metadata->data.vorbis_comment.comments[i].entry,
                                   metadata->data.vorbis_comment.comments[i].length,
                                   field,value);
        flac_decoder->parseComment(field,value);
     }
     break;
    default:
      break;
  }
}


void FlacDecoder::_error_callback(const FLAC__StreamDecoder* decoder,
                                  FLAC__StreamDecoderErrorStatus status,
                                  void* client_data)
{
 //std::cerr << "FlacDecoder error: Error callback call with status code " << status << std::endl;
  ((FlacDecoder*)client_data)->_error=true;
}
