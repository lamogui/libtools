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
  * FILE         : tempo.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.1 olol
  * DEPENDENCIES : signal.hpp
  */
  
#ifndef LIBTOOLS_TEMPO_HPP
#define LIBTOOLS_TEMPO_HPP

#include <libtools/core/signal.hpp>
#include <vector>

class LIBTOOLS_PRIVATE TempoSensorA
{
  public:
    TempoSensorA(float pushSec,
                float minBpm=80.f, 
                float maxBpm=150.f,
                float integrateTime=2.f)
    {
      _bpms.reserve(8);
      _bpms.resize(8);
      reset(pushSec, minBpm,maxBpm,integrateTime);
    }
  
    void reset(float pushSec,
               float minBpm=80.f, 
               float maxBpm=150.f,
               float integrateTime=1.f);
    
    void push(float value);
    inline float average() { return _average; }
    inline float divergence() { return _divergence; }
    inline float bpm() {return _bpm;}//Return currente computed BPM in -1 if failed 
    inline float lastBeat() { return (float)_lastBeat/_pushSec;} //Return last beat distance from now in seconds
    inline float minBeatDist() { return _minBeatDist;}
    inline float maxBeatDist() { return _maxBeatDist;}
    inline bool valid() { return _bpm >= _minBpm && _bpm <= _maxBpm; }
    
  protected:
    void _compute_average();
    void _compute_divergence();
    
    std::vector<float> _values;
    std::vector<float>::iterator _iterator;
    std::vector<float> _bpms;
    std::vector<float>::iterator _bpmsIterator;
    float _lastValue;
    float _lastLongValue;
    float _pushSec;
    float _minBeatDist; // must be > 60/_integrateTime
    float _maxBeatDist;
    float _minBpm;
    float _maxBpm;
    float _bpm;
    float _integrateTime;
    float _maxOverLongValue;
    float _average;
    float _divergence;
    unsigned int _lastBeat;
    unsigned int _maxOverLongDist;
    bool _overLong;
};

#endif
