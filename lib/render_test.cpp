#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include "parser.h"
#include "state.h"
#include "render.h"
#include "version.h"

void test_very_simple_ones() {
  {
    std::cout << "test_very_simple_ones" << std::endl;

    std::string rendered;

    rendered = replace_ansi(render_logo(), true);
    debug_print_ui(rendered);
    assert(rendered ==
      "{BOLD}cronedit{RESET}"
    );

    rendered = replace_ansi(render_item_begin(1, 1), true);
    debug_print_ui(rendered);
    assert(rendered ==
      "{DIM}1 | {RESET}"
    );
    rendered = replace_ansi(render_item_begin(1, 2), true);
    debug_print_ui(rendered);
    assert(rendered ==
      "{DIM} 1 | {RESET}"
    );

    rendered = replace_ansi(render_error_message("foobar"), true);
    debug_print_ui(rendered);
    assert(rendered ==
      "{RED_BG}{BLACK}foobar{RESET}"
    );
  }
}

void test_calculate_visible_item_indexes() {
  {
    std::cout << "calculate_visible_item_indexes: BASIC" << std::endl;

    VisibleItemIndexes indexes;
    int space_for;
    int active_item;
    int total_items;

    space_for = 3;
    active_item = -1;
    total_items = 0;
    indexes = calculate_visible_item_indexes(space_for, active_item, total_items);
    std::cout << indexes.begin_at << "->" << indexes.end_at << std::endl;
    assert(indexes.begin_at == -1);
    assert(indexes.end_at == -1);

    space_for = 3;
    active_item = -1;
    total_items = 3;
    indexes = calculate_visible_item_indexes(space_for, active_item, total_items);
    std::cout << indexes.begin_at << "->" << indexes.end_at << std::endl;
    assert(indexes.begin_at == 0);
    assert(indexes.end_at == 3);

    space_for = 3;
    active_item = -1;
    total_items = 5;
    indexes = calculate_visible_item_indexes(space_for, active_item, total_items);
    std::cout << indexes.begin_at << "->" << indexes.end_at << std::endl;
    assert(indexes.begin_at == 0);
    assert(indexes.end_at == 3);

    space_for = 3;
    active_item = 0;
    total_items = 5;
    indexes = calculate_visible_item_indexes(space_for, active_item, total_items);
    std::cout << indexes.begin_at << "->" << indexes.end_at << std::endl;
    assert(indexes.begin_at == 0);
    assert(indexes.end_at == 3);
    active_item = 1;
    indexes = calculate_visible_item_indexes(space_for, active_item, total_items);
    std::cout << indexes.begin_at << "->" << indexes.end_at << std::endl;
    assert(indexes.begin_at == 0);
    assert(indexes.end_at == 3);
    active_item = 2;
    indexes = calculate_visible_item_indexes(space_for, active_item, total_items);
    std::cout << indexes.begin_at << "->" << indexes.end_at << std::endl;
    assert(indexes.begin_at == 1);
    assert(indexes.end_at == 4);
    active_item = 3;
    indexes = calculate_visible_item_indexes(space_for, active_item, total_items);
    std::cout << indexes.begin_at << "->" << indexes.end_at << std::endl;
    assert(indexes.begin_at == 2);
    assert(indexes.end_at == 5);
    active_item = 4;
    indexes = calculate_visible_item_indexes(space_for, active_item, total_items);
    std::cout << indexes.begin_at << "->" << indexes.end_at << std::endl;
    assert(indexes.begin_at == 2);
    assert(indexes.end_at == 5);
  }
}

