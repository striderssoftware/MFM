/*                                              -*- mode:C++ -*-
  Audio.h 
*/

/**
  \file Audio.h 
  \ ADDED VTD
  \
  \ Persistence. 
  \ GUI is persisted on the screen even though the event window is processing/changing 
  \ a small part of the "world". 
  \ How would one persist the sound while the event window was processing another part of
  \ the "world".   Instead of playing some sort of data file (wave, mp3 etc...) I think 
  \ more of using a synthizer to create these sounds and so the synthesizer would be to sound 
  \ what the screen is to visual aspects.  
 */

#ifndef AUDIO_H
#define AUDIO_H


#define DOAUDIO
//#define DOAUDIOINPUT
#define DOAUDIOLOGGING
//#define AUDIONOTYET

#include <iostream>

#include "AuiConstants.h"
#include "Synth.h"

using namespace std;

//#include "FileByteSink.h" // For STDERR

//TODO VDT - get rid of releative paths?
#include "../../core/include/Logger.h"

namespace MFM
{

  class Audio 
  {

  public:

    Audio()
    {
#ifdef DOAUDIOLOGGING
      LOG.Message("Audio: constructor was called");
#endif
      m_synth.Init();
    }

    virtual ~Audio()
    {
    }

    /**
     * A test function
     *
     */
    bool AuiTestFunction(u32 uSound)
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
    bool ProcessAudio(u32 uSound)
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
    istream*  GetAudio()
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
    bool CheckForAudioEvent()
    {
      return false;

    }
    
  private:

    /**
     * This makes a system call to play a wav file.
     * currently this maps u32 values to wav files that are musical notes in the Western Tonal system. 
     */
    bool PlaySound(u32 uSound)
    {
#ifdef DOAUDIO
          LOG.Message("Audio::PlaySound uSound was  %x", static_cast<int>(uSound));
          // TODO VDT play a sound  i.e. -  aplay wavFiles/error.wav

	  m_synth.AddSound(uSound);
	  m_synth.BeginPlaying();
#endif      	  
	  return true;
      
    }//END play sound    

    Synth m_synth;
  };

} /* namespace MFM */

#endif /*AUDIO_H*/
