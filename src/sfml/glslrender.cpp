/*

THIS FILE IS SUBJECT TO STRICT RULES OF BDE NE666 COPYDOWN. 
NOBODY IS PERMITTED TO COPY AND DISTRIBUTE VERBATIM OR MODIFIED COPIES OF
THIS LICENSE HEADER. A SECURITY LEVEL IS ASSIGNED TO THE FILE AND A VISIBILITY.
THIS FILE MUST NOT BE COPIED OR REDISTRIBUTED IF IT'S VISIBILITY IS NOT PUBLIC. 
VISIBILITY HAS 3 POSSIBLE STATES:
- PRIVATE: LIMITED TO PROJECTS WITH THE SAME SECURITY LEVEL WITHIN THE BDE NE666. (DEFAULT VISIBILITY)
- PROTECTED: LIMITED TO PROJECTS USING THE SAME SECURITY RULES WITHIN THE BDE NE666. 
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
  * FILE         : glslrender.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  * DEPENDENCIES : glslrender.hpp
  */

#include <libtools/sfml/glslrender.hpp>


GLSLRender::GLSLRender():
_rectangle(sf::Vector2f(1.f,1.f))
{
  _rectangle.setOrigin(sf::Vector2f(.5f,.5f));
}

GLSLRender::~GLSLRender()
{
}
    
void GLSLRender::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  sf::Shader::bind(this);
  target.draw(_rectangle,states);
  sf::Shader::bind(NULL);
}


std::string GLSLRender::getVertexShaderCode() {
  static std::string vertexShader = "varying vec2 v;"
  ""
  "void main()"
  "{"
      "gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
      "v           = gl_Vertex.xy;"
      "gl_TexCoord[0] = gl_MultiTexCoord0;"
  "}";
  return vertexShader;
}

