# ledstripstudio
An application for realtime audio analysis and ledstrip visualisation written in C++.

### Features
- Control a full color RGB ledstrip @ 60 FPS (5 m strip)
- Capture audio from any music player, browser or microphone
- Spectrum analysis
- Tone detection
- Beat detection [In progress]
- Play ledstrip animation from image
- ...

### Dependencies
- Boost (thread and system)
- Qt 5
- PortAudio
- FFTW

### References
- https://en.wikipedia.org/wiki/Loudness
- https://en.wikipedia.org/wiki/Weber-Fechner_law
- https://en.wikipedia.org/wiki/Cepstrum, https://en.wikipedia.org/wiki/Mel-frequency_cepstrum
- https://en.wikipedia.org/wiki/Audio_to_video_synchronization
- http://electronics.stackexchange.com/questions/12407/what-is-the-relation-between-fft-length-and-frequency-resolution
- "Real-time temporal segmentation of note objects in music signals" - http://aubio.org/articles/brossier04realtimesegmentation.pdf


## Controllers 

### Teensy

For the Teensy 3.1, 3.2, one of the following Hardware SPI data/clock pin combinations should be used,

 - Hardware SPI - data 11, clock 13
 - Hardware SPI - data 7, clock 14
 
See a list of configurations for other controllers [here](https://github.com/FastLED/FastLED/wiki/SPI-Hardware-or-Bit-banging).
