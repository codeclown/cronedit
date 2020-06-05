#include <sstream>
#include <iostream>
#include <string>
#include <assert.h>
#include <vector>
#include "parser.h"
#include "state.h"
#include "stringify.h"

int main()
{
  State state;
  state.parse_line("");
  state.parse_line("# https://stackoverflow.com/a/5823331/239527");
  state.parse_line("SSH_AUTH_SOCK=/private/tmp/com.apple.launchd.WvQxE0oPYh/Listeners");
  state.parse_line("0 */6 * * * bash /asd/backup.sh >/asd/backup.log 2>&1");
  state.parse_line("#* * * * * bash /asd/backup.sh >/asd/backup.log 2>&1");
  state.parse_line("");
  state.parse_line("");

  std::string stringified = stringify_items(state.get_items());
  std::cout << "---" << std::endl;
  std::cout << stringified << std::endl;
  std::cout << "---" << std::endl;
  assert(stringified == "\
\n\
# https://stackoverflow.com/a/5823331/239527\n\
SSH_AUTH_SOCK=/private/tmp/com.apple.launchd.WvQxE0oPYh/Listeners\n\
0 */6 * * * bash /asd/backup.sh >/asd/backup.log 2>&1\n\
# * * * * * bash /asd/backup.sh >/asd/backup.log 2>&1\n\
\n\
\n\
");

  std::cout << "PASS" << std::endl;
}
