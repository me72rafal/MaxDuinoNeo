#ifndef SORT_DIRS_H_INCLUDED

#define SORT_DIRS_H_INCLUDED

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

#define ELEMENT_COUNT_MAX 1024

struct dirEntry {
  char name[256];
  uint16_t index;
  bool isDir;
};

static bool comp(const dirEntry& a, const dirEntry& b) {
  return (strcmp(a.name, b.name));
}

class MaxduinoDirectory {
 private:
  // dirEntry storage_array [ELEMENT_COUNT_MAX];
  std::vector<dirEntry> entries;

 public:
  /**
  @brief Constructs a new Directory object
  */
  MaxduinoDirectory() {
    // entries.setStorage( storage_array );
  }

  /**
  @brief Destroys the Directory object
  */
  ~MaxduinoDirectory() { clear(); }

  /**
  @brief clears Directory object
  */
  void clear() { entries.clear(); }

  /**
  @brief returs number of entries
  */
  size_t size() { return entries.size(); }

  dirEntry* get(size_t position) {
    if (position >= 0 && position < entries.size()) {
      return &entries[position];
    }
    return nullptr;
  }

  /**
   * @brief Appends element to the list of entries
   */
  void append(const dirEntry& value) { entries.push_back(value); }

  /**
   * @brief Sorts the directory entries
   */
  void sort() {
    for (int f = 0; f < entries.size(); f++) {
      Serial.printf("Before Entry %d Name: %s Index %u IsDir %d \n ", f, entries[f].name, entries[f].index, entries[f].isDir);
    }
      Serial.printf("Sorting\n");

    std::sort(entries.begin(), entries.end(),
              [](const dirEntry& a, const dirEntry& b) {
                if (a.isDir != b.isDir) return (a.isDir < b.isDir);
     return ( (bool) strcmp( a.name, b.name) );
              });

    for (int f = 0; f < entries.size(); f++) {
      Serial.printf("After Entry %d Name: %s Index %u IsDir %d\n ", f,
                    entries[f].name, entries[f].index, entries[f].isDir);
    }
  }
};

#endif  // SORT_DIRS_H_INCLUDED