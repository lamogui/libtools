

#include <libtools/decoders/agldecoder.hpp>
#include <libtools/core/file.h>
#include <cassert>

AglDecoder::AglDecoder():
  _xmldoc()
{
}


AglDecoder::~AglDecoder()
{
}


unsigned int AglDecoder::fetch(Signal& outleft, Signal& outright)
{
}

bool AglDecoder::ended() const
{
}

void AglDecoder::rewind()
{
}

double AglDecoder::length() const
{
}


bool AglDecoder::_open(const string_t& filename)
{
#if defined(LIBTOOLS_WINDOWS) && defined(string_t_w_available)
  FILE* f=_wfopen(string_t_to_stdw(filename).c_str(),"rb");
#else
  FILE* f=fopen(string_t_to_std(filename).c_str(),"rb");
#endif
  if (!f)
  {
#ifndef NDEBUG
    std::cerr << "AglDecoder error: Unable to open the file " << string_t_to_std(filename) << std::endl;
#endif
    return false;
  }
  tinyxml2::XMLError load_error=_xmldoc.LoadFile(f);
  fclose(f);
  if (load_error)
  {
#ifndef NDEBUG
    std::cerr << "TinyXML error: " << load_error << " " << _xmldoc.GetErrorStr1() << std::endl;
#endif
    return false;
  }
  return _parse();
}

bool AglDecoder::_load(const uint8_t* buffer, unsigned int size)
{
  assert(buffer && size);
  tinyxml2::XMLError load_error=_xmldoc.Parse(buffer,size);
  if (load_error)
  {
#ifndef NDEBUG
    std::cerr << "TinyXML error: " << load_error << " " << _xmldoc.GetErrorStr1() << std::endl;
#endif
    return false;
  }
  return _parse();
}



void AglDecoder::_reset()
{
  _xmldoc.Clear();
}

bool AglDecoder::_parse()
{

}
