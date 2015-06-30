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
  * FILE         : glslrender.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  * DEPENDENCIES : glslrender.hpp
  *                shader.h
  */

#include <libtools/sfml/glslrender.hpp>
#include <libtools/shaders/shaders.h>
#include <cstdlib>
#include <cstring>

GLSLRender::GLSLRender():
_rectangle(sf::Vector2f(1.f,1.f))
{
  _rectangle.setOrigin(sf::Vector2f(.5f,.5f));
  _rectangle.setFillColor(sf::Color(255,255,255,0));
}

GLSLRender::~GLSLRender()
{
}
    
void GLSLRender::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  sf::Shader* s=const_cast<GLSLRender*>(this); //olol
  s->setParameter("xy_scale_factor",getScale().x/getScale().y);
  sf::Shader::bind(this);
  target.draw(_rectangle,states);
  sf::Shader::bind(NULL);
}


std::string GLSLRender::getVertexShaderCode() {
  return coords_vs_src;
}


bool GLSLRender::loadFromFile(const std::string& fragment_file)
{
  FILE* f=fopen(fragment_file.c_str(),"r");
  if (f)
  {
    fseek(f,0,SEEK_END);
    unsigned int size=ftell(f);
    fseek(f,0,SEEK_SET);
    char* buffer=(char*)malloc(size+5);
    memset((void*)buffer,0,size+5);
    fread((void*)buffer,1,size,f);
    fclose(f);
    bool success=loadFromMemory(GLSLRender::getVertexShaderCode(),buffer);
    free(buffer);
    return success;
  }
  return false;
}
