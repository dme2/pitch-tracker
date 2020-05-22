#include <algorithm>
#include "audio_stream.hpp"
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

#define FORMAT RTAUDIO_SINT16

//for now we'll just write the results to stdout 
void trackPitch(double time){
  /* first, lets set up rtaudio for getting our input stream */
  unsigned int channels = 1;
  int sampleRate = 44100;
  unsigned int buffSize = 512; //recommended window size for MPM
  //unsigned int nBuffers = 4;
  //double* buffer;
  unsigned int device = 0; //default
  unsigned int offset = 0;
    
  RtAudio audio;
  RtAudio::StreamParameters iParams;

  if(audio.getDeviceCount() < 1){
    std::cout << "No devices found" << std::endl;
    return;
  }

  if(device==0)
    iParams.deviceId = audio.getDefaultInputDevice();
  else
    iParams.deviceId = device;

  iParams.nChannels = channels;
  iParams.firstChannel = offset; //??

  InputData data;
  data.buffer = 0;

  try {
    audio.openStream(NULL, &iParams, FORMAT, sampleRate, &buffSize, &input, (void *)&data);
  }
  catch (RtAudioError& e) {
      std:: cout << '\n' << e.getMessage() << '\n' << std::endl;
      return;
  }
  
  data.bufferBytes = buffSize * channels * sizeof (double);
  data.totalFrames = (unsigned long) (sampleRate * time);
  data.frameCounter = 0;
  data.channels = channels;
  unsigned long totalBytes;
  totalBytes = data.totalFrames * channels * sizeof (double);
  
  //malloc the buffer - probably need to figure something else out for streaming purposes
  data.buffer = (double*) malloc(totalBytes);
  if(data.buffer == 0){
    std::cout << "ALLOC ERROR\n";
    return;
  }

  try{
    audio.startStream();
  } catch(RtAudioError& e){
    std:: cout << '\n' << e.getMessage() << '\n';
    return;
  }

  double* buff_temp = data.buffer;
  //look into window overlapping (might be necessary)
  while (audio.isStreamRunning()){
    /* analyze every 100ms of frames using MPM */
    /* possible approach: continuosly divide the buffer into chunks of data, and analyze each chunk
       e.g. [0....cur_buffer_position] | -> chunk -> MPM(chunk) [old_buffer_position .... cur_buffer_position] -> chunk -> MPM(Chunk)
       */
    SLEEP(100);
    std::vector<double> temp_buffer(data.buffer,data.buffer+512);
    std::cout << temp_buffer.size() << std::endl;
    data.buffer = buff_temp;
  }

  try{
    audio.stopStream();
  }catch(RtAudioError &e){
    e.printMessage();
  }
  if(data.buffer)
    free(data.buffer);
}

int main(){
  //...cli here...
  double time=0;
  std::cout << "Track time: ";
  std:: cin >> time;
  std::cout << "\nRunning recorder ";
  trackPitch(time);
  return 0;
}
