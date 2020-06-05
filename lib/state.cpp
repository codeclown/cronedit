#include <iostream>
#include <string>
#include <vector>
#include "parser.h"
#include "state.h"

State::State(void) {
  active_item = -1;
  active_column = 0;
  column_widths[0] = 1;
  column_widths[1] = 1;
  column_widths[2] = 1;
  column_widths[3] = 1;
  column_widths[4] = 1;
  column_widths[5] = 1;
  editing_value = false;
  error_message = "";
  quitting = false;
}

std::vector<StateItem> State::get_items() {
  return items;
}

int State::get_active_item() {
  return active_item;
}

int State::get_active_column() {
  return active_column;
}

int* State::get_column_widths() {
  return column_widths;
}

bool State::is_editing_value() {
  return editing_value;
}

bool State::is_quitting() {
  return quitting;
}

std::string State::get_saved_file() {
  return saved_file;
}

std::string State::get_backup_file() {
  return backup_file;
}

std::string State::get_error_message() {
  return error_message;
}

void State::update_column_widths() {
  for (int i = 0; i < 6; i++) {
    column_widths[i] = 1;
  }
  for (std::vector<StateItem>::iterator item = items.begin(); item != items.end(); ++item) {
    if (item->type == "job") {
      column_widths[0] = std::max(column_widths[0], int(item->job.minute.length()));
      column_widths[1] = std::max(column_widths[1], int(item->job.hour.length()));
      column_widths[2] = std::max(column_widths[2], int(item->job.day_of_month.length()));
      column_widths[3] = std::max(column_widths[3], int(item->job.month.length()));
      column_widths[4] = std::max(column_widths[4], int(item->job.day_of_week.length()));
      column_widths[5] = std::max(column_widths[5], int(item->job.command.length()));
    }
  }
}

int State::next_selectable_item(int step) {
  if (items.size() == 0) {
    return -1;
  }
  if (step > 0) {
    for (int i = active_item + 1; i < items.size(); i += step) {
      if (items[i].type != "unknown") {
        return i;
      }
    }
  }
  if (step < 0) {
    for (int i = active_item - 1; i >= 0; i += step) {
      if (items[i].type != "unknown") {
        return i;
      }
    }
  }
  return active_item;
}

int State::next_selectable_column(int step) {
  if (active_item == -1) {
    return 0;
  }
  int max_column = items[active_item].type == "job" ? 5 : 0;
  return std::max(0, std::min(max_column, active_column + step));
}

void State::parse_line(std::string line) {
  try {
    CrontabJob job = parse_crontab_job(line);
    items.push_back(StateItem());
    items[items.size() - 1].type = "job";
    items[items.size() - 1].job = job;
    if (active_item == -1) {
      active_item = items.size() - 1;
    }
    update_column_widths();
    return;
  } catch(NotACrontabJobException&) {}

  CrontabUnknown unknown = parse_crontab_unknown(line);
  items.push_back(StateItem());
  items[items.size() - 1].type = "unknown";
  items[items.size() - 1].unknown = unknown;
}

void State::trim_newlines() {
  int leading_newlines = 0;
  for (std::vector<StateItem>::iterator item = items.begin(); item != items.end(); ++item) {
    if (item->type == "unknown" && item->unknown.text == "") {
      leading_newlines++;
    } else {
      break;
    }
  }
  int trailing_newlines = 0;
  for (std::vector<StateItem>::reverse_iterator item = items.rbegin(); item != items.rend(); ++item) {
    if (item->type == "unknown" && item->unknown.text == "") {
      trailing_newlines++;
    } else {
      break;
    }
  }
  std::vector<StateItem> trimmed;
  for (std::vector<StateItem>::iterator item = items.begin() + leading_newlines; item != items.end() - trailing_newlines; ++item) {
    trimmed.push_back(*item);
  }
  items = trimmed;
}

void State::print_debug() {
  for (std::vector<StateItem>::iterator item = items.begin(); item != items.end(); ++item) {
    if (item->type == "job") {
      print_job(item->job);
    } else if (item->type == "unknown") {
      print_unknown(item->unknown);
    } else {
      std::cout << "Unknown type: " + item->type << std::endl;
    }
  }
}

void State::on_key_up() {
  error_message = "";
  active_item = next_selectable_item(-1);
}

void State::on_key_down() {
  error_message = "";
  active_item = next_selectable_item(1);
}

void State::on_key_left() {
  error_message = "";
  active_column = next_selectable_column(-1);
}

void State::on_key_right() {
  error_message = "";
  active_column = next_selectable_column(1);
}

void State::on_toggle_comment() {
  error_message = "";
  if (active_item >= 0) {
    items[active_item].job.commented = !items[active_item].job.commented;
  }
}

void State::on_create_new() {
  error_message = "";
  parse_line("* * * * * echo \"change this\"");
  update_column_widths();
}

void State::on_duplicate() {
  if (active_item >= 0) {
    CrontabJob job = items[active_item].job;
    StateItem item = items[active_item];
    item.job = job;
    items.push_back(item);
    update_column_widths();
  }
}

void State::on_remove() {
  if (active_item >= 0) {
    items.erase(items.begin() + active_item);
    active_item = next_selectable_item(-1);
    if (active_item == -1) {
      active_item = next_selectable_item(1);
    }
    update_column_widths();
  }
}

bool State::on_begin_editing_value() {
  if (active_item >= 0) {
    editing_value = true;
    return true;
  }
  return false;
}

std::string trim(std::string value) {
  int leading = 0;
  for(unsigned i = 0; i < value.length(); ++i) {
    if (!std::isspace(value.at(i))) {
      break;
    }
    leading++;
  }
  if (leading == value.length()) {
    return "";
  }
  int trailing = 0;
  for(unsigned i = value.length() - 1; i >= 0; --i) {
    if (!std::isspace(value.at(i))) {
      break;
    }
    trailing++;
  }
  return value.substr(leading, value.length() - trailing - leading);
}

void State::on_end_editing_value(std::string input) {
  error_message = "";
  editing_value = false;
  input = trim(input);
  if (input.length() > 0) {
    if (active_column == 0) {
      if (!is_valid_scheduling_segment(input)) {
        error_message = input + " is not a valid value";
      } else {
        items[active_item].job.minute = input;
      }
    }
    if (active_column == 1) {
      if (!is_valid_scheduling_segment(input)) {
        error_message = input + " is not a valid value";
      } else {
        items[active_item].job.hour = input;
      }
    }
    if (active_column == 2) {
      if (!is_valid_scheduling_segment(input)) {
        error_message = input + " is not a valid value";
      } else {
        items[active_item].job.day_of_month = input;
      }
    }
    if (active_column == 3) {
      if (!is_valid_scheduling_segment(input)) {
        error_message = input + " is not a valid value";
      } else {
        items[active_item].job.month = input;
      }
    }
    if (active_column == 4) {
      if (!is_valid_scheduling_segment(input)) {
        error_message = input + " is not a valid value";
      } else {
        items[active_item].job.day_of_week = input;
      }
    }
    if (active_column == 5) {
      items[active_item].job.command = input;
    }
  }
  update_column_widths();
}

void State::on_quit(std::string _saved_file, std::string _backup_file) {
  quitting = true;
  saved_file = _saved_file;
  backup_file = _backup_file;
}