void test_render_quitting() {
  {
    std::cout << "render_quitting: BASIC" << std::endl;

    std::string rendered;

    rendered = replace_ansi(render_quitting("", ""), true);
    debug_print_ui(rendered);
    assert(rendered ==
      "{RESET}"
    );

    rendered = replace_ansi(render_quitting("test.txt", ""), true);
    debug_print_ui(rendered);
    assert(rendered ==
      "Changes saved in test.txt\n{RESET}"
    );

    rendered = replace_ansi(render_quitting("test.txt", "backup.txt"), true);
    debug_print_ui(rendered);
    assert(rendered ==
      "Changes saved in test.txt\nPrevious version was backed up in backup.txt\n{RESET}"
    );

    rendered = replace_ansi(render_quitting("", "backup.txt"), true);
    debug_print_ui(rendered);
    assert(rendered ==
      "Previous version was backed up in backup.txt\n{RESET}"
    );
  }
}

void test_render_job() {
  {
    std::cout << "render_job: BASIC" << std::endl;

    int terminal_width = 64;
    int column_widths[6] = { 1, 1, 1, 1, 1, 25 };
    CrontabJob job = parse_crontab_job("* * * * * bash /asd/scripts/test.sh");

    int active_column = -1;
    std::string rendered = replace_ansi(render_job(job, terminal_width, column_widths, active_column), true);
    std::cout << "active_column = " << active_column << std::endl;
    debug_print_ui(rendered);
    assert(rendered ==
      "{DIM}>{RESET} {YELLOW} *  *  *  *  * {RESET}{MAGENTA} bash /asd/scripts/test.sh {RESET}"
    );

    std::string expected[6] = {
      "{DIM}>{RESET} {YELLOW_BG}{BLACK} * {RESET}{YELLOW} *  *  *  * {RESET}{MAGENTA} bash /asd/scripts/test.sh {RESET}",
      "{DIM}>{RESET} {YELLOW} * {RESET}{YELLOW_BG}{BLACK} * {RESET}{YELLOW} *  *  * {RESET}{MAGENTA} bash /asd/scripts/test.sh {RESET}",
      "{DIM}>{RESET} {YELLOW} *  * {RESET}{YELLOW_BG}{BLACK} * {RESET}{YELLOW} *  * {RESET}{MAGENTA} bash /asd/scripts/test.sh {RESET}",
      "{DIM}>{RESET} {YELLOW} *  *  * {RESET}{YELLOW_BG}{BLACK} * {RESET}{YELLOW} * {RESET}{MAGENTA} bash /asd/scripts/test.sh {RESET}",
      "{DIM}>{RESET} {YELLOW} *  *  *  * {RESET}{YELLOW_BG}{BLACK} * {RESET}{YELLOW}{RESET}{MAGENTA} bash /asd/scripts/test.sh {RESET}",
      "{DIM}>{RESET} {YELLOW} *  *  *  *  * {RESET}{MAGENTA_BG}{BLACK} bash /asd/scripts/test.sh {RESET}"
    };

    for (int active_column = 0; active_column < 6; ++active_column) {
      rendered = replace_ansi(render_job(job, terminal_width, column_widths, active_column), true);
      std::cout << "active_column = " << active_column << std::endl;
      debug_print_ui(rendered);
      assert(rendered == expected[active_column]);
    }
  }

  {
    std::cout << "render_job: COLUMN WIDTHS" << std::endl;

    int terminal_width = 64;
    int column_widths[6] = { 2, 3, 1, 4, 1, 25 };
    CrontabJob job = parse_crontab_job("* */6 * 1234 * bash /asd/scripts/test.sh");

    std::string expected[6] = {
      "{DIM}>{RESET} {YELLOW_BG}{BLACK} *  {RESET}{YELLOW} */6  *  1234  * {RESET}{MAGENTA} bash /asd/scripts/test.sh {RESET}",
      "{DIM}>{RESET} {YELLOW} *  {RESET}{YELLOW_BG}{BLACK} */6 {RESET}{YELLOW} *  1234  * {RESET}{MAGENTA} bash /asd/scripts/test.sh {RESET}",
      "{DIM}>{RESET} {YELLOW} *   */6 {RESET}{YELLOW_BG}{BLACK} * {RESET}{YELLOW} 1234  * {RESET}{MAGENTA} bash /asd/scripts/test.sh {RESET}",
      "{DIM}>{RESET} {YELLOW} *   */6  * {RESET}{YELLOW_BG}{BLACK} 1234 {RESET}{YELLOW} * {RESET}{MAGENTA} bash /asd/scripts/test.sh {RESET}",
      "{DIM}>{RESET} {YELLOW} *   */6  *  1234 {RESET}{YELLOW_BG}{BLACK} * {RESET}{YELLOW}{RESET}{MAGENTA} bash /asd/scripts/test.sh {RESET}",
      "{DIM}>{RESET} {YELLOW} *   */6  *  1234  * {RESET}{MAGENTA_BG}{BLACK} bash /asd/scripts/test.sh {RESET}"
    };

    std::string rendered;
    for (int active_column = 0; active_column < 6; ++active_column) {
      rendered = replace_ansi(render_job(job, terminal_width, column_widths, active_column), true);
      std::cout << "active_column = " << active_column << std::endl;
      debug_print_ui(rendered);
      assert(rendered == expected[active_column]);
    }
  }

  {
    std::cout << "render_job: COMMENTED" << std::endl;

    int terminal_width = 64;
    int column_widths[6] = { 1, 1, 1, 1, 1, 25 };
    CrontabJob job = parse_crontab_job("* * * * * bash /asd/scripts/test.sh");
    job.commented = true;

    std::string expected[6] = {
      "{GREEN}#{RESET} {GREEN_BG}{BLACK} * {RESET}{GREEN} *  *  *  * {RESET}{GREEN} bash /asd/scripts/test.sh {RESET}",
      "{GREEN}#{RESET} {GREEN} * {RESET}{GREEN_BG}{BLACK} * {RESET}{GREEN} *  *  * {RESET}{GREEN} bash /asd/scripts/test.sh {RESET}",
      "{GREEN}#{RESET} {GREEN} *  * {RESET}{GREEN_BG}{BLACK} * {RESET}{GREEN} *  * {RESET}{GREEN} bash /asd/scripts/test.sh {RESET}",
      "{GREEN}#{RESET} {GREEN} *  *  * {RESET}{GREEN_BG}{BLACK} * {RESET}{GREEN} * {RESET}{GREEN} bash /asd/scripts/test.sh {RESET}",
      "{GREEN}#{RESET} {GREEN} *  *  *  * {RESET}{GREEN_BG}{BLACK} * {RESET}{GREEN}{RESET}{GREEN} bash /asd/scripts/test.sh {RESET}",
      "{GREEN}#{RESET} {GREEN} *  *  *  *  * {RESET}{GREEN_BG}{BLACK} bash /asd/scripts/test.sh {RESET}"
    };

    std::string rendered;
    for (int active_column = 0; active_column < 6; ++active_column) {
      rendered = replace_ansi(render_job(job, terminal_width, column_widths, active_column), true);
      std::cout << "active_column = " << active_column << std::endl;
      debug_print_ui(rendered);
      assert(rendered == expected[active_column]);
    }
  }

  {
    std::cout << "render_job: TRUNCATED" << std::endl;

    int terminal_width = 34;
    int column_widths[6] = { 1, 1, 1, 1, 1, 25 };
    int active_column = 0;
    CrontabJob job = parse_crontab_job("* * * * * bash /asd/scripts/test.sh");
    std::string rendered = replace_ansi(render_job(job, terminal_width, column_widths, active_column), true);
    debug_print_ui(rendered);
    assert(rendered ==
      "{DIM}>{RESET} {YELLOW_BG}{BLACK} * {RESET}{YELLOW} *  *  *  * {RESET}{MAGENTA} bash /asd/scri… {RESET}"
    );
    active_column = 5;
    rendered = replace_ansi(render_job(job, terminal_width, column_widths, active_column), true);
    debug_print_ui(rendered);
    assert(rendered ==
      "{DIM}>{RESET} {YELLOW} *  *  *  *  * {RESET}{MAGENTA_BG}{BLACK} bash /asd/scri… {RESET}"
    );
  }
}

