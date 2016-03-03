
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
  * FILE         : signal.c
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V1.0 olol
  * DEPENDENCIES : config.h
  * ROLE         : Shaders utilisables avec signal
  */

#include <libtools/public/config.h>

#ifdef __cplusplus
extern "C" {
#endif


const char* lightsaber_signal_fs_src =
"varying vec2 coords;"
"uniform sampler1D signal;"
"uniform float thickness;"
"uniform bool reversed;"
"uniform vec4 color;"
"void main() {"
  "float vs;"
  "if (reversed)"
    "vs=texture1D(signal, 1.0-coords.x).x;"
  "else "
    "vs=texture1D(signal, coords.x).x;"
  /*
  "const float eps=vs-coords.y;"
  "const float thick=thickness*0.5;"
  */
  "float eps=vs-coords.y;" /* fucking intel graphics compliance */
  "float thick=thickness*0.5;"
  "if (eps > -thick && eps < thick)"
    "gl_FragColor = mix(vec4(1.0,1.0,1.0,1.0),color,pow(abs(eps/thick),1.5));"
  "else "
    "gl_FragColor = vec4(0.0,0.0,0.0,0.0);"
"}";

const char* classic_signal_fs_src=
"varying vec2 coords;"
"uniform sampler1D signal;"
"uniform float thickness;"
"uniform bool reversed;"
"uniform vec4 color;"
"uniform bool yflip;"
"void main() {"
  "vec2 c=coords;"
  "if (yflip) c.y=1.-c.y;"
  "float vs;"
  "if (reversed)"
    "vs=texture1D(signal, 1.0-c.x).x;"
  "else "
    "vs=texture1D(signal, c.x).x;"
  /*
  "const float eps=vs-coords.y;"
  "const float thick=thickness*0.5;"
  */
  "float eps=vs-c.y;" /* fucking intel graphics compliance */
  "float thick=thickness*0.5;"
  "if (eps > -thick && eps < thick)"
    "gl_FragColor = vec4(color.rgb, color.a*(1.0-pow(abs(eps/thick),1.5)));"
  "else "
    "gl_FragColor = vec4(0.,0.,0.0,0.0);"
"}";

const char* classic_flux_fs_src=
"varying vec2 coords;"
"uniform sampler1D flux;"
"uniform float offset;"
"uniform float step;"
"uniform float thickness;"
"uniform bool reversed;"
"uniform vec4 color;"
"void main() {"
  "float pvs;"
  "float vs;"
  /*"float m=1.0+0.5*step;" use setRepeated on the sampler*/

  "if (reversed) {"
    "float x=offset-coords.x;"
    "vs=texture1D(flux,x).x;"
    "if (coords.x >= 1.0 - step || coords.x <= step)"
      "pvs=0.0;"
    "else "
      "pvs=texture1D(flux,x-step).x;"

  "} else {"
    "float x=offset-(1.0-(coords.x+0.5*step));"
    "vs=texture1D(flux,x).x;"
    "if (coords.x <= step)"
      "pvs=vs;"
    "else "
      "pvs=texture1D(flux,x-step).x;"

  "}"


  /*
  "const float eps=vs-coords.y;"
  "const float thick=thickness*0.5;"
  */
  "float eps=vs-coords.y;" /* fucking intel graphics compliance */
  "float thick=thickness*0.5;"
  "if (abs(vs-pvs) > thickness && ((coords.y > vs && coords.y < pvs) || (coords.y > pvs && coords.y < vs)))"
    "gl_FragColor = color;"
  "else if (eps > -thick && eps < thick)"
    "gl_FragColor = vec4(color.rgb, color.a*(1.0-pow(abs(eps/thick),1.5)));"
  "else "
    "gl_FragColor = vec4(0.,0.,0.0,0.0);"
"}";

const char* classic_cardio_flux_fs_src=
"varying vec2 coords;"
"uniform sampler1D flux;"
"uniform float offset;"
"uniform float step;"
"uniform float thickness;"
"uniform vec4 color;"
"void main() {"
  "float pvs;"
  "float vs;"
  "float x=offset-coords.x;"
  "pvs=texture1D(flux, -x-step).x;"
  "vs=texture1D(flux, -x).x;"
  /*
  "const float eps=vs-coords.y;"
  "const float thick=thickness*0.5;"
  */
  "float eps=vs-coords.y;" /* fucking intel graphics compliance */
  "float thick=thickness*0.5;"
  "if (abs(vs-pvs) > thickness && ((coords.y > vs && coords.y < pvs) || (coords.y > pvs && coords.y < vs)))"
    "gl_FragColor = color;"
  "else if (eps > -thick && eps < thick)"
    "gl_FragColor = vec4(color.rgb, color.a*(1.0-pow(abs(eps/thick),1.5)));"
  "else "
    "gl_FragColor = vec4(0.,0.,0.0,0.0);"
"}";

#ifdef __cplusplus
}
#endif
