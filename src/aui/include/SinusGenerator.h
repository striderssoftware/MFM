
#ifndef SINUS_GENERATOR_H
#define SINUS_GENERATOR_H

#include <iostream>
#include <math.h>

#include "SoundGenerator.h"

using namespace std;

class SinusGenerator : public SoundGenerator
{
public:
  SinusGenerator(float volume, float freq);
    
  virtual void next(float &left, float &right, float speed=1.0);
    
private:
  float m_a;
  float m_da;

};

#endif
