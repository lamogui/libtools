

#include <libtools/decoders/agldecoder.hpp>
#include <libtools/core/file.h>
#include <cassert>
#include <iostream>

AglDecoder::AglDecoder():
  MusicDecoder("AGL Decoder"),
  _xmldoc(),
  _frame(0),
  _musiclength(0.0),
#if defined(SFML_GRAPHICS_HPP)
  _tex(NULL),
  _shader(NULL),
#else
  _tex(0),
#endif
  _code(),
  _pixelsbuffer(NULL)
{
  _sampleRate=Signal::frequency;
}


AglDecoder::~AglDecoder()
{
  _reset();
}


unsigned int AglDecoder::fetch(Signal& outleft, Signal& outright)
{
  if (ended())
    return 0;

#ifdef SFML_GRAPHICS_HPP
  if (!_tex || !_shader)
  {
    return 0;
  }

  sf::RectangleShape shape(sf::Vector2f(32.f,32.f));
  double track_time=static_cast<double>(_frame)/Signal::refreshRate;
  _shader->setParameter("track_time",static_cast<float>(track_time));
  _shader->setParameter("frame",(int)_frame);
  std::cout << "track_time "  << track_time << " signal_size  " << Signal::size << std::endl;
  _tex->setActive(true);
  sf::Shader::bind(_shader);
  _tex->draw(shape);
  sf::Shader::bind(NULL);
  _tex->display();

  _image=_tex->getTexture().copyToImage();
  const uint8_t* pixelsbuffer=_image.getPixelsPtr();
  unsigned int signal_size=Signal::size;
  const uint8_t* it=pixelsbuffer;
  for (unsigned int k=0; k < signal_size; )
  {
    uint16_t lu=it[0] | (it[1] << 8);
    uint16_t ru=it[2] | (it[3] << 8);

    float l= -1.f + static_cast<float>(lu)*2.f/65535.f;
    float r= -1.f + static_cast<float>(ru)*2.f/65535.f;
    outleft.samples[k]=l;
    outright.samples[k]=r;
    k++;
    it+=4;
  }
#else

#endif
  _frame++; //always
  return Signal::size;
}

bool AglDecoder::ended() const
{
  return (_musiclength < static_cast<double>(_frame)/Signal::refreshRate);
}

void AglDecoder::rewind()
{
  _frame=0;
}

double AglDecoder::length() const
{
  return _musiclength;
}


bool AglDecoder::_open(const string_t& filename)
{
#if defined(LIBTOOLS_WINDOWS) && defined(string_t_w_available)
  FILE* f=_wfopen(string_t_to_stdw(filename).c_str(),L"rb");
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
    std::cerr << "TinyXML error: " << load_error << " " << _xmldoc.GetErrorStr1() << " " << _xmldoc.GetErrorStr2() << std::endl;
#endif
    return false;
  }
  return _parse();
}

bool AglDecoder::_load(const uint8_t* buffer, unsigned int size)
{
  assert(buffer && size);
  tinyxml2::XMLError load_error=_xmldoc.Parse((const char*)buffer,size);
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
  _sampleRate=Signal::frequency;
  _xmldoc.Clear();
  _frame=0;
  _musiclength=0;
  _code.clear();
#if defined(SFML_GRAPHICS_HPP)
   if (_tex)
  {
    delete _tex;
     _tex=NULL;
  }
  if (_shader)
  {
    delete _shader;
    _shader=NULL;
  }
#else

  if (_tex)
  {
    glDeleteTextures(1,&_tex);
    _tex=0;
  }
#endif

  if (_pixelsbuffer)
  {
    free(_pixelsbuffer);
    _pixelsbuffer=NULL;
  }

}

bool AglDecoder::_createTexture(unsigned size)
{
#if defined(SFML_GRAPHICS_HPP)
  _tex=new sf::RenderTexture();
  bool r=_tex->create(size,size,false);
  _tex->setSmooth(true);
  return r;
#else
  glGenTextures(1,&_tex);
  glBindTexture(GL_TEXTURE_2D,size);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, null);
  glBindTexture(GL_TEXTURE_2D, null);
  return true;
#endif
}

bool AglDecoder::_createShader()
{
#if defined(SFML_GRAPHICS_HPP)
  _shader=new sf::Shader();
  std::cout << "Code:" << std::endl;
  std::cout << _code << std::endl;
  return _shader->loadFromMemory(_code, sf::Shader::Fragment);
#else
  return false;
#endif

}

