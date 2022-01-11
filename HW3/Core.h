
#ifndef _CORE_H_
#define _CORE_H_

// Core.h
#include <iostream>

class Core
{
 private:
 public:
  static unsigned int core_count;
  std::string host_url;
  std::string owner_vsID;
  std::string class_id;
  std::string object_id;
  Core(void);
  Core(std::string, std::string, std::string, std::string);
};

#endif  /* _CORE_H_ */
