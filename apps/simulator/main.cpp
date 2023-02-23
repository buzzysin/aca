#include <TkArch/Core.h>

#include <string>

#include "args.h"

int main(int n, char **argv) {
  IGNORE(n);
  IGNORE(argv);
  
  simulator_args args;
  simulator_get_args(n, argv, args);

  simulator(args);
}