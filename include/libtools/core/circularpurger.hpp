

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
  * © COPYDOWN LAMOGUI ALL RIGHTS RESERVED
  *
  * FILE         : circularpurger.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V1.0 olol
  * DEPENDENCIES : config.h
  */

#ifndef LIBTOOLS_CIRCULARPURGER_HPP
#define LIBTOOLS_CIRCULARPURGER_HPP

#include <libtools/public/config.h>
#include <vector>


//C'est un buffer particulier qui detruit les valeurs les 
//plus anciennes lorsqu'il est plein

//to get from the newer to older use this double loop
//for (auto it=cp.newer(); it!=cp.end(); it++);
//for (auto it=cp.begin(); it!=cp.newer(); it++);

template<typename T>
class circularpurger 
{
  public:
    circularpurger(unsigned int size, const T& init=0) :
    _offset(size-1)
    {
      _vector.reserve(size);
      _vector.resize(size);
      _newer=_vector.begin();
      _older=_vector.begin();
      typename std::vector<T>::iterator it=_vector.begin();
      for (;it != _vector.end(); it++)
        *it=init;
    }
    
    virtual ~circularpurger() {
    }
    
    inline typename std::vector<T>::const_iterator newer() const {
      return _newer;
    }
    
    inline typename std::vector<T>::const_iterator older() const {
      return _older;
    }
    
    /*inline T older() const {
      return *_older;
    }*/
    
    inline T last() const {
      return *_newer;
    }
    
    inline typename std::vector<T>::const_iterator begin() const
    {
      return _vector.begin();
    }
    
    inline typename std::vector<T>::const_iterator end() const
    {
      return _vector.end();
    }
    
    inline void push(const T& v) {
      *_older=v;
      _newer=_older;
      _older++;
      _offset++;
      if (_older==_vector.end()) {
        _older=_vector.begin();
      }
      if (_offset>=_vector.size())
      {
        _offset=0;
      }
    }
    
    inline unsigned int size() const {
      return _vector.size();
    }
    
    inline const T* data() const {
      return _vector.data();
    }
    
#if (_MSC_VER == 1500)
	inline T* data() {
      return &(_vector[0]);
    }
#else

    inline T* data() {
      return _vector.data();
    }
#endif
    
    inline unsigned int offset() const
    {
      return _offset;
    }

    inline float offsetf() const
    {
      return static_cast<float>(offset())/static_cast<float>(size());
    }
    
  private:
    typename std::vector<T> _vector;
    typename std::vector<T>::iterator _older;
    typename std::vector<T>::iterator _newer;
    unsigned int _offset;
};

#endif
