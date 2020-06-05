#include <sstream>
#include <iostream>
#include <string>
#include <assert.h>
#include "parser.h"

int main()
{
  //
  // JOBS
  //

  assert(is_valid_scheduling_segment("*") == true);
  assert(is_valid_scheduling_segment("1") == true);
  assert(is_valid_scheduling_segment("18") == true);
  assert(is_valid_scheduling_segment("*/6") == true);
  assert(is_valid_scheduling_segment("6/*") == true);
  assert(is_valid_scheduling_segment("2/12") == true);

  CrontabJob job1 = parse_crontab_job("0 */6 * * * bash /asd/scripts/1password_backup.sh >/asd/scripts/1password_backup.log 2>&1");
  // print_job(job1);
  assert(job1.minute == "0");
  assert(job1.hour == "*/6");
  assert(job1.day_of_month == "*");
  assert(job1.month == "*");
  assert(job1.day_of_week == "*");
  assert(job1.command == "bash /asd/scripts/1password_backup.sh >/asd/scripts/1password_backup.log 2>&1");
  assert(job1.commented == false);

  CrontabJob job2 = parse_crontab_job("  0 */6  *   * * bash /asd/scripts/1password_backup.sh >/asd/scripts/1password_backup.log 2>&1");
  // print_job(job2);
  assert(job2.minute == "0");
  assert(job2.hour == "*/6");
  assert(job2.day_of_month == "*");
  assert(job2.month == "*");
  assert(job2.day_of_week == "*");
  assert(job2.command == "bash /asd/scripts/1password_backup.sh >/asd/scripts/1password_backup.log 2>&1");
  assert(job2.commented == false);

  CrontabJob job3 = parse_crontab_job("#0 */6 * * * bash /asd/scripts/1password_backup.sh >/asd/scripts/1password_backup.log 2>&1");
  // print_job(job3);
  assert(job3.minute == "0");
  assert(job3.hour == "*/6");
  assert(job3.day_of_month == "*");
  assert(job3.month == "*");
  assert(job3.day_of_week == "*");
  assert(job3.command == "bash /asd/scripts/1password_backup.sh >/asd/scripts/1password_backup.log 2>&1");
  assert(job3.commented == true);

  CrontabJob job4 = parse_crontab_job("# 0 */6 * * * bash /asd/scripts/1password_backup.sh >/asd/scripts/1password_backup.log 2>&1");
  // print_job(job4);
  assert(job4.minute == "0");
  assert(job4.hour == "*/6");
  assert(job4.day_of_month == "*");
  assert(job4.month == "*");
  assert(job4.day_of_week == "*");
  assert(job4.command == "bash /asd/scripts/1password_backup.sh >/asd/scripts/1password_backup.log 2>&1");
  assert(job4.commented == true);

  CrontabJob job5 = parse_crontab_job(" # 0 */6 * * * bash /asd/scripts/1password_backup.sh >/asd/scripts/1password_backup.log 2>&1");
  // print_job(job5);
  assert(job5.minute == "0");
  assert(job5.hour == "*/6");
  assert(job5.day_of_month == "*");
  assert(job5.month == "*");
  assert(job5.day_of_week == "*");
  assert(job5.command == "bash /asd/scripts/1password_backup.sh >/asd/scripts/1password_backup.log 2>&1");
  assert(job5.commented == true);

  try {
    CrontabJob job = parse_crontab_job("");
    assert(false);
  } catch(NotACrontabJobException&) {}

  try {
    CrontabJob job = parse_crontab_job("  ");
    assert(false);
  } catch(NotACrontabJobException&) {}

  try {
    CrontabJob job = parse_crontab_job("asd");
    assert(false);
  } catch(NotACrontabJobException&) {}

  try {
    CrontabJob job = parse_crontab_job("0 */6 * * bash foobar.sh");
    assert(false);
  } catch(NotACrontabJobException&) {}

  try {
    CrontabJob job = parse_crontab_job("SSH_AUTH_SOCK=/private/tmp/com.apple.launchd.WvQxE0oGYh/Listeners");
    assert(false);
  } catch(NotACrontabJobException&) {}

  try {
    CrontabJob job = parse_crontab_job("# https://stackoverflow.com/a/5823331/239527");
    assert(false);
  } catch(NotACrontabJobException&) {}

  try {
    CrontabJob job = parse_crontab_job("#");
    assert(false);
  } catch(NotACrontabJobException&) {}

  try {
    CrontabJob job = parse_crontab_job("# ");
    assert(false);
  } catch(NotACrontabJobException&) {}

  try {
    CrontabJob job = parse_crontab_job("  #");
    assert(false);
  } catch(NotACrontabJobException&) {}

  //
  // UNKNOWN
  //

  CrontabUnknown unknown1 = parse_crontab_unknown("");
  // print_unknown(unknown1);
  assert(unknown1.text == "");

  CrontabUnknown unknown2 = parse_crontab_unknown("   ");
  // print_unknown(unknown2);
  assert(unknown2.text == "   ");

  CrontabUnknown unknown3 = parse_crontab_unknown("foo bar 123");
  // print_unknown(unknown3);
  assert(unknown3.text == "foo bar 123");

  std::cout << "PASS" << std::endl;
}
