#ifndef USERDATA_H
#define USERDATA_H

#include <vector>
#include <stack>
#include <deque>

#include <mutex>

class paUserData
{
public:
  paUserData();
  ~paUserData();

public:
//  std::mutex g_mutexer;
//  int frameIndex;  //Index into sample array
//  int maxFrameIndex;
  std::mutex data_mutex;
  std::deque<float> recordedSamplesVec;
  int nChannels;
  double sampleRate;
  unsigned int nSamples;
  unsigned long n_framesPerBuffer;
};

#endif // USERDATA_H
