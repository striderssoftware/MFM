
#ifndef SOUND_GENERATOR_H
#define SOUND_GENERATOR_H

#include <iostream>
#include <string>
#include <map>

using namespace std;

class SoundGenerator
{
public:
  virtual ~SoundGenerator();

  float GetFrequency();
  
  // Next sample to add to left and right 
  // added value should be from -1 to 1
  // speed = samples/sec modifier
  virtual void next(float &left, float &right, float speed=1.0) = 0;
  
  float rand();
    
protected:
  SoundGenerator();
  SoundGenerator(float volume, float freq);
  
  float m_volume;
  float m_freq;
};

#endif
