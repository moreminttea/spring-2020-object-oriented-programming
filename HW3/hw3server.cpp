// for Json::value
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include <string>

// for JsonRPCCPP
#include <iostream>
#include "hw3server.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <stdio.h>

// ecs36b
#include "Core.h"
#include "Person.h"
#include "Transaction.h"
#include <time.h>
#include "Shadow_Flight.h"
#include "Shadow_Person.h"

using namespace jsonrpc;
using namespace std;

class Myhw3Server : public hw3Server
{
public:
  Myhw3Server(AbstractServerConnector &connector, serverVersion_t type);
  virtual Json::Value GetDistance(const std::string& action, const std::string& arguments, const std::string& class_id, const std::string& host_url, const std::string& object_id, const std::string& owner_vsID);
  virtual Json::Value Conflict_of_Interest(const std::string& action, const std::string& arguments, const std::string& class_id, const std::string& host_url, const std::string& object_id, const std::string& owner_vsID);
  virtual Json::Value OperatorEqEq(const std::string& aPerson_class_id, const std::string& aPerson_host_url, const std::string& aPerson_object_id, const std::string& aPerson_owner_vsID, const std::string& action, const std::string& arguments, const std::string& class_id, const std::string& host_url, const std::string& object_id, const std::string& owner_vsID);
  virtual Json::Value GetVsID(const std::string& action, const std::string& arguments, const std::string& class_id, const std::string& host_url, const std::string& object_id, const std::string& owner_vsID);
};

Myhw3Server::Myhw3Server(AbstractServerConnector &connector, serverVersion_t type)
  : hw3Server(connector, type)
{
  std::cout << "Myhw3Server Object created" << std::endl;
}

Person *p1_ptr;
Person *p2_ptr;
Flight *f1_ptr;
Flight *f2_ptr;
Flight *f3_ptr;

// member functions
Json::Value
Myhw3Server::GetDistance
(const std::string& action, const std::string& arguments,
 const std::string& class_id, const std::string& host_url,
 const std::string& object_id, const std::string& owner_vsID)
{
  Json::Value result;
  std::cout << action << " " << arguments << " " << owner_vsID << std::endl;
  std::string strJson;

  double d;
  if ((class_id != "Flight") || ((object_id != "00000002") && (object_id != "00000003")))
    {
      strJson += "{\"status\" : \"failed\", ";
      if (class_id != "Flight")
          strJson += ("\"reason\": \"class " + class_id + " unknown\"}");
      else
          strJson += ("\"reason\": \"object " + object_id + " unknown\"}");
    }
  else
    {
      if (object_id == "00000001")
          d = f1_ptr->getDistance();
      if (object_id == "00000002")
          d = f2_ptr->getDistance();
      if (object_id == "00000003")
          d = f3_ptr->getDistance();
        
      strJson += "{\"status\" : \"successful\", ";
      strJson += ("\"distance\": " + to_string(d) + "}");
    }

  Json::CharReaderBuilder builder;
  Json::CharReader* reader = builder.newCharReader();
  std::string errors;
  bool parsingSuccessful = reader->parse
    (strJson.c_str(), strJson.c_str() + strJson.size(), &result, &errors);
  delete reader;

  if (!parsingSuccessful) {
    std::cout << "Failed to parse the JSON, errors:" << std::endl;
    std::cout << errors << std::endl;
  }

  return result;
}

Json::Value
Myhw3Server::Conflict_of_Interest
(const std::string& action, const std::string& arguments,
 const std::string& class_id, const std::string& host_url,
 const std::string& object_id, const std::string& owner_vsID)
{
  Json::Value result;
  std::cout << action << " " << arguments << " " << owner_vsID << std::endl;
  std::string strJson;

  bool res;
  if ((class_id != "Flight") && ((object_id != "00000002") && (object_id != "00000003")))
    {
      strJson += "{\"status\" : \"failed\", ";
      if (class_id != "Flight")
          strJson += ("\"reason\": \"class " + class_id + " unknown\"}");
      else
          strJson += ("\"reason\": \"object " + object_id + " unknown\"}");
    }
  else
    {
      if (object_id == "00000001")
          res = f1_ptr->Conflict_of_Interest();
      if (object_id == "00000002")
          res = f2_ptr->Conflict_of_Interest();
      if (object_id == "00000003")
          res = f3_ptr->Conflict_of_Interest();
        
      strJson += "{\"status\" : \"successful\", ";
        strJson += ("\"result\": " + std::string { (res ? "true" : "false") } + "}");
    }

  Json::CharReaderBuilder builder;
  Json::CharReader* reader = builder.newCharReader();
  std::string errors;
  bool parsingSuccessful = reader->parse
    (strJson.c_str(), strJson.c_str() + strJson.size(), &result, &errors);
  delete reader;

  if (!parsingSuccessful) {
    std::cout << "Failed to parse the JSON, errors:" << std::endl;
    std::cout << errors << std::endl;
  }

  return result;
}

