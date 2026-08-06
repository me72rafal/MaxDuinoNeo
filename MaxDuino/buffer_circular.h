#ifndef BUFFER_CIRCULAR_H_INCLUDED
#define BUFFER_CIRCULAR_H_INCLUDED

#include "Arduino.h"
#include <algorithm>

class BufferCircular {

 private:
  uint16_t* buffer;
  uint16_t readPos;   // only used within the ISR, never accessed outside, so doesn't need to be volatile
  uint16_t writePos;  // only used within the main loop, never accessed by ISR
  uint16_t available;  // number of words available to read
  uint16_t bufferSize;

 public:
  BufferCircular(uint16_t size) : readPos(0), writePos(0), available(0), bufferSize(size) {
    buffer = (uint16_t*)ps_malloc(size * sizeof(uint16_t));
  }

  ~BufferCircular() {
    if (buffer) {
      free(buffer);
    }
  }

  // clears buffer state but does not free the memory.  This is useful if you
  // want to reuse the buffer without reallocating it.
  void  clearBuffer(void) {
    writePos = 0;
    readPos = 0;
    available = 0;
  }

  // writes
  size_t write_word( const uint16_t data ) {

    #ifdef SERIAL_DEBUG
    Serial.printf("write_word: %lu writePos=%lu (readpos=%lu)", data, writePos, readPos);   
   
    #endif 

    if (!buffer) return 0;

    buffer[writePos] = data;

    return 1;
  }


  size_t modify_read_word( const uint16_t data ) {

    if ( available == 0 || !buffer) return 0;

   buffer[readPos] = data;
   return 1;
  }

  // writes word to a buffer, but does not advance the write pointer.  This is useful if you want to write multiple words in a row and then advance the pointer once at the end.
  // need to check if buffer is full before calling this function, otherwise it will overwrite the oldest data in the buffer.
  void advance_write_word( void ) {

    if ( available == bufferSize) return; // nothing to write

    writePos = (writePos + 1) % bufferSize;
    available++;
  }

  size_t read_word( uint16_t* data ) {

    if (!buffer) return 0;

    *data = buffer[readPos];

    #ifdef SERIAL_DEBUG
    Serial.printf("read_word: %lu readPos=%lu (writePos=%lu)", *data, readPos, writePos);   
    #endif 

    return 1;
  }



  // advance buffer pointers by one word
 void advance_read_word( void ) {

    if (available == 0) return; // nothing to read
    
    readPos = (readPos + 1) % bufferSize;
    available--;
}

// returns true if buffer full
  bool full()  {
    return (available + 16) == bufferSize;
  }
};

extern BufferCircular wbuffer;

#endif // BUFFER_CIRCULAR_H_INCLUDED