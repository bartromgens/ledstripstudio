#ifndef UNIVERSALSLEEP_H
#define UNIVERSALSLEEP_H

#ifdef WIN32
  #include <Windows.h>
#else
  #include <unistd.h>  // for (u)sleep on Linux
#endif

namespace universalsleep
{
  static void sleep_ms(unsigned int)
  {
    #ifdef WIN32
      Sleep(10);
    #else
      usleep(10*1000);
    #endif
  }
}

#endif // UNIVERSALSLEEP_H
