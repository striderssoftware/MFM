#include "Audio.h"
#include <unistd.h>

#include <iostream>
#include "AuiConstants.h"
#include "Synth.h"
//#include "FileByteSink.h" // For STDERR
//TODO VDT - get rid of releative paths?
#include "../../core/include/Logger.h"

//#define DOAUDIOINPUT
#define DOAUDIOOUTPUT
#define DOAUDIOLOGGING

using namespace std;

MFM::Audio::Audio()
{
#ifdef DOAUDIOLOGGING
  LOG.Message("Audio: constructor was called");
#endif
  m_synth.Init();
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
  if ( uSound == 0 )
    return true;

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
#ifdef DOAUDIOINPUT
  
#ifdef DOAUDIOLOGGING
  LOG.Message("Audio:GetAudio - was called");
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
#ifdef DOAUDIOINPUT
  return m_synth.CheckForAudioEvent();
#else
  return false;
#endif
}

/**
 * This uses the Synth class to play a frequency.
 */
bool MFM::Audio::PlaySound(u32 uSound)
{
#ifdef DOAUDIOOUTPUT
  LOG.Message("Audio::PlaySound uSound was  %x", static_cast<int>(uSound));

  //TODO VDT -  remove this mapping -solve -  note sound attributes are being extended
  if ( uSound == 0xFF000055 )
    uSound = 659;
  else if ( uSound == 0xFF000011 )
    uSound = 440;
  else if ( uSound == 0xFF000033 )
    uSound = 523;
        
  //m_synth.StopPlaying();
  m_synth.RemoveAllSounds();
  m_synth.AddSound(uSound);
  m_synth.BeginPlaying();
  usleep(100000);
  m_synth.StopPlaying();
  
#endif
  
  return true;
  
}//END play sound    

