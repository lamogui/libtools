
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
  * FILE         : ncprecomputer.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.7 olol
  */
  

#include <libtools/core/ncprecomputer.hpp>

#include <list>
#include <iostream>


template<typename T> 
static bool compare_values (const T& first, const T& second)
{
  return ( first < second );
}

template<typename T> 
void NCPrecomputer::medianFilter(
                             const std::vector<T>& brut,
                             std::vector<T>& low,
                             std::vector<T>& med,
                             std::vector<T>& high,
                             const unsigned int samples)
{
  std::list<T> values, ordered;
  const unsigned int avance=samples*2 + 1;
  if (brut.size() <= avance) return;
  
  low.reserve(brut.size());
  med.reserve(brut.size());
  high.reserve(brut.size());
  
  for (unsigned int i=0; i < samples; i++)
  {
    values.push_back(0);
  }
  for (unsigned int i=samples; i <= avance; i++)
  {
    values.push_back(brut[i-samples]);
  }
  
  const unsigned int size=brut.size()-samples;
  for (unsigned int i=0; i < size; i++)
  {
    ordered=values;
    ordered.sort(compare_values<T>);
    
    typename
    std::list<T>::iterator mid=ordered.begin();
    std::advance(mid, samples);
    
    low.push_back(ordered.front());
    med.push_back(*mid);
    high.push_back(ordered.back());
    
    values.pop_front();
    values.push_back(brut[i+samples]);
  }
  
  for (unsigned int i=size; i < brut.size();i++)
  {
    ordered=values;
    ordered.sort(compare_values<T>);
    
    typename
    std::list<T>::iterator mid=ordered.begin();
    std::advance(mid, samples);
    
    low.push_back(ordered.front());
    med.push_back(*mid);
    high.push_back(ordered.back());
    
    values.pop_front();
    values.push_back(0);
  }
}

template<typename T>
void NCPrecomputer::averageFilter(
  const std::vector<T>& brut,
  std::vector<T>& aver,
  const unsigned int samples)
{
  const unsigned int avance=samples*2 + 1;
  if (brut.size() <= avance) return;
  
  T sum=0;
  aver.reserve(brut.size());
  const T divisor=avance;
  
  
  for (unsigned int i=0; i <= samples; i++)
  {
    sum+=brut[i]; 
  }
  for (unsigned int i=0; i < samples; i++)
  {
    aver.push_back(sum/divisor);
    sum+=brut[i+samples+1]; 
  }
  
  
  const unsigned int size=brut.size()-samples-1;
  for (unsigned int i=samples; i < size; i++)
  {
    aver.push_back(sum/divisor);
    
    sum+=brut[i+samples+1];
    sum-=brut[i-samples];
  }
  
  for (unsigned int i=size; i < brut.size();i++)
  {
    aver.push_back(sum/divisor);
    sum-=brut[i-samples];
  }
}


//Specialiation int16_t
template<>
void NCPrecomputer::averageFilter(
  const std::vector<int16_t>& brut,
  std::vector<int16_t>& aver,
  const unsigned int samples)
{
  const unsigned int avance=samples*2 + 1;
  if (brut.size() <= avance) return;
  
  int64_t sum=0;
  aver.reserve(brut.size());
  const unsigned int divisor=avance;
  
  for (unsigned int i=0; i <= samples; i++)
  {
    sum+=(int64_t)brut[i]; 
  }
  for (unsigned int i=0; i < samples; i++)
  {
    aver.push_back(sum/divisor);
    sum+=(int64_t)brut[i+samples+1]; 
  }
  
  
  const unsigned int size=brut.size()-samples-1;
  for (unsigned int i=samples; i < size; i++)
  {
    aver.push_back(sum/divisor);
    
    sum+=brut[i+samples+1];
    sum-=brut[i-samples];
  }
  
  for (unsigned int i=size; i < brut.size();i++)
  {
    aver.push_back(sum/divisor);
    sum-=brut[i-samples];
  }
  
  //std::cerr << "v[samples] " << aver[samples] << std::endl;
}

template<>
void NCPrecomputer::averageFilter(
  const std::vector<float>& brut,
  std::vector<float>& aver,
  const unsigned int samples)
{
  const unsigned int avance=samples*2 + 1;
  if (brut.size() <= avance) return;
  
  int64_t sum=0;
  aver.reserve(brut.size());
  const double divisor=avance*100000.0;
  
  for (unsigned int i=0; i <= samples; i++)
  {
    sum+=brut[i]*(100000.0); 
  }
  for (unsigned int i=0; i < samples; i++)
  {
    aver.push_back((double)sum/(double)(divisor));
    sum+=brut[i+samples+1]*(100000.0); 
  }
  
  
  const unsigned int size=brut.size()-samples-1;
  for (unsigned int i=samples; i < size; i++)
  {
    aver.push_back((double)sum/(double)(divisor));
    
    sum+=brut[i+samples+1]*100000.0;
    sum-=brut[i-samples]*100000.0;
  }
  
  for (unsigned int i=size; i < brut.size();i++)
  {
    aver.push_back((double)sum/(double)(divisor));
    sum-=brut[i-samples]*100000.0;
  }
}




