#include <sstream>
#include <iostream>
#include <string>
#include <assert.h>
#include "args.h"

int main()
{
  char *argv1[1] = { (char*)"./bin/cronedit" };
  Args args1 = parse_args(1, argv1);
  assert(args1.show_usage == false);
  assert(args1.show_version == false);
  assert(args1.filename == "");

  char *argv2[2] = { (char*)"./bin/cronedit", (char*)"-h" };
  Args args2 = parse_args(2, argv2);
  assert(args2.show_usage == true);
  assert(args2.show_version == false);
  assert(args2.filename == "");

  char *argv3[2] = { (char*)"./bin/cronedit", (char*)"--help" };
  Args args3 = parse_args(2, argv3);
  assert(args3.show_usage == true);
  assert(args3.show_version == false);
  assert(args3.filename == "");

  char *argv4[2] = { (char*)"./bin/cronedit", (char*)"-v" };
  Args args4 = parse_args(2, argv4);
  assert(args4.show_usage == false);
  assert(args4.show_version == true);
  assert(args4.filename == "");

  char *argv5[2] = { (char*)"./bin/cronedit", (char*)"--version" };
  Args args5 = parse_args(2, argv5);
  assert(args5.show_usage == false);
  assert(args5.show_version == true);
  assert(args5.filename == "");

  char *argv6[2] = { (char*)"./bin/cronedit", (char*)"foobar" };
  Args args6 = parse_args(2, argv6);
  assert(args6.show_usage == false);
  assert(args6.show_version == false);
  assert(args6.filename == "foobar");

  char *argv7[3] = { (char*)"./bin/cronedit", (char*)"foobar", (char*)"lorem" };
  Args args7 = parse_args(3, argv7);
  assert(args7.show_usage == true);
  assert(args7.show_version == false);
  assert(args7.filename == "");

  std::cout << "PASS" << std::endl;
}
