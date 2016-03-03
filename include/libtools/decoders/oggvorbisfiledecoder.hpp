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
  * FILE         : oggvorbisfiledecoder.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.0
  */

#ifndef LIBTOOLS_OGGVORBISFILEDECODER_HPP
#define LIBTOOLS_OGGVORBISFILEDECODER_HPP

#include <libtools/public/config.h>
#include <libtools/core/decoder.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

class OggVorbisFileDecoder : public MusicDecoder
{
  public:
    OggVorbisFileDecoder();
    virtual ~OggVorbisFileDecoder();

    virtual unsigned int fetch(Signal& outleft, Signal& outright);

    inline virtual bool ended() const
    {
      return _ended;
    }

    virtual void rewind();
    virtual double length() const;

  protected:
    virtual bool _open(const string_t& filename);
    virtual bool _load(const uint8_t *data, unsigned int size);

  private:
    bool _checkInfos();
    void _parseComments();
    void _reset();
    void _resetCallbacks();


    static size_t _read_func(void* ptr, size_t size, size_t nmeb, void* datasource);
    static int _seek_func(void *datasource, ogg_int64_t offset, int whence);
    static long _tell_func(void *datasource);

    ov_callbacks _callbacks;
    mutable OggVorbis_File _vf;
    vorbis_info _infos;
    FILE* _file;
    const uint8_t* _data;
    unsigned int _dataindex;
    unsigned int _datasize;

    int _current_bitstream;
    bool _opened;
    bool _ended;

};

#endif // LIBTOOLS_OGGVORBISFILEDECODER_HPP
