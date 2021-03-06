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

#include <iostream>
#include "AuiConstants.h"
#include "Synth.h"
#include "Logger.h"

//#include "FileByteSink.h" // For STDERR

using namespace std;

#define DOAUDIOINPUT
#define DOAUDIOOUTPUT
#define DOAUDIOLOGGING

enum AudioState {ADD, REMOVE, PLAY};

namespace MFM
{
  class Audio 
  {
    
  public:
    
    Audio();
    
    virtual ~Audio();
    
    /**
     * A test function
     *
     */
    bool AuiTestFunction(u32 uSound);
        
    /**
     * This takes a u32 sound attribute maps that to wav files and plays them.
     * Currentlly used to play an Elements audio attribute.
     */
    bool ProcessAudio(u32 uSound, AudioState state);
        
    /**
     * This WILL return an input stream of MIC data.
     *
     */
    istream*  GetAudio();
    
    /**
     * This returns if there has been an Audio Event,
     * Currentlly an Audio Event is defined as MIC input above a volume threshold
     */
    bool CheckForAudioEvent();
        
  private:

    /**
     * This makes a system call to play a wav file.
     * currently this maps u32 values to wav files that are musical notes in the Western Tonal system. 
     */
    bool PlaySound(u32 uSound, AudioState state);

    /**
     * For playing tones
     */
    bool PlayTones(u32 uSound);

    /**
     * For playing drones
     */
    bool PlayDrones(AudioState state);

    /**
     * For playing tones cumulative
     */
    bool PlayCumulativeTones(u32 uSound, AudioState state);
    
    Synth* m_pSynth;
  };

} /* namespace MFM */

#endif /*AUDIO_H*/
