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