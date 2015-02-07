/*

THIS FILE IS SUBJECT TO STRICT RULES OF BDE NE666 COPYDOWN. 
NOBODY IS PERMITTED TO COPY AND DISTRIBUTE VERBATIM OR MODIFIED COPIES OF
THIS LICENSE HEADER. A SECURITY LEVEL IS ASSIGNED TO THE FILE AND A VISIBILITY.
THIS FILE MUST NOT BE COPIED OR REDISTRIBUTED IF IT'S VISIBILITY IS NOT PUBLIC. 
VISIBILITY HAS 3 POSSIBLE STATES:
- PRIVATE: LIMITED TO PROJECTS WITH THE SAME SECURITY LEVEL WITHIN THE BDE NE666. (DEFAULT VISIBILITY)
- PROTECTED: LIMITED TO PROJECTS USING THE SAME SECURITY RULES WITHIN THE BDE NE666. 
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
  * FILE         : music.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.7 olol
  * DEPENDENCIES : config.h
  *                fft.hpp
  *                decoder.hpp
  *                tempo.hpp
  */



#ifndef LIBTOOLS_MUSIC_HPP
#define LIBTOOLS_MUSIC_HPP

#include <libtools/public/config.h>
#include <libtools/core/decoder.hpp>
#include <libtools/core/fft.hpp>
#include <libtools/core/tempo.hpp>
#include <vector>

class LIBTOOLS_PRIVATE Music 
{
  public:
    Music(Decoder& decoder,FFT& fft):
    _decoder(&decoder),
    _fft(&fft),
    _energyTempo((float)Signal::frequency/(float)Signal::size),
    _bassTempo((float)Signal::frequency/(float)Signal::size)
    {
      _reset();
    }
    
    virtual ~Music() {
    }

    bool open(const string_t& filename) { _reset(); return _decoder->open(filename); }
    bool ended() { return _decoder->ended(); }
    void rewind() { _reset(); _decoder->rewind(); }
    unsigned int nextFrame(Signal& left, Signal& right);
    
    inline string_t name() { return _decoder->name(); }
    inline string_t author() { return _decoder->author(); }
    inline Production::GenreId genre() { return _decoder->genre(); }
    inline float bpm() {return _bpm;}//Return BPM in real time -1 if failed 
    inline float energy() { return _energy; } //return real-time Energy
    inline float bass() { return _bass; }
    inline float bassTime() { return _bassTime; }
    inline float boostTime() { return _boostTime; }
    
  protected:
    void _reset();
    Decoder* _decoder;
    FFT* _fft;
    
    float _boostTime;
    float _step;
    float _energy;
    float _bassTime;
    float _bass;
    float _signalSec;   
    float _bpm;
    TempoSensorA _energyTempo;
    TempoSensorA _bassTempo;
};

#endif
