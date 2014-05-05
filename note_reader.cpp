
#include "note_reader.hpp"

//   None, Ab, A, Bb, B, C, Db, D, Eb, E, F, Gb, G

std::istream& operator>>(std::istream& input, Note& note) {
  std::string n;

  note = Note::None;

  input >> n;
  if ("Ab" == n) {
    note = Note::Ab;
  } else if ("A" == n) {
    note = Note::A;
  } else if ("Bb" == n || "A#" == n) {
    note = Note::Bb;
  } else if ("B" == n) {
    note = Note::B;
  } else if ("C" == n) {
    note = Note::C;
  } else if ("Db" == n) {
    note = Note::Db;
  } else if ("D" == n) {
    note = Note::D;
  } else if ("Eb" == n) {
    note = Note::Eb;
  } else if ("E" == n) {
    note = Note::E;
  } else if ("F" == n) {
    note = Note::F;
  } else if ("Gb" == n) {
    note = Note::Gb;
  } else if ("G" == n) {
    note = Note::G;
  } 
  return input;
}

std::istream& operator>>(std::istream& input, NoteToPlay& note) {
  input >> note.note;
  if (Note::None != note.note) {
    input >> note.duration >> note.volume;
  }
  return input;
}

std::ostream& operator<<(std::ostream& o, const Note& note) {
#define PRINT_NOTE(N) case Note::N: o << #N; break;
  switch (note) {
    PRINT_NOTE(Ab);
    PRINT_NOTE(A);
    PRINT_NOTE(B);
    PRINT_NOTE(Bb);
    PRINT_NOTE(C);
    PRINT_NOTE(Db);
    PRINT_NOTE(D);
    PRINT_NOTE(Eb);
    PRINT_NOTE(E);
    PRINT_NOTE(F);
    PRINT_NOTE(Gb);
    PRINT_NOTE(G);
    default: o << "??"; break;
  }
#undef PRINT_NOTE
  return o;
}

std::ostream& operator<<(std::ostream& output, const NoteToPlay& note) {
  if (Note::None != note.note) {
    output << note.note << "\t" << note.duration << "\t" << note.volume;
  }
  return output;
}
