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

  bool _fillbuffer();


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
  uint8_t* _pixelsbuffer;
#endif
  uint64_t _sample;
  unsigned int _blockoffset;
  std::string _code;

  std::vector<sample> _leftbuffer;
  std::vector<sample> _rightbuffer;


};

#endif
