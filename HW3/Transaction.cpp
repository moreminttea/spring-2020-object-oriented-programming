
#include "Transaction.h"

unsigned int Transaction::transaction_count { };
// unsigned int Flight::flight_count { };

Transaction::Transaction(unsigned int arg_type) 
  : tr_type { arg_type }
{ 
  transaction_count++;
  std::cout << "basic Constructor" << std::endl;
}

Transaction::Transaction(std::string core_arg_host_url, std::string core_arg_owner_vsID,
			 std::string core_arg_class_id, std::string core_arg_object_id,
			 unsigned int arg_type, std::string arg_desc, Person& arg_agent,
			 GPS_DD& arg_where, IP_Address& arg_src, time_t arg_when)
  : Core { core_arg_host_url, core_arg_owner_vsID, core_arg_class_id, core_arg_object_id },
    tr_type { arg_type }, tr_description { arg_desc }, tr_when { arg_when }, tr_agent { arg_agent },
    tr_where { arg_where }, tr_srcIP { arg_src }
{
  transaction_count++;
  std::cout << "full Constructor" << std::endl;
}

Transaction::Transaction(std::string core_arg_host_url, std::string core_arg_owner_vsID,
			 std::string core_arg_class_id, std::string core_arg_object_id)
  : Core { core_arg_host_url, core_arg_owner_vsID, core_arg_class_id, core_arg_object_id }
{
  transaction_count++;
  std::cout << "Core Constructor" << std::endl;
}

void
Transaction::setData(void *arg_data)
{
  this->tr_data = arg_data;
}

void *
Transaction::getData(void)
{
  return this->tr_data;
}

std::string
Transaction::getDescription()
{
  return tr_description;
}

Flight::Flight
(std::string core_arg_host_url, std::string core_arg_owner_vsID,
 std::string core_arg_class_id, std::string core_arg_object_id,
 unsigned int transaction_arg_type, std::string transaction_arg_desc, Person& transaction_arg_agent,
 GPS_DD& transaction_arg_where, IP_Address& transaction_arg_src, time_t transaction_arg_when,
 Person& arg_passenger, Airline& arg_airline, std::string arg_ticket,
 GPS_DD& arg_departure, GPS_DD& arg_arrival)
  : Transaction { core_arg_host_url, core_arg_owner_vsID, core_arg_class_id, core_arg_object_id,
    transaction_arg_type, transaction_arg_desc, transaction_arg_agent, transaction_arg_where,
    transaction_arg_src, transaction_arg_when },
    fl_passenger { arg_passenger }, fl_airline { arg_airline }, fl_ticket_number { arg_ticket },
    fl_departure { arg_departure }, fl_arrival { arg_arrival }
{
}

GPS_DD dummy { 0.0, 0.0 };

Flight::Flight(std::string core_arg_host_url, std::string core_arg_owner_vsID,
	       std::string core_arg_class_id, std::string core_arg_object_id)
  : Transaction { core_arg_host_url, core_arg_owner_vsID, core_arg_class_id, core_arg_object_id },
	       fl_departure { dummy }, fl_arrival { dummy }
{
}


bool
Flight::Conflict_of_Interest()
{
  return (this->tr_agent == this->fl_passenger);
}

double
Flight::getDistance()
{
  return this->fl_departure.distance(this->fl_arrival);
}