void test_render_unknown() {
  {
    std::cout << "render_unknown: BASIC" << std::endl;

    int terminal_width;
    CrontabUnknown unknown;
    std::string rendered;

    terminal_width = 64;
    unknown = parse_crontab_unknown("* * * bash /asd/scripts/test.sh");
    rendered = render_unknown(unknown, terminal_width);
    debug_print_ui(rendered);
    assert(rendered ==
      "* * * bash /asd/scripts/test.sh"
    );

    terminal_width = 10;
    unknown = parse_crontab_unknown("* * * bash /asd/scripts/test.sh");
    rendered = render_unknown(unknown, terminal_width);
    debug_print_ui(rendered);
    assert(rendered ==
      "* * * bas…"
    );
  }
}

void test_render_toolbar_line_1() {
  {
    std::cout << "render_toolbar_line_1: BASIC" << std::endl;

    int active_column = -1;
    std::string rendered = replace_ansi(render_toolbar_line_1(active_column), true);
    std::cout << "active_column = " << active_column << std::endl;
    debug_print_ui(rendered);
    assert(rendered ==
      "{DIM}<arrow keys> move   <s> save and quit   <q> quit{RESET}"
    );

    std::string expected[6] = {
      "{DIM}<arrow keys> move   <s> save and quit   <q> quit   <enter> edit {RESET}{YELLOW}minute{RESET}",
      "{DIM}<arrow keys> move   <s> save and quit   <q> quit   <enter> edit {RESET}{YELLOW}hour{RESET}",
      "{DIM}<arrow keys> move   <s> save and quit   <q> quit   <enter> edit {RESET}{YELLOW}day of month{RESET}",
      "{DIM}<arrow keys> move   <s> save and quit   <q> quit   <enter> edit {RESET}{YELLOW}month{RESET}",
      "{DIM}<arrow keys> move   <s> save and quit   <q> quit   <enter> edit {RESET}{YELLOW}day of week{RESET}",
      "{DIM}<arrow keys> move   <s> save and quit   <q> quit   <enter> edit {RESET}{MAGENTA}command{RESET}"
    };

    for (int active_column = 0; active_column < 6; ++active_column) {
      rendered = replace_ansi(render_toolbar_line_1(active_column), true);
      std::cout << "active_column = " << active_column << std::endl;
      debug_print_ui(rendered);
      assert(rendered == expected[active_column]);
    }
  }
}

