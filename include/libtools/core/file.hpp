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
  * FILE         : file.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.7 olol
  * DEPENDENCIES : config.h
  *                file.h
  * ROLE         : Classes utiles pour manipuler facilement des 
  *                fichiers
  */

#ifndef LIBTOOLS_FILE_HPP
#define LIBTOOLS_FILE_HPP

#include <libtools/public/config.h>
#include <libtools/core/file.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

class LIBTOOLS_PRIVATE AbstractFileParser
{
  public:
    AbstractFileParser(); 
    virtual ~AbstractFileParser(); 
    
    inline unsigned int read_from_buffer(const unsigned char* buffer, 
                                         unsigned int size) {
       unsigned int offset=0;
       //std::cout << "Call  AbstractFileParser " << size << std::endl;
       return this->read_from_buffer_offset(buffer,size,offset);
    }
    
    virtual unsigned int read_from_buffer_offset(const unsigned char* buffer, 
                                                 unsigned int size,
                                                 unsigned int& offset) = 0;
    
    
    /* Return the number of writed bytes */
    virtual unsigned int write_to_buffer_offset( unsigned char* buffer, 
                                                 unsigned int size,
                                                 unsigned int& offset) 
                                                 const = 0;
    
    inline unsigned int write_to_buffer( unsigned char* buffer, 
                                         unsigned int size) const
    {
       unsigned int offset=0;
       return write_to_buffer_offset(buffer,size,offset);
    }    
    
    unsigned int write_to_file(FILE* file) const;
    
    virtual unsigned int byte_size() const = 0;
      
  protected:
};

#ifdef SFML_INPUTSTREAM_HPP
class FileStream : public sf::InputStream
{
public :

    FileStream();

    virtual ~FileStream();

    bool open(const string_t& filename);

    virtual int64_t read(void* data, int64_t size);

    virtual int64_t seek(int64_t position);

    virtual int64_t tell();

    virtual int64_t getSize();
    
    virtual string_t getFilename();
    
    virtual string_t getCompleteFilename();
    
    void save(const string_t& name);

protected :

    std::FILE* m_file;
    string_t _filename;
};

class CryptedFileStream : public FileStream
{
   public :

      CryptedFileStream();

      virtual ~CryptedFileStream();

      bool open(const string_t& filename);

      virtual int64_t read(void* data, int64_t size);

      virtual int64_t seek(int64_t position);
      

   private :
      string_t _key;
      std::size_t _offset;

};

#endif


#endif
