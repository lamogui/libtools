
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
