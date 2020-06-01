#include "RtAudio.h"
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#define SLEEP(ms) usleep( (unsigned long) (ms * 1000.0))

/** the purpose of this portion is simply to set up necessities for the active audio buffer **/

//input data buffer and information
struct InputData {
  signed short* buffer;
  unsigned long bufferBytes;
  unsigned long totalFrames;
  unsigned long frameCounter;
  unsigned int  channels;
};
