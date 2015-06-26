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
