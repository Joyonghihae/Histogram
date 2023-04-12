// FILE          : common.c
// PROJECT       : 
// programmer    : Euyoung Kim, Yujin Jeong
// FIRST VERSION : 2023-04-10
// DESCRIPTION   : 

#include "../inc/common.h"


void allPowerfulSignalHandler (int signal_number)
{
  static int counter = 0;
  

  switch (signal_number) 
  {
    
    case SIGALRM:	
    {
      alarm (5);
    }
    case SIGINT:	
    {
      counter++;
      printf ("\nCheck signal: %d!\n", counter);
      
      signal (SIGINT, SIG_DFL);
        
  }

  // reactivate our custom signal handler for next time ...
  signal (signal_number, allPowerfulSignalHandler);
}
}
