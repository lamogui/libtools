/*

THIS FILE IS SUBJECT TO STRICT RULES OF BDE NE777 COPYDOWN. 
NOBODY IS PERMITTED TO COPY AND DISTRIBUTE VERBATIM OR MODIFIED COPIES OF
THIS LICENSE HEADER. A SECURITY LEVEL IS ASSIGNED TO THE FILE AND A VISIBILITY.
THIS FILE MUST NOT BE COPIED OR REDISTRIBUTED IF IT'S VISIBILITY IS NOT PUBLIC. 
VISIBILITY HAS 3 POSSIBLE STATES:
- PRIVATE: LIMITED TO PROJECTS WITH THE SAME SECURITY LEVEL WITHIN THE BDE NE777. (DEFAULT VISIBILITY)
- PROTECTED: LIMITED TO PROJECTS USING THE SAME SECURITY RULES WITHIN THE BDE NE777. 
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
