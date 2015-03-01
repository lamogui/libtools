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
  * VISIBILITY     : PRIVATE
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : file.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.7 olol
  * DEPENDENCIES : file.hpp
  * ROLE         : Classes utiles pour manipuler facilement des 
  *                fichiers
  */
  
#include <libtools/core/file.hpp>


AbstractFileParser::AbstractFileParser() 
{

}

AbstractFileParser::~AbstractFileParser()
{

}

unsigned int AbstractFileParser::write_to_file(FILE* file) const
{
  unsigned int target_size=byte_size();
  unsigned int real_size;
  if (!target_size) return 0;
  unsigned char* buffer = (unsigned char*) malloc(target_size);
  if (real_size=write_to_buffer(buffer,target_size))
  {
    if (real_size!=fwrite((void*)buffer,1,real_size,file))
    {
      printf("fwrite error !\n"); 
      free((void*)buffer);
      return 0;
    }
  }
  else 
  {
    free((void*)buffer);
    return 0;
  }
  free((void*)buffer);
  return real_size;
}

#ifdef SFML_INPUTSTREAM_HPP

FileStream::FileStream() :
m_file(NULL)
{
}

FileStream::~FileStream()
{
    if (m_file)
        std::fclose(m_file);
}

bool FileStream::open(const string_t& filename)
{
    if (m_file)
        std::fclose(m_file);

    m_file = std::fopen(string_t_to_std(filename).c_str(), "rb");
    _filename = filename;

    return m_file != NULL;
}

int64_t FileStream::read(void* data, int64_t size)
{
    if (m_file)
        return std::fread(data, 1, static_cast<std::size_t>(size), m_file);
    else
        return -1;
}

int64_t FileStream::seek(int64_t position)
{
    if (m_file)
    {
        std::fseek(m_file, static_cast<std::size_t>(position), SEEK_SET);
        return tell();
    }
    else
    {
        return -1;
    }
}

int64_t FileStream::tell()
{
    if (m_file)
        return std::ftell(m_file);
    else
        return -1;
}

int64_t FileStream::getSize()
{
    if (m_file)
    {
        int64_t position = tell();
        std::fseek(m_file, 0, SEEK_END);
        int64_t size = tell();
        seek(position);
        return size;
    }
    else
    {
        return -1;
    }
}

string_t FileStream::getFilename()
{
  string_t f=_filename;
  //f.erase(0,f.find_last_of('/')+1);
  //f.erase(0,f.find_last_of('\\')+1);
  while (string_t_find_success(f.find('/')))
    f.erase(0,f.find('/')+1);
  while (string_t_find_success(f.find('\\')))
    f.erase(0,f.find('\\')+1);
  return f;
}
    
string_t FileStream::getCompleteFilename()
{
   return _filename;
}

void FileStream::save(const string_t& name)
{
  if (!m_file) return;
  seek(0);
  int size = getSize();
  char* buf = new char[size];
  read(buf,size);
  FILE* f = fopen(string_t_to_std(name).c_str(),"wb");
  fwrite(buf,1,size,f);
  fclose(f);
}


CryptedFileStream::CryptedFileStream() :
_key(),
_offset(0)
{
}

CryptedFileStream::~CryptedFileStream()
{
}

bool CryptedFileStream::open(const string_t& filename)
{
    if (m_file)
        std::fclose(m_file);

    m_file = std::fopen(string_t_to_std(filename).c_str(), "rb");
    _filename = filename;
    _offset = 0;
    _key = getFilename();
    //printf("key : %s\n",_key.c_str());
    return m_file != NULL;
}

int64_t CryptedFileStream::read(void* data, int64_t size)
{
    if (m_file){
        std::size_t r = std::fread(data, 
                                      1, 
         static_cast<std::size_t>(size), m_file);
         std::size_t s = string_t_size(_key);
        for (char* p = (char*) data; p < (char*) data+r; p++)
        {
            *p = *p^(char)_key[_offset%s];
            _offset++;
        }
        return r;
    }
    else
        return -1;
}

int64_t CryptedFileStream::seek(int64_t position)
{
    if (m_file)
    {
        std::fseek(m_file, static_cast<std::size_t>(position), SEEK_SET);
        _offset = position;
        return _offset;
    }
    else
    {
        return -1;
    }
}
#endif