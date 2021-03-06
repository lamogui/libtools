﻿
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
  * FILE         : note.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  * DEPENDENCIES : note.hpp
  * ROLE         : Déclare les classes permettant de 
  *                manipuler des données rythmiques en 
  *                lien avec un morceau et un instrument
  */


#include <libtools/core/note.hpp>
 
double Note::noteFrequency[256]={
27.5,29.1353,30.8677,32.7032,34.6479,36.7081,38.8909,41.2035,43.6536,46.2493,48.9995,51.9130,
55,  58.2705,61.7354,65.4064,69.2957,73.4162,77.7817,82.4069,87.3071,92.4986,97.9989,103.826,
110, 116.541,123.471,130.813,138.591,146.832,155.563,164.814,174.614,184.997,195.998,207.652,
220, 233.082,246.942,261.626,277.183,293.665,311.127,329.628,349.228,369.994,391.995,415.305,
440, 466.164,493.883,523.251,554.365,587.33, 622.254,659.255,698.456,739.989,783.991,830.609,
880, 932.328,987.767,1046.5, 1108.73,1174.66,1244.51,1318.51,1396.91,1479.98,1567.98,1661.22,
1760,1864.66,1975.53,2093,2217.46,2349.32,2489.02,2637.02,2793.02,2959.96,3135.96,3322.44,
3520,3729.31,3951.07,4186.01 };
 
Note::Note(unsigned int st, unsigned char i, float v):
_start(st),
_length(0),
_velocity(v),
_id(i)
{
}

Note::Note(const Note& c) : 
_start(c._start),
_length(c._length),
_velocity(c._velocity),
_id(c._id)
{

}

Note::~Note()
{
}

Note& Note::operator=(const Note& c)
{
  _start = c._start;
  _length = c._length;
  _id = c._id;
  _velocity = c._velocity;
  return *this;
}


bool Note::isBlack(unsigned char id)
{
  const unsigned n=(id+9)%12;
  const unsigned impaire=n%2;
  return ((impaire && n<5) || (n>5 && !impaire));
}
