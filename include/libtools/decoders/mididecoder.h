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
  * THIS FILE IS PART OF
  * SECURITY LEVEL : 3
  * VISIBILITY     : PUBLIC
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED
  *
  * FILE         : mididecoder.h
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.0
  */

#ifndef MIDIDECODER_H
#define MIDIDECODER_H

#include <libtools/public/config.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  char id[4];
  uint32_t size;
  uint16_t type;
  uint16_t tracks;
  uint16_t time_division;
} midi_header;


typedef struct {
  char id[4];
  uint32_t size;
} midi_track_header;




#ifdef __cplusplus
}
#endif


#endif // MIDIDECODER_H

