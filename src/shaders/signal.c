
/*

THIS FILE IS SUBJECT TO STRICT RULES OF BDE NE666 COPYDOWN. 
NOBODY IS PERMITTED TO COPY AND DISTRIBUTE VERBATIM OR MODIFIED COPIES OF
THIS LICENSE HEADER. A SECURITY LEVEL IS ASSIGNED TO THE FILE AND A VISIBILITY.
THIS FILE MUST NOT BE COPIED OR REDISTRIBUTED IF IT'S VISIBILITY IS NOT PUBLIC. 
VISIBILITY HAS 3 POSSIBLE STATES:
- PRIVATE: LIMITED TO PROJECTS WITH THE SAME SECURITY LEVEL WITHIN THE BDE NE666. (DEFAULT VISIBILITY)
- PROTECTED: LIMITED TO PROJECTS USING THE SAME SECURITY RULES WITHIN THE BDE NE666. 
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
"varying vec2 v;"
"uniform sampler1D signal;"
"uniform float thickness;"
"uniform bool reversed;"
"uniform vec4 color;"
"void main() {"
  "float vs;"
  "if (reversed)"
    "vs=texture1D(signal, 1.0-v.x);"
  "else "
    "vs=texture1D(signal, v.x);"
  "const float eps=vs-v.y;"
  "const float thick=thickness*0.5;"
  "if (eps > -thick && eps < thick)"
    "gl_FragColor = mix(vec4(1.0,1.0,1.0,1.0),color,pow(abs(eps/thick),1.5));"
  "else "
    "gl_FragColor = vec4(0.0,0.0,0.0,0.0);"
"}";
  
const char* classic_signal_fs_src=
"varying vec2 v;"
"uniform sampler1D signal;"
"uniform float thickness;"
"uniform bool reversed;"
"uniform vec4 color;"
"void main() {"
  "float vs;"
  "if (reversed)"
    "vs=texture1D(signal, 1.0-v.x);"
  "else "
    "vs=texture1D(signal, v.x);"
  "const float eps=vs-v.y;"
  "const float thick=thickness*0.5;"
  "if (eps > -thick && eps < thick)"
    "gl_FragColor = vec4(color.rgb, color.a*(1.0-pow(abs(eps/thick),1.5)));"
  "else "
    "gl_FragColor = vec4(0.,0.,0.0,0.0);"
"}";

#ifdef __cplusplus
}
#endif
