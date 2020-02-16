/*                                              -*- mode:C++ -*-
  Monitor.cpp
*/

/**
  \file MicMonitor.cpp
  \ ADDED VTD
  \
  \ Should this go into a .tcc file ??
 */

#include <iostream>
#include <stdio.h>
#include <unistd.h>


//added for pipes
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


#include "MicMonitor.h"
#include "Audio.h"

using namespace std;

MicMonitor::MicMonitor()
{
}


bool
MicMonitor::messageParent(int iVolume)
{
  cout << "messageParent was called" << endl;

  SendMessage();
  
  return true;
}

bool
MicMonitor::monitorMic()
{
  cout << "Monitor::Mic was called" << endl;
  
  FILE* fp;

  fp = popen("arecord -D hw:0,0 -c1 -d 3 -r 22050 -f S16_LE -V mono /dev/null  2>&1", "r");

  sleep(3);

  int iCount = 1; //TODO remove this, it is only for testing.
  string output;
  do {

    char c = fgetc(fp);
    if (feof(fp))
      break;

    //cout << "here is c:" << c ;

    string sVolume;
    int iVolume;
    if ( c == '\r' )
      {
	//cout << "FOUND ONE" << endl;
	int index = output.find("%");
	//cout << "here is index:" << index << endl;
	if ( index > 3 )
	  sVolume = output.substr(index-2, 2);
	else
	  continue;
	
	//cout << sVolume << endl;

	try{
	  iVolume = 0; //TODO VDT get stoi(sVolume); working
	}catch(exception* e){
	  cout << "stoi exception thown, EXITING" << endl;
	  return false;
	}
	
	//cout << iVolume << endl;
	if ( iVolume == 0 && iCount % 10 == 0) 
	  {
	    //cout << "here is iCount:" << iCount << endl;
	    sVolume.empty();
	    messageParent(iVolume);
	  }
	
	output.empty();
      }
    else
      {
	//cout << "something" << endl;
	output += c;
      }

    iCount++;
  }while (true);

  pclose(fp);

  // cout << output;

  return true;

  //here is what a line of output looks like:
  // #+                                                 | 00%
  
}


bool
MicMonitor::SendMessage()
{
   int fd;

   int stringlen;
   char sendbuf[80];
 
   printf("FIFO_CLIENT: Send message to parent \n");
   fd = open(FIFO_FILE, O_CREAT, O_CREAT|O_WRONLY);
 
   strcpy(sendbuf, "dosomething");
   stringlen = strlen(sendbuf);
   sendbuf[stringlen - 1] = '\0';

   printf("Sent string: \"%s\" and string length is %d\n", sendbuf, (int)strlen(sendbuf));
   
   int iWriteResult = write(fd, sendbuf, strlen(sendbuf));   
   if ( iWriteResult < 0 )
     return false;
   
   return true;
}

