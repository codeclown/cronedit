#include <cmath>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "parser.h"
#include "state.h"
#include "render.h"

std::string RESET = "\033[0m";
std::string BLACK = "\033[30m";
std::string RED_BG = "\033[41m";
std::string GREEN = "\033[32m";
std::string GREEN_BG = "\033[42m";
std::string YELLOW = "\033[33m";
std::string YELLOW_BG = "\033[43m";
std::string MAGENTA = "\033[35m";
std::string MAGENTA_BG = "\033[45m";
std::string BOLD = "\033[1;37m";
std::string DIM = "\033[2m";

std::string CLEAR_LINE = "\033[2K";
std::string CURSOR_UP = "\033[1A";

std::string pad_right(std::string value, int amount) {
  for (int i = value.length() + 1; i <= amount; i++) {
    value += " ";
  }
  return value;
}

std::string pad_left(std::string value, int amount) {
  std::string final;
  for (int i = value.length() + 1; i <= amount; i++) {
    final += " ";
  }
  final += value;
  return final;
}

std::string render_logo() {
  return BOLD + "cronedit" + RESET;
}

std::string render_item_begin(std::string line_number, int width) {
  return DIM + pad_left(line_number, width) + " | " + RESET;
}

std::string render_item_begin(int line_number, int width) {
  return render_item_begin(std::to_string(line_number), width);
}

std::string render_blank_slate() {
  return DIM + "Empty crontab. Press <n> to add a job." + RESET;
}

std::string render_quitting(std::string saved_file, std::string backup_file) {
  std::stringstream output;
  if (saved_file != "") {
    output << "Changes saved in " << saved_file << std::endl;
  }
  if (backup_file != "") {
    output << "Previous version was backed up in " << backup_file << std::endl;
  }
  output << RESET;
  return output.str();
}

VisibleItemIndexes calculate_visible_item_indexes(int space_for, int active_item, int total_items) {
  VisibleItemIndexes indexes;
  if (total_items == 0) {
    indexes.begin_at = -1;
    indexes.end_at = -1;
  } else {
    indexes.begin_at = std::max(0, std::min(total_items - space_for, (int)(active_item - std::floor(space_for / 2))));
    indexes.end_at = std::min(indexes.begin_at + space_for, total_items);
  }
  return indexes;
}

std::string get_column_value(CrontabJob job, int index) {
  if (index == 0) {
    return job.minute;
  }
  if (index == 1) {
    return job.hour;
  }
  if (index == 2) {
    return job.day_of_month;
  }
  if (index == 3) {
    return job.month;
  }
  if (index == 4) {
    return job.day_of_week;
  }
  return job.command;
}

std::string get_column_name(int index) {
  if (index == 0) {
    return "minute";
  }
  if (index == 1) {
    return "hour";
  }
  if (index == 2) {
    return "day of month";
  }
  if (index == 3) {
    return "month";
  }
  if (index == 4) {
    return "day of week";
  }
  if (index == 5) {
    return "command";
  }
  return "";
}

std::string render_job(CrontabJob job, int space_on_line, int* column_widths, int active_column) {
  std::string output;

  std::string schedule_color = YELLOW;
  std::string schedule_active = YELLOW_BG + BLACK;
  std::string command_color = MAGENTA;
  std::string command_active = MAGENTA_BG + BLACK;

  if (job.commented) {
    schedule_color = GREEN;
    schedule_active = GREEN_BG + BLACK;
    command_color = GREEN;
    command_active = GREEN_BG + BLACK;
  }

  if (job.commented) {
    output += GREEN + "#" + RESET;
  } else {
    output += DIM + ">" + RESET;
  }

  output += " ";

  if (active_column > 0 || active_column == -1) {
    output += schedule_color;
  }

  int adjusted_column_widths[6];
  int total_column_width;
  int available_space = space_on_line;
  available_space -= 2; // "> "
  for (int i = 0; i < 6; i++) {
    total_column_width = column_widths[i];
    total_column_width += 2; // surrounding spaces
    adjusted_column_widths[i] = std::min(total_column_width, available_space);
    available_space -= adjusted_column_widths[i];
  }

  for (int i = 0; i < 6; i++) {
    if (i == active_column && i < 5) {
      if (i > 0) {
        output += RESET;
      }
      output += schedule_active;
    } else if (i == active_column && i == 5) {
      output += RESET + command_active;
    } else if (i == 5) {
      output += RESET + command_color;
    }
    output += " ";
    std::string value = get_column_value(job, i);
    int limit = adjusted_column_widths[i];
    limit -= 2; // surrounding spaces
    if (value.length() > limit) {
      value = value.substr(0, limit - 1) + "…";
    }
    output += pad_right(value, limit);
    output += " ";
    if (i == active_column && i < 5) {
      output += RESET + schedule_color;
    }
  }

  output += RESET;

  return output;
}

