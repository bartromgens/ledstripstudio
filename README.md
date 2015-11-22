# ledstripstudio
An application for realtime audio analysis and ledstrip visualisation written in C++.

### Features
- Control a full color RGB ledstrip @ 35 FPS (5 m strip)
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
