#include "tonestudio.h"

ToneStudio::ToneStudio()
{
}


ToneStudio::~ToneStudio()
{
}


Animation
ToneStudio::createToneAnimation(unsigned int nLEDs, std::map<std::string, double> tones)
{
  Animation animation;

  Frame frame(nLEDs);

//  for (auto iter = tones.begin(); iter != tones.end(); ++iter)
//  {
//    std::cout << "Tone: " << iter->first << " : " ;
//    for (std::size_t i = 0; i < iter->second/15; ++i)
//    {
//      std::cout << "-";
//    }
//    std::cout << std::endl;
//  }

  double currentMax = 0.0;
  std::string maxTone;

  for(auto it = tones.cbegin(); it != tones.cend(); ++it )
  {
    if (it->second > currentMax)
    {
      maxTone = it->first;
      currentMax = it->second;
    }
  }

//          std::cout << "AudioInput::createToneAnimation() - maxTone: " << maxTone << std::endl;

  for (unsigned int i = 0; i < nLEDs; ++i)
  {
//    double r = 0.0;
//    double g = 0.0;
//    double b = 0.0;

//    for (auto iter = tones.begin(); iter != tones.end(); ++iter)
//    {
//      if (iter->first == "C")
//      {
//        r += iter->second;
//      }
//      else if (iter->first == "D")
//      {
//        r += iter->second/2.0;
////        g += iter->second/2.0;
//      }
//      else if (iter->first == "E")
//      {
//        g += iter->second;
//      }
//      else if (iter->first == "F")
//      {
//        g += iter->second/2.0;
////        b += iter->second/2.0;
//      }
//      else if (iter->first == "G")
//      {
//        b += iter->second;
//      }
//      else if (iter->first == "A")
//      {
//        b += iter->second/2.0;
////        r += iter->second/2.0;
//      }
//      else if (iter->first == "B")
//      {
//        r += iter->second;
//      }
//    }

//    int norm = sqrt(r*r+g*g+b*b);
//    int rNorm = r/norm*127;
//    int gNorm = g/norm*127;
//    int bNorm = b/norm*127;

//    Color color(rNorm, gNorm, bNorm);

    Color color;
    if (maxTone == "C")
    {
      color = Color(127, 0, 0);
    }
    else if (maxTone == "D")
    {
      color = Color(127, 127, 0);
    }
    else if (maxTone == "E")
    {
      color = Color(0, 127, 0);
    }
    else if (maxTone == "F")
    {
      color = Color(0, 127, 127);
    }
    else if (maxTone == "G")
    {
      color = Color(0, 0, 127);
    }
    else if (maxTone == "A")
    {
      color = Color(127, 0, 127);
    }
    else if (maxTone == "B")
    {
      color = Color(127, 50, 50);
    }

    LED led(i, color);
    frame.addLED(led);
  }

  animation.addFrame(frame);

  return animation;
}