std::string render_unknown(CrontabUnknown unknown, int space_on_line) {
  if (unknown.text.length() > space_on_line) {
    return unknown.text.substr(0, space_on_line - 1) + "…";
  }
  return unknown.text;
}

std::string render_toolbar_line_1(int active_column) {
  std::stringstream output;
  output << DIM;
  output << "<arrow keys> move   ";
  output << "<s> save and quit   ";
  output << "<q> quit";
  if (active_column >= 0) {
    output << "   <enter> edit ";
    output << RESET;
    output << (active_column == 5 ? MAGENTA : YELLOW);
    output << get_column_name(active_column);
  }
  output << RESET;
  return output.str();
}

std::string render_toolbar_line_2(bool job_is_active, bool job_is_commented) {
  std::stringstream output;
  output << DIM;
  output << "<n> new";
  if (job_is_active) {
    output << "   ";
    if (job_is_commented) {
      output << RESET << GREEN << "<c> uncomment  " << RESET << DIM;
    } else {
      output << "<c> comment    ";
    }
    output << "<d> duplicate   ";
    output << "<r> remove   ";
  }
  output << RESET;
  return output.str();
}

std::string render_editing_notice(int active_column, std::string previous_value, int terminal_width) {
  std::stringstream output;
  output << DIM;
  output << "editing ";
  output << RESET;
  output << (active_column == 5 ? MAGENTA : YELLOW);
  std::string column_name = get_column_name(active_column);
  output << column_name;
  output << RESET << DIM;
  output << " (leave empty to undo)";
  int until_previous_value = 30 + 7;
  if (terminal_width > until_previous_value + 4) {
    output << ", was: ";
    if (previous_value.length() > terminal_width - until_previous_value) {
      previous_value = previous_value.substr(0, terminal_width - until_previous_value - 1) + "…";
    }
    output << previous_value;
  }
  output << RESET;
  return output.str();
}

std::string render_error_message(std::string error_message) {
  return RED_BG + BLACK + error_message + RESET;
}

std::string render_state(State& state, int terminal_width, int terminal_height) {
  if (state.is_quitting()) {
    return render_quitting(state.get_saved_file(), state.get_backup_file());
  }

  std::stringstream output;

  std::vector<StateItem> items = state.get_items();
  bool is_editing_value = state.is_editing_value();
  std::string error_message = state.get_error_message();
  int active_item = state.get_active_item();
  int active_column = state.get_active_column();
  int* column_widths = state.get_column_widths();

  int space_for = terminal_height - 5;
  VisibleItemIndexes item_indexes = calculate_visible_item_indexes(space_for, active_item, (int)items.size());
  if (item_indexes.begin_at == -1) {
    output << render_item_begin(" ", 1);
    output << render_blank_slate();
    output << std::endl;
  } else {
    int longest_line_number = std::to_string(item_indexes.end_at).length();
    int space_on_line = terminal_width - longest_line_number - 3;
    for (int index = item_indexes.begin_at; index < item_indexes.end_at; ++index) {
      output << render_item_begin(index + 1, longest_line_number);
      if (items.at(index).type == "job") {
        output << render_job(items.at(index).job, space_on_line, column_widths, index == active_item ? active_column : -1);
      } else if (items.at(index).type == "unknown") {
        output << render_unknown(items.at(index).unknown, space_on_line);
      }
      output << std::endl;
    }
  }
  output << render_logo();
  if (is_editing_value) {
    output << std::endl;
    output << std::endl;
  } else {
    bool job_is_active = active_item >= 0 && items.at(active_item).type == "job";
    bool job_is_commented = job_is_active && items.at(active_item).job.commented;
    output << "   " << render_toolbar_line_1(job_is_active ? active_column : -1) << std::endl;
    output << "           " << render_toolbar_line_2(job_is_active, job_is_commented) << std::endl;
  }

  // Getline produces a newline and we can't do anything about it... so when invoking it,
  // we actually move the cursor up by one, so that the produced newline does not overflow
  // what we already rendered. This newline accommodates for that.
  output << std::endl;

  if (is_editing_value) {
    output << render_editing_notice(active_column, get_column_value(items.at(active_item).job, active_column), terminal_width);
  } else if (error_message != "") {
    output << render_error_message(error_message);
  }
  return output.str();
}

