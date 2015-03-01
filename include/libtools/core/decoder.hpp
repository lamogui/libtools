
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

class LIBTOOLS_PRIVATE Decoder : public MusicProduction
{
  public:
    Decoder() {}
    virtual ~Decoder() {}

    bool open(const string_t& filename);
    inline string_t name() { return _name; }
    inline string_t author() { return _artist; }
    inline string_t filename() { return _filename; }
    inline Production::GenreId genre() {return _genre;}
    
    virtual unsigned int fetch(Signal& outleft, Signal& outright) = 0;
  
    virtual bool ended() = 0; //can fetch ?
    virtual void rewind() = 0; //restart the production
    virtual double length() = 0; //Length of the music in seconds
  
  protected:
  
    virtual bool _open(const string_t& filename)=0;
    
    void setName(const string_t& name);
    void setAuthor(const string_t& artist);
    void setGenre(const string_t& genre);
    
  private:
  
    void _namereset();
  
    string_t _filename;
    string_t _name;
    string_t _artist;
    Production::GenreId _genre;
};

#endif