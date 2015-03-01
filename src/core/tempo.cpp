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
  * VISIBILITY     : PRIVATE
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : tempo.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.1 olol
  * DEPENDENCIES : tempo.hpp
  */

#include <libtools/core/tempo.hpp>

void TempoSensorA::reset(float pushSec,
                        float minBpm, 
                        float maxBpm,
                        float integrateTime)  { 
  _pushSec=pushSec;
  const unsigned int vector_size = (unsigned int)(integrateTime*_pushSec + 0.5f);
  _values.reserve(vector_size);
  _values.resize(vector_size);
  _iterator=_values.begin();
  _bpmsIterator=_bpms.begin();
  for (std::vector<float>::iterator i=_values.begin();
       i != _values.end(); i++) {
    *i=0.f;
  }
  for (std::vector<float>::iterator i=_bpms.begin();
       i != _bpms.end(); i++) {
    *i=-1.f;
  }
  
  _lastValue=0.f;
  _lastLongValue=0.f;
  _lastBeat=0;
  _minBeatDist=(float)_pushSec/(float)(maxBpm/(60.f)); 
  _maxBeatDist=(float)_pushSec/(float)(minBpm/(60.f)); 
  _minBpm=minBpm;
  _maxBpm=maxBpm;
  _bpm=-1.f;
  _integrateTime=integrateTime;
  _maxOverLongValue=0.f;
  _overLong=false;
  _maxOverLongDist=0;
  _average=-1.f;
  _divergence=-1.f;
}

void TempoSensorA::push(float value){
  *_iterator=value;
  _iterator++;
  if (_iterator == _values.end()) _iterator=_values.begin();
  _lastLongValue=0.f;
  for (std::vector<float>::iterator i=_values.begin();
       i != _values.end(); i++)
  {
    _lastLongValue+=*i;
  }
  _lastLongValue/=(float)_values.size();
  _lastBeat++;
  
  if (_overLong) {
    _maxOverLongDist++;
    if (value > _lastLongValue) {
      if (value > _maxOverLongValue) {
        _maxOverLongValue=value;
        _maxOverLongDist=0;
      }
    } else {
      const float dist=(float)(_lastBeat - _maxOverLongDist);
      if (dist > _minBeatDist) { //avoid "double beat"
        if (dist > _maxBeatDist) { //Forget about it... Your music haven't rythm with these kind of values
          _bpm=-1.f;
        } else {
          _bpm=60.f*_pushSec/dist;
        }
        *_bpmsIterator=_bpm;
        _bpmsIterator++;
        if (_bpmsIterator == _bpms.end()) _bpmsIterator=_bpms.begin();
        _compute_average();
        _lastBeat=_maxOverLongDist;
      } 
      _overLong=false;
    }
  } else {
    if (value > _lastLongValue) {
      _maxOverLongValue=value;
      _maxOverLongDist=0;
      _overLong=true;
    }
  }
  _lastValue=value;
}

void TempoSensorA::_compute_average()
{
  _average=0.f;
  unsigned int errors=0;
  for (std::vector<float>::iterator i=_bpms.begin();
       i != _bpms.end(); i++) {
    if (*i >= _minBpm && *i <= _maxBpm)
      _average+=*i;
    else errors++;
  }
  if (errors > _bpms.size()/2) _average=-1.f;
  else _average /= (float)(_bpms.size()-errors);
}

void TempoSensorA::_compute_divergence()
{
  unsigned int errors=0;
  _divergence=0.f;
  for (std::vector<float>::iterator i=_bpms.begin();
       i != _bpms.end(); i++) {
    if (*i >= _minBpm && *i <= _maxBpm)
      _average+=(*i) * (*i);
    else errors++;
  }
  if (errors > _bpms.size()/2) _divergence=-1.f;
  else {
    _divergence /= (float)(_bpms.size()-errors);
    _divergence -= _average*_average;
  }
}
