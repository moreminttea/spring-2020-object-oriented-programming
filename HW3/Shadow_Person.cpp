#include "Shadow_Person.h"

// JSON RPC part
#include <stdlib.h>
#include "hw3client.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

using namespace jsonrpc;
using namespace std;

Shadow_Person::Shadow_Person
(std::string arg_host_url, std::string arg_vsID, std::string arg_class_id, std::string arg_object_id)
: Person { arg_host_url, arg_vsID, arg_class_id, arg_object_id }
{
  std::cout << "Shadow created" << std::endl;
}

bool
Shadow_Person::operator==
(Person& aPerson)
{
  HttpClient httpclient((this->host_url).c_str());
  hw3Client myClient(httpclient, JSONRPC_CLIENT_V2);
  Json::Value myv;

  try {
    myv = myClient.OperatorEqEq((aPerson.class_id).c_str(), (aPerson.host_url).c_str(), (aPerson.object_id).c_str(), (aPerson.owner_vsID).c_str(), "operator==", "This is a Person JSON string!", (this->class_id).c_str(), (this->host_url).c_str(), (this->object_id).c_str(), (this->owner_vsID).c_str());
    cout << myv.toStyledString() << endl;
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }
  // std::cout << myv["distance"] << std::endl;
  return (myv["result"]).asBool();
}

std::string
Shadow_Person::getVsID()
{
    HttpClient httpclient((this->host_url).c_str());
    hw3Client myClient(httpclient, JSONRPC_CLIENT_V2);
    Json::Value myv;

    try {
      myv = myClient.GetVsID("getVsID", "This is a Flight JSON string!", (this->class_id).c_str(), (this->host_url).c_str(), (this->object_id).c_str(), (this->owner_vsID).c_str());
      cout << myv.toStyledString() << endl;
    } catch (JsonRpcException &e) {
      cerr << e.what() << endl;
    }
    // std::cout << myv["distance"] << std::endl;
    return (myv["result"]).asString();
}
