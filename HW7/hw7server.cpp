// for Json::value
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include <string>
#include <string.h>
#include <ctype.h>

// for JsonRPCCPP
#include <iostream>
#include "hw7server.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <stdio.h>

// ecs36b_hw7
#include "Post.h"

std::map<std::string, Post *> Post_Map;
std::map<std::string, Person *> vsID_Map;
std::map<std::string, JvTime *> RT_Map;

using namespace jsonrpc;
using namespace std;

const char *hw7error[] =
{
    "Normal",
    "Incorrect vsID in history",
    "Incorrect vsID in Post ID",
    "JSON_Post_Merging failed",
    "lresult lSize mismatch",
    "JSON Parsing Error",
    "JSON_2_Post failed",
    "JSON Post No ID",
    "JSON Value pointer NULL",
    "File not Exist",
    "File Read Error",
    "File Name Pointer Null",
    "File Writer Error",
    "Less Than 20 History Records",
    "No Search Keyword Found",
    "No OKey Found",
    "No OKey Found and very Strange",
    "Smack Stack Detected",
    "Search Keyword Format Incorrect",
    "std set struct tm error",
    "Null C++ Post Object Pointer",
    "Post ID Mismatched",
};

class Myhw7Server : public hw7Server
{
    public:
        Myhw7Server(AbstractServerConnector &connector, serverVersion_t type);
        virtual Json::Value update(const std::string& updating_json);
        virtual Json::Value search(const std::string& search_clause);
        virtual Json::Value validate(const std::string& validating_json);
};

Myhw7Server::Myhw7Server(AbstractServerConnector &connector, serverVersion_t type)
  : hw7Server(connector, type)
{
    std::cout << "Myhw7Server Object created" << std::endl;
}

// member functions

int
checkTimeInterval
(const char *data, int length)
{
    int *int_ptr = (int *) data;
    int int_array_size = length / sizeof(int);
    int i;
    int result = 95616;
    for (i = 0; i < int_array_size; i++)
    {
        result ^= int_ptr[i];
    }
    return result;
}

Json::Value
Myhw7Server::validate(const std::string& validating_json)
{
    Json::Value result_json;
    char fname_buf[1024];
    FILE * post_f_ptr;

    int rc = HW7_ERROR_NORMAL;
    Post * post_ptr = NULL;
#ifdef _ECS36B_DEBUG_
    std::cout << "validate" << " " << validating_json << std::endl;
#endif /* _ECS36B_DEBUG_ */

    //print log
    myPrintLog(validating_json, "hw7server_validate.log");
    
    try
    {
        Json::Value myv_message;
        std::cout << "validate p0\n";
        
        rc = checkBigID(validating_json);
        if (rc != HW7_ERROR_NORMAL)
        {
            throw rc;
        }
        
        rc = myParseJSON(validating_json, &myv_message);
        if (rc != HW7_ERROR_NORMAL)
        {
            throw rc;
        }
        
        std::cout << "Validate p1\n";
        
        if (((myv_message["id"]).isNull() == true) && ((myv_message["id"]).isString() == false) && (checkPostID((myv_message["id"]).asString()) != 0))
        {
            throw HW7_ERROR_JSON_POST_NO_ID;
        }
        
        //check for timestamp
        rc = checkTimeInterval((myv_message["id"]).asString());
        if (rc != HW7_ERROR_NORMAL)
        {
            throw rc;
        }
        
        //set json_post file name
        bzero(fname_buf, 1024);
        sprintf(fname_buf, "post_%s.json", ((myv_message["id"]).asString()).c_str());
        
        Post *current_ptr = NULL;
        
        if (Post_Map.find((myv_message["id"]).asString()) == Post_Map.end())
        {
            // not in memory
            post_f_ptr = fopen(fname_buf, "r");
            if (post_f_ptr == NULL)
            {
                //neither in file
                throw HW7_ERROR_FILE_NOT_EXIST;
            }
        }
        else
        {
            Json::Value pjf_v;
            fclose(post_f_ptr);
            rc = myFile2JSON(fname_buf, &pjf_v);
            if (rc != HW7_ERROR_NORMAL)
            {
                std::cout << "validate p113 [" << fname_buf << "]\n";
            }
            
            else
            {
                current_ptr = new Post((Person *) NULL, (Message *) NULL);
                rc = JSON_2_Post(rjf_v, current_ptr);
                std::cout << "[ " << hw7error[(-1) * rc] << "]\n";
                if (rc != HW7_ERROR_NORMAL)
                {
                    delete current_ptr;
                    current_ptr = NULL;
                    //throw rc;
                }
                Post_Map[(myv_message["id"]).asString()] = current_ptr;
            }
        }
        else
        {
            current_ptr = Post_Map[(myv_message["id"]).asString()];
        }
    
        Global_Current_Hack = 0;
        if (current_ptr != NULL)
        {
            if (current_ptr->history == NULL)
            {
                Global_Current_Hack = (current_ptr->history)->size();
            }
        }
        std::cout << "validate p2\n";
    
        //now we check
        post_ptr = new Post((Person *) NULL, (Message *) NULL);
        rc = Json_Post_Merging(myv_message, post_ptr);
        if (rc != HW7_ERROR_NORMAL)
        {
            throw rc;
        }
    
        std::cout << "validate p3\n";
        
        if ((post_ptr->history == NULL) || ((post_ptr->history)->size() < 20))
        {
            //now check for real one
            throw HW7_ERROR_LESS_THAN_20;
        }
    }
    catch (int error_code)
    {
        int erc = produceErrorJSON(error_code, "hw7server_validate.log", &result_json);
        if (erc != HW7_ERROR_NORMAL)
        {
            std::cout << "more failure\n";
        }
        return result_json;
    }
    
    Json::Value *jv_ptr = post_ptr->dumpJ();
    const char *lv_str = (jv_ptr->toStyledString()).c_str();
    (*jv_ptr)["validate"] = simpleChecksum(lv_str, strlen(lv_str) + 1);
    return (*(jv_ptr));
}

