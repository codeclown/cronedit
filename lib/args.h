#ifndef ARGS_H
#define ARGS_H

struct Args {
  bool show_usage;
  bool show_version;
  std::string filename;
};

Args parse_args(int argc, char* argv[]);

#endif
