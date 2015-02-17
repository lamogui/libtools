

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
    
    inline T* data() {
      return _vector.data();
    }
    
    inline unsigned int offset() const
    {
      return _offset;
    }
    
  private:
    typename std::vector<T> _vector;
    typename std::vector<T>::iterator _older;
    typename std::vector<T>::iterator _newer;
    unsigned int _offset;
};

#endif