Json::Value
Myhw7Server::update(const std::string& updating_json)
{
    Json::Value result_json;
    char fname_buf[1024];
    FILE * post_f_ptr;

    int rc = 0;

    std::cout << Post_Map.size() << std::endl;

#ifdef _ECS36B_DEBUG_
    std::cout << "update" << " " << updating_json << std::endl;
#endif /* _ECS36B_DEBUG_ */

    //print log
    //myPrintLog(updating_json, "hw7server_update.log");
    //std::cout << Post_Map.size() << std::endl;
    Post * post_ptr = NULL;

    Json::CharReaderBuilder builder;
    Json::CharReader* reader;
    std::string errors;
    bool parsingSuccessful;

    Json::Value myv_message;
    reader = builder.newCharReader();
    parsingSuccessful = reader->parse(updating_json.c_str(), updating_json.c_str() + updating_json.size(), &myv_message, &errors);
    delete reader;

    if (!parsingSuccessful)
    {
        std::cout << "Failed to parse the content of the first JSON, errors:" << std::endl;
        std::cout << errors << std::endl;
        // should not continue
    }

    // set the json_post file name
    // I should always check whether this myv_message has a valid "id"
    if ((myv_message["id"]).isNull() == true)
      {
        std::cerr << "id field missing\n";
        exit(-1);
      }
    
    bzero(fname_buf, 1024);
    sprintf(fname_buf, "post_%s.json", ((myv_message["id"]).asString()).c_str());

    long lSize;
    size_t lresult;

    if (Post_Map.find((myv_message["id"]).asString()) != Post_Map.end()) //case 1
    {
        // we need to merge
        rc = 0;
        post_ptr = Post_Map[(myv_message["id"]).asString()];
        rc = Json_Post_Merging(myv_message, post_ptr);
    }
    else
    {
        post_ptr = Json_2_Post(myv_message);
        Post_Map[(myv_message["id"]).asString()] = post_ptr;
    }

    if (rc < 0)
    {
        std::string strJson;
        strJson += "{\"status\" : \"failed\"}";
        reader = builder.newCharReader();
        parsingSuccessful = reader->parse
        (strJson.c_str(), strJson.c_str() + strJson.size(), &result_json, &errors);
        delete reader;

        if (!parsingSuccessful)
        {
          std::cout << "Failed to parse the JSON, errors:" << std::endl;
          std::cout << errors << std::endl;
        }
        return result_json;
    }

    // write it back
    // std::cout << fname_buf <<  std::endl;
    post_f_ptr = fopen(fname_buf, "w");
    if (post_f_ptr != NULL)
    {
        char post_json_buf[8192]; // limited size
        bzero(post_json_buf, 8192);
        sprintf(post_json_buf, "%s", ((*(post_ptr->dumpJ())).toStyledString()).c_str());
        lSize = strlen(post_json_buf);
        std::cout << fname_buf << " " << lSize <<  std::endl;
        lresult = fwrite(post_json_buf, 1, lSize, post_f_ptr);
        if (lresult != lSize)
        {
            fputs("Reading error", stderr);
            exit (-3);
        }
        fclose(post_f_ptr);
    }
    std::cout << Post_Map.size() << std::endl;
    return (*(post_ptr->dumpJ()));
}

