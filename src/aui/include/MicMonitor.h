/*                                              -*- mode:C++ -*-
  MicMonitor.h 
*/

/**
  \file MicMonitor.h 
  \ ADDED VTD
  \
  \ This is a helper class to monitor Microphone input.
  \ This currentlly uses system calls to arecord to accomplish this
  \ I believe it should use an API to communicate directlly to the 
  \ sound card. The API must be able to available on the MFM/Tile OS.
 */


#ifndef MICMONITOR_H
#define MICMONITOR_H

class MicMonitor
{

public:
  MicMonitor();
  ~MicMonitor();
  
  bool messageParent(int iVolume);
  bool monitorMic();

private:
  bool SendMessage();
  int m_fd;
};

#endif 