std::string render_diff(std::string current, std::string previous) {
  int lines = 0;
  for(unsigned i = 0; i < previous.length(); ++i) {
    char character = previous.at(i);
    if (character == '\n') {
      lines++;
    }
  }
  std::stringstream output;
  for(unsigned i = 0; i < lines; ++i) {
    output << CLEAR_LINE << CURSOR_UP;
  }
  output << current;
  return output.str();
}

std::string replace_ansi(std::string value, bool with_string) {
  std::string final;
  for (int i = 0; i < value.length(); i++) {
    if (value[i] == '\033') {
      if (value.substr(i, RESET.length()) == RESET) {
        final += with_string ? "{RESET}" : "";
        i += RESET.length() - 1;
      } else if (value.substr(i, BLACK.length()) == BLACK) {
        final += with_string ? "{BLACK}" : "";
        i += BLACK.length() - 1;
      } else if (value.substr(i, RED_BG.length()) == RED_BG) {
        final += with_string ? "{RED_BG}" : "";
        i += RED_BG.length() - 1;
      } else if (value.substr(i, GREEN_BG.length()) == GREEN_BG) {
        final += with_string ? "{GREEN_BG}" : "";
        i += GREEN_BG.length() - 1;
      } else if (value.substr(i, GREEN.length()) == GREEN) {
        final += with_string ? "{GREEN}" : "";
        i += GREEN.length() - 1;
      } else if (value.substr(i, YELLOW_BG.length()) == YELLOW_BG) {
        final += with_string ? "{YELLOW_BG}" : "";
        i += YELLOW_BG.length() - 1;
      } else if (value.substr(i, YELLOW.length()) == YELLOW) {
        final += with_string ? "{YELLOW}" : "";
        i += YELLOW.length() - 1;
      } else if (value.substr(i, MAGENTA_BG.length()) == MAGENTA_BG) {
        final += with_string ? "{MAGENTA_BG}" : "";
        i += MAGENTA_BG.length() - 1;
      } else if (value.substr(i, MAGENTA.length()) == MAGENTA) {
        final += with_string ? "{MAGENTA}" : "";
        i += MAGENTA.length() - 1;
      } else if (value.substr(i, BOLD.length()) == BOLD) {
        final += with_string ? "{BOLD}" : "";
        i += BOLD.length() - 1;
      } else if (value.substr(i, DIM.length()) == DIM) {
        final += with_string ? "{DIM}" : "";
        i += DIM.length() - 1;
      } else if (value.substr(i, CLEAR_LINE.length()) == CLEAR_LINE) {
        final += with_string ? "{CLEAR_LINE}" : "";
        i += CLEAR_LINE.length() - 1;
      } else if (value.substr(i, CURSOR_UP.length()) == CURSOR_UP) {
        final += with_string ? "{CURSOR_UP}" : "";
        i += CURSOR_UP.length() - 1;
      } else {
        final += "";
      }
    } else {
      final += value[i];
    }
  }
  return final;
}

void debug_print_ui(std::string value) {
  std::cout << "---\n";
  for (int i = 0; i < value.length(); i++) {
    if (value[i] == '\n') {
      std::cout << "\\n";
    } else {
      std::cout << value[i];
    }
  }
  std::cout << "\n---\n";
}
