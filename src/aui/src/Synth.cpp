#include <iostream>
#include <sstream>
#include <list>
#include <unistd.h> // TODO VDT - added for Test function sleep call if Test is removed remove this.
#include <asm/byteorder.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_audio.h>

#include "Synth.h"

#include "SinusGenerator.h"

using namespace std;

static  std::list<SoundGenerator*> SoundGeneratorList;
static size_t bits_per_sample;
static int vumeter;

Synth::Synth()
{
  cout << "*********Synth constructor was called" << endl;
  SetInputParameters();
}

Synth::~Synth()
{
  SDL_CloseAudioDevice(m_OutputDevice);
  SDL_Quit();  
}

bool
Synth::Init()
{
  bool bReturn = false;
  bReturn =  Synth::InitOutput();
  bReturn = Synth::InitInput();

  return bReturn;
}

bool
Synth::InitOutput()
{
  SDL_Log("Synth::Init was called");
  uint32_t ech = 48000;  //TODO VDT ech - ech  i.e 48000 is a magic number, Figure this out.
  
  int iError = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER);
  if (  iError != 0 )
    {
      cout << "SDL init failed here is error:" << SDL_GetError() << endl;
      return false;
    }

  // SDL_AudioSpec defines a (void*) user data field ( m_AudioSpecOuputWant.userdata) i.e. -  m_AudioSpecOuputWant.userdata = this;
  SDL_memset(&m_AudioSpecOutputWant, 0, sizeof(m_AudioSpecOutputWant)); /* or SDL_zero(want) */
  m_AudioSpecOutputWant.freq = ech; 
  m_AudioSpecOutputWant.format = AUDIO_S16SYS;
  m_AudioSpecOutputWant.channels = 2;
  m_AudioSpecOutputWant.samples = BUF_LENGTH;
  m_AudioSpecOutputWant.callback = SynthAudioCallback;  //Use NULL callback and SDL_QueueAudio to push data.
  m_OutputDevice = SDL_OpenAudioDevice(NULL, 0, &m_AudioSpecOutputWant, &m_AudioSpecHave, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
  
  if (m_OutputDevice == 0) {
    SDL_Log("Failed to open audio: %s", SDL_GetError());
    
    return false;

  } else {

    if (m_AudioSpecHave.format != m_AudioSpecOutputWant.format)  /* this is NOT an error- the format can change */
      SDL_Log("Did NOT get Float32 audio format.");
    
    
    if (m_AudioSpecHave.samples != BUF_LENGTH)  /* this is NOT an error  */
      cout << "Got buffer length : " << m_AudioSpecHave.samples << " wanted: " << BUF_LENGTH << endl; 

    }

  return true;
}

bool
Synth::InitInput()
{
  int iError = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER);
  if (  iError != 0 )
    {
      cout << "SDL init failed here is error:" << SDL_GetError() << endl;
      return false;
    }

  SDL_memset(&m_AudioSpecInputWant, 0, sizeof(m_AudioSpecInputWant)); /* or SDL_zero(want) */
  m_InputDevice = SDL_OpenAudioDevice(NULL, SDL_TRUE, &m_AudioSpecInputWant, &m_AudioSpecInputWant, 0);

  if (m_InputDevice == 0) {
    SDL_Log("Failed to open audio: %s", SDL_GetError());

    return false;

  } else {

    if (m_AudioSpecInputWant.samples != BUF_LENGTH)  /* this is NOT an error  */
      cout << "Got buffer length : " << m_AudioSpecInputWant.samples << " wanted: " << BUF_LENGTH << endl;
    }

  return true;
}

bool
Synth::BeginPlaying()
{
  SDL_PauseAudioDevice(m_OutputDevice, 0); /* start audio playing. */

  return true;
}

bool
Synth::Close()
{
  SDL_CloseAudioDevice(m_InputDevice);
  SDL_CloseAudioDevice(m_OutputDevice);
  SDL_Quit();
  
  return true;
}

