# pitch-tracker
track/determine the changes in pitch from a monophonic instrument being played into the microphone on the command line.

the makefile does not work at the moment, but you can compile with the command found in testmake.txt (run it on the src folder).

this project depends on the [Fastest Fourier Transform in the South](https://github.com/anthonix/ffts) and [RtAudio](https://github.com/thestk/rtaudio)

This project is a work in progress.

todo:
	[] fix makefile (or replace with something else)
	[] fix outstream
	[] program args? (sample rate, pitch out/frequency out)

great resource/implementation for the Mcleod Pitch Method and other pitch-detecting algorithms:
https://github.com/sevagh/pitch-detection
