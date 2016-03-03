
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
  * FILE         : decoder.hpp
  * AUTHOR       : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.1
  * DEPENDENCIES : signal.hpp
  *                config.h
  *                production.hpp
  */



#ifndef LIBTOOLS_DECODER_HPP
#define LIBTOOLS_DECODER_HPP

#include <libtools/public/config.h>
#include <libtools/core/signal.hpp>
#include <libtools/core/production.hpp>

class MusicDecoders;

class LIBTOOLS_PRIVATE MusicDecoder : public MusicProduction
{
    friend class MusicDecoders;

  public:
    MusicDecoder(const string_t& decoder_name) :
      _sampleRate(0),
      _decodername(decoder_name)
    {}
    virtual ~MusicDecoder() {}

    bool open(const string_t& filename);
    bool load(const uint8_t* buffer, unsigned int data);
    inline string_t name() { return _name; }
    inline string_t author() { return _artist; }
    inline string_t filename() { return _filename; }
    inline Production::GenreId genre() {return _genre;}

    bool parseComment(const string_t& field, const string_t &value); //Ogg vorbis comments specs

    static bool splitComment( const char *comment, unsigned int size,
                              string_t& field, string_t& value);


    virtual unsigned int fetch(Signal& outleft, Signal& outright) = 0;

    virtual bool ended() const = 0; //can fetch ?
    virtual void rewind() = 0; //restart the production
    virtual double length() const = 0; //Length of the music in seconds

    unsigned int sampleRate() const { return _sampleRate; } //Sample rate

    void reset(); //Reset the decoder

    inline string_t decoder() const { return _decodername; }

  protected:

    virtual bool _open(const string_t& filename)=0;
    virtual bool _load(const uint8_t* buffer, unsigned int data)=0;
    virtual void _reset() = 0;

    void setName(const string_t& name);
    void setAuthor(const string_t& artist);
    void setGenre(const string_t& genre);

    unsigned int _sampleRate;

  private:

    void _infosreset();

    string_t _filename;
    string_t _name;
    string_t _artist;
    string_t _decodername;
    Production::GenreId _genre;

};

/**
  Music decoder can aggregate multiples decoders
  When you try to open a file, MusicDecoders will try to open it using
  the Decoders you passed it with the repect of the order.
  If it find a decoder that open correctly the file it will continue to decode
  with this decoder else it will return false.

  When MusicDecoders is deleted it will delete all the decoders.
*/
class LIBTOOLS_PRIVATE MusicDecoders : public MusicDecoder
{
  public:
    MusicDecoders();
    virtual ~MusicDecoders();

    bool openWithDecoder(const string_t& filename, MusicDecoder& decoder);
    virtual unsigned int fetch(Signal& outleft, Signal& outright);

    virtual bool ended() const;
    virtual void rewind();
    virtual double length() const;

    void append(MusicDecoder* decoder);

  protected:
    virtual bool _open(const string_t& filename);
    virtual bool _load(const uint8_t* buffer, unsigned int size);
    virtual void _reset();

  private:

    void _transmitInfos();

    std::vector<MusicDecoder*> _decoders;
    MusicDecoder* _currentDecoder;


};

#endif
