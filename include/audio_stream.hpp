#include "RtAudio.h"
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#define SLEEP(ms) usleep( (unsigned long) (ms * 1000.0))

/** the purpose of this portion is simply to set up necessities for the active audio buffer **/

//input data buffer and information
struct InputData {
  double* buffer;
  unsigned long bufferBytes;
  unsigned long totalFrames;
  unsigned long frameCounter;
  unsigned int  channels;
};

/**** callback ****/

int input(void * /*outputBuffer */, void *inputBuffer, unsigned int nBufferFrames,
          double /* streamTime */, RtAudioStreamStatus /*status*/, void *data){
  InputData *iData = (InputData*) data;
  //copies data to the allocated buffer
  
  unsigned int frames = nBufferFrames;
  if(iData -> frameCounter + nBufferFrames > iData->totalFrames){
    frames = iData->totalFrames - iData -> frameCounter;
    iData->bufferBytes = frames * iData->channels *sizeof(double);
  }

  unsigned long offset = iData->frameCounter * iData->channels;
  memcpy(iData->buffer+offset, inputBuffer, iData->bufferBytes);
  iData->frameCounter += frames;

  if(iData->frameCounter >= iData-> totalFrames) return -1;
  return 0;
}
