
#ifndef _JVTIME_H_
#define _JVTIME_H_

// for Json::value
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>

// popular ones
#include <strings.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <exception> 
#include <time.h>

using namespace std;

class JvTime
{
 private:
 protected:
 public:
  bool good;
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
  char tail4[64];

  JvTime(const char *);
  JvTime() { }
  struct std::tm * getStdTM(void);
  int setStdTM(struct std::tm *);
  std::string * getTimeString(void);
  virtual bool operator==(JvTime& arg_jvt);
  virtual double operator-(JvTime& arg_jvt);
};

#endif /* _JVTIME_H_ */
