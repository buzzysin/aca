#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <fstream>
#include <iostream>
#include <string>

#include <TkArch/Core.h>
namespace sim {
  static const std::string VERSION = "0.0.1";

  struct args {
    std::string file;
  };

  void usage();
  void version();
  void get_args(int n, char **argv, args &args);
  void prompt_generate(args &args);
  void generate();
  void run(const args &args);

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

  void get_args(int n, char **argv, args &args) {

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

  void prompt_generate(args &args) {
    DEBUG_MODE_ONLY(IGNORE(args));
    RELEASE_MODE_ONLY(
        std::cout
            << "No file specified, would you like to generate a new file? "
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

  void generate() {
    //
  }

  void run(args &args) {
    if (args.file.empty()) {
      prompt_generate(args);
    }

    create_printer("Simulator");
    print_info("Welcome to the simulator!");

    processor processor;

    unsigned int seed;
    for (unsigned int i = 0; i < processor.mem.size; i++) {
      processor.mem.write(i, rand_r(&seed) % 256);
    }

    while (true) {
      processor.advance();
    }

    processor.stop();
  }
} // namespace sim

#endif