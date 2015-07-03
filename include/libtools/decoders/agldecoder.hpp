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
  * SECURITY LEVEL : 8
  * VISIBILITY     : PROTECTED
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED
  *
  * FILE         : agldecoder.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.0
  */

#ifndef LIBTOOLS_AGLDECODER_HPP
#define LIBTOOLS_AGLDECODER_HPP
#include <SFML/Graphics.hpp>

#include <libtools/public/config.h>
#include <libtools/core/decoder.hpp>
#include <tinyxml2.h>
#include <GL/gl.h>
#include <GL/glu.h>


class AglDecoder : public MusicDecoder
{
public:

  AglDecoder();
  virtual ~AglDecoder();

  virtual unsigned int fetch(Signal& outleft, Signal& outright);

  virtual bool ended() const;
  virtual void rewind();
  virtual double length() const;

  static string_t utf8ToString_t(const char* utf8);

protected:
  virtual bool _open(const string_t& filename);
  virtual bool _load(const uint8_t* buffer, unsigned int size);
  virtual void _reset();


  bool _createTexture(unsigned size);
  bool _createShader();
  bool _parse(); //Parse the xml


  tinyxml2::XMLDocument _xmldoc;

  bool _parseShaderToyCode(const std::string& code);

private:
  unsigned int _frame;
  double _musiclength;

#if defined(SFML_GRAPHICS_HPP)
  sf::RenderTexture* _tex;
  sf::Shader* _shader;
  sf::Image _image;
#else
  GLuint _tex;
#endif

  std::string _code;
  uint8_t* _pixelsbuffer;



};

#endif
