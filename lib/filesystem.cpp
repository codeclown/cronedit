#include <sstream>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

void make_dir_if_not_exists(std::string path) {
  // read/write/search permissions for owner
  // read/write/search permissions for group
  // read/search permissions for others
  mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

std::string read_file(std::string path) {
  std::ifstream file_arg_handle(path);
  std::stringstream buffer;
  buffer << file_arg_handle.rdbuf();
  return buffer.str();
}

bool write_file(std::string path, std::string contents) {
  std::ofstream file (path, std::ios::out);
  if (file.is_open()) {
    file << contents;
    file.close();
    return true;
  }
  return false;
}

bool remove_file(std::string path) {
  if (unlink(path.c_str()) == 0) {
    return true;
  }
  return false;
}
