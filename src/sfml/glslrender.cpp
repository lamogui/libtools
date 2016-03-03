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

 #include <iostream>
bool GLSLRender::shortcutLoadFSFromFile(const std::string& fragment_file)
{
  FILE* f=fopen(fragment_file.c_str(),"rb");
  if (f)
  {
    fseek(f,0,SEEK_END);
    uint64_t size=ftell(f);
    fseek(f,0,SEEK_SET);
    char* buffer=(char*)malloc(size+1);
    memset((void*)buffer,0,size+1);
    fread((void*)buffer,1,size,f);
    fclose(f);
    //std::cout << "file:\n" << buffer << std::endl;
    bool success=loadFromMemory(GLSLRender::getVertexShaderCode(),buffer);
    free(buffer);
    return success;
  }
  return false;
}