NCPrecomputer::NCPrecomputer() :
_thread(&NCPrecomputer::_thread_func, this),
_left(),
_right(),
_fullyDecoded(false),
_fullyComputed(false),
_endOfThread(true),
_frames(0),
_currentFrame(0),
_maxframes(Signal::refreshRate*60*9), //9 minutes max
_precomputationTime(0.f),
_computationprogress(0.f),
_name(),
_artist(),
_genre()
{
  reset();
}

NCPrecomputer::~NCPrecomputer()
{
}

void NCPrecomputer::reset()
{
  _computationprogress=0.f;
  _left.clear();
  _right.clear();
  
  _noise.clear();
  _clean.clear();
  _bass.clear();
  _power.clear();
  //_level.clear();

  unsigned int max_samples=_maxframes*Signal::size; 
  _left.reserve(max_samples);
  _right.reserve(max_samples);
  
  _noise.reserve(_maxframes);
  _clean.reserve(_maxframes);
  _bass.reserve(_maxframes);
  _power.reserve(_maxframes);
  //_level.reserve(_maxframes);

  _fullyDecoded=false;
  _fullyComputed=false;
  
  _frames=0;
  _currentFrame=0;
  
  _name=string_t();
  _artist=string_t();
  
  additionalReset();

}

bool NCPrecomputer::process(MusicProduction& decoder)
{
  _chrono.restart();
  reset();
  
  decoder.rewind();
  _name=decoder.name();
  _artist=decoder.author();
  _genre=decoder.genre();

  const unsigned int signal_size=Signal::size;
  
  Signal leftdecode, rightdecode;
  VisualSignal visual;
  FFT leftfft(1024);
  
  float p=0.0;
  unsigned int fps=Signal::refreshRate;
  float length=decoder.length();
  std::cout << "l " << length << std::endl;
  while (decoder.fetch(leftdecode, rightdecode)>0 && _frames < _maxframes)
  {
    for (unsigned int k=0; k < signal_size; k++)
    {
      _left.push_back(leftdecode.samples[k]);
      _right.push_back(rightdecode.samples[k]);
    }
    visual.update(leftdecode);
    leftfft.pushSignal(leftdecode);
    leftfft.computeModule();
    _bass.feed(leftfft.bass());
    _noise.feed(visual.noise);
    _clean.feed(visual.clean);
    _power.feed(leftdecode.power());
    //_level.feed(leftdecode.level());
    additionalSignalComputation(leftdecode,rightdecode);
    _frames++;
    p=_frames/fps;///decoder.length()*fps;
    p/=length;
    p*=0.8f;
    _computationprogress=p;
  }
  
  if (!_left.size())
  {
    _precomputationTime=_chrono.getElapsedTime().asSeconds();
    return false;
  }
  _fullyDecoded=decoder.ended();
  
  _noise.finalize();
  _computationprogress=0.8f;
  _bass.finalize();
  _computationprogress=0.85f;
  _power.finalize();
  _computationprogress=0.9f;
  //_level.finalize();
  _clean.finalize();
  _computationprogress=0.95f;

  additionalFinalize();
  std::cout << "Bass tempo" << std::endl;
  _bass.computeHighTempo();
  std::cout << "Noise tempo" << std::endl;
  _noise.computeHighTempo();
  
  
  _fullyComputed=true;
  _precomputationTime=_chrono.getElapsedTime().asSeconds();
  std::cout << "Computed " << _frames << " frames in " << _precomputationTime << " sec" << std::endl;
  _computationprogress=1.f;
  return true;
}

unsigned int NCPrecomputer::fetch(Signal& outleft, Signal& outright)
{
  assert(this->threadReady());
  if (!ended())
  {
    const unsigned int start=Signal::size*_currentFrame;
    const unsigned int signal_size=Signal::size;
    
    for (unsigned int k=0; k < signal_size; k++)
    {
      outleft.samples[k]=_left[k+start];
      outright.samples[k]=_right[k+start];
    }
    _currentFrame++;
    return signal_size;
  }
  return 0;
}

