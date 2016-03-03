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
  * FILE         : fft.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  *                Kwon-Young Choi (kwonyoungchoi93@gmail.com)
  * VERSION      : 1.7 olol
  * DEPENDENCIES : signal.hpp
  *                config.h
  * ROLE         : Implémente une classe permettant de faire une transformé de 
  *                fourrier discrete...
  */

#ifndef LIBTOOLS_FFT_HPP
#define LIBTOOLS_FFT_HPP

#include <libtools/public/config.h>
#include <libtools/core/signal.hpp>

/*#if defined(SFML_GRAPHICS_HPP)   
#include <libtools/sfml/sampler1d.hpp>
#endif*/

class LIBTOOLS_PRIVATE FFT
{
  public:
    FFT(unsigned int size);
    ~FFT();

    inline unsigned int size() {
      return _size;
    }

    //get nearest inferior power of 2
    static unsigned short getInfPow2(unsigned int v);

    //get nearest superior power of 2
    static unsigned short getSupPow2(unsigned int v);

    //push signal and compute when have enough samples
    void pushSignal(const Signal &s);
    
    
    //Direct immediate compute from samples (with truncate or zero padding...)
    void compute(const sample* s, unsigned int size);
    void compute(const Signal &s);

    //Realloc 
    void realloc(unsigned int size);

    inline sample* getReal() {
      return _real;
    }
    inline sample* getImaginary() {
      return _imaginary;
    }

    inline sample* getModule() {
      return _module;
    }
    
    //Compute module if needed !
    void computeModule();
    
    float bass();
  private:
    sample* _input; //Real input buffer;
    sample* _window;
    sample* _real;
    sample* _imaginary;
    sample* _module;
    unsigned int* _indexTable;
    sample* _twidleFactor;
    sample* _twidleFactorI;
/*#if defined(SFML_GRAPHICS_HPP) 
    Sampler1D _texture;
#endif*/
    unsigned int _size;
    unsigned int _inputOffset;
/*#if defined(SFML_GRAPHICS_HPP) 
    unsigned int _texOffset;
#endif*/
    unsigned short _pow2;


};


#endif