void test_render_toolbar_line_2() {
  {
    std::cout << "render_toolbar_line_2: BASIC" << std::endl;

    bool job_is_active;
    bool job_is_commented;
    std::string rendered;

    job_is_active = false;
    job_is_commented = false;
    rendered = replace_ansi(render_toolbar_line_2(job_is_active, job_is_commented), true);
    debug_print_ui(rendered);
    assert(rendered ==
      "{DIM}<n> new{RESET}"
    );

    job_is_active = true;
    job_is_commented = false;
    rendered = replace_ansi(render_toolbar_line_2(job_is_active, job_is_commented), true);
    debug_print_ui(rendered);
    assert(rendered ==
      "{DIM}<n> new   <c> comment    <d> duplicate   <r> remove   {RESET}"
    );

    job_is_active = true;
    job_is_commented = true;
    rendered = replace_ansi(render_toolbar_line_2(job_is_active, job_is_commented), true);
    debug_print_ui(rendered);
    assert(rendered ==
      "{DIM}<n> new   {RESET}{GREEN}<c> uncomment  {RESET}{DIM}<d> duplicate   <r> remove   {RESET}"
    );
  }
}

void test_render_editing_notice() {
  {
    std::cout << "render_editing_notice: BASIC" << std::endl;

    int active_column;
    std::string previous_value = "foobar 123";
    int terminal_width = 100;
    std::string rendered;

    std::string expected[6] = {
      "{DIM}editing {RESET}{YELLOW}minute{RESET}{DIM} (leave empty to undo), was: foobar 123{RESET}",
      "{DIM}editing {RESET}{YELLOW}hour{RESET}{DIM} (leave empty to undo), was: foobar 123{RESET}",
      "{DIM}editing {RESET}{YELLOW}day of month{RESET}{DIM} (leave empty to undo), was: foobar 123{RESET}",
      "{DIM}editing {RESET}{YELLOW}month{RESET}{DIM} (leave empty to undo), was: foobar 123{RESET}",
      "{DIM}editing {RESET}{YELLOW}day of week{RESET}{DIM} (leave empty to undo), was: foobar 123{RESET}",
      "{DIM}editing {RESET}{MAGENTA}command{RESET}{DIM} (leave empty to undo), was: foobar 123{RESET}",
    };

    for (int active_column = 0; active_column < 6; ++active_column) {
      rendered = replace_ansi(render_editing_notice(active_column, previous_value, terminal_width), true);
      std::cout << "active_column = " << active_column << std::endl;
      debug_print_ui(rendered);
      assert(rendered == expected[active_column]);
    }

    active_column = 5;
    terminal_width = 45;
    rendered = replace_ansi(render_editing_notice(active_column, previous_value, terminal_width), true);
    std::cout << "truncated previous value" << std::endl;
    debug_print_ui(rendered);
    assert(rendered ==
      "{DIM}editing {RESET}{MAGENTA}command{RESET}{DIM} (leave empty to undo), was: foobar …{RESET}"
    );

    active_column = 5;
    terminal_width = 30;
    rendered = replace_ansi(render_editing_notice(active_column, previous_value, terminal_width), true);
    std::cout << "no room for previous value" << std::endl;
    debug_print_ui(rendered);
    assert(rendered ==
      "{DIM}editing {RESET}{MAGENTA}command{RESET}{DIM} (leave empty to undo){RESET}"
    );
  }
}

