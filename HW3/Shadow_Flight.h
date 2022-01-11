#ifndef _SHADOW_FLIGHT_H_
#define _SHADOW_FLIGHT_H_

#include "Transaction.h"

class Shadow_Flight: public Flight
{
 private:
 public:
    Shadow_Flight(std::string, std::string, std::string, std::string);
    virtual double getDistance();
    virtual bool Conflict_of_Interest();
};

#endif // _SHADOW_FLIGHT_H_
