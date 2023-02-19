#include <fstream>
#include <iostream>

#include <TkArch/Core.h>
#include <TkArch/Util.h>

static char program_name[] = "TkArchSimulator";

void usage() {
  std::cout << "Usage: " << program_name << " <instructions_file>";
}

int main(int argc, char **argv) {
  // Check for the correct number of arguments
  if (argc != 2) {
    usage();
    return 1;
  }

  // Get the path to the instructions file
  std::string instr_path = argv[1];

  // Accept an instructions file as a command line argument
  std::cout << instr_path << "\n";

  // Open the instructions file
  std::ifstream instr_file(instr_path);

  // Create a processor
  Processor *processor = new Processor();

  processor->start();

  return 0;
}
