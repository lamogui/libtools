

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
  * FILE         : vertex.c
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V1.0 olol
  * DEPENDENCIES : config.h
  * ROLE         : Vertex shaders
  */

#include <libtools/public/config.h>

#ifdef __cplusplus
extern "C" {
#endif 

const char* v_vs_src=
"varying vec2 v;"
"void main()"
"{"
    "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
    "v           = gl_Vertex.xy;"
    "gl_TexCoord[0] = gl_MultiTexCoord0;"
"}";



const char* coords_vs_src=
"varying vec2 coords;"
"void main()"
"{"
    "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
    "coords      = gl_Vertex.xy;"
    "gl_TexCoord[0] = gl_MultiTexCoord0;"
"}";

#ifdef __cplusplus
}
#endif
