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
#if defined(SFML_GRAPHICS_HPP)     
    sf::Texture texture;
    inline unsigned int getTextureOffset() { return _texOffset;}
#endif   
  private:
    sample* _input; //Real input buffer;
    sample* _real;
    sample* _imaginary;
    sample* _module;
    unsigned int* _indexTable;
    sample* _twidleFactor;
    sample* _twidleFactorI;
#if defined(SFML_GRAPHICS_HPP) 
    uint8_t* _pixels;
#endif
    unsigned int _size;
    unsigned int _inputOffset;
#if defined(SFML_GRAPHICS_HPP) 
    unsigned int _texOffset;
#endif
    unsigned short _pow2;


};


#endif

