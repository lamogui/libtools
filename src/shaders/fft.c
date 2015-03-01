
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
  * FILE         : fft.c
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V1.0 olol
  * DEPENDENCIES : config.h
  * ROLE         : Shaders utilisables avec une fft
  */

#include <libtools/public/config.h>

#ifdef __cplusplus
extern "C" {
#endif 

const char* vls_fft_fs_src=
"varying vec2 v;"
"uniform float basslimit;"
"uniform sampler1D spectrum;"
"uniform bool yflip;"
"void main()"
"{"
  "vec2 mv=v;"  // ->  [0 1]
  "if (yflip) mv.y=1.0-mv.y;"
  "float coord = mv.x*0.19;" // -> 22100 Hz to 4200Hz
  //Attention normalisé pour pour FFT entre 0 et 1.0 SANS symétrie
  "float texturev=sqrt(texture1D(spectrum, coord).x);"
  "if(texturev >= mv.y ) {"
      "if (coord < basslimit)"
          "gl_FragColor = vec4(0.7+0.3*mv.y,(1.-mv.y)*.75,0.3,1.);"
      "else "
          "gl_FragColor = vec4(mv.y,(1.-mv.y)*.75,(1.-mv.y)*.1,1.);"
  "}"
  "else "
      "gl_FragColor = vec4(1.0,1.0,1.0,0.0);"
"}";

#ifdef __cplusplus
}
#endif