Json::Value
Myhw3Server::OperatorEqEq
(const std::string& aPerson_class_id, const std::string& aPerson_host_url, const std::string& aPerson_object_id, const std::string& aPerson_owner_vsID, const std::string& action, const std::string& arguments, const std::string& class_id, const std::string& host_url, const std::string& object_id, const std::string& owner_vsID)
{
  Json::Value result;
  std::cout << action << " " << arguments << " " << owner_vsID << std::endl;
  std::string strJson;

  bool res;
  if ((class_id != "Flight") || ((object_id != "00000005") && (object_id != "00000006")) || ((aPerson_object_id != "00000005") && (aPerson_object_id != "00000006")))
    {
      strJson += "{\"status\" : \"failed\", ";
      if (class_id != "Flight")
          strJson += ("\"reason\": \"class " + class_id + " unknown\"}");
      else
          strJson += ("\"reason\": \"object " + object_id + " unknown\"}");
    }
  else
    {
      if ((object_id == "00000005") && (aPerson_object_id == "00000006"))
          res = ((*p1_ptr) == (*p2_ptr));
      if ((object_id == "00000005") && (aPerson_object_id == "00000005"))
          res = ((*p1_ptr) == (*p1_ptr));
      if ((object_id == "00000006") && (aPerson_object_id == "00000005"))
          res = ((*p2_ptr) == (*p1_ptr));
      if ((object_id == "00000006") && (aPerson_object_id == "00000006"))
          res = ((*p2_ptr) == (*p2_ptr));
        
      strJson += "{\"status\" : \"successful\", ";
        strJson += ("\"result\": " + std::string { (res ? "true" : "false") } + "}");
    }

  Json::CharReaderBuilder builder;
  Json::CharReader* reader = builder.newCharReader();
  std::string errors;
  bool parsingSuccessful = reader->parse
    (strJson.c_str(), strJson.c_str() + strJson.size(), &result, &errors);
  delete reader;

  if (!parsingSuccessful) {
    std::cout << "Failed to parse the JSON, errors:" << std::endl;
    std::cout << errors << std::endl;
  }

  return result;
}

Json::Value
Myhw3Server::GetVsID
(const std::string& action, const std::string& arguments,
const std::string& class_id, const std::string& host_url,
const std::string& object_id, const std::string& owner_vsID)
{
  Json::Value result;
  std::cout << action << " " << arguments << " " << owner_vsID << std::endl;
  std::string strJson;

  std::string res;
  if ((class_id != "Person") || ((object_id != "00000005") && (object_id != "00000006")))
    {
      strJson += "{\"status\" : \"failed\", ";
      if (class_id != "Flight")
          strJson += ("\"reason\": \"class " + class_id + " unknown\"}");
      else
          strJson += ("\"reason\": \"object " + object_id + " unknown\"}");
    }
  else
    {
      if (object_id == "00000005")
          res = p1_ptr->getVsID();
      if (object_id == "00000006")
          res = p2_ptr->getVsID();
        
      strJson += "{\"status\" : \"successful\", ";
        strJson += ("\"result\": " + res + "\"}");
    }

  Json::CharReaderBuilder builder;
  Json::CharReader* reader = builder.newCharReader();
  std::string errors;
  bool parsingSuccessful = reader->parse
    (strJson.c_str(), strJson.c_str() + strJson.size(), &result, &errors);
  delete reader;

  if (!parsingSuccessful) {
    std::cout << "Failed to parse the JSON, errors:" << std::endl;
    std::cout << errors << std::endl;
  }

  return result;
}

int main()
{
  Airline United;
  GPS_DD gps_Davis {38.5, -121.7};
  GPS_DD gps_NYC {40.7, -73.95};
  GPS_DD gps_Sydney {-33.87, 151.213};
  IP_Address IP_Davis {12345678};
  IP_Address IP_Sydney {12345678};
  IP_Address IP_NYC {12375678};

  //Person p1 {"Lion King", "123456789123456789"};
  //Person p2 {"Lion Queen", "223456789123456789"};

  Person p1 {"http://169.237.6.102", "1234567890", "Person", "00000005",
      "Lion King", "123456789123456789"};
  Shadow_Person sp2 {"http:localhost:8385", "1234567890", "Person", "00000006"};
    
  Flight f1 {"http://169.237.6.102", "1234567890", "Flight", "00000001",
      TR_TYPE_FLIGHT, "desc", (Person&) sp2, gps_Davis, IP_Davis, time(NULL), p1, United, "1234567890", gps_Sydney, gps_Davis};
  Flight f2 {"http://169.237.6.102", "1234567890", "Flight", "00000002",
      TR_TYPE_FLIGHT, "desc", p1, gps_Davis, IP_Davis, time(NULL), (Person&) sp2, United, "1234567890", gps_Davis, gps_NYC};
  Flight f3 {"http://169.237.6.102", "1234567890", "Flight", "00000003",
      TR_TYPE_FLIGHT, "desc", p1, gps_Davis, IP_Davis, time(NULL),
      p1, United, "9999999999", gps_NYC, gps_Sydney};

  p1_ptr = (&p1);
  p2_ptr = (&((Person&) sp2));
  f1_ptr = (&f1);
  f2_ptr = (&f2);
  f3_ptr = (&f3);

  HttpServer httpserver(8384);
  Myhw3Server s(httpserver,
		JSONRPC_SERVER_V1V2); // hybrid server (json-rpc 1.0 & 2.0)
  s.StartListening();
  std::cout << "Hit enter to stop the server" << endl;
  getchar();

  s.StopListening();
  return 0;
}
