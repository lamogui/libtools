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
  * VISIBILITY     : PROTECTED
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED
  *
  * FILE         : flacdecoder.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.0
  */

#ifndef LIBTOOLS_FLACDECODER_HPP
#define LIBTOOLS_FLACDECODER_HPP

#include <libtools/public/config.h>
#include <libtools/core/decoder.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <FLAC/stream_decoder.h>

class FlacDecoder : public MusicDecoder
{
  public:
    FlacDecoder();
    virtual ~FlacDecoder();

    virtual unsigned int fetch(Signal& outleft, Signal& outright);

    inline virtual bool ended() const
    {
      return _ended;
    }

    virtual void rewind();
    virtual double length() const;

    bool load(const uint8_t *data, unsigned int size);


  protected:
    virtual bool _open(const string_t& filename);
    virtual bool _load(const uint8_t* buffer, unsigned int size);
    virtual void _reset();

  private:


    static FLAC__StreamDecoderWriteStatus _write_callback(
        const FLAC__StreamDecoder* decoder,
        const FLAC__Frame* frame,
        const FLAC__int32* const buffer[],
        void* client_data);

    static void _metadata_callback(const FLAC__StreamDecoder* decoder,
                                   const FLAC__StreamMetadata* metadata,
                                   void* client_data);
    static void _error_callback(const FLAC__StreamDecoder* decoder,
                                FLAC__StreamDecoderErrorStatus status,
                                void* client_data);


    //Flac struct
    FLAC__StreamDecoder* _streamdecoder;

    //File
    FILE* _file;

    //Buffering from char*
    const uint8_t* _data;
    unsigned int _dataindex;
    unsigned int _datasize;

    //Playing state
    std::vector<sample> _bufferl;
    std::vector<sample> _bufferr;
    uint64_t _bufferpos;
    bool _opened;
    bool _ended;
    bool _error;

};

#endif // LIBTOOLS_FLACDECODER_HPP
