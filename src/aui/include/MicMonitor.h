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
class MicMonitor
{

public:
  MicMonitor();
  
  bool messageParent(int iVolume);
  bool monitorMic();

private:
  bool SendMessage();
};

