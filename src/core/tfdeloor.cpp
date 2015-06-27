

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
  * FILE         : tfdeloor.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  * DEPENDENCIES : tfdeloor.hpp
  */


#include <libtools/core/tfdeloor.hpp>
  
TFDeLoor::TFDeLoor()
{
  //Compute DeLoor Factors
  for (unsigned int n=0; n<NB_NOTES;n++) {
    const float f=Note::getFrequencyFromID(n);
    for (unsigned int i=0; i<TFDeLoor_SIZE;i++) {
       _deloorFactor[n][i]=cos(-2.f*3.14159265359f*f*i/(float)Signal::frequency);
       _deloorFactorI[n][i]=sin(-2.f*3.14159265359f*f*i/(float)Signal::frequency);
    }
    const unsigned size = (unsigned int)(2.f*(float)Signal::frequency/f+0.5);
    if (size < Signal::size) 
      _bufferNeeded[n]=Signal::size;
    else 
      _bufferNeeded[n]=size;
  }

}

TFDeLoor::~TFDeLoor()
{
}

    
//Direct immediate compute from samples (with truncate )
void TFDeLoor::compute(const sample* s, unsigned int size) {
  for (unsigned int n=0; n<NB_NOTES;n++) {
    _real[n]=0;
    _imaginary[n]=0;
    const unsigned int bsize=_bufferNeeded[n];
    if (bsize > size) 
      continue;
    for (unsigned int i=size-bsize; i<size;i++) {
       _real[n]+=s[i]*_deloorFactor[n][i];
       _imaginary[n]+=s[i]*_deloorFactorI[n][i];
    }
  }
}

void TFDeLoor::compute(const Signal &s)
{
  compute(s.samples, Signal::size);
}

//push signal and compute when have enough samples
void TFDeLoor::pushSignal(const Signal &s){
  /*const unsigned int copy_size = _inputOffset + Signal::size > TFDeLoor_SIZE ? TFDeLoor_SIZE : _inputOffset + Signal::size;
  unsigned int i=0;
  while (_inputOffset< copy_size)
    _input[_inputOffset++] = s.samples[i++];
  if (_inputOffset==TFDeLoor_SIZE) _inputOffset = 0;
  if (TFDeLoor_SIZE > Signal::size) {
	  while (i<Signal::size)
		_input[_inputOffset++] = s.samples[i++]; 
  }*/

#ifdef _MSC_VER
  memmove_s((void*)(_input),TFDeLoor_SIZE*sizeof(sample),(void*)(_input+Signal::size),TFDeLoor_SIZE*sizeof(sample)-Signal::byteSize);
#else
  memmove((void*)(_input),(void*)(_input+Signal::size),TFDeLoor_SIZE*sizeof(sample)-Signal::byteSize);
#endif
  const unsigned a=TFDeLoor_SIZE-Signal::size;
  for (unsigned int i=0; i < Signal::size; i++){
    _input[a+i]=s.samples[i];
  }

  compute(_input,TFDeLoor_SIZE);
}

void TFDeLoor::computeModule() {
  if (_real && _imaginary) {
    //static float maxi=0;
    for (unsigned int i=0; i<NB_NOTES; i++) {
      _module[i]=2.f*(sample)sqrt((_real[i]*_real[i] + _imaginary[i]*_imaginary[i]))/(float)_bufferNeeded[i];
    }
    //maxi=std::max(maxi,*(std::max_element(_module, _module+_size)));
    //std::cout << maxi << std::endl;
  }
}

#ifdef SFML_THREAD_HPP

TFDeLoorPrecomputer::TFDeLoorPrecomputer() :
  _onAnalyse(false),
  _thread(&TFDeLoorPrecomputer::_compute,this),
  _frames(0),
  _progress(0),
  _decoder(0),
  _table(0)
{


}

TFDeLoorPrecomputer::~TFDeLoorPrecomputer() 
{
  if(_table) free(_table);
}

bool TFDeLoorPrecomputer::start(MusicDecoder* decoder)
{
  sf::Lock lock(_mutex);
  _progress=0;
  _decoder=decoder;
  if (_decoder) {   
    _onAnalyse=true;
    _frames=_decoder->length()/Signal::refreshRate;
    _thread.launch();
    return true;
  }
  return false;
}

void TFDeLoorPrecomputer::_compute()
{
  _mutex.lock();
  _progress=0;
  _table=(sample*)realloc((void*)_table,_frames*NB_NOTES*sizeof(sample));
  
  Signal l,r;
  TFDeLoor* ltfd=new TFDeLoor; 
  TFDeLoor* rtfd=new TFDeLoor; 
  for (unsigned int k =0; k < _frames; k++) 
  {
    _mutex.unlock();
    _decoder->fetch(l,r);
    ltfd->pushSignal(l);
    rtfd->pushSignal(r);
    ltfd->computeModule();
    rtfd->computeModule();
    sample* c = _table + k*NB_NOTES;
    for (unsigned int n=0; n < NB_NOTES; n++) {
      c[n] = ltfd->getModule()[n]*0.5f + rtfd->getModule()[n]*0.5f;
    }

    _mutex.lock();
    _progress++;
  }
  _mutex.unlock();
  delete ltfd;
  delete rtfd;
}

#endif
