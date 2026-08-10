#include "buffer_circular.h"

#ifdef LARGEBUFFER
  #define BUFFER_SIZE 512
#else
  #define BUFFER_SIZE 128
#endif

BufferCircular wbuffer( 60000 ); // circular buffer instance

