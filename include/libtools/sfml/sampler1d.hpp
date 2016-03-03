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