Json::Value
Myhw7Server::search
(const std::string& search_clause)
{
    Json::Value result_json;
    int rc = 0;

#ifdef _ECS36B_DEBUG_
    std::cout << "search" << " " << search_clause << std::endl;
#endif /* _ECS36B_DEBUG_ */
    std::cout << "search" << " " << search_clause << std::endl;

    Json::CharReaderBuilder builder;
    Json::CharReader* reader;
    std::string errors;
    bool parsingSuccessful;

    Json::Value myv_message;
    reader = builder.newCharReader();
    parsingSuccessful = reader->parse(search_clause.c_str(),
				    search_clause.c_str() + search_clause.size(),
				    &myv_message, &errors);
    delete reader;

    if (!parsingSuccessful)
    {
        std::cout << "Failed to parse the content of the first JSON, errors:" << std::endl;
        std::cout << errors << std::endl;
    }

    Json::Value json_data;
    if ((myv_message["keyword"]).isNull() == true)
    {
        rc = -1;
    }
    else
    {
        std::vector<Post *> *pv_ptr = getPostbyKey((myv_message["keyword"]).asString());
        if (pv_ptr == NULL)
        {
            rc = -2;
        }
        else
        {
            int i;
            Json::Value *dumpjv_ptr;
            for (i = 0; i < pv_ptr->size(); i++)
            {
                dumpjv_ptr = ((*pv_ptr)[i])->dumpJ();
                json_data[i] = (*dumpjv_ptr);
                delete dumpjv_ptr;
            }
            result_json["data"] = json_data;
            result_json["count"] = ((int) pv_ptr->size());
        }
    }

    // std::cout << "rc = " << " " << rc << std::endl;
    // std::cout << (json_data).toStyledString() << std::endl;

    if (rc < 0)
    {
        std::string strJson;
        strJson += "{\"status\" : \"failed\"}";
        reader = builder.newCharReader();
        parsingSuccessful = reader->parse
        (strJson.c_str(), strJson.c_str() + strJson.size(), &result_json, &errors);
        delete reader;

        if (!parsingSuccessful)
        {
            std::cout << "Failed to parse the JSON, errors:" << std::endl;
            std::cout << errors << std::endl;
        }
    }
    // std::cout << (result_json).toStyledString() << std::endl;
    return result_json;
}

Json::Value
Myhw7Server::validate
(const std::string& validating_json)
{
    Json::Value result_json;
    int rc = 0;

#ifdef _ECS36B_DEBUG_
    std::cout << "validate" << " " << validating_json << std::endl;
#endif /* _ECS36B_DEBUG_ */
    std::cout << "validate" << " " << validating_json << std::endl;

    Json::CharReaderBuilder builder;
    Json::CharReader* reader;
    std::string errors;
    bool parsingSuccessful;

    Json::Value myv_message;
    reader = builder.newCharReader();
    parsingSuccessful = reader->parse(validating_json.c_str(),
                    validating_json.c_str() + validating_json.size(),
                    &myv_message, &errors);
    delete reader;

    if (!parsingSuccessful)
    {
        std::cout << "Failed to parse the content of the first JSON, errors:" << std::endl;
        std::cout << errors << std::endl;
    }

    Json::Value json_data;
    if ((myv_message["keyword"]).isNull() == true)
    {
        rc = -1;
    }
    else
    {
        std::vector<Post *> *pv_ptr = getPostbyKey((myv_message["keyword"]).asString());
        if (pv_ptr == NULL)
        {
            rc = -2;
        }
        else
        {
            int i;
            Json::Value *dumpjv_ptr;
            for (i = 0; i < pv_ptr->size(); i++)
            {
                dumpjv_ptr = ((*pv_ptr)[i])->dumpJ();
                json_data[i] = (*dumpjv_ptr);
                delete dumpjv_ptr;
            }
            result_json["data"] = json_data;
            result_json["count"] = ((int) pv_ptr->size());
        }
    }

    // std::cout << "rc = " << " " << rc << std::endl;
    // std::cout << (json_data).toStyledString() << std::endl;

    if (rc < 0)
    {
        std::string strJson;
        strJson += "{\"status\" : \"failed\"}";
        reader = builder.newCharReader();
        parsingSuccessful = reader->parse
        (strJson.c_str(), strJson.c_str() + strJson.size(), &result_json, &errors);
        delete reader;

        if (!parsingSuccessful)
        {
            std::cout << "Failed to parse the JSON, errors:" << std::endl;
            std::cout << errors << std::endl;
        }
    }
    // std::cout << (result_json).toStyledString() << std::endl;
    return result_json;
}


int
main(void)
{
    HttpServer httpserver(8384);
    Myhw7Server s(httpserver, JSONRPC_SERVER_V1V2); // hybrid server (json-rpc 1.0 & 2.0)
    s.StartListening();
    std::cout << "Hit enter to stop the server" << endl;
    getchar();

    s.StopListening();
    return 0;
}
