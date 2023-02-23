#ifndef ARGS_H
#define ARGS_H

#include <fstream>
#include <iostream>
#include <string>

#include <TkArch/Core.h>

static const std::string VERSION = "0.0.1";

struct simulator_args {
  std::string file;
};

void usage();
void version();
void simulator_get_args(int n, char **argv, simulator_args &args);
void simulator_prompt_generate(simulator_args &args);
void simulator(const simulator_args &args);

void usage() {
  std::cout << "./Simulator [options]" << printer::newline;
  std::cout << "Options:" << printer::newline;

  std::cout << "  -h, --help\t"
            << "Print this help message" << printer::newline;

  std::cout << "  -v, --version\t"
            << "Print the version of the simulator" << printer::newline;

  std::cout << "  -f, --file=<file>\t"
            << "The file to simulate" << printer::newline;
}

void version() { std::cout << "Simulator v0.0.1" << printer::newline; }

void simulator_get_args(int n, char **argv, simulator_args &args) {

  for (int i = 1; i < n; i++) {
    std::string arg = argv[i];
    if (arg == "-h" || arg == "--help") {
      usage();
      exit(0);
    } else if (arg == "-v" || arg == "--version") {
      version();
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
#if DEBUG_MODE
  IGNORE(args);
#else
  std::cout
      << "No file specified, would you like to generate a new file? [y/N]: ";

  char input;

  std::cin.get(input);

  if (input == 'y' || input == 'Y') {
    std::cout << "Generating file..." << printer::newline;
    args.file = "../programs/vec_add.bin";
  } else {
    std::cout << "Exiting..." << printer::newline;
    exit(0);
  }
#endif
}

void simulator(simulator_args &args) {
  if (args.file.empty()) {
    simulator_prompt_generate(args);
  }

  info("Welcome to the simulator!");
  info("Press any key to stop the simulation.");

  std::chrono::milliseconds interval(1000);
  std::this_thread::sleep_for(interval);

  processor processor;

  processor.start();

  std::cin.get();

  processor.stop();

  info("Simulation stopped.");

  exit(0);
}

#endif