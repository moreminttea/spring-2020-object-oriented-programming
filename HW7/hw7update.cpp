#include <iostream>
#include <stdio.h>

// JSON RPC part
#include <stdlib.h>
#include "hw7client.h"
#include <jsonrpccpp/client/connectors/httpclient.h>
#include "Post.h"

using namespace jsonrpc;
using namespace std;

int
checkBigID
(char *ss_ptr)
{
    int i, j;
    for (i = 0; i < strlen(ss_ptr); i++)
    {
        char buff[64];
        bzero(buff, 64);
        strncpy(buff, &(ss_ptr[i]), 4);
        if (strcmp(buff, "\"id\"") == 0)
        {
            char *sd_ptr = (&(ss_ptr[i])) + 4;
            char buff2[64];
            bzero(buff2, 64);
            sscanf(sd_ptr, "%[^\"]", buff2);
            sd_ptr += strlen(buff2) + 1;
            std::cout << "Hit One [" << buff2 << "]\n";
            
            for (j = 0; j < strlen(sd_ptr); j++)
            {
                if ((sd_ptr[j] == '\"') && (j > 0) && (j < 128))
                {
                    return 0;
                }
                
                if (j >= 128)
                {
                    return -1;
                }
                
                if (((sd_ptr[j] < '0') || (sd_ptr[j] > '9')) && (sd_ptr[j] != '_'))
                {
                    return -1;
                }
            }
        }
    }
    return -1;
}

int
main(int argc, char *argv[])
{
    if (argc != 2)
    {
        return -1;
    }

    FILE *jf1 = fopen(argv[1], "r");
    if (jf1 == NULL)
    {
        return -1;
    }
    
    /*
    int c = getc(jf1);
    while (c != EOF)
    {
       putchar(c);
       c = getc(jf1);
    }
     */

    Json::CharReaderBuilder builder;
    Json::CharReader* reader;
    std::string errors;
    bool parsingSuccessful;

    // *** process JSON File ***
    long lSize1;
    size_t lresult1;

    // obtain file size:
    fseek(jf1, 0 , SEEK_END);
    lSize1 = ftell(jf1);
    rewind(jf1);

    // allocate memory to contain the whole file:
    char *jf1_ptr = (char *) malloc(sizeof(char)*lSize1);
    // copy the file into the buffer:
    lresult1 = fread(jf1_ptr, 1, lSize1, jf1);
    if (lresult1 != lSize1)
    {
        fputs("Reading error", stderr);
        exit (-3);
    }
    
    if (checkBigID(jf1_ptr) != 0)
    {
        printf("big\n");
        exit(-1);
    }
    
    Json::Value myv_message;
    reader = builder.newCharReader();
    parsingSuccessful = reader->parse(jf1_ptr, jf1_ptr + lSize1, &myv_message, &errors);
    delete reader;

    if (!parsingSuccessful)
    {
        std::cout << "Failed to parse the content of the first JSON, errors:" << std::endl;
        std::cout << errors << std::endl;
    }
    
    fclose(jf1);
    //getchar();
    
    Post * my_post_ptr = new Post((Person *) NULL, (Message *) NULL);
    int rc = Json_Post_Merging(myv_message, my_post_ptr);
    //Post * my_post_ptr = Json_2_Post(myv_message);

    HttpClient httpclient("http://169.237.6.102:55407");
    hw7Client myClient(httpclient, JSONRPC_CLIENT_V2);
    
    Json::Value result_json;
    
    try
    {
        std::cout << (my_post_ptr->dumpJ())->toStyledString() << std::endl;
        result_json = myClient.update((my_post_ptr->dumpJ())->toStyledString());
        std::cout << result_json.toStyledString() << endl;
    }
    catch (JsonRpcException &e)
    {
        cerr << e.what() << endl;
    }
    return 0;
}
