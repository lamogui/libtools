
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
    "vs=texture1D(signal, 1.0-v.x).x;"
  "else "
    "vs=texture1D(signal, v.x).x;"
  /*
  "const float eps=vs-v.y;"
  "const float thick=thickness*0.5;"
  */
  "float eps=vs-v.y;" /* fucking intel graphics compliance */
  "float thick=thickness*0.5;"
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
    "vs=texture1D(signal, 1.0-v.x).x;"
  "else "
    "vs=texture1D(signal, v.x).x;"
  /*
  "const float eps=vs-v.y;"
  "const float thick=thickness*0.5;"
  */
  "float eps=vs-v.y;" /* fucking intel graphics compliance */
  "float thick=thickness*0.5;"
  "if (eps > -thick && eps < thick)"
    "gl_FragColor = vec4(color.rgb, color.a*(1.0-pow(abs(eps/thick),1.5)));"
  "else "
    "gl_FragColor = vec4(0.,0.,0.0,0.0);"
"}";

const char* classic_flux_fs_src=
"varying vec2 v;"
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
    "float x=offset-v.x;"
    "vs=texture1D(flux,x).x;"
    "if (v.x >= 1.0 - step || v.x <= step)"
      "pvs=0.0;"
    "else "
      "pvs=texture1D(flux,x-step).x;"
    
  "} else {"
    "float x=offset-(1.0-(v.x+0.5*step));"
    "vs=texture1D(flux,x).x;"
    "if (v.x <= step)"
      "pvs=vs;"
    "else "
      "pvs=texture1D(flux,x-step).x;"
    
  "}"
  
  
  /*
  "const float eps=vs-v.y;"
  "const float thick=thickness*0.5;"
  */
  "float eps=vs-v.y;" /* fucking intel graphics compliance */
  "float thick=thickness*0.5;"
  "if (abs(vs-pvs) > thickness && ((v.y > vs && v.y < pvs) || (v.y > pvs && v.y < vs)))"
    "gl_FragColor = color;"
  "else if (eps > -thick && eps < thick)"
    "gl_FragColor = vec4(color.rgb, color.a*(1.0-pow(abs(eps/thick),1.5)));"
  "else "
    "gl_FragColor = vec4(0.,0.,0.0,0.0);"
"}";

const char* classic_cardio_flux_fs_src=
"varying vec2 v;"
"uniform sampler1D flux;"
"uniform float offset;"
"uniform float step;"
"uniform float thickness;"
"uniform vec4 color;"
"void main() {"
  "float pvs;"
  "float vs;"
  "float x=offset-v.x;"
  "pvs=texture1D(flux, -x-step).x;"
  "vs=texture1D(flux, -x).x;"
  /*
  "const float eps=vs-v.y;"
  "const float thick=thickness*0.5;"
  */
  "float eps=vs-v.y;" /* fucking intel graphics compliance */
  "float thick=thickness*0.5;"
  "if (abs(vs-pvs) > thickness && ((v.y > vs && v.y < pvs) || (v.y > pvs && v.y < vs)))"
    "gl_FragColor = color;"
  "else if (eps > -thick && eps < thick)"
    "gl_FragColor = vec4(color.rgb, color.a*(1.0-pow(abs(eps/thick),1.5)));"
  "else "
    "gl_FragColor = vec4(0.,0.,0.0,0.0);"
"}";

#ifdef __cplusplus
}
#endif
