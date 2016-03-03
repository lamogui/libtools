
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
  * FILE         : audiostream.hpp
  * AUTHOR       : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0
  * DEPENDENCIES : signal.hpp
  *                config.h
  * ROLE         : Permet de convertir les signaux en flux 
  *                tendu pour la carte son
  */


#ifndef LIBTOOLS_AUDIOSTREAM_HPP
#define LIBTOOLS_AUDIOSTREAM_HPP

#include <libtools/public/config.h>
#include <libtools/core/signal.hpp>



//AudioStream is just a fixed size FIFO buffer
// IN -> audio to diffuse
// OUT -> audio diffused
//But it can be used in invert
//You can lock() an audioStream before use it if you use threads
//All length variables are in samples (not in bytes)
class LIBTOOLS_PRIVATE AudioStream : public mutex_t
{
  public:
    AudioStream(unsigned int buffer_length);
    virtual ~AudioStream(); 
    
    //you can resize the buffer only if there is no samples inside (all samples are free)
    bool setBufferLength(unsigned int buffer_length);
    
    //return buffer size in samples
    inline unsigned int getBufferLength()
    {
      return _length;
    }
    
    //try to read samples into a buffer, return the number of samples readed
    unsigned int read(short* buffer, unsigned int len);
    //try to write samples into the buffer, return the number of samples writed
    unsigned int write(const short* buffer, unsigned int len);
    
    //try to write an entire signal into the buffer return true if succeed
    bool writeSignal(const Signal& signal); 
    bool writeStereoSignal(const Signal& left,const Signal& right); 
    //try to read an entire signal from the buffer return true if succeed
    //bool readSignal(Signal& signal); //TODO
    
    inline unsigned int getAvailableSamplesCount()
    {
      return _count;
    }    
    inline unsigned int getFreeSamplesCount()
    {
      return _length-_count;
    }

  protected:
    short* _buffer;
    short* _end; //end of the buffer
    short* _pwrite; // pointeur d’écriture
    short* _pread; // pointeur de lecture
    unsigned int _length; //size in samples
    unsigned int _count; //nombre de samples disponibles
  
};

/*
//DirectOutputAudioStream is an audiostream using driver buffers
//When using an DirectOutputAudioStream there is no need of an AudioDriver
//because the driver is included in the stream
//There should only have once DirectOutputAudioStream at time
//All length variables are in samples (not in bytes)
class LIBTOOLS_PRIVATE DirectOutputAudioStream
{
  public:
    DirectOutputAudioStream(); //Creating the object may requiert AudioDriver Object
    virtual ~DirectOutputAudioStream();
  
    //Initialize the driver (can fail)
    virtual bool init(unsigned int rate)=0;
    
    //free the driver
    virtual void free()=0;

    //Push a signal block into driver buffers
    virtual bool pushStereoSignal(const Signal& left,const Signal& right)=0; 
    
    //Return unplayed samples (at least an estimation)
    //virtual unsigned int getAvailableSamplesCount()=0;
    
    //Return played samples (at least an estimation)
    virtual unsigned int getFreeSamplesCount()=0;
};

//DirectInputAudioStream provide samples access from real time recorded input
//directly from the driver buffers
//If possible convert all intos Signals
class DirectInputAudioStream
{
  public:
    DirectInputAudioStream();  //Creating the object may requiert AudioDriver Object
    virtual ~DirectInputAudioStream();

    virtual bool start()=0;
    virtual void stop()=0;

    virtual bool active()=0;

    //Try to pop a signal block from driver buffers
    virtual bool popStereoSignal(Signal& left,Signal& right)=0; 
};
*/
#endif

