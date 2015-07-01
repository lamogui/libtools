

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
  if (!_pixelsbuffer || !_tex || !_shader)
  {
    return 0;
  }

  sf::RectangleShape shape(sf::Vector2f(32.f,32.f));
  _tex->setActive(true);
  sf::Shader::bind(_shader);
  _tex->draw(shape);
  sf::Shader::bind(NULL);
  sf::Texture::bind(&(_tex->getTexture()));
  glGetTexImage(_tex->getTexture().getNativeHandle(),0,GL_RGBA,GL_UNSIGNED_BYTE,(void*)&_pixelsbuffer);
  sf::Texture::bind(NULL);

  uint8_t* end=&(_pixelsbuffer[Signal::size*4]);
  unsigned k=0;
  for (uint8_t* it=_pixelsbuffer; it < end; it+=4)
  {
    int16_t l=it[0] | (it[1] << 8);
    int16_t r=it[2] | (it[3] << 8);
    outleft.samples[k]=static_cast<float>(l)/65536.f;
    outright.samples[k++]=static_cast<float>(l)/65536.f;
  }
#else

#endif
  _frame++; //always
  return Signal::size;
}

bool AglDecoder::ended() const
{
  return (_musiclength < static_cast<float>(_frame)/static_cast<float>(_sampleRate));
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
  return _tex->create(size,size,false);
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

static const char* _vertex_shader_code=
    "void main()"
    "{"
    "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
    "gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"
    "gl_FrontColor = gl_Color;"
    "}";

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
  return sf::String::fromUtf8(_utf8, &(_utf8[strlen(_utf8)+1]));
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
  _pixelsbuffer=(uint8_t*)malloc(32*32*4);

#ifdef SFML_GRAPHICS_HPP
  _tex->setView(sf::View(sf::FloatRect(0.f,0.f,32.f,32.f)));
#else
#endif

  char numbuffer[33]={0};
  itoa(sampleRate(),numbuffer,10);

  _code="#version 110\n";
  _code+="const float signal_frequency=";
  _code+=numbuffer;
  _code+=".0;";

  itoa(Signal::size,numbuffer,10);
  _code+="const float signal_size=";
  _code+=numbuffer;
  _code+=".0;";

  _code+="uniform float track_time;\n";
 // _code+=code;
  _code+="vec2 mainSound(float time)"
         "{"
         "return vec2(cos(6.*200.*time));"
         "}";
  _code+=
    "\nvoid main()"
    "{"
        "float pos=gl_FragCoord.x + gl_FragCoord.y*32.0;"
        "if (pos <= signal_size)"
        "{"
          "float t = track_time + (pos/signal_frequency);"

          "vec2 y = mainSound( t );"

          "vec2 v  = floor((0.5+0.5*y)*65536.0);"
          "vec2 vl =   mod(v,256.0)/255.0;"
          "vec2 vh = floor(v/256.0)/255.0;"
          "gl_FragColor = vec4(vl.x,vh.x,vl.y,vh.y);"
        "}"
        "else { gl_FragColor = vec4(.0,.0,.0,.0); }"
    "}\n";

  bool r=_createShader();
  if (!r)
  {
    std::cerr << "Failed to compile audio shader" <<std::endl;
  }
  return r;
  //return _createShader();
}
