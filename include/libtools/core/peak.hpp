
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
  * FILE         : peak.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  * DEPENDENCIES : config.h
  *                note.hpp
  *                signal.hpp
  */



#ifndef LIBTOOLS_PEAK_HPP
#define LIBTOOLS_PEAK_HPP

#include <libtools/public/config.h>
#include <vector>
#include <libtools/core/signal.hpp>
#include <libtools/core/note.hpp>

class LIBTOOLS_PUBLIC TFDeLoorPeak 
{
  public:
    sample amp;
    sample max_amp;
    sample l;
    sample r;
    unsigned int frames;
    unsigned char note_id;
    unsigned char begin_note_id;
    unsigned char width;
    unsigned char begin_width;
    bool updated;
    bool alive;
    bool oscillate;
};

class LIBTOOLS_PRIVATE TFDeLoorPeakManager
{
  public:
    TFDeLoorPeakManager();
    ~TFDeLoorPeakManager();

    void reset();


    int rouge;
    int vert;
    int bleu;
    int jaune;

    void update(bool create, bool kill, sample* l, sample* r=0);
    unsigned int mountains_position;

    TFDeLoorPeak* peaks;
    sample previouspeaksl[NB_NOTES];
    sample previouspeaksr[NB_NOTES];

  private:
    void _unupdate();
    void _clean();
};  

#endif 
