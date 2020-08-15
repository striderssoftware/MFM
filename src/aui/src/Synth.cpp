#include <iostream>
#include <sstream>
#include <list>
#include <unistd.h> // TODO VDT - added for Test function sleep call if Test is removed remove this.

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_audio.h>

#include "Synth.h"

#include "SinusGenerator.h"

using namespace std;

static  std::list<SoundGenerator*> SoundGeneratorList;

Synth::Synth()
{
  cout << "Synth constructor was called" << endl;
}

Synth::~Synth()
{
  SDL_CloseAudioDevice(m_device);
  SDL_Quit();
  
}

bool
Synth::Init()
{
  uint32_t ech = 48000;  //TODO VDT ech - ech  i.e 48000 is a magic number, Figure this out.
  
  int iError = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER);
  if (  iError != 0 )
    {
      cout << "SDL init failed here is error:" << SDL_GetError() << endl;
      return false;
    }

  // SDL_AudioSpec defines a (void*) user data field ( m_AudioSpecWant.userdata) i.e. -  m_AudioSpecWant.userdata = this;
  SDL_memset(&m_AudioSpecWant, 0, sizeof(m_AudioSpecWant)); /* or SDL_zero(want) */
  m_AudioSpecWant.freq = ech; 
  m_AudioSpecWant.format = AUDIO_S16SYS;
  m_AudioSpecWant.channels = 2;
  m_AudioSpecWant.samples = BUF_LENGTH;
  m_AudioSpecWant.callback = SynthAudioCallback;  //Use NULL callback and SDL_QueueAudio to push data.
  m_device = SDL_OpenAudioDevice(NULL, 0, &m_AudioSpecWant, &m_AudioSpecHave, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
  
  if (m_device == 0) {
    SDL_Log("Failed to open audio: %s", SDL_GetError());
    
    return false;

  } else {

    if (m_AudioSpecHave.format != m_AudioSpecWant.format)  /* this is NOT an error- the format can change */
      SDL_Log("Did NOT get Float32 audio format.");
    
    
    if (m_AudioSpecHave.samples != BUF_LENGTH)  /* this is NOT an error  */
      cout << "Got buffer length : " << m_AudioSpecHave.samples << " wanted: " << BUF_LENGTH << endl; 

    }

  return true;
}

bool
Synth::BeginPlaying()
{
  SDL_PauseAudioDevice(m_device, 0); /* start audio playing. */

  return true;
}

bool
Synth::Close()
{
  SDL_CloseAudioDevice(m_device);
  SDL_Quit();
  
  return true;
}

bool
Synth::AddSound(double iFrequency)
{
  cout << "Synth::AddSound was called. here is frequency:" << iFrequency << endl;

  SoundGenerator * psound = new SinusGenerator(1, iFrequency);
  SoundGeneratorList.push_front(psound);
  
  return true;
}

bool
Synth::RemoveSound(double iFrequency)
{
  cout << "Synth::RemoveSound was called. here is frequency:" << iFrequency << endl;
  
  if ( SoundGeneratorList.size() < 1 )
    return true;
  
  SoundGenerator* removeItem = 0;
  for (auto pSound: SoundGeneratorList)
    {
      if ( pSound->GetFrequency() == iFrequency )
	{
	  removeItem = pSound;
	  break;
	}
    }
  
  SoundGeneratorList.remove(removeItem);
  
  return true;
}

bool
Synth::Test()
{
  Init();
  AddSound(440);
  BeginPlaying();
  sleep(5);  // let the sound play for 5 sec
  Close();

  return true;
}

void
Synth::SynthAudioCallback(void *unused, Uint8 *byteStream, int byteStreamLength)
{
  if ( SoundGeneratorList.size() == 0 )
    return;
  
  uint32_t ech = byteStreamLength / sizeof(int16_t);  //TODO VDT ech - here ECH is the number of "steps" in the stream. Figure this out.
  int16_t* stream =  reinterpret_cast<int16_t*>( byteStream );
  uint32_t i;
  
  for (i = 0; i < ech; i+=2)
    {
      float left = 0;
      float right = 0;
      
      for (auto pSound: SoundGeneratorList)
	pSound->next(left, right);
      
      stream[i] = 32767 * left / SoundGeneratorList.size(); //TODO VDT -  32767 is a Volume scaler - for the device
      stream[i+1] = 32767 * right / SoundGeneratorList.size(); //TODO VDT -  32767 is a Volume scaler - for the device
    }
  
}

void
Synth::EnumerateDevices()
{
  SDL_Log("Enumerate Devices was called");
  int devcount = SDL_GetNumAudioDevices(SDL_TRUE);
  for (int i = 0; i < devcount; i++) {
    SDL_Log(" Capture device #%d: '%s'\n", i, SDL_GetAudioDeviceName(i, SDL_TRUE));
  }

}


