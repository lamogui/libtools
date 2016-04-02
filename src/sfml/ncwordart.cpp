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
  * THIS FILE IS PART OF NOISECONTROLLERS 
  * SECURITY LEVEL : 8 (CRITICAL)  
  * VISIBILITY     : PRIVATE
  * © COPYDOWN LAMOGUI ALL RIGHTS RESERVED
  *
  * FILE         : ncwordart.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  */
  
 #include <libtools/sfml/ncwordart.hpp>

const std::string NCWordArt::fadeFragmentShader=
"uniform sampler2D texture;"
"uniform float t;"
"uniform float duration;"
"varying float start;"
"void main()"
"{"
    // récupère le pixel dans la texture
    "vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);"
    "float a=pixel.a;"
    "if (t < start) "
      "a=0.0;"
    "else if ( t <= start + 0.1) "
      "a*=pow((t-start)/0.1,2.0);"
    "else if (t > duration-1.0) "
      "a*=pow(duration-t,2.0);"
    "pixel=vec4(pixel.rgb,a);"
    // et multiplication avec la couleur pour obtenir le pixel final"
    "gl_FragColor = gl_Color * pixel;"
"}";

const std::string NCWordArt::cosVertexShader=
"uniform float t;"
"uniform vec2 size;"
"uniform int len;"
"varying float start;"
"void main()"
"{"
    "float anim_t=1.1;"
    // transforme la position du vertex
    "vec4 vertex=gl_Vertex;"
    "float y=0.0;"
    "float i=floor((vertex.x/size.x)*len);"
    "float len_t=anim_t/len;"
    "start=i*len_t;"
    "float my_t=t;"
    "if (my_t >= start && my_t < anim_t) {"
      "y=sin((my_t-start)/len_t)*size.y*0.2*pow((t-anim_t),2.0);"
      "vertex+=vec4(0.0,y,0.0,0.0);"
    "}"
    
    "vec4 position = gl_ModelViewProjectionMatrix * vertex;"
    "gl_Position=position;"
    // transforme les coordonnées de texture
    "gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"
    // transmet la couleur
    "gl_FrontColor = gl_Color;"
"}";

const std::string NCWordArt::staticVertexShader=
"uniform float t;"
"uniform vec2 size;"
"uniform int len;"
"varying float start;"
"void main()"
"{"
    "float anim_t=1.1;"
    // transforme la position du vertex
    "vec4 vertex=gl_Vertex;"
    "float i=floor((vertex.x/size.x)*len);"
    "float len_t=anim_t/len;"
    "start=i*len_t;"
    "vec4 position = gl_ModelViewProjectionMatrix * vertex;"
    "gl_Position=position;"
    // transforme les coordonnées de texture
    "gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"
    // transmet la couleur
    "gl_FrontColor = gl_Color;"
"}";
  

NCWordArt::NCWordArt(const sf::Font& font):
_text("",font,30),
_shader(NULL),
_animationTime(0),
_len(0)
{
  
}

NCWordArt::~NCWordArt()
{
  
}

void NCWordArt::start(
      const string_t str,
      float length,
      sf::Shader* shader,
      const sf::Color& color)
{
  _shader=shader;
  _text.setColor(color);
  _text.setString(str);
  _animationTime=length;
  sf::FloatRect bounds=_text.getLocalBounds();
  _size=sf::Vector2f(bounds.width,bounds.height);
  _text.setOrigin(_size*0.5f);
  _len=string_t_size(str);
  _timer.restart();
  
  if (_shader)
  {
    _shader->setParameter("duration",_animationTime);
    _shader->setParameter("size",_size);
    _shader->setParameter("len",(int)_len);
  }
}

void NCWordArt::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  float t=_timer.getElapsedTime().asSeconds();
  if (t < _animationTime)
  {
    states.transform *= getTransform();
    if (_shader)
    {
      _shader->setParameter("t",t);
      sf::Shader::bind(_shader);
      target.draw(_text,states);
      sf::Shader::bind(NULL); 
    }
    else 
    {
      target.draw(_text,states);
    }
  }
}
