#include <algorithm>
#include "pitchtracker.hpp"
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
  int count =0;
  int channels = 1;
  int sampleRate = 44100;
  int buffSize = 512; //recommended window size for MPM
  int nBuffers = 4;
  double* buffer;
  int device = 0; //default

  RtAudio audio;
  RtAudio::StreamParameters iParams;

  if(device==0)
    iParams.deviceId = adc.getDefaultInputDevice();
  else:
    iParams.deviceId = device;
  iParams.nChannels = channels;
  iParams.firstChannel = offset; //??

  InputData data;
  data.buffer = 0;

  try {
    audio.openStream(NULL, &iParams, FORMAT, sampleRate, buffSize, &input, (void *)&data);
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
    cout << "ALLOC ERROR\n";
    return;
  }

  try{
    audio.startStream();
  } catch(RtAudioError& e){
    std:: cout << '\n' << e.getMessage() << '\n';
    return;
  }

  int buff_pos_2 = 0;
  int buff_pos_1 = 0;

  //look into window overlapping (might be necessary)
  while (audio.isStreamRunning()){
    /* analyze every 100ms of frames using MPM */
    /* possible approach: continuosly divide the buffer into chunks of data, and analyze each chunk
       e.g. [0....cur_buffer_position] | -> chunk -> MPM(chunk) [old_buffer_position .... cur_buffer_position] -> chunk -> MPM(Chunk)
       */
    SLEEP(100);
    buff_pos_2 += bufferSize;
    vector<double> temp_buffer(data.buffer+buff_pos_1,data.buff+buff_pos_2)
    buff_pos_1 = buff_pos_2;
    double estimation = mpm(temp_buffer, frameRate);
    std::cout << '\n' << estimatation << std::endl;
    count+=bufferSize;
  }
  try{
    audio->stopStream();
  }catch(RtError &e){
    e.printMessage();
  }
  delete audio;
}

int main(){
  //...cli here...
  double time=0;
  std::cout << "Track time: ";
  std:: cin >> time;
  std::cout << "\nRunning tracker ";
  trackPitch(time);
  return 0;
}
