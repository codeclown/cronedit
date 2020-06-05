#ifndef RENDER_H
#define RENDER_H

#include "state.h"

std::string render_logo();

std::string render_item_begin(int line_number, int width);

std::string render_quitting(std::string saved_file, std::string backup_file);

struct VisibleItemIndexes {
  int begin_at;
  int end_at;
};

VisibleItemIndexes calculate_visible_item_indexes(int space_for, int active_item, int total_items);

std::string render_unknown(CrontabUnknown unknown, int space_on_line);

std::string render_job(CrontabJob job, int space_on_line, int* column_widths, int active_column);

std::string render_toolbar_line_1(int active_column);

std::string render_toolbar_line_2(bool job_is_active, bool job_is_commented);

std::string render_editing_notice(int active_column, std::string previous_value, int terminal_width);

std::string render_error_message(std::string error_message);

std::string render_state(State& state, int terminal_width, int terminal_height);

std::string render_diff(std::string current, std::string previous);

std::string replace_ansi(std::string value, bool with_string);

void debug_print_ui(std::string value);

#endif
