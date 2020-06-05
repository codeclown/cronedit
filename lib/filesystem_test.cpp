#include <sstream>
#include <iostream>
#include <string>
#include <assert.h>
#include "filesystem.h"

int main()
{
  std::string path = ".filesystem_test.tmp";
  std::string contents = "test\nfoobar";

  assert(write_file(path, contents) == true);
  assert(read_file(path) == contents);
  assert(remove_file(path) == true);

  std::cout << "PASS" << std::endl;
}
