
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
"varying vec2 coords;"
"uniform float basslimit;"
"uniform sampler1D spectrum;"
"uniform bool yflip;"
"void main()"
"{"
  "vec2 mv=coords;"  // ->  [0 1]
  "if (yflip) mv.y=1.0-mv.y;"
  "const float p=4.;"
  "float coord = pow(mv.x*0.5  + 0.18,p);" // mv.x*0.19 -> 22100 Hz to 4200Hz
  //Attention normalisé pour pour FFT entre 0 et 1.0 SANS symétrie
  "float texturev=texture1D(spectrum, coord).x;"
  "if(min(texturev*4.,1.) >= mv.y ) {"
      "if (coord < basslimit)"
          "gl_FragColor = vec4(0.7+0.3*mv.y,(1.-mv.y)*.75,0.3,1.);"
      "else "
          "gl_FragColor = vec4(mv.y,(1.-mv.y)*.75,(1.-mv.y)*.1,1.);"
  "}"
  "else "
      "gl_FragColor = vec4(1.0,1.0,1.0,0.0);"
"}";

const char* vls_log_fft_fs_src=
"varying vec2 coords;"
"uniform float basslimit;"
"uniform sampler1D spectrum;"
"uniform bool yflip;"
"void main()"
"{"
  "const float eps=1./512.;"
  "const float p=4;"
  "vec2 mv=coords;"  // ->  [0 1]
  "if (yflip) mv.y=1.0-mv.y;"
  "float coord = pow(mv.x, p);"//(exp(mv.x)-1.)/1.72;"
  "float coorda = pow(mv.x-eps,p);"
  "float coordb = pow(mv.x+eps,p);"
  "float texturev=texture1D(spectrum, coord).x*0.5 + texture1D(spectrum, coorda).x*0.25 + texture1D(spectrum, coordb).x*0.25;"
  "texturev=1.+(20.*log10(texturev)/60.);" //db
  "if (texturev < 0.) texturev=0.;"
  "if(texturev >= mv.y ) {"
      "if (coord < basslimit)"
          "gl_FragColor = vec4(0.7+0.3*mv.y,(1.-mv.y)*.75,0.3,1.);"
      "else "
          "gl_FragColor = vec4(mv.y,(1.-mv.y)*.75,(1.-mv.y)*.1,1.);"
  "}"
  "else "
      "gl_FragColor = vec4(1.0,1.0,1.0,0.0);"
"}";

const char* shadertoy_fft_fs_src=
"varying vec2 coords;"
"uniform sampler1D spectrum;"
"uniform bool yflip;"
"uniform float bars;"      //32.0   // How many buckets to divide spectrum into
"float barSize=1.0 / bars;"        // Constant to avoid division in main loop
"float barGap=0.1 * barSize;"      // 0.1 represents gap on both sides, so a bar is
                                  // shown to be 80% as wide as the spectrum it samples
"uniform float sampleSize;" //0.02// How accurately to sample spectrum, must be a factor of 1.0
                                  // Setting this too low may crash your browser!

// Helper for intensityToColour
"float h2rgb(float h) {"
  "if(h < 0.0) h += 1.0;"
  "if(h < 0.166666) return 0.1 + 4.8 * h;"
  "if(h < 0.5) return 0.9;"
  "if(h < 0.666666) return 0.1 + 4.8 * (0.666666 - h);"
  "return 0.1;"
"}"

// Map [0, 1] to rgb using hues from [240, 0] - ie blue to red
"vec3 intensityToColour(float i) {"
  // Algorithm rearranged from http://www.w3.org/TR/css3-color/#hsl-color
  // with s = 0.8, l = 0.5
  "float h = 0.666666 - (i * 0.666666);"

  "return vec3(h2rgb(h + 0.333333), h2rgb(h), h2rgb(h - 0.333333));"
"}"

"void main() {"
  "vec2 uv=coords;"
  "if (yflip) uv.y=1.-uv.y;"
  // Get the starting x for this bar by rounding down
  "float barStart = floor(uv.x * bars) / bars;"

  // Discard pixels in the 'gap' between bars
  "if(uv.x - barStart < barGap || uv.x > barStart + barSize - barGap) {"
    "gl_FragColor = vec4(0.0);"
  "}"
   "else"
    "{"
  // Sample spectrum in bar area, keep cumulative total
  "float intensity = 0.0;"
  "for(float s = 0.0; s < barSize; s += barSize * sampleSize) {"
    // Shader toy shows loudness at a given frequency at (f, 0) with the same value in all channels
    "intensity += 4.*pow(texture1D(spectrum, barStart + s).r,0.3);"
  "}"
  "intensity *= sampleSize;" // Divide total by number of samples taken (which is 1 / sampleSize)
  "intensity = clamp(intensity, 0.005, 1.0);" // Show silent spectrum to be just poking out of the bottom

  // Only want to keep this pixel if it is lower (screenwise) than this bar is loud
  "float i = float(intensity > uv.y);" // Casting a comparison to float sets i to 0.0 or 1.0

  //fragColor = vec4(intensityToColour(uv.x), 1.0);       // Demo of HSL function
  //fragColor = vec4(i);                                  // Black and white output
  "gl_FragColor = vec4(intensityToColour(intensity) * i, i);"  // Final output
    "}"
  // Note that i2c output is multiplied by i even though i is sent in the alpha channel
  // This is because alpha is not 'pre-multiplied' for fragment shaders, you need to do it yourself
"}";


#ifdef __cplusplus
}
#endif
