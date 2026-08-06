#include "buffer_circular.h"

#ifdef LARGE_BUFFER
  #define BUFFER_SIZE 512
#else
  #define BUFFER_SIZE 128
#endif

BufferCircular wbuffer(BUFFER_SIZE); // circular buffer instance

