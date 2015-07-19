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
  * FILE         : decoder.cpp
  * AUTHOR       : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.1
  * DEPENDENCIES : decoder.hpp
  *                config.h
  *                production.hpp
  */

#include <libtools/public/config.h>
#include <libtools/core/decoder.hpp>
#include <assert.h>
  
void MusicDecoder::_infosreset() {
  _filename=string_t();
  _name=string_t();
  _artist=string_t();
  _genre=0;
  _sampleRate=0;
}


//#if defined(STRING_T_IS_SF_STRING)
static void remove_useless_spaces_before(string_t& str){
  while (!string_t_empty(str) && (str[0] == ' ' || str[0] == '_'))
    str.erase(0,1);
}

static void remove_useless_spaces_after(string_t& str){
  while (string_t_size(str) > 0 && (str[string_t_size(str)-1] == ' ' || str[string_t_size(str)-1] == '_'))
    str.erase(string_t_size(str)-1,1);
}

static void remove_track_number(string_t& str) {
  unsigned int end_num=0;
  remove_useless_spaces_before(str);
  for (end_num=0; end_num < string_t_size(str); end_num++)
  {
    if ((char) str[end_num] >= '0' && (char) str[end_num] <= '9') continue;
    else if ((char) str[end_num] == ' ' || 
             (char) str[end_num] == '_' || 
             (char) str[end_num] == ')' || 
             (char) str[end_num] == '.') break;
    else {
      end_num=string_t_size(str)+1;
      break;
    }
  }
  if (end_num < 5 && end_num+1 < string_t_size(str)) {
    str.erase(0,end_num+1);
    remove_useless_spaces_before(str);
  }
}

static void detect_fucking_warez_ad_tag(string_t& str) {
  
  if (string_t_find_success(str.find("(www."))) 
  {
    size_t pos=str.find("(www.");
    str.erase(pos, string_t_size(str));
  }
  else if (string_t_find_success(str.find("(http://"))) 
  {
    size_t pos=str.find("(http://");
    str.erase(str.find("(http://"), string_t_size(str));
    str.erase(pos, string_t_size(str));
  }
  else if (string_t_find_success(str.find("http://"))) 
  {
    size_t pos=str.find("http://");
    str.erase(pos, string_t_size(str));
  }
  else if (string_t_find_success(str.find("www."))) {
    size_t pos=str.find("www.");
    str.erase(pos, string_t_size(str));
  }
  remove_useless_spaces_before(str);
}

static void remove_file_extension(string_t& str) {
  for (int p=string_t_size(str)-1; p > 0; p--) {
    if (str[p]=='.') {
      str.erase(p,string_t_size(str)-p);
      break;
    }
  }
}

//#endif


bool MusicDecoder::open(const string_t& filename){
  reset();
  bool r=this->_open(filename);
  if (r) {
    _filename=filename;
    while (string_t_find_success(_filename.find('/')))
      _filename.erase(0,_filename.find('/')+1);
    while (string_t_find_success(_filename.find('\\')))
      _filename.erase(0,_filename.find('\\')+1);
    
    
    //detect author and name from filename
    string_t partist;
    string_t pname;
    
    #if defined(SFML_STRING_HPP) 
    if (string_t_find_success(_filename.find('-')))
    {
      partist=_filename;
      pname=_filename;
      pname.erase(0,pname.find('-')+1);
      partist.erase(partist.find('-'), string_t_size(partist));
      
      remove_file_extension(pname);
      
      //Remove useless spaces and '_'
      remove_useless_spaces_before(pname);
      remove_useless_spaces_after(pname);
      
      //Detect track number from filename 
      remove_track_number(partist);
      remove_useless_spaces_after(partist);
      
    }
    else {
      //Identifying filename as title
      pname=_filename;
      remove_file_extension(pname);
      remove_track_number(pname);
      remove_useless_spaces_after(pname);
    }
    #endif
    
    if (string_t_empty(_name))
      setName(pname);
    if (string_t_empty(_artist))
      setAuthor(partist);
  }
  else _infosreset();
  return r;
}

bool MusicDecoder::load(const uint8_t* buffer, unsigned int size)
{
  reset();
  return _load(buffer,size);
}

