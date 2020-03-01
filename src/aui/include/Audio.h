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


#define DOAUDIO = true;
#define DOAUDIOINPUT = true;
#define DOAUDIOLOGGING = true;
//#define AUDIONOTYET = true;    

#include <iostream>

//added for pipes
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "AuiConstants.h"

using namespace std;

//#include "FileByteSink.h" // For STDERR

//TODO VDT - get rid of releative paths?
#include "../../core/include/Logger.h"


namespace MFM
{
 static bool initialize = true;
  
  class Audio 
  {

  public:

    Audio() :
      m_fd(-1),
      m_micfp(NULL)
    {
#ifdef DOAUDIOINPUT
      if ( initialize )
	{
#ifdef DOAUDIOLOGGING
      LOG.Message("Audio: constructor was called");
#endif
	  CreatePipe();
	  StartMicMonitor();
	  initialize = false;
	}
#endif
    }

    virtual ~Audio()
    {
      close(m_fd);
      pclose(m_micfp);
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
     * This will start a new process to listen to the MIC
     * 
     */
    bool StartMicMonitor()
    {
      LOG.Message("Starting the Mic Monitor EXE");

      m_micfp = popen(MIC_MONITOR_EXE_PATH, "r");

      if ( m_micfp == NULL )
	{
#ifdef DOAUDIOLOGGING
	  LOG.Message("The Monitor exe failed to start");
#endif
	  return false;
	}
      
      return true;
    }
    
    /**
     * This will create a named pipe.
     */
    bool CreatePipe()
    {
      /* Create the FIFO if it does not exist */
      //umask(0);
      mknod(FIFO_FILE, S_IFIFO|0777, 0);
      
      return true;
    }
    

    /**
     * This takes a u32 sound attribute maps that to wav files and plays them.
     * Currentlly used to play an Elements audio attribute.
     */
    bool ProcessAudio(u32 uSound)
    {
#ifdef DOAUDIOLOGGING
      //LOG.Message("Audio:  ProcessAudio - was called");
      //LOG.Message("uSound was  %x", static_cast<int>(uSound));
#endif
      
      return true; //PlaySound(uSound);
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
      //system("arecord -D hw:0,0 -c1 -d 0 -r 22050 -f S16_LE");  //TODO VDT get output into the stream m_pMicInput
      system("arecord -D hw:0,0 -c1 -d 1 -r 22050 -f S16_LE test.wav");  //TODO VDT get output into the stream m_pMicInput
#endif
#endif
      return m_pMicInput;
    }

    
    /**
     * This returns if there has been an Audio Event,
     * Currentlly an Audio Event is defined as MIC input above a volume threshold
     */
    bool CheckForAudioEvent()
    {
#ifdef DOAUDIOINPUT
      int fd;
      char readbuf[11]; //TODO the messages (dosomething) are "pings". They should be of a fixed size.
      int read_bytes;

      //LOG.Message("Audio:CheckForAudioEvent - was called");

      if ( m_fd == -1 )
	{
	  m_fd = open(FIFO_FILE, O_RDONLY);
	  fcntl(m_fd, F_SETFL, O_NONBLOCK);
	}
      if ( m_fd < 0 )
	{
	  // TODO VDT log error
	  LOG.Message("Audio::CheckForAudioEvent  - open failed errno was: %i", errno);
	  return false;
	}
      
      read_bytes = read(m_fd, readbuf, sizeof(readbuf));
      if ( read_bytes < 0 )
	{
	  if ( errno != EAGAIN )
	    {
	      // TODO ERROR CASE, stop this, stop monitor??
	      LOG.Message("Audio::CheckForAudioEvent -  error");
	    }
	  
	  return false;
	}
      else if ( read_bytes == 0 )
	{
	   LOG.Message("Audio::CheckForAudioEvent - no bytes to read");
	  return false;
	}

      readbuf[read_bytes] = '\0';
      LOG.Message("Received a message in namedPipe");

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
#ifdef DOAUDIO
      // TODO VDT - development code it will go away!                                                                                          
      // TODO VDT - sorry about the paths, fix them to point into your distro.        

      if ( uSound == 0u )
        {
          return true;
        }
      else if ( uSound == (u32)0xff000011 )
        {
          system("aplay -d 1 ~/code/T2ProjectCodeRepo/MFM/res/audio/aMinorScale/A.wav");
          return true;
        }
      else if ( uSound == (u32)0xff000033 )
        {
          system("aplay -d 1 ~/code/T2ProjectCodeRepo/MFM/res/audio/aMinorScale/C.wav");
          return true;
        }
      else if ( uSound == (u32)0xff000055 )
        {
          system("aplay -d 1 ~/code/T2ProjectCodeRepo/MFM/res/audio/aMinorScale/E.wav");
          return true;
        }
      else if ( uSound == (u32)0xff000077 )
        {
          system("aplay -d 1 ~/code/T2ProjectCodeRepo/MFM/res/audio/aMinorScale/G#.wav");
          return true;
        }
      else
        {
          LOG.Message("uSound was  %x", static_cast<int>(uSound));
          // TODO VDT play a sound  i.e. -  aplay wavFiles/error.wav
	}
#endif      
                return true;
      
    }//END play sound    

   
    
    int m_fd;
    FILE* m_micfp;
    istream* m_pMicInput;
    
  };

} /* namespace MFM */

#endif /*AUDIO_H*/
