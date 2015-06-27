
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

class LIBTOOLS_PRIVATE MusicDecoder : public MusicProduction
{
  public:
    MusicDecoder() : _sampleRate(0) {}
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
    Production::GenreId _genre;
};

#endif
