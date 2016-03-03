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
