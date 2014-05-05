
#include <iostream>
#include <fstream>
#include "note_reader.hpp"

#include <vector>

int main (int argc, char **argv) {
  for (int i = 1; i < argc; ++i) {
    std::vector<NoteToPlay> notes;
    // std::ifstream ifs (argv[i], std::ifstream::in);
    while (std::cin) {
      NoteToPlay note;
      std::cin >> note;
      if (Note::None != note.note) {
        notes . push_back (note);
      }
    }

    for (const NoteToPlay& note : notes) {
      std::cout << note << "\n";
    }
  }

  return 0;
}
