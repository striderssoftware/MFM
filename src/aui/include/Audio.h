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

#include <iostream>

//added for pipes
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


using namespace std;


//#include "FileByteSink.h" // For STDERR

//TODO VDT - get rid of releative paths?
#include "../../core/include/Logger.h"

#define FIFO_FILE "MYFIFO"

namespace MFM
{
  class Audio
  {

  public:

    Audio(){}

    virtual ~Audio(){}

    /**
     * A test function
     *
     */
    bool AuiTestFunction(u32 uSound)
    {
#ifdef NOTNOW
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
#ifdef NOTNOW
      LOG.Message("Audio:  AuiTestFunction - was called");
      LOG.Message("uSound was  %x", static_cast<int>(uSound));
#endif
      
      return PlaySound(uSound);
    }

    
    /**
     * This WILL return an input stream of MIC data.
     *
     */
    istream*  GetAudio()
    {
      LOG.Message("Audio:GetAudio - was called");
#ifdef NOTNOW
      //system("arecord -D hw:0,0 -c1 -d 0 -r 22050 -f S16_LE");  //TODO VDT get output into the stream m_pMicInput
      system("arecord -D hw:0,0 -c1 -d 1 -r 22050 -f S16_LE test.wav");  //TODO VDT get output into the stream m_pMicInput
#endif      
      return m_pMicInput;
    }

    
    /**
     * This returns if there has been an Audio Event,
     * Currentlly an Audio Event is defined as MIC input above a volume threshold
     */
    bool CheckForAudioEvent()
    {
#ifdef NOTYET
      int fd;
      char readbuf[80]; //TODO the messages are "pings". They should be of a fixed size.
      int read_bytes;

      cout <<  "in CheckForMessage loop" << endl;
      
      fd = open(FIFO_FILE, O_RDONLY);
      if ( fd == -1 )
	{
	  // TODO VDT check errno
	  // TODO VDT log error
	  return false;
	}
      
      read_bytes = read(fd, readbuf, sizeof(readbuf));
      if ( read_bytes == -1 )
	{
	  // TODO VDT check errno
	  // TODO VDT log error
	  return false;
	}
      
      readbuf[read_bytes] = '\0';
      cout << "Received string:" <<  readbuf << " and length is" <<  (int)strlen(readbuf) << endl;
#endif
      
      //TODO the messages are "pings", no need to check message contents.
      return true;  // There WAS an audioEvent
    }
    
  private:

    /**
     * This makes a system call to play a wav file.
     * currently this maps u32 values to wav files that are musical notes in the Western Tonal system. 
     */
    bool PlaySound(u32 uSound)
    {
      // development code it will go away!
      // sorry about the paths
      
      if ( uSound == 0u )
        {
          return true;
        }
      else if ( uSound == (u32)0xff000011 )
        {
	  system("aplay -d 1 ~/gitT2/TransferT2/T2_TileProject/MFM/res/audio/aMinorScale/A.wav");
          return true;
        }
      else if ( uSound == (u32)0xff000033 )
        {
	  system("aplay -d 1 ~/gitT2/TransferT2/T2_TileProject/MFM/res/audio/aMinorScale/C.wav");
	  return true;
        }
      else if ( uSound == (u32)0xff000055 )
        {
	  system("aplay -d 1 ~/gitT2/TransferT2/T2_TileProject/MFM/res/audio/aMinorScale/E.wav");
	  return true;
        }
      else if ( uSound == (u32)0xff000077 )
        {
	  system("aplay -d 1 ~/gitT2/TransferT2/T2_TileProject/MFM/res/audio/aMinorScale/G#.wav");
          return true;
        }
      else
        {
          LOG.Message("uSound was  %x", static_cast<int>(uSound));
	  // TODO VDT play a sound  i.e. -  aplay wavFiles/error.wav
          return true;
        }

    }  //END play sound
    
    istream* m_pMicInput;
    
  };

} /* namespace MFM */

#endif /*AUDIO_H*/
