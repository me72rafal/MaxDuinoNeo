#ifndef BUFFER_CIRCULAR_H_INCLUDED
#define BUFFER_CIRCULAR_H_INCLUDED

#include "Arduino.h"
#include <algorithm>

class BufferCircular {

 private:
  uint16_t* buffer;
  uint16_t readPos;   // only used within the ISR, never accessed outside, so doesn't need to be volatile
  uint16_t writePos;  // only used within the main loop, never accessed by ISR
  uint16_t usedPositions;  // number of buffer positions used
  uint16_t bufferSize; // size of the buffer
  bool isPrefilled; // indicate if buffer is prefilled and one can start playing

 public:
  BufferCircular(uint16_t size) : readPos(0), writePos(0), usedPositions(0), bufferSize(size), isPrefilled(false) {
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
    usedPositions = 0;
    isPrefilled = false;
  }

  // writes
  size_t write_word( const uint16_t data ) {

    /*#ifdef SERIALSCREEN
    Serial.printf("write_word: %u writePos=%u (readpos=%u)\n", data, writePos, readPos);   
    #endif */
    if (!buffer) return 0;

    buffer[writePos] = data;
    return 1;
  }


  size_t modify_read_word( const uint16_t data ) {

    if ( usedPositions == 0 || !buffer) return 0;

   buffer[readPos] = data;
   return 1;
  }

  // writes word to a buffer, but does not advance the write pointer.  This is useful if you want to write multiple words in a row and then advance the pointer once at the end.
  // need to check if buffer is full before calling this function, otherwise it will overwrite the oldest data in the buffer.
  void advance_write_word( void ) {

    if ( usedPositions == bufferSize) return; // nothing to write

    writePos = (writePos + 1) % bufferSize;
    usedPositions++;

    // if half  positions are used, we indicate that playback can start
    if ( usedPositions > (bufferSize>>1) ) isPrefilled = true; // 
  }

  size_t read_word( uint16_t* data ) {

    if (!buffer) return 0;

    if ( usedPositions == 0 ) {
    assert(false && "Triggered manual halt for analysis");
    }

    *data = buffer[readPos];

    return 1;
  }



  // advance buffer pointers by one word
 void advance_read_word( void ) {

    if (usedPositions == 0) return; // nothing to read
    
    readPos = (readPos + 1) % bufferSize;
    usedPositions--;
}

// returns true if buffer full
  bool full()  {
    return (usedPositions == bufferSize );
  }

// returns true if buffer can fit 8 values
  bool canfit8()  {
    return ( usedPositions < ( bufferSize - 8 ));
  }

// returns true if buffer can be safely emptied
  bool filled() {
    return ( usedPositions > 8 );
  }

  // returns true if the buffer is filled by half and can be played out
  bool canPlay() {
    return (isPrefilled);
  }
};



extern BufferCircular wbuffer;

#endif // BUFFER_CIRCULAR_H_INCLUDED