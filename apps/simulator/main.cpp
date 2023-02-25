#include <TkArch/Core.h>

#include "simulator.h"

#include <string>

int main(int n, char ** argv) {

  simulator_args args;
  simulator_get_args(n, argv, args);

  simulator_prompt_generate(args);

  simulator(args);

  return 0;
}