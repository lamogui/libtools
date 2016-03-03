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
    Music(MusicDecoder& decoder,FFT& fft):
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
    MusicDecoder* _decoder;
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