void test_render_state() {
  {
    std::cout << "render_state: EMPTY STATE" << std::endl;

    State state;
    std::string rendered = replace_ansi(render_state(state, 100, 50), false);
    debug_print_ui(rendered);
    assert(
      rendered == "\
cronedit   <arrow keys> move   <s> save and quit   <q> quit\n\
           <n> new\n\n\
");
  }

  {
    std::cout << "render_state: ONLY UNKNOWNS" << std::endl;

    State state;
    state.parse_line("line 1");
    state.parse_line("line 2");
    state.parse_line("line 3");
    std::string rendered = replace_ansi(render_state(state, 100, 50), false);
    debug_print_ui(rendered);
    assert(
      rendered == "\
1 | line 1\n\
2 | line 2\n\
3 | line 3\n\
cronedit   <arrow keys> move   <s> save and quit   <q> quit\n\
           <n> new\n\n\
");
  }

  {
    std::cout << "render_state: VARIOUS" << std::endl;

    std::string rendered;

    State state;
    state.parse_line("# lorem ipsum a dolor sit amet confusing long text on this line not a cronjob but long");
    state.parse_line("0 6/* * * * bash test2.sh");
    state.parse_line("* * * 24 * bash /path/to/foobar.sh");
    state.parse_line("* * * * * bash /path/to/very/long/path/but/foobar/maybe/foobar.sh 2>&1 > /dev/null");

    rendered = replace_ansi(render_state(state, 100, 50), false);
    debug_print_ui(rendered);
    assert(
      rendered == "\
1 | # lorem ipsum a dolor sit amet confusing long text on this line not a cronjob but long\n\
2 | >  0  6/*  *  *   *  bash test2.sh                                                            \n\
3 | >  *  *    *  24  *  bash /path/to/foobar.sh                                                  \n\
4 | >  *  *    *  *   *  bash /path/to/very/long/path/but/foobar/maybe/foobar.sh 2>&1 > /dev/null \n\
cronedit   <arrow keys> move   <s> save and quit   <q> quit   <enter> edit minute\n\
           <n> new   <c> comment    <d> duplicate   <r> remove   \n\n\
");

    rendered = replace_ansi(render_state(state, MIN_TERMINAL_WIDTH, 50), false);
    debug_print_ui(rendered);
    assert(
      rendered == "\
1 | # lorem ipsum a dolor sit amet confusing long text on this line not a cronjob bu…\n\
2 | >  0  6/*  *  *   *  bash test2.sh                                               \n\
3 | >  *  *    *  24  *  bash /path/to/foobar.sh                                     \n\
4 | >  *  *    *  *   *  bash /path/to/very/long/path/but/foobar/maybe/foobar.sh 2>… \n\
cronedit   <arrow keys> move   <s> save and quit   <q> quit   <enter> edit minute\n\
           <n> new   <c> comment    <d> duplicate   <r> remove   \n\n\
");

    rendered = replace_ansi(render_state(state, 100, MIN_TERMINAL_HEIGHT), false);
    debug_print_ui(rendered);
    assert(
      rendered == "\
1 | # lorem ipsum a dolor sit amet confusing long text on this line not a cronjob but long\n\
2 | >  0  6/*  *  *   *  bash test2.sh                                                            \n\
3 | >  *  *    *  24  *  bash /path/to/foobar.sh                                                  \n\
cronedit   <arrow keys> move   <s> save and quit   <q> quit   <enter> edit minute\n\
           <n> new   <c> comment    <d> duplicate   <r> remove   \n\n\
");
  }

  {
    std::cout << "render_state: EDITING" << std::endl;

    std::string rendered;
    State state;

    state.parse_line("0 6/* * * * bash test2.sh");
    state.on_begin_editing_value();
    rendered = replace_ansi(render_state(state, 100, 50), false);
    std::cout << "Beginning to edit" << std::endl;
    debug_print_ui(rendered);
    assert(
      rendered == "\
1 | >  0  6/*  *  *  *  bash test2.sh \n\
cronedit\n\
\n\
\n\
editing minute (leave empty to undo), was: 0\
");

    state.on_end_editing_value("super invalid value 123");
    rendered = replace_ansi(render_state(state, 100, 50), false);
    std::cout << "Inputting an invalid value" << std::endl;
    debug_print_ui(rendered);
    assert(
      rendered == "\
1 | >  0  6/*  *  *  *  bash test2.sh \n\
cronedit   <arrow keys> move   <s> save and quit   <q> quit   <enter> edit minute\n\
           <n> new   <c> comment    <d> duplicate   <r> remove   \n\
\n\
super invalid value 123 is not a valid value\
");
  }

  {
    std::cout << "render_state: QUITTING" << std::endl;

    State state;
    state.on_quit("saved.txt", "backup.txt");
    std::string rendered = replace_ansi(render_state(state, 100, 50), false);
    debug_print_ui(rendered);
    assert(
      rendered == "\
Changes saved in saved.txt\n\
Previous version was backed up in backup.txt\n\
");
  }

  {
    std::cout << "render_state: QUITTING" << std::endl;

    State state;
    state.on_quit("saved.txt", "backup.txt");
    std::string rendered = replace_ansi(render_state(state, 100, 50), false);
    debug_print_ui(rendered);
    assert(
      rendered == "\
Changes saved in saved.txt\n\
Previous version was backed up in backup.txt\n\
");
  }
}

