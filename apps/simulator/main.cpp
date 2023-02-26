#include <TkArch/Core.h>

#include "simulator.h"

#include <string>

int main(int n, char **argv) {
  // Acquire the arguments
  sim::args args;
  sim::get_args(n, argv, args);

  // If no file was specified, prompt the user to generate one
  sim::prompt_generate(args);

  // Run the simulator
  sim::run(args);

  // Exit
  return 0;
}