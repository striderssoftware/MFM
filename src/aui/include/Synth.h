#ifndef SYNTH_H
#define SYNTH_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>

#include <list>
#include <mutex>

const int BUF_LENGTH = 1024;
const int VOLUME_THRESHOLD = 90;

class SoundGenerator;

enum {
        VUMETER_NONE,
        VUMETER_MONO,
        VUMETER_STEREO
};

class Synth
{

public:

  static Synth* GetInstance();

  bool Init();
  bool InitInput();
  bool InitOutput();
  bool BeginPlaying();
  bool StopPlaying();
  bool Close();
  bool AddSound(double frequency);
  bool RemoveSound(double frequency);
  bool RemoveAllSounds();
  bool TestInput();
  bool TestOutput();
  void EnumerateDevices();
  bool BeginRecording();
  bool CheckForAudioEvent();
  


  static void SynthAudioCallback(void *unused, Uint8 *byteStream, int byteStreamLength);

protected:
   Synth();
  ~Synth();
private:
  void OutputAudioSpecs();
  void SetInputParameters();
  int ComputeMaxPeak(u_char *data, size_t count);
  
  SDL_AudioSpec m_AudioSpecOutputWant, m_AudioSpecHave;
  SDL_AudioSpec m_AudioSpecInputWant;
  SDL_AudioDeviceID m_OutputDevice;
  SDL_AudioDeviceID m_InputDevice;

  static Synth* m_instance;
};

#endif
