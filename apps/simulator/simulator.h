#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <fstream>
#include <iostream>
#include <string>

#include <TkArch/Core.h>

static const std::string VERSION = "0.0.1";

struct simulator_args {
  std::string file;
};

void simulator_usage();
void simulator_version();
void simulator_get_args(int n, char **argv, simulator_args &args);
void simulator_prompt_generate(simulator_args &args);
void simulator(const simulator_args &args);

void simulator_usage() {
  std::cout << "./Simulator [options]" << printer::newline;
  std::cout << "Options:" << printer::newline;

  std::cout << "  -h, --help\t"
            << "Print this help message" << printer::newline;

  std::cout << "  -v, --version\t"
            << "Print the version of the simulator" << printer::newline;

  std::cout << "  -f, --file=<file>\t"
            << "The file to simulate" << printer::newline;
}

void simulator_version() {
  std::cout << "Simulator v0.0.1" << printer::newline;
}

void simulator_get_args(int n, char **argv, simulator_args &args) {

  for (int i = 1; i < n; i++) {
    std::string arg = argv[i];
    if (arg == "-h" || arg == "--help") {
      simulator_usage();
      exit(0);
    } else if (arg == "-v" || arg == "--version") {
      simulator_version();
      exit(0);
    } else if (arg == "-f" || arg == "--file") {
      if (i + 1 < n) {
        args.file = argv[i + 1];
        std::cout << "File: " << args.file << printer::newline;
        i++;
      }
    } else {
      std::cout << "Unknown argument: " << arg << printer::newline;
      exit(1);
    }
  }
}

void simulator_prompt_generate(simulator_args &args) {
  DEBUG_MODE_ONLY(IGNORE(args));
  RELEASE_MODE_ONLY(
      std::cout << "No file specified, would you like to generate a new file? "
                   "[y/N]: ";

      char input;
      std::cin.get(input);

      if (input == 'y' || input == 'Y') {
        std::cout << "Generating file..." << printer::newline;
        args.file = "../programs/vec_add.bin";
      } else {
        std::cout << "Exiting..." << printer::newline;
        exit(0);
      });
}

void simulator(simulator_args &args) {
  if (args.file.empty()) {
    simulator_prompt_generate(args);
  }

  create_printer("Simulator");

  print_info("Welcome to the simulator!");
  print_info("Press any key to stop the simulation.");

  std::chrono::milliseconds interval(1000);
  std::this_thread::sleep_for(interval);

  processor processor;

  unsigned int seed;

  for (unsigned int i = 0; i < processor.mem.size; i++) {
    processor.mem.write(i, rand_r(&seed) % 256);
  }

  while (true) {
    // std::cout << "step > ";
    
    // char c;
    // c = std::cin.get();

    // if (c == 'q') {
    //   break;
    // }

    processor.advance();
    // std::this_thread::sleep_for(interval);
  }

  processor.stop();
  

  exit(0);
}

#endif