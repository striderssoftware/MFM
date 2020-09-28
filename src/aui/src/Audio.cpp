#include "Audio.h"
#include <unistd.h>

#include <iostream>
#include "AuiConstants.h"
#include "Synth.h"
#include "Logger.h"

using namespace std;

MFM::Audio::Audio() :
  m_pSynth(nullptr)
{

  m_pSynth = Synth::GetInstance();

#ifdef DOAUDIOLOGGING
  LOG.Message("Audio: constructor was called");
#endif
  
  m_pSynth->Init();
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
 * This takes a u32 sound attribute and uses the 
 * Synth class to play that frequency.
 * Currentlly used to play an Elements audio attribute.
 */
bool MFM::Audio::ProcessAudio(u32 uSound, AudioState state)
{
  if ( uSound == 0 )
    return true;

#ifdef DOAUDIOLOGGING
  LOG.Message("Audio:  ProcessAudio - was called");
  LOG.Message("uSound was  %x", static_cast<int>(uSound));
#endif
  
  return PlaySound(uSound, state);// TODO SDL2PORT - uSound to double - PlaySound(uSound);
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
  return m_pSynth->CheckForAudioEvent();
#else
  return false;
#endif
}

/**
 * This uses the Synth class to play a frequency.
 */
bool MFM::Audio::PlaySound(u32 uSound, AudioState state)
{
#ifdef DOAUDIOOUTPUT

#ifdef DOAUDIOLOGGING
  LOG.Message("Audio::PlaySound uSound was  %x and state was: %x", static_cast<int>(uSound), static_cast<int>(state));
#endif

  if ( uSound == 0xFF000000 )
    return true;
  
  //TODO VDT -  remove this mapping -solve -  note sound attributes are being extended
  if ( uSound == 0xFF000055 )
    uSound = 659;
  else if ( uSound == 0xFF000011 )
    uSound = 440;
  else if ( uSound == 0xFF000033 )
    uSound = 523;
  else
    return true;
  
  PlayTones(uSound);
  //PlayCumulativeTones(uSound, state);
  //PlayDrones(state);
  
#endif
  
  return true;
  
}//END play sound    


/**
 * 
 */
bool MFM::Audio::PlayTones(u32 uSound)
{
  m_pSynth->RemoveAllSounds();
  m_pSynth->AddSound(uSound);
  m_pSynth->BeginPlaying();
  usleep(100000);
  m_pSynth->StopPlaying();
  
  return true;
}


/**
 * 
 */
bool MFM::Audio::PlayDrones(AudioState state)
{
  return true;
}


/**
 * 
 */
bool MFM::Audio::PlayCumulativeTones(u32 uSound, AudioState state)
{
  if ( state == AudioState::ADD )
    m_pSynth->AddSound(uSound);

  m_pSynth->BeginPlaying();
  usleep(100000);
  m_pSynth->StopPlaying();

  return true;
}

