// FILE          : common.c
// PROJECT       : 
// programmer    : Euyoung Kim, Yujin Jeong
// FIRST VERSION : 2023-04-10
// DESCRIPTION   : 

#include "../inc/common.h"


// wake-up every 2 seconds 
void alarmHandler(int signal_number)
{
  // this is where you could do period work!
  // like saving a file, calculating statistics, etc.
  printf ("\n2 seconds have elapsed!\n\n");

  alarm (2);	// reset alarm

  // reactivate signal handler for next time ...

  signal (signal_number, alarmHandler);
}