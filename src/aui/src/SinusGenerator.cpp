
#include <iostream>
#include <math.h>

#include "SoundGenerator.h"

#include "SinusGenerator.h"

using namespace std;

SinusGenerator::SinusGenerator(float volume, float freq)
    : SoundGenerator(volume, freq)
{
  m_a = 0;
  m_da = (2 * M_PI * freq) / (float)48000; //TODO use ech here;
}

void
SinusGenerator::next(float &left, float &right, float speed /*=1.0 */)
{
  static int count=0;
  count++;
  m_a += m_da * speed;
  float s = sin(m_a);
  left += m_volume * s;
  right += m_volume * s;
  
  if (m_a > 2*M_PI)
    {
      count = 0;
      m_a -= 2*M_PI;
    }
}


