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
#include <stdlib.h>

//added for pipes
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
//#include <string>

#include "../include/MicMonitor.h"
#include "../include/AuiConstants.h"


using namespace std;

int main()
{
  cout << "MicMonitor exe was started" << endl;
  MicMonitor micMonitor;
  micMonitor.monitorMic();

  return 0;
}


MicMonitor::MicMonitor() :
  m_fd(-1)
{
    m_fd = open(FIFO_FILE, O_WRONLY);
}

MicMonitor::~MicMonitor()
{
  close(m_fd);
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
  fp = popen("arecord -D hw:1,0 -c1 -d 0 -r 22050 -f S16_LE -V mono /dev/null  2>&1", "r");

  if ( fp < 0 )
    {
      cout << "popen failed, popen failed, popen failed" << endl;
      return false;
    }
  
  //sleep(3);
  
  string output = "";
  do {

    char c = fgetc(fp);
    if (feof(fp))
      break;

    //cout << "here is c:" << c ;

    string sVolume = "";
    int iVolume = 0;
    if ( c == '%' && output != "" )
      {
	//cout << "FOUND ONE" << endl;
	output += c;
	int index = output.find("%");
	//cout << "here is index:" << index << endl;
	if ( index > 3 )
	  sVolume = output.substr(index-2, 2);
	else
	  continue;
	
	//cout << sVolume << endl;

	try{
	  const char *cstr = sVolume.c_str();
	  iVolume = atoi(cstr);// working
	}catch(exception* e){
	  cout << "stoi exception thown, EXITING" << endl;
	  return false;
	}
	
	cout << "******************* Here is iVolume:" << iVolume << endl;
	if ( iVolume > 30 ) 
	  {
	    sVolume = "";
	    messageParent(iVolume);
	    iVolume = 0;
	  }
	
	output = "";
      }
    else
      {
	//cout << "something" << endl;
	output += c;
      }

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
   int stringlen;
   char sendbuf[200];
 
   //cout << "MicMonitor:: Sent message to parent" << endl;
   strcpy(sendbuf, "dosomething");
   stringlen = strlen(sendbuf);
   sendbuf[stringlen + 1] = '\0';

   int iWriteResult = write(m_fd, sendbuf, strlen(sendbuf));
   if ( iWriteResult < 0 )
     return false;
   
   //printf("Sent string: \"%s\" and string length is %d\n", sendbuf, (int)strlen(sendbuf));
   
   return true;
}
