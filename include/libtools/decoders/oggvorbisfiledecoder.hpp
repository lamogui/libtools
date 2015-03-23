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
  * THIS FILE IS PART OF
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

class OggVorbisFileDecoder : public Decoder
{
  public:
    OggVorbisFileDecoder();
    virtual ~OggVorbisFileDecoder();

    virtual unsigned int fetch(Signal& outleft, Signal& outright);

    inline virtual bool ended()
    {
      return _ended;
    }

    virtual void rewind();
    virtual double length();

    static bool splitComment(
        const char *comment,
        unsigned int size,
        string_t& field,
        string_t& value);

    bool parseComment(const string_t& field, const string_t &value);

    bool load(const uint8_t *data, unsigned int size);

  protected:
    virtual bool _open(const string_t& filename);

  private:
    bool _checkInfos();
    void _parseComments();
    void _reset();
    void _resetCallbacks();


    static size_t _read_func(void* ptr, size_t size, size_t nmeb, void* datasource);
    static int _seek_func(void *datasource, ogg_int64_t offset, int whence);
    static long _tell_func(void *datasource);

    std::vector<sample> _buffer;
    ov_callbacks _callbacks;
    OggVorbis_File _vf;
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
