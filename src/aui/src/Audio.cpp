#include "Audio.h"

#include <iostream>
#include "AuiConstants.h"
#include "Synth.h"
//#include "FileByteSink.h" // For STDERR
//TODO VDT - get rid of releative paths?
#include "../../core/include/Logger.h"

#define DOAUDIO
//#define DOAUDIOINPUT
#define DOAUDIOLOGGING
//#define AUDIONOTYET

using namespace std;

MFM::Audio::Audio()
{
#ifdef DOAUDIOLOGGING
  LOG.Message("Audio: constructor was called");
#endif
  m_synth.Init();
  m_synth.BeginRecording();
}

MFM::Audio::~Audio()
{
}

/**
 * A test function
 *
 */
bool MFM::Audio::AuiTestFunction(u32 uSound)
{
#ifdef DOAUDIOLOGGING
  LOG.Message("Audio:  AuiTestFunction - was called");
  LOG.Message("uSound was  %x", static_cast<int>(uSound));
#endif
  return true;
}

/**
 * This takes a u32 sound attribute maps that to wav files and plays them.
 * Currentlly used to play an Elements audio attribute.
 */
bool MFM::Audio:: ProcessAudio(u32 uSound)
{
#ifdef DOAUDIOLOGGING
  LOG.Message("Audio:  ProcessAudio - was called");
  LOG.Message("uSound was  %x", static_cast<int>(uSound));
#endif
  
  return PlaySound(uSound); // TODO SDL2PORT - uSound to double - PlaySound(uSound);
}


/**
 * This WILL return an input stream of MIC data.
 *
 */
istream*  MFM::Audio::GetAudio()
{
#ifdef AUDIONOTYET
#ifdef DOAUDIOINPUT
  
#ifdef DOAUDIOLOGGING
  LOG.Message("Audio:GetAudio - was called");
#endif
  
#endif
#endif
  istream* pMicInput;
  return pMicInput;
}


/**
 * This returns if there has been an Audio Event,
 * Currentlly an Audio Event is defined as MIC input above a volume threshold
 */
bool MFM::Audio::CheckForAudioEvent()
{
  return m_synth.CheckForAudioEvent();
}



/**
 * This makes a system call to play a wav file.
 * currently this maps u32 values to wav files that are musical notes in the Western Tonal system. 
 */
bool MFM::Audio::PlaySound(u32 uSound)
{
#ifdef DOAUDIO
  LOG.Message("Audio::PlaySound uSound was  %x", static_cast<int>(uSound));
  // TODO VDT play a sound  i.e. -  aplay wavFiles/error.wav
  
  m_synth.AddSound(uSound);
  m_synth.BeginPlaying();
#endif      	  
  return true;
  
}//END play sound    