void MusicDecoder::reset()
{
  _infosreset();
  _reset();
}

void MusicDecoder::setName(const string_t& name){
  
  _name=name;
  detect_fucking_warez_ad_tag(_name);
}

void MusicDecoder::setAuthor(const string_t& artist){
  
  _artist=artist;
  detect_fucking_warez_ad_tag(_artist);
}

void MusicDecoder::setGenre(const string_t& genre){
  
}

bool MusicDecoder::parseComment(
    const string_t& field,
    const string_t& value)
{
  if (field==string_t("TITLE"))
  {
    setName(value);
    return true;
  }
  else if (field==string_t("ARTIST"))
  {
    setAuthor(value);
    return true;
  }
  return false;
}


bool MusicDecoder::splitComment(
    const char* comment,
    unsigned int size,
    string_t& field,
    string_t& value)
{
#ifdef STRING_T_IS_SF_STRING
  sf::String str=sf::String::fromUtf8(comment, &(comment[size]));
  size_t split=str.find(sf::String("="));
  if (split==sf::String::InvalidPos){
    return false;
  } else {
    field=str.substring(0,split);
    value=str.substring(split+1);
    sf::String::Iterator it;
    for (it=field.begin(); it !=field.end(); it++)
    {
      *it=toupper(*it);
    }

    //std::cout << "field " << field.toAnsiString() << "=" << value.toAnsiString() << std::endl;
    return true;
  }
#else
  return false;
#endif
}


MusicDecoders::MusicDecoders():
  MusicDecoder("no decoders"),
  _currentDecoder(NULL)
{
}

MusicDecoders::~MusicDecoders()
{
  std::vector<MusicDecoder*>::iterator it;
  for (it=_decoders.begin();it != _decoders.end(); it++)
  {
    delete *it;
  }
}

bool MusicDecoders::ended() const
{
  if (_currentDecoder)
  {
    return _currentDecoder->ended();
  }
  return true;
}
void MusicDecoders::rewind()
{
  if (_currentDecoder)
  {
    _currentDecoder->rewind();
  }
}

double MusicDecoders::length() const
{
  if (_currentDecoder)
  {
    return _currentDecoder->length();
  }
  return 0.;
}


bool MusicDecoders::openWithDecoder(const string_t& filename, MusicDecoder& decoder)
{
  if (decoder.open(filename))
  {
    if (&decoder != _currentDecoder)
    {
      reset();
      _currentDecoder=&decoder;
    }
    _transmitInfos();
    return true;
  }
  return false;
}

bool MusicDecoders::_open(const string_t& filename)
{

  std::vector<MusicDecoder*>::iterator it;
  for (it=_decoders.begin();it != _decoders.end(); it++)
  {
    if ((*it)->_open(filename))
    {
      _currentDecoder=*it;
      _transmitInfos();
      return true;
    }
  }
  _currentDecoder=NULL;
  return false;
}

bool MusicDecoders::_load(const uint8_t* buffer, unsigned int data)
{
  std::vector<MusicDecoder*>::iterator it;
  for (it=_decoders.begin();it != _decoders.end(); it++)
  {
    if ((*it)->_load(buffer,data))
    {
      _currentDecoder=*it;
      _transmitInfos();
      return true;
    }
  }
  _currentDecoder=NULL;
  return false;
}

void MusicDecoders::_reset()
{
  if (_currentDecoder)
  {
    _currentDecoder->_reset();
  }
}


void MusicDecoders::_transmitInfos()
{
  //if (_currentDecoder)
  assert(_currentDecoder);
  {
    setName(_currentDecoder->name());
    setAuthor(_currentDecoder->author());
    _genre=_currentDecoder->genre();
    _sampleRate=_currentDecoder->sampleRate();
    _decodername=_currentDecoder->decoder();
  }
}


void MusicDecoders::append(MusicDecoder* decoder)
{
  assert(decoder);
  _decoders.push_back(decoder);
}


unsigned int MusicDecoders::fetch(Signal& outleft, Signal& outright)
{
  if (_currentDecoder)
  {
    return _currentDecoder->fetch(outleft,outright);
  }
  return 0;
}


