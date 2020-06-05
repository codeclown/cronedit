#ifndef STATE_H
#define STATE_H

struct StateItem {
  std::string type;
  CrontabJob job;
  CrontabUnknown unknown;
};

class State {
  std::vector<StateItem> items;
  int active_item;
  int next_selectable_item(int step);
  int active_column;
  int next_selectable_column(int step);
  int column_widths[6];
  void update_column_widths();
  bool editing_value;
  std::string error_message;
  bool quitting;
  std::string saved_file;
  std::string backup_file;
public:
  State();
  std::vector<StateItem> get_items();
  int get_active_item();
  int get_active_column();
  int* get_column_widths();
  bool is_editing_value();
  bool is_quitting();
  std::string get_saved_file();
  std::string get_backup_file();
  std::string get_error_message();
  void parse_line(std::string line);
  void trim_newlines();
  void print_debug();
  void on_key_up();
  void on_key_down();
  void on_key_left();
  void on_key_right();
  void on_toggle_comment();
  void on_create_new();
  void on_duplicate();
  void on_remove();
  bool on_begin_editing_value();
  void on_end_editing_value(std::string input);
  void on_quit(std::string saved_file, std::string backup_file);
};

#endif