bool NCPrecomputer::getFrame(ncprecomputerframe& frame, unsigned int i)
{
  assert(this->threadReady());
  if (i < _frames)
  {
    _noise.getValueFrame(frame.noise,i);
    _clean.getValueFrame(frame.clean,i);
    _bass.getValueFrame(frame.bass,i);
    _power.getValueFrame(frame.power,i);
    //_level.getValueFrame(frame.level,i);
    
    return true;
  }
  return false;
}


template<typename T>
float NCPrecomputer::computeTempo(const std::vector<T>& brut,
                                  const std::vector<T>& filtered,
                                  const std::vector<T>& more_filtered)
{
  const float bpm_max=160.f;
  const float bpm_min=50.f;
  const unsigned int size=brut.size();
  const int dist_min = (int)((Signal::refreshRate/(bpm_max/60.f)));
  const int dist_max = (int)((Signal::refreshRate/(bpm_min/60.f))+0.5f);

  if (size != filtered.size() || size != more_filtered.size())
  {
    std::cerr << "error: unable to compute tempo with different size tables" << std::endl;
  }

  std::cout << "tempo computation min dist " << dist_min << " max dist " << dist_max << std::endl;

  bool ignore=false;
  int last_beat=0;
  std::list<int> distances;
  for (unsigned int i=0; i < size; i++)
  {
    if (brut[i] >= filtered[i] && filtered[i] > more_filtered[i])
    {
      if (!ignore)
      {
        int dist=i-last_beat;
        if (dist >= dist_min && dist <= dist_max)
        {
          last_beat=i;
          ignore=true;
          distances.push_back(dist);
        }
        else if (dist > dist_max)
        {
          last_beat=i;
          ignore=true;
        }
      }
    }
    else
    {
      ignore=false;
    }
  }
  distances.sort();

  int64_t sum_dist=0;
  std::list<int>::const_iterator it;
  for (it=distances.begin();it!=distances.end();it++)
  {
    sum_dist+=*it;
  }

  it=distances.begin();
  unsigned int distances_size=distances.size();
  std::advance(it,distances_size/2);
  double mean_dist=((double)sum_dist/(double)distances_size);
  double med_dist=(double)*it;

  sum_dist=0;
  it=distances.begin();
  std::advance(it,distances_size/2 - distances_size/8);
  for (unsigned int i=0; i < distances_size/4; i++)
  {
    sum_dist+=*it;
    it++;
  }

  double avermed_dist=(4.0*(double)sum_dist/(double)distances_size);

  float mean_bpm=(60.f*Signal::refreshRate/mean_dist);
  float med_bpm=(60.f*Signal::refreshRate/med_dist);
  float avermed_bpm=(60.f*Signal::refreshRate/avermed_dist);

  std::cout << "proceeded on " << distances_size << " beats over " << size << " frames" << std::endl;

  std::cout << "mean bpm " << mean_bpm << std::endl;
  std::cout << "med bpm " << med_bpm << std::endl;
  std::cout << "average median bpm " << avermed_bpm << std::endl;

  double beat_average=(double)distances_size/((double)size/(double)avermed_dist);
  std::cout << "mesured along " << beat_average*100.0 << "% of the track" << std::endl;

  return avermed_bpm;
}

template<typename T>
void NCPrecomputerValueTable<T>::finalize()
{
  NCPrecomputer::medianFilter(_brut,_l11,_m11, _h11, 5);

  NCPrecomputer::averageFilter(_l11,_l11a11, 5);
  NCPrecomputer::averageFilter(_m11,_m11a11, 5);
  NCPrecomputer::averageFilter(_h11,_h11a11, 5);

  NCPrecomputer::averageFilter(_l11,_l11a128, 64);
  NCPrecomputer::averageFilter(_m11,_m11a128, 64);
  NCPrecomputer::averageFilter(_h11,_h11a128, 64);

  _ready=true;
}

void NCPrecomputer::_thread_func()
{
  MusicDecoder* decoder=dynamic_cast<MusicDecoder*>(_decoder);
  if (decoder && !string_t_empty(_fileToOpen))
  {
    if (decoder->open(_fileToOpen)){
      this->process(*decoder);
    } else {
      std::wcerr << "Failed to open the file: " << _fileToOpen.toWideString() << std::endl;
    }
  }
  else
  {
    this->process(*_decoder);
  }

  _endOfThread=true;
}

void NCPrecomputer::launch(MusicProduction& decoder, const string_t& f)
{
  assert(this->threadReady());
  _decoder=&decoder;
  _fileToOpen=f;
  _endOfThread=false;
  _thread.launch();
}

void NCPrecomputer::additionalSignalComputation(
        const Signal& left,
        const Signal& right)
{
  (void) left;
  (void) right;
}

void NCPrecomputer::additionalFinalize()
{

}
void NCPrecomputer::additionalReset()
{

}
