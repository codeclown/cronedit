#include <iostream>
#include <string>
#include <vector>
#include "parser.h"

//
// JOBS
//

bool is_crontab_number(char character) {
  return isdigit(character) || character == '*';
}

bool is_valid_scheduling_segment(std::string segment) {
  if (segment == "*") {
    return true;
  }
  if (is_crontab_number(segment[0])) {
    bool slash_encountered = false;
    for(unsigned i = 0; i < segment.length(); ++i) {
      char character = segment[i];
      if (slash_encountered && !is_crontab_number(character)) {
        return false;
      }
      if (slash_encountered && character == '/') {
        return false;
      }
      if (character == '/') {
        slash_encountered = true;
      }
      if (is_crontab_number(character)) {
        continue;
      }
    }
    return true;
  }
  return false;
}

CrontabJob parse_crontab_job(std::string line) {
  // std::vector<std::string> segments;
  // std::string segment;
  // segments.push_back(segment);
  // for(unsigned i = 0; i < line.length(); ++i) {
  //   char character = line.at(i);
  //   if (character == ' ' && segment.size() > 1) {
  //     segment = "";
  //     segments.push_back(segment);
  //   } else if (character == ' ') {
  //     // Skip over subsequent whitespace
  //   } else {
  //     segment += character;
  //   }
  // }
  // std::cout << segments << std::endl;
  // CrontabJob job;
  // return job;

  CrontabJob job;
  job.commented = false;
  int prop = 0;
  if (line.length() == 0) {
    throw NotACrontabJobException();
  }
  for(unsigned i = 0; i < line.length(); ++i) {
    char character = line.at(i);
    if (character == '#' && job.minute.length() == 0) {
      job.commented = true;
      continue;
    } else if (character == ' ' && (i < 1 || line[i - 1] != ' ') && job.minute.length() > 0 && prop < 5) {
      prop++;
      continue;
    } else if (character != ' ' && prop == 0) {
      job.minute.append(1, character);
    } else if (character != ' ' && prop == 1) {
      job.hour.append(1, character);
    } else if (character != ' ' && prop == 2) {
      job.day_of_month.append(1, character);
    } else if (character != ' ' && prop == 3) {
      job.month.append(1, character);
    } else if (character != ' ' && prop == 4) {
      job.day_of_week.append(1, character);
    } else if (prop == 5) {
      job.command.append(1, character);
    }
  }
  if (!is_valid_scheduling_segment(job.minute)) {
    throw NotACrontabJobException();
  }
  if (!is_valid_scheduling_segment(job.hour)) {
    throw NotACrontabJobException();
  }
  if (!is_valid_scheduling_segment(job.day_of_month)) {
    throw NotACrontabJobException();
  }
  if (!is_valid_scheduling_segment(job.month)) {
    throw NotACrontabJobException();
  }
  if (!is_valid_scheduling_segment(job.day_of_week)) {
    throw NotACrontabJobException();
  }
  return job;
}

void print_job(CrontabJob job) {
  std::cout << "CrontabJob {" << std::endl;
  std::cout << "  minute: \"" << job.minute << "\"" << std::endl;
  std::cout << "  hour: \"" << job.hour << "\"" << std::endl;
  std::cout << "  day_of_month: \"" << job.day_of_month << "\"" << std::endl;
  std::cout << "  month: \"" << job.month << "\"" << std::endl;
  std::cout << "  day_of_week: \"" << job.day_of_week << "\"" << std::endl;
  std::cout << "  command: \"" << job.command << "\"" << std::endl;
  std::cout << "  commented: " << (job.commented ? "true" : "false") << std::endl;
  std::cout << "}" << std::endl;
}

//
// UNKNOWNS
//

CrontabUnknown parse_crontab_unknown(std::string line) {
  CrontabUnknown unknown;
  unknown.text = line;
  return unknown;
}

void print_unknown(CrontabUnknown unknown) {
  std::cout << "CrontabUnknown {" << std::endl;
  std::cout << "  text: \"" + unknown.text + "\"" << std::endl;
  std::cout << "}" << std::endl;
}
