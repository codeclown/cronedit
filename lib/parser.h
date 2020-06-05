#ifndef PARSER_H
#define PARSER_H

//
// JOBS
//

struct CrontabJob {
  std::string minute;
  std::string hour;
  std::string day_of_month;
  std::string month;
  std::string day_of_week;
  std::string command;
  bool commented;
};

struct NotACrontabJobException : public std::exception {};

bool is_valid_scheduling_segment(std::string segment);

CrontabJob parse_crontab_job(std::string line);

void print_job(CrontabJob job);

//
// UNKNOWNS
//

struct CrontabUnknown {
  std::string text;
};

CrontabUnknown parse_crontab_unknown(std::string line);

void print_unknown(CrontabUnknown unknown);

#endif