bool
Synth::AddSound(double iFrequency)
{

  iFrequency = 440;
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
Synth::TestOutput()
{
  Init();
  AddSound(440);
  BeginPlaying();
  sleep(5);  // let the sound play for 5 sec
  Close();

  return true;
}

bool
Synth::BeginRecording()
{
  cout << " BeginRecording was called" << endl;
  SDL_PauseAudioDevice(m_InputDevice, 0); /* start audio playing. */

  return true;
}

/*
  Do I want to poll, or receive a callback with data
*/
bool
Synth::CheckForAudioEvent()
{
  cout << "CheckForAudioEvent was called" << endl;

  int maxVolume = -1;
  while (SDL_TRUE) {  //TODO VDT - This is the "apps" run loop.
    Uint8 buf[1024];
    const Uint32 bytesRead = SDL_DequeueAudio(m_InputDevice, buf, sizeof (buf));
    if ( bytesRead < 1 ) {
      continue;
    }

    //cout << "bytes read was:" <<  bytesRead << endl;
    //OutputAudioSpecs();
    
    int count = bytesRead; //1024/8;                                                                                         
    maxVolume = ComputeMaxPeak(&buf[0], count);

    //TODO VDT - remove this when called from app 
    //TODO VDT Volume seems to sensitive - check this once comput_max_peak is done.
#define TESTING
#ifdef TESTING
    if ( maxVolume > VOLUME_THRESHOLD )
      break;
#endif
    //cout << "CheckForAudioEvent maxVolume was: " << maxVolume << " returning true" << endl;
  }

  if ( maxVolume > VOLUME_THRESHOLD )
    {
      cout << "CheckForAudioEvent maxVolume was: " << maxVolume << " returning true" << endl;
      return true;
    }
  return false;

}

void
Synth::SynthAudioCallback(void *unused, Uint8 *byteStream, int byteStreamLength)
{
  //SDL_Log("SynthAudioCallback was called");

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

bool
Synth::TestInput()
{
  bool result = Init();

  //EnumerateDevices();
  

  if ( result )
    result = BeginRecording();
  if ( result )
    result = CheckForAudioEvent();

  Close();
  return result;
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

void
Synth::OutputAudioSpecs()
{
  
    cout << "---------------- (Recording) Getting data for - m_AudioSpecInputWant ------------" << endl;

    cout << "Audio Spec Freq: " << m_AudioSpecInputWant.freq << endl;
    cout << "Audio Spec Format: " << m_AudioSpecInputWant.format << endl;
    cout << "Audio Spec Channels: " << m_AudioSpecInputWant.channels << endl;
    cout << "Audio Spec Silence: " << m_AudioSpecInputWant.silence << endl;
    cout << "Audio Spec Samples: " << m_AudioSpecInputWant.samples << endl;
    cout << "Audio Spec Size: " << m_AudioSpecInputWant.size << endl;

    
    cout << "---------------- (Recording) Getting data for - m_AudioSpecOutputWant ------------" << endl;

    cout << "Audio Spec Freq: " << m_AudioSpecOutputWant.freq << endl;
    cout << "Audio Spec Format: " << m_AudioSpecOutputWant.format << endl;
    cout << "Audio Spec Channels: " << m_AudioSpecOutputWant.channels << endl;
    cout << "Audio Spec Silence: " << m_AudioSpecOutputWant.silence << endl;
    cout << "Audio Spec Samples: " << m_AudioSpecOutputWant.samples << endl;
    cout << "Audio Spec Size: " << m_AudioSpecOutputWant.size << endl;
}

/*                                                                                                                                        
/ see arecord.c - static void set_params(void) 
*/
void
Synth::SetInputParameters()
{
  //TODO VDT I changed this to 16, see alsa lib comments
  
  bits_per_sample = 16; //snd_pcm_format_physical_width(hwparams.format);
  
  vumeter = VUMETER_STEREO;

#ifdef NOTYET
  /* stereo VU-meter isn't always available... */
  if (vumeter == VUMETER_STEREO) {
    if (hwparams.channels != 2 || !interleaved || verbose > 2)
      vumeter = VUMETER_MONO;
  }

  bits_per_frame = bits_per_sample * hwparams.channels;
#endif

}

int
Synth::ComputeMaxPeak(u_char *data, size_t count)
{
  int iResult = -1;

  signed int val, max, perc[2], max_peak[2];
  static        int     run = 0;
  //size_t ocount = count;
  int   format_little_endian = 1; // TODO VDT use - snd_pcm_format_little_endian(hwparams.format);
  int ichans, c;

  if (vumeter == VUMETER_STEREO)
    ichans = 2;
  else
    ichans = 1;
  memset(max_peak, 0, sizeof(max_peak));
  switch (bits_per_sample) {
  case 8: {
    signed char *valp = (signed char *)data;
    signed char mask = 0; //TODO VDT use - snd_pcm_format_silence(hwparams.format);
    
    c = 0;
    while (count-- > 0) {
      val = *valp++ ^ mask;
      val = abs(val);
      if (max_peak[c] < val)
        max_peak[c] = val;
      if (vumeter == VUMETER_STEREO)
        c = !c;
    }
    break;
  }
 case 16: {
    cout <<" you shoud get here------------------" << endl;

    signed short *valp = (signed short *)data;
    signed short mask = 0; // TODO VDT use - snd_pcm_format_silence_16(hwparams.format);
    
    signed short sval;

    count /= 2;
    c = 0;
    while (count-- > 0) {
      if (format_little_endian)
        sval = __le16_to_cpu(*valp);
      else
        sval = __be16_to_cpu(*valp);
      sval = abs(sval) ^ mask;
      if (max_peak[c] < sval)
        max_peak[c] = sval;
      valp++;
      if (vumeter == VUMETER_STEREO)
        c = !c;
    }
    //TODO fix this
    //return perc[0];

    break;
  }
  case 24: {
    unsigned char *valp = data;
    signed int mask = 0; // TODO VDT use - snd_pcm_format_silence_32(hwparams.format);
    

    count /= 3;
    c = 0;
    while (count-- > 0) {
      if (format_little_endian) {
        val = valp[0] | (valp[1]<<8) | (valp[2]<<16);
      } else {
        val = (valp[0]<<16) | (valp[1]<<8) | valp[2];
      }
      /* Correct signed bit in 32-bit value */
      if (val & (1<<(bits_per_sample-1))) {
        val |= 0xff<<24;        /* Negate upper bits too */
      }
      val = abs(val) ^ mask;
      if (max_peak[c] < val)
        max_peak[c] = val;
      valp += 3;
      if (vumeter == VUMETER_STEREO)
        c = !c;
    }
    break;
  }
  case 32: {
    signed int *valp = (signed int *)data;
    signed int mask = 0; //TODO VDT use - snd_pcm_format_silence_32(hwparams.format);
    

    count /= 4;
    c = 0;
    while (count-- > 0) {
      if (format_little_endian)
        val = __le32_to_cpu(*valp);
      else
        val = __be32_to_cpu(*valp);
      val = abs(val) ^ mask;
      if (max_peak[c] < val)
        max_peak[c] = val;
      valp++;
      if (vumeter == VUMETER_STEREO)
        c = !c;
    }
    break;
  }
  default:
    if (run == 0) {
      fprintf(stderr, ("Unsupported bit size %d.\n"), (int)bits_per_sample);
      run = 1;
    }
    return iResult;
  }
  max = 1 << (bits_per_sample-1);
  if (max <= 0)
    max = 0x7fffffff;
 for (c = 0; c < ichans; c++) {
    if (bits_per_sample > 16)
      perc[c] = max_peak[c] / (max / 100);
    else
      perc[c] = max_peak[c] * 100 / max;
  }

 // OLD FUNCTIN RETURNED HERE                                                                                                             
#ifdef NOMORE
  if (interleaved && verbose <= 2) {
    static int maxperc[2];
    static time_t t=0;
    const time_t tt=time(NULL);
    if(tt>t) {
      t=tt;
      maxperc[0] = 0;
      maxperc[1] = 0;
    }
    for (c = 0; c < ichans; c++)
      if (perc[c] > maxperc[c])
        maxperc[c] = perc[c];

    putchar('\r');

#ifdef NOMORE
    print_vu_meter(perc, maxperc);
#endif

    fflush(stdout);

    cout << " here is iResult:" << iResult << endl;
    //iResult = perc[0]; //TODO VDT use c ?
    //return iResult;

  }
  else if(verbose==3) {
    printf(("Max peak (%li samples): 0x%08x "), (long)ocount, max_peak[0]);
    for (val = 0; val < 20; val++)
      if (val <= perc[0] / 5)
        putchar('#');
      else
        putchar(' ');
    printf(" %i%%\n", perc[0]);
    fflush(stdout);
  }

#endif

  cout << "here is max iReturn:" << perc[0] << endl;
  iResult = perc[0];
  return iResult;

  //end new stuff
  
}// end computemacpeak

