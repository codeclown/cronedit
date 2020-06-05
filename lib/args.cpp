#include <string>
#include <vector>
#include "args.h"

Args parse_args(int argc, char* argv[]) {
  Args args;
  args.show_usage = false;
  args.show_version = false;
  args.filename = "";

  std::vector<std::string> filenames;

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      args.show_usage = true;
      return args;
    }
    if (arg == "--version" || arg == "-v") {
      args.show_version = true;
      return args;
    }
    filenames.push_back(arg);
  }

  if (filenames.size() > 1) {
    args.show_usage = true;
  } else if (filenames.size() > 1) {
    args.show_usage = true;
  } else if (filenames.size() == 1) {
    args.filename = filenames[0];
  }

  return args;

  // bool show_usage = false;
  // if (argc >= 2) {
  //   for (int i = 1; i < argc; i++) {
  //     std::string arg = argv[i];
  //     if (arg == "--help" || arg == "-h") {
  //       show_usage = true;
  //       break;
  //     }
  //   }
  // }
  // if (show_usage) {
  //   print_usage(argv[0]);
  //   exit(0);
  // } else if (argc > 2) {
  //   print_usage(argv[0]);
  //   exit(1);
  // }
  // std::string file_arg = "";
  // if (argc == 2) {
  //   file_arg = argv[1];
  // }

  // return args;
}
