#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include "parser.h"
#include "state.h"
#include "render.h"

int main()
{
  {
    std::cout << "Initial state" << std::endl;
    State state;
    assert(state.get_items().size() == 0);
    assert(state.get_active_item() == -1);
    assert(state.get_active_column() == 0);
    assert(state.get_column_widths()[0] == 1);
    assert(state.get_column_widths()[1] == 1);
    assert(state.get_column_widths()[2] == 1);
    assert(state.get_column_widths()[3] == 1);
    assert(state.get_column_widths()[4] == 1);
    assert(state.get_column_widths()[5] == 1);
    assert(state.is_editing_value() == false);
    assert(state.is_quitting() == false);
    assert(state.get_saved_file() == "");
    assert(state.get_backup_file() == "");
    assert(state.get_error_message() == "");
  }

  {
    std::cout << "Can call any event in initial state" << std::endl;
    State state;
    state.on_key_up();
    state.on_key_down();
    state.on_key_left();
    state.on_key_right();
    state.on_toggle_comment();
    state.on_duplicate();
    state.on_remove();
    state.on_begin_editing_value();
    state.on_end_editing_value("foobar");
    state.on_quit("saved_file.test", "backup_file.test");
  }

  {
    std::cout << "Can create new items" << std::endl;
    State state;
    state.on_create_new();
    assert(state.get_items().size() == 1);
    assert(state.get_active_item() == 0);
    assert(state.get_active_column() == 0);
    assert(state.get_column_widths()[0] == 1);
    assert(state.get_column_widths()[1] == 1);
    assert(state.get_column_widths()[2] == 1);
    assert(state.get_column_widths()[3] == 1);
    assert(state.get_column_widths()[4] == 1);
    assert(state.get_column_widths()[5] == 18);
  }

  {
    std::cout << "Can't begin editing if active item is not a job" << std::endl;
    State state;
    state.parse_line("foobar");
    assert(state.on_begin_editing_value() == false);
    assert(state.is_editing_value() == false);
  }

  {
    std::cout << "Can begin editing if active item is not a job" << std::endl;
    State state;
    state.parse_line("0 6/* * * * bash test2.sh");
    assert(state.on_begin_editing_value() == true);
    assert(state.is_editing_value() == true);
  }

  std::cout << "PASS" << std::endl;
}
