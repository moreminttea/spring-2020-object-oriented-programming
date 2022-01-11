#ifndef _SHADOW_PERSON_H_
#define _SHADOW_PERSON_H_

#include "Person.h"

class Shadow_Person: public Person
{
 private:
 public:
    Shadow_Person(std::string, std::string, std::string, std::string);
    virtual bool operator == (Person& aPerson);
    virtual std::string getVsID();
};

#endif // _SHADOW_PERSON_H_
