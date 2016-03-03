
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
  * FILE         : visualsignal.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  * DEPENDENCIES : visualsignal.hpp
  */

#include <libtools/core/visualsignal.hpp> 
  
VisualSignal::VisualSignal() :
noise(0),
clean(0),
distord(0),
linear(0),
grain(0),
triangles(0),
previous(50),
previous2(50),
dprevious(0),
dprevious2(0)
{
  memset(samples,0,1024);
}
VisualSignal::~VisualSignal() {
}


void VisualSignal::update(const Signal& signal) {
  const unsigned int size = Signal::size > 1024 ? 1024 : Signal::size;
  for (unsigned int i = 0; i < size; i++){
    samples[i] = (int16_t) (signal.samples[i]*50+50);
  }

  noise=0;
  clean=0;
  distord=0;
  linear=0;
  grain=0;
  triangles=0;

  const int previous_save=previous;
  for (unsigned int i = 0; i < size; i++){
    const int d31 = previous - samples[i];
    const int d32 = samples[i] - previous;
    const int d3 = d31 >= 0 ? d31 : d32;
    const int d3i = d31 < 0 ? d31 : d32;

    const int d3p = dprevious > 0 ? dprevious : -dprevious; 
    const int d3pp = dprevious2 > 0 ? dprevious2 : -dprevious2; 

    const int dd31 = dprevious - d31;
    const int dd32 = d31 - dprevious;
    const int dd3 = dd31 >= 0 ? dd31 : dd32;
    const int dd3i = dd31 < 0 ? dd31 : dd32;


    //if ((d31 > 0 && dprevious < 0)||(d31 < 0 && dprevious > 0))
    //  noise += d3;
    if (d3>5) {
      noise += d3;    
      grain++;
    }
    else if ((d31 >= 0 && dprevious >= 0) || ( d31 < 0 && dprevious < 0)) {
      if (d3p <=3 && d3pp <=3 && ((d31 >= 0 && dprevious2 >= 0) || ( d31 < 0 && dprevious2 < 0)))
      {
        clean+=6;
      }
      else clean++;
    }

    if (dd3 > 3) distord += dd3;
    if (d31 == dprevious && dprevious == dprevious2) linear += 3;

    previous2=previous;
    previous=samples[i];
    dprevious2=dprevious;
    dprevious=d31;
  }

  noise = noise*1024/size;
  clean = clean*1024/size;
  distord = distord*1024/size;
  linear = linear*1024/size;
  grain = grain*1024/size;
  
  /*previous=previous_save;
  int min=100;
  int max=0;
  unsigned int x1=0;
  unsigned int x2=0;
  // (x1,max)  
  // /\
  //   \
  //    \  /
  //     \/
  //     (x2,min)
  for (unsigned int i = 0; i < size; i++){
    if (samples[i] > previous && min==100) {
      x1=i;
      max = samples[i]; //Find the max
    }
    else if (samples[i] < previous) {
      x2=i;
      min = samples[i]; //next find the min
    }
    else if (samples[i] > previous)
    {
      //testons la linéarité de la pente
      const int d = x2 - x1;
      const int m = (max - min);
      if (d > 30 && m > 3) {
        const float c = m/(float)d;
        float y = (float) max;
        unsigned bruits = 0;
        for (unsigned int i=x1; i <= x2;i++)
        {
          y-=c;
          if (fabs(y-(float)samples[i]) > 2) { //suffisament linéaire ?
            bruits++;
          }
        }
        if (bruits < m/3) {
          triangles+=1;
        }
      }
      min=100;
      max=0;
    }
   
    previous=samples[i];
  }

  
  triangles = triangles*1024/size;
*/
}
