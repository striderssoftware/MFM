#ifndef SYNTH_H
#define SYNTH_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>

#include <list>
#include <mutex>

const int BUF_LENGTH = 1024;

class SoundGenerator;

class Synth
{

public:
  
  Synth();
  ~Synth();

  bool Init();
  bool BeginPlaying();
  bool Close();
  bool AddSound(double frequency);
  bool RemoveSound(double frequency);
  
  bool Test();

  static void SynthAudioCallback(void *unused, Uint8 *byteStream, int byteStreamLength);

private:
  void EnumerateDevices();

  SDL_AudioSpec m_AudioSpecWant, m_AudioSpecHave;
  SDL_AudioDeviceID m_device;

};

#endif
