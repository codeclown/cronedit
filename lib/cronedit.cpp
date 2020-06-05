#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "args.h"
#include "exec.h"
#include "filesystem.h"
#include "parser.h"
#include "render.h"
#include "state.h"
#include "stringify.h"
#include "terminal.h"
#include "version.h"

int terminal_width = get_terminal_width();
int terminal_height = get_terminal_height();

State state;

void print_version() {
  std::cout << "cronedit v" << CRONEDIT_VERSION << std::endl;
}

void print_usage(char* program) {
  print_version();
  std::cout << std::endl;
  std::cout << "Usage: " << std::endl;
  std::cout << "  " << program << std::endl;
  std::cout << "  " << program << " <file>" << std::endl;
  std::cout << std::endl;
  std::cout << "Arguments:" << std::endl;
  std::cout << "  file            If given, this file is read and written to" << std::endl;
  std::cout << "                  instead of modifying crontab directly via" << std::endl;
  std::cout << "                  the 'crontab' command" << std::endl;
  std::cout << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "  -h, --help      Show this help message" << std::endl;
  std::cout << "  -v, --version   Show cronedit version" << std::endl;
  std::cout << std::endl;
  std::cout << "Information about backups:" << std::endl;
  std::cout << "  Upon saving changes, previous crontab version is backed up" << std::endl;
  std::cout << "  into folder '$HOME/.cronedit'." << std::endl;
  std::cout << std::endl;
  std::cout << "Source:" << std::endl;
  std::cout << "  This program is open source and available at:" << std::endl;
  std::cout << "  https://github.com/codeclown/cronedit" << std::endl;
  std::cout << std::endl;
}

std::string ui;
std::string previous;
void render() {
  // state.print_debug();
  ui = render_state(state, terminal_width, terminal_height);
  std::cout << render_diff(ui, previous);
  previous = ui;
}

std::string saved_file;
std::string backup_file;
void quit(int code) {
  state.on_quit(saved_file, backup_file);
  render();
  revert_terminal();
  exit(code);
}

void on_sigint(sig_atomic_t s) {
  quit(1);
}

void save_changes(std::string original, std::string file_arg) {
  std::string cronedit_dir;
  const char *homedir = getenv("HOME");
  if (homedir != NULL) {
    std::string home = homedir;
    cronedit_dir = home + "/.cronedit";
    make_dir_if_not_exists(cronedit_dir);
  }
  std::string stringified = stringify_items(state.get_items());
  if (file_arg != "") {
    if (write_file(file_arg, stringified)) {
      saved_file = file_arg;
    }
  } else {
    if (cronedit_dir != "") {
      std::string tmp_filename = cronedit_dir + "/tmp_" + std::to_string(std::time(nullptr));
      if (write_file(tmp_filename, stringified)) {
        exec("crontab " + tmp_filename);
        remove_file(tmp_filename);
        saved_file = "crontab";
      }
    }
  }
  if (saved_file != "" && cronedit_dir != "") {
    std::string backup_filename = cronedit_dir + "/backup_" + std::to_string(std::time(nullptr));
    if (write_file(backup_filename, original)) {
      backup_file = backup_filename;
    }
  }
}

int main(int argc, char* argv[]) {
  if (!is_tty()) {
    std::cerr << "cronedit requires a TTY to function." << std::endl;
    exit(1);
  }

  int min_width = MIN_TERMINAL_WIDTH;
  int min_height = MIN_TERMINAL_HEIGHT;
  if (terminal_width < min_width || terminal_height < min_height) {
    std::cerr << "cronedit expects terminal size to be at least ";
    std::cerr << min_width << "x" << min_height;
    std::cerr << " (current: ";
    std::cerr << terminal_width << "x" << terminal_height;
    std::cerr << ")";
    std::cerr << std::endl;
    exit(1);
  }

  Args args = parse_args(argc, argv);
  if (args.show_usage) {
    print_usage(argv[0]);
    exit(0);
  }
  if (args.show_version) {
    print_version();
    exit(0);
  }

  signal(SIGINT, on_sigint);

  std::string crontab;
  if (args.filename != "") {
    crontab = read_file(args.filename);
  } else {
    crontab = exec("crontab -l 2>&1");
    if (crontab.substr(0, 23) == "crontab: no crontab for") {
      crontab = "";
    }
  }

  if (crontab.length() > 0) {
    std::istringstream lines(crontab);
    std::string line;
    while (std::getline(lines, line)) {
      state.parse_line(line);
    }
  }

  init_terminal();

  int ch = 0;
  bool editing_value;
  std::string input;
  while (true) {
    render();

    editing_value = state.is_editing_value();
    if (editing_value) {
      prepare_for_input();
      input = "";
      std::getline(std::cin, input);
      state.on_end_editing_value(input);
      init_terminal();
      continue;
    }

    ch = terminal_get_char();
    // std::cout << ch << std::endl;

    if (ch == KEY_UP) {
      state.on_key_up();
    } else if (ch == KEY_DOWN) {
      state.on_key_down();
    } else if (ch == KEY_LEFT) {
      state.on_key_left();
    } else if (ch == KEY_RIGHT) {
      state.on_key_right();
    } else if (ch == KEY_C) {
      state.on_toggle_comment();
    } else if (ch == KEY_N) {
      state.on_create_new();
    } else if (ch == KEY_D) {
      state.on_duplicate();
    } else if (ch == KEY_R) {
      state.on_remove();
    } else if (ch == KEY_S) {
      save_changes(crontab, args.filename);
      break;
    } else if (ch == KEY_ENTER && !editing_value) {
      if (state.on_begin_editing_value()) {
        revert_terminal();
      }
    } else if (ch == KEY_Q/* || ch == KEY_ESC*/) {
      break;
    }
  }

  quit(0);

  return 0;
}
