#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include "poker.h"
#include "thread.h"
#include "util.h"

using namespace std;

void set_threads(istringstream &is) {

  string token;
  if (is >> token)
    Threads.set(stoi(token));
}

void go(istringstream &is) {

  string token, pos;

  while (is >> token)
    pos += token + " ";

  Spot s(pos);
  if (!s.valid()) {
    cerr << "Error in: " << pos << endl;
    return;
  }

  unsigned results[10];
  memset(results, 0, sizeof(results));
  Threads.run(s, 1000 * 1000, results);
  print_results(results, s.players());
}

void eval(istringstream &is) {

  string token, pos = "1P ";

  while (is >> token)
    pos += token + " ";

  Spot s(pos);
  if (!s.valid()) {
    cerr << "Error in: " << pos << endl;
    return;
  }

  cout << "Score is: " << s.eval() << "\n"
       << pretty_hand(s.eval(), false) << endl;
}

void enumerate(istringstream &is) {

  string token, pos;

  while (is >> token)
    pos += token + " ";

  Spot s(pos);
  if (!s.valid()) {
    cerr << "Error in: " << pos << endl;
    return;
  }

  auto size = s.set_enumerate_mode();
  if (!size)
    return;

  unsigned results[10];
  memset(results, 0, sizeof(results));
  Threads.run(s, size, results);
  print_results(results, s.players());
}

int main(int argc, char *argv[]) {

  init_score_mask();
  Threads.set(1);

  string token, cmd;

  for (int i = 1; i < argc; ++i)
    cmd += std::string(argv[i]) + " ";

  do {

    if (argc == 1 && !getline(cin, cmd)) // Block here waiting for input or EOF
      cmd = "quit";

    istringstream is(cmd);

    token.clear(); // Avoid a stale if getline() returns empty or blank line
    is >> skipws >> token;

    if (token == "quit")
      break;
    else if (token == "threads")
      set_threads(is);
    else if (token == "go")
      go(is);
    else if (token == "enum")
      enumerate(is);
    else if (token == "eval")
      eval(is);
    else if (token == "bench")
      bench(is);
    else
      cout << "Unknown command: " << cmd << endl;

  } while (token != "quit" && argc == 1); // Command line args are one-shot

  cout << endl; // FXIME

  return 0;
}
