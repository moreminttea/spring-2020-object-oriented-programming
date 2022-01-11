
#include "Shadow_Flight.h"

// JSON RPC part
#include <stdlib.h>
#include "hw3client.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

using namespace jsonrpc;
using namespace std;

Shadow_Flight::Shadow_Flight
(std::string arg_host_url, std::string arg_vsID, std::string arg_class_id, std::string arg_object_id)
  : Flight { arg_host_url, arg_vsID, arg_class_id, arg_object_id }
{
  std::cout << "Shadow created" << std::endl;
}

double
Shadow_Flight::getDistance()
{
  HttpClient httpclient((this->host_url).c_str());
  hw3Client myClient(httpclient, JSONRPC_CLIENT_V2);
  Json::Value myv;

  try {
    myv = myClient.GetDistance("GetDistance", "This is a flight JSON string!",
			      (this->class_id).c_str(), (this->host_url).c_str(),
			      (this->object_id).c_str(), (this->owner_vsID).c_str());
    cout << myv.toStyledString() << endl;
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }
  // std::cout << myv["distance"] << std::endl;
  return (myv["distance"]).asDouble();
}
    
bool
Shadow_Flight::Conflict_of_Interest()
{
  HttpClient httpclient((this->host_url).c_str());
  hw3Client myClient(httpclient, JSONRPC_CLIENT_V2);
  Json::Value myv;

  try {
    myv = myClient.Conflict_of_Interest("Conflict_of_Interest", "This is a flight JSON string!", (this->class_id).c_str(), (this->host_url).c_str(), (this->object_id).c_str(), (this->owner_vsID).c_str());
    cout << myv.toStyledString() << endl;
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }
  // std::cout << myv["distance"] << std::endl;
  return (myv["result"]).asBool();
}

