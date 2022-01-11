
#ifndef _TRANSACTION_H_
#define _TRANSACTION_H_

#include "Core.h"

// Transaction.h
#include <iostream>
#include "Person.h"

#define TR_TYPE_FLIGHT 1

class Transaction : public Core
{
 private:
  static unsigned int transaction_count;
  unsigned int tr_type;
  std::string tr_description;
  unsigned int tr_status;
  time_t tr_when;
 protected:
  Person tr_agent;
  GPS_DD tr_where;
  IP_Address tr_srcIP;
  void *tr_data;
 public:
  Transaction(std::string, std::string, std::string, std::string);
  Transaction(unsigned int);
  Transaction(std::string, std::string, std::string, std::string,
	      unsigned int, std::string, Person&,
	      GPS_DD&, IP_Address&, time_t);

  void setData(void *);
  void *getData();
  virtual std::string getDescription();
};

class Airline
{
 private:
 protected:
 public:
};

class Flight: public Transaction
{
 private:
  static unsigned int fl_count;
 protected:
  Person fl_passenger;
  Airline fl_airline;
  std::string fl_ticket_number;

 public:
  Flight(std::string, std::string, std::string, std::string, unsigned int, std::string, Person&, GPS_DD&,
	 IP_Address&, time_t, Person&, Airline&, std::string _ticket, GPS_DD&, GPS_DD&);
  Flight(std::string, std::string, std::string, std::string);
  GPS_DD& fl_departure;
  GPS_DD& fl_arrival;
  virtual double getDistance();
  bool Conflict_of_Interest();
  virtual std::string getDescription() { return "Flying"; }
};

#endif  /* _TRANSACTION_H_ */
