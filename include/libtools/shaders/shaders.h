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
  * FILE         : shaders.h
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V1.0 olol
  * DEPENDENCIES : config.h
  * ROLE         : Quelques shaders
  */
  
#ifndef LIBTOOLS_SHADERS_H
#define LIBTOOLS_SHADERS_H

#ifdef __cplusplus
extern "C" {
#endif 
 
//Signal
extern const char* lightsaber_signal_fs_src;
extern const char* classic_signal_fs_src;

//flux
extern const char* classic_flux_fs_src;
extern const char* classic_cardio_flux_fs_src;

//FFT
extern const char* vls_fft_fs_src;
extern const char* vls_log_fft_fs_src;
extern const char* shadertoy_fft_fs_src;

//Vertex
extern const char* v_vs_src; //OBSELETE !!!!!
extern const char* coords_vs_src;

#ifdef __cplusplus
}
#endif

#endif
