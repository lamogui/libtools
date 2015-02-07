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
  * FILE         : sampler1d.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  * DEPENDENCIES : config.h
  */

  #ifndef LIBTOOLS_SFMLSAMPLER1D_HPP
  #define LIBTOOLS_SFMLSAMPLER1D_HPP
  
  #include <libtools/public/config.h>
  
  class Sampler1D : public sf::GlTexture
  {
    public:
      Sampler1D();
      virtual ~Sampler1D();
      bool create(unsigned int size, const float*);
      void update(const float* );
      void update(const float* , unsigned int size, unsigned int x=0);
      
      virtual void bind() const;
      
      inline unsigned int getSize() const {
        return _size;
      }
      
      inline bool isSmooth() const {
        return _smooth;
      }
      
      inline bool isRepeated() const {
        return _isRepeated;
      }
      
      void setSmooth(bool);
      void setRepeated(bool);
      
    private:
      unsigned int _realSize;
      unsigned int _size;
      
      bool _smooth;
      bool _isRepeated;
      
  };
  
  
  #endif