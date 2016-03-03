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
  * FILE         : sampler1d.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  * DEPENDENCIES : sampler1d.hpp
  */


#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <libtools/sfml/sampler1d.hpp>
#include <iostream>

#define glCheck(x) x

Sampler1D::Sampler1D() :
_realSize(0),
_size(0),
_smooth(false),
_isRepeated(false)
{
}

Sampler1D::~Sampler1D()
{
}


bool Sampler1D::create(unsigned int size, const float* data)
{
  unsigned int realSize = GlTexture::getValidSize(size);
  unsigned int maxSize = GlTexture::getMaximumSize();
  if (realSize > maxSize)
  {
    std::cerr << "Failed to create texture, its internal size is too high "
    << "maximum is " << maxSize << std::endl;
    return false;
  }

  _realSize=realSize;
  _size=size;

  ensureGlContext();

  if (!m_texture)
  {

    GLuint texture;
    glCheck(glGenTextures(1, &texture));
    m_texture = static_cast<unsigned int>(texture);
    glCheck(glEnable(GL_TEXTURE_1D));
  }

  glCheck(glBindTexture(GL_TEXTURE_1D, m_texture));
  glCheck(glTexImage1D( GL_TEXTURE_1D, 0, GL_LUMINANCE, _realSize, 0, GL_LUMINANCE, GL_FLOAT, (const GLvoid*) 0 ));
  glCheck(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, _isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
  glCheck(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, _isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
  glCheck(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, _smooth ? GL_LINEAR : GL_NEAREST));
  glCheck(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, _smooth ? GL_LINEAR : GL_NEAREST));
  glCheck(glBindTexture(GL_TEXTURE_1D, 0));

  update(data, size);

  return true;
}

void Sampler1D::update(const float* data){
  update(data, _size);
}

void Sampler1D::update(const float* data, unsigned int size, unsigned int x)
{
  if (data && m_texture)
  {
    ensureGlContext();

    // Copy pixels from the given array to the texture
    glCheck(glBindTexture(GL_TEXTURE_1D, m_texture));
    glCheck(glTexSubImage1D(GL_TEXTURE_1D, 0, x, size, GL_LUMINANCE, GL_FLOAT, (const GLvoid*) data));
    glCheck(glBindTexture(GL_TEXTURE_1D, 0));
  }
}

void Sampler1D::bind() const
{
  ensureGlContext();
  glCheck(glBindTexture(GL_TEXTURE_1D, m_texture));
  if (m_texture && _size != _realSize)
  {
    GLfloat matrix[16] = {1.f, 0.f, 0.f, 0.f,
                          0.f, 1.f, 0.f, 0.f,
                          0.f, 0.f, 1.f, 0.f,
                          0.f, 0.f, 0.f, 1.f};
    matrix[0] = (float) _size /(float)_realSize;
    // Load the matrix
    glCheck(glMatrixMode(GL_TEXTURE));
    glCheck(glLoadMatrixf(matrix));

    // Go back to model-view mode (sf::RenderTarget relies on it)
    glCheck(glMatrixMode(GL_MODELVIEW));
  }
}

void Sampler1D::setSmooth(bool smooth)
{
  if (smooth != _smooth)
  {
      _smooth = smooth;

      if (m_texture)
      {
          ensureGlContext();

          glCheck(glBindTexture(GL_TEXTURE_1D, m_texture));
          glCheck(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, _smooth ? GL_LINEAR : GL_NEAREST));
          glCheck(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, _smooth ? GL_LINEAR : GL_NEAREST));
      }
  }
}

void Sampler1D::setRepeated(bool repeated)
{
  if (repeated != _isRepeated)
  {
      _isRepeated = repeated;

      if (m_texture)
      {
          ensureGlContext();

          glCheck(glBindTexture(GL_TEXTURE_1D, m_texture));
          glCheck(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, _isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
          glCheck(glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, _isRepeated ? GL_REPEAT : GL_CLAMP_TO_EDGE));
      }
  }
}