bool AglDecoder::_parse()
{
  tinyxml2::XMLHandle hDoc(&_xmldoc);
  tinyxml2::XMLElement* pElem;
  tinyxml2::XMLHandle hRoot(0);
  hRoot=hDoc.FirstChildElement("agl");
  if (pElem=hRoot.ToElement())
  {
    if ((pElem=hRoot.FirstChildElement("length").ToElement())||
        (pElem=hRoot.FirstChildElement("duration").ToElement()))
    {
      //tinyxml2::XMLHandle hLength(pElem);
      if (pElem->QueryDoubleText(&_musiclength)!=tinyxml2::XML_SUCCESS)
      {
#ifndef NDEBUG
        std::cerr << "AglDecoder Error: duration invalid" << std::endl;
#endif
        return false;
      }
    }
    else
    {
#ifndef NDEBUG
      std::cerr << "AglDecoder Error: duration excepted" << std::endl;
#endif
      return false;
    }
    if (pElem=hRoot.FirstChildElement("tags").ToElement())
    {
      tinyxml2::XMLElement* pTags;
      if (pTags=tinyxml2::XMLHandle(pElem).FirstChildElement("name").ToElement())
      {
        if (pTags->GetText())
        {
          setName(utf8ToString_t(pTags->GetText()));
        }
      }
      if (pTags=tinyxml2::XMLHandle(pElem).FirstChildElement("author").ToElement())
      {
        if (pTags->GetText())
        {
          setAuthor(utf8ToString_t(pTags->GetText()));
        }
      }
    }

    if (pElem=hRoot.FirstChildElement("code").ToElement())
    {
      tinyxml2::XMLElement* pCode;
      if (pCode=tinyxml2::XMLHandle(pElem).FirstChildElement("shadertoy").ToElement())
      {
        if (pCode->GetText())
        {
          return _parseShaderToyCode(utf8ToString_t(pCode->GetText()));
        }
      }
      else
      {
#ifndef NDEBUG
      std::cerr << "AglDecoder Error: No compatible code found inside code section" << std::endl;
#endif
      }
    }
    else
    {
#ifndef NDEBUG
      std::cerr << "AglDecoder Error: No code section inside agl section" << std::endl;
#endif
      return false;
    }
  }
#ifndef NDEBUG
      std::cerr << "AglDecoder Error: xml doesn't contains an agl section" << std::endl;
#endif
  return false;

}

string_t AglDecoder::utf8ToString_t(const char *_utf8)
{
  assert(_utf8);
#if defined(STRING_T_IS_SF_STRING)
  return sf::String::fromUtf8(_utf8, &(_utf8[strlen(_utf8)]));
#else
  return _utf8;
#endif
}


bool AglDecoder::_parseShaderToyCode(const std::string& code)
{
  if (!_createTexture(32))
  {
    return false;
  }

#ifdef SFML_GRAPHICS_HPP
  _tex->setView(sf::View(sf::FloatRect(0.f,0.f,32.f,32.f)));
#else
  _pixelsbuffer=(uint8_t*)malloc(32*32*4);
#endif

  char numbuffer[33]={0};
  itoa(sampleRate(),numbuffer,10);

  _code="#version 110\n";
  _code+="#extension GL_OES_standard_derivatives : enable\n";
  _code+="const float signal_frequency=";
  _code+=numbuffer;
  _code+=".0;";

  itoa(Signal::size,numbuffer,10);
  _code+="const float signal_size=";
  _code+=numbuffer;
  _code+=".0;";

  _code+="uniform int frame;";
  _code+="uniform float track_time;\n";
  _code+=code;
  _code+=
    "\nvoid main()"
    "{"
        "int x=int(gl_FragCoord.x+.5);"
        "int y=int(gl_FragCoord.y+.5);"
        "int ipos=x*+(32-y)*32;"
        //"float pos=float(ipos);"
        "float pos=gl_FragCoord.x-.5+(31. - (gl_FragCoord.y-.5))*32.;"
        "if (pos <= signal_size)"
        "{"
          //"float t = track_time + (pos/signal_frequency);"
          "float t = ((float(frame)*signal_size+pos)/signal_frequency);"

          "vec2 y = mainSound( t );"

          "vec2 v  = floor((0.5+0.5*y)*65536.0);"
          "vec2 vl =   mod(v,256.0)/255.0;"
          "vec2 vh = floor(v/256.0)/255.0;"
          "gl_FragColor = vec4(vl.x,vh.x,vl.y,vh.y);"
        "}"
        "else { gl_FragColor = vec4(.5); }"
    "}\n";

  bool r=_createShader();
  if (!r)
  {
    std::cerr << "Failed to compile audio shader" <<std::endl;
  }
  return r;
  //return _createShader();
}
