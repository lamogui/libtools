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
