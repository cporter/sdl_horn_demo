
#pragma once

#include <iostream>

enum class Note {
  None, Ab, A, Bb, B, C, Db, D, Eb, E, F, Gb, G
};

struct NoteToPlay {
  Note note;
  double duration, volume;
};

std::istream& operator>>(std::istream&, NoteToPlay&);
std::ostream& operator<<(std::ostream&, const NoteToPlay&);
