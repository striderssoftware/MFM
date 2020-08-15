
#include <iostream>
#include <string>
#include <map>

#include "SoundGenerator.h"

using namespace std;

SoundGenerator::SoundGenerator() {}
SoundGenerator::~SoundGenerator() {}

SoundGenerator::SoundGenerator(float volume, float freq) :
  m_volume(volume),
  m_freq(freq)
{
}

float
SoundGenerator::GetFrequency()
{
  return m_freq;
}

float
SoundGenerator::rand()
{
  return (float)::rand() / ((float)RAND_MAX/2) -1.0;
}


