#include <algorithm>
#include "pitchtracker.h"
#include <iterator>
#include <iostream>
#include "RtAudio.h"

/* 
   main file for this project
   functions:
     int main
     start_record [starts an rtaudio stream for n seconds (in the futures, maybe listen for keypresses instead]

     we need to acess the stream every 300 milliseconds (just guessing here) and run
     mpm on it, outputting the results continously. hopefully mpm can return the 
     estimate fast enough.
 */



//for now we'll just write the results to stdout 
void trackPitch(double time){
  /* first, lets set up rtaudio for getting our input stream */
  unsigned int bufferFrames, device,offest = 0;
  FILE *fd;

  RtAudio adc;
  if(adc.getDeviceCount() <1){
    std::cout << "\n no audio device found!\n";
    return -1:
  }
  unsigend channels = 1;
  unsigned int frameRate = 44000;
  bufferFrames = 512;

  RtAudio::StreamParameters iParams;
  if(device == 0)
    iParams.deviceId = adc.getDefaultInputDevice();
  else
    iParamds.deviceId = device;
  iparams.nChannels = channels;
  iParams.firstChannels = offset;

  InputData data;
  data.buffer = 0;
  try{
    adc.openStream(NULL, &iParams, FORMAT, frameRate, &bufferFrames, &input, (void *) &data);
  }
  catch (RtAudioError& e){
    std::cout << '\n' << e.getMessage() << '\n' << std::endl;
    return -1 
  }

  data.bufferBytes = bufferFrames * channels * sizeof(double);
  data.totalFrames = (unsigned long) (frameRate*time);
  data.frameCounter = 0;
  data.channels = channels;
  unsigned long totalBytes;
  totalBytes = data.totalFrames * channels * sizeof(double);
  
  try {
    adc.startStream();
    std::cout << "starting stream\n"; 
  }
  catch (RtAudioError& e) {
    std::cout << '\n' << e.getMessage() << std::endl;
    return -1;
  }
  std::cout << "RECORDING\n";
  unsigned int buff_pos_1, buff_pos_2 = 0;
  while (adc.isStreamRunning()){
    /* analyze every 100ms of frames using MPM */
    /* possible approach: continuosly divide the buffer into chunks of data, and analyze each chunk
       e.g. [0....cur_buffer_position] | -> chunk -> MPM(chunk) [old_buffer_position .... cur_buffer_position] -> chunk -> MPM(Chunk)
       */
    buff_pos_2 = std::size(data.buffer);
    vector<double> temp_buffer(data.buffer+buff_pos_1,data.buff+buff_pos_2)
    buff_pos_1 = buff_pos_2+1;
    double estimation = mpm(temp_buffer, frameRate);
    std::cout << '\n' << estimatation << std::endl;
    SLEEP(100);
  }
}

int main(){
  //...cli here...

  return 0;
}