void test_render_diff() {
  {
    std::cout << "render_diff: BASIC" << std::endl;

    std::string rendered;
    std::string current;
    std::string previous;

    current = "one\ntwo1";
    previous = "one\ntwo2";
    rendered = replace_ansi(render_diff(current, previous), true);
    debug_print_ui(rendered);
    assert(rendered == "{CLEAR_LINE}{CURSOR_UP}one\ntwo1");

    current = "one\ntwo\nthree1";
    previous = "one\ntwo\nthree2";
    rendered = replace_ansi(render_diff(current, previous), true);
    debug_print_ui(rendered);
    assert(rendered == "{CLEAR_LINE}{CURSOR_UP}{CLEAR_LINE}{CURSOR_UP}one\ntwo\nthree1");

    current = "";
    previous = "one\ntwo\nthree2";
    rendered = replace_ansi(render_diff(current, previous), true);
    debug_print_ui(rendered);
    assert(rendered == "{CLEAR_LINE}{CURSOR_UP}{CLEAR_LINE}{CURSOR_UP}");

    current = "one\ntwo\nthree1";
    previous = "";
    rendered = replace_ansi(render_diff(current, previous), true);
    debug_print_ui(rendered);
    assert(rendered == "one\ntwo\nthree1");
  }
}

int main()
{
  test_very_simple_ones();
  test_calculate_visible_item_indexes();
  test_render_quitting();
  test_render_job();
  test_render_unknown();
  test_render_toolbar_line_1();
  test_render_toolbar_line_2();
  test_render_editing_notice();
  test_render_state();
  test_render_diff();

//   // Editable lines
//   State state3;
//   state3.parse_line("0 */6 * * * bash test.sh");
//   state3.parse_line("0 * * * * bash test2.sh");
//   std::string ui3 = replace_ansi(render_state(state3, 100, 50), true);
//   // debug_print_ui(ui3);
//   assert(
//     ui3 == "\
// {DIM}| {RESET}{DIM}> {RESET}{YELLOW_BG}{BLACK} 0 {RESET}{YELLOW} */6 {RESET}{YELLOW} * {RESET}{YELLOW} * {RESET}{YELLOW} * {RESET}{MAGENTA} bash test.sh {RESET}\n\
// {DIM}| {RESET}{DIM}> {RESET}{YELLOW} 0 {RESET}{YELLOW} *   {RESET}{YELLOW} * {RESET}{YELLOW} * {RESET}{YELLOW} * {RESET}{MAGENTA} bash test2.sh {RESET}\n\
// {BOLD}cronedit   {RESET}{DIM}<arrow keys> move   <s> save and quit   <q> quit   <enter> edit {RESET}{YELLOW}minute      {RESET}{DIM}\n\
//            <n> new   <c> comment    {RESET}{DIM}<d> duplicate   <r> remove   {RESET}\n\n\
// ");

//   // Moving around
//   State state4;
//   state4.parse_line("0 */6 * * * bash test.sh");
//   state4.parse_line("0 * * * * bash test2.sh");
//   state4.on_key_down();
//   state4.on_key_right();
//   state4.on_key_right();
//   std::string ui4 = replace_ansi(render_state(state4, 100, 50), true);
//   // debug_print_ui(ui4);
//   assert(
//     ui4 == "\
// {DIM}| {RESET}{DIM}> {RESET}{YELLOW} 0 {RESET}{YELLOW} */6 {RESET}{YELLOW} * {RESET}{YELLOW} * {RESET}{YELLOW} * {RESET}{MAGENTA} bash test.sh {RESET}\n\
// {DIM}| {RESET}{DIM}> {RESET}{YELLOW} 0 {RESET}{YELLOW} *   {RESET}{YELLOW_BG}{BLACK} * {RESET}{YELLOW} * {RESET}{YELLOW} * {RESET}{MAGENTA} bash test2.sh {RESET}\n\
// {BOLD}cronedit   {RESET}{DIM}<arrow keys> move   <s> save and quit   <q> quit   <enter> edit {RESET}{YELLOW}day of month{RESET}{DIM}\n\
//            <n> new   <c> comment    {RESET}{DIM}<d> duplicate   <r> remove   {RESET}\n\n\
// ");

//   // Terminal width
//   State state5;
//   state5.parse_line("0 * * * * bash /path/to/very/long/path/yes/must/be/over/65/or/something/test1.sh");
//   state5.parse_line("0 * * * * bash /path/to/very/long/path/yes/must/be/over/65/or/something/test2.sh");
//   state5.parse_line("0 * * * * bash /path/to/very/long/path/yes/must/be/over/65/or/something/test3.sh");
//   state5.parse_line("0 * * * * bash /path/to/very/long/path/yes/must/be/over/65/or/something/test4.sh");
//   state5.parse_line("0 * * * * bash /path/to/very/long/path/yes/must/be/over/65/or/something/test5.sh");
//   std::string ui5_1 = replace_ansi(render_state(state5, 64, 8), false);
//   // debug_print_ui(ui5_1);
//   assert(
//     ui5_1 == "\
// | >  0  *  *  *  *  bash /path/to/very/long/path/yes/must/be/over/65/or/something/test1.sh \n\
// | >  0  *  *  *  *  bash /path/to/very/long/path/yes/must/be/over/65/or/something/test2.sh \n\
// | >  0  *  *  *  *  bash /path/to/very/long/path/yes/must/be/over/65/or/something/test3.sh \n\
// cronedit   <arrow keys> move   <s> save and quit   <q> quit   <enter> edit minute      \n\
//            <n> new   <c> comment    <d> duplicate   <r> remove   \n\n\
// ");
//   state5.on_key_down();
//   std::string ui5_2 = replace_ansi(render_state(state5, 64, 8), false);
//   // debug_print_ui(ui5_2);
//   assert(
//     ui5_2 == "\
// | >  0  *  *  *  *  bash /path/to/very/long/path/yes/must/be/over/65/or/something/test1.sh \n\
// | >  0  *  *  *  *  bash /path/to/very/long/path/yes/must/be/over/65/or/something/test2.sh \n\
// | >  0  *  *  *  *  bash /path/to/very/long/path/yes/must/be/over/65/or/something/test3.sh \n\
// cronedit   <arrow keys> move   <s> save and quit   <q> quit   <enter> edit minute      \n\
//            <n> new   <c> comment    <d> duplicate   <r> remove   \n\n\
// ");
//   state5.on_key_down();
//   std::string ui5_3 = replace_ansi(render_state(state5, 64, 8), false);
//   // debug_print_ui(ui5_3);
//   assert(
//     ui5_3 == "\
// | >  0  *  *  *  *  bash /path/to/very/long/path/yes/must/be/over/65/or/something/test2.sh \n\
// | >  0  *  *  *  *  bash /path/to/very/long/path/yes/must/be/over/65/or/something/test3.sh \n\
// | >  0  *  *  *  *  bash /path/to/very/long/path/yes/must/be/over/65/or/something/test4.sh \n\
// cronedit   <arrow keys> move   <s> save and quit   <q> quit   <enter> edit minute      \n\
//            <n> new   <c> comment    <d> duplicate   <r> remove   \n\n\
// ");
//   state5.on_key_down();
//   std::string ui5_4 = replace_ansi(render_state(state5, 64, 8), false);
//   // debug_print_ui(ui5_4);
//   assert(
//     ui5_4 == "\
// | >  0  *  *  *  *  bash /path/to/very/long/path/yes/must/be/over/65/or/something/test3.sh \n\
// | >  0  *  *  *  *  bash /path/to/very/long/path/yes/must/be/over/65/or/something/test4.sh \n\
// | >  0  *  *  *  *  bash /path/to/very/long/path/yes/must/be/over/65/or/something/test5.sh \n\
// cronedit   <arrow keys> move   <s> save and quit   <q> quit   <enter> edit minute      \n\
//            <n> new   <c> comment    <d> duplicate   <r> remove   \n\n\
// ");
//   state5.on_key_down();
//   std::string ui5_5 = replace_ansi(render_state(state5, 64, 8), false);
//   debug_print_ui(ui5_5);
//   assert(
//     ui5_5 == "\
// | >  0  *  *  *  *  bash /path/to/very/long/path/yes/must/be/over/65/or/something/test3.sh \n\
// | >  0  *  *  *  *  bash /path/to/very/long/path/yes/must/be/over/65/or/something/test4.sh \n\
// | >  0  *  *  *  *  bash /path/to/very/long/path/yes/must/be/over/65/or/something/test5.sh \n\
// cronedit   <arrow keys> move   <s> save and quit   <q> quit   <enter> edit minute      \n\
//            <n> new   <c> comment    <d> duplicate   <r> remove   \n\n\
// ");

  std::cout << "PASS" << std::endl;
}
