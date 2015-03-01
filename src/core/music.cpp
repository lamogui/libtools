
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
  * FILE         : music.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.7 olol
  * DEPENDENCIES : music.hpp
  */


#include <libtools/core/music.hpp>

unsigned int Music::nextFrame(Signal& left, Signal& right)
{
  const unsigned int decoded=_decoder->fetch(left,right);
  if (decoded) {
    const float left_energy=left.energy();
    const float right_energy=right.energy();
    _energy=left_energy+right_energy;
    _energyTempo.push(_energy);
    
    _fft->compute(left);
    _fft->computeModule();
    _bass=_fft->bass();
    _bassTime+=_bass*1.f/Signal::refreshRate;
    _bassTempo.push(_bass);
    _bpm=_bassTempo.average();
    
    if (_bassTempo.average() > 0.f && _bassTempo.divergence() < 2.f)
    {
      _bpm=_bassTempo.average();
    }
    else if (_energyTempo.average() > 0.f && _energyTempo.divergence() < 2.f)
    {
      _bpm=_energyTempo.average();
    }
    else {
      _bpm=-1.f;
    }
    
    if (_bpm > 0.f) 
      _boostTime+=_step*_bpm/120.f;
    
  }
  return decoded;
}

void Music::_reset()
{ 
  _energy=0.f;
  _boostTime=0.f;
  _signalSec=(float)Signal::frequency/(float)Signal::size;
  _energyTempo.reset(_signalSec);
  _bassTempo.reset(_signalSec);
  _bassTime=0.f;
  _bass=0.f;
  _bpm=-1.f;
  _step=(float)Signal::size/(float)Signal::frequency;
}