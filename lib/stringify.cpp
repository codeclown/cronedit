#include <sstream>
#include <string>
#include <vector>
#include "parser.h"
#include "state.h"

std::string stringify_items(std::vector<StateItem> items) {
  std::stringstream output;
  for (std::vector<StateItem>::iterator item = items.begin(); item != items.end(); ++item) {
    if (item->type == "job") {
      if (item->job.commented) {
        output << "# ";
      }
      output << item->job.minute;
      output << " ";
      output << item->job.hour;
      output << " ";
      output << item->job.day_of_month;
      output << " ";
      output << item->job.month;
      output << " ";
      output << item->job.day_of_week;
      output << " ";
      output << item->job.command;
      output << std::endl;
    } else if (item->type == "unknown") {
      output << item->unknown.text << std::endl;
    }
  }
  return output.str();
}
