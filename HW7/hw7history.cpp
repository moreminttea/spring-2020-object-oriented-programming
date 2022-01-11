
// hw7history.cpp
// #define _ECS36B_DEBUG_

#include "Transaction.h"
#include "Post.h"
#include <time.h>

// ./hw7history <vsID> <avatar_name> <type> <status> <latitude> <longitude> <IP_address> <post_json>

JvTime *
getNowJvTime
(void)
{
    time_t ticks;
    ticks = time(NULL);
    struct std::tm * my_tm_ptr = gmtime(&ticks);
    JvTime * jvT_ptr = new JvTime();
    int rc = jvT_ptr->setStdTM(my_tm_ptr);
    if (rc != 0)
    {
        std::cout << "error: failed to set time!" << std::endl;
        delete jvT_ptr;
        return NULL;
    }
    return jvT_ptr;
}

int
main(int argc, char *argv[])
{
    if (argc != 7)
    {
        char arg_string[] = " <vsID> <avatar_name> <latitude> <longitude> <IP_address> [<post_json>]";
        std::cout << "usage: " << argv[0] << arg_string << std::endl;
        return -1;
    }

    Transaction * transaction_ptr = new Transaction(0);
    std::string tr_agent_avatar_name { argv[2] };
    std::string tr_agent_vsID        { argv[1] };
    Person *agent_ptr = new Person(tr_agent_avatar_name, tr_agent_vsID);
    transaction_ptr->agent = agent_ptr;

    double tr_latitude   = atof(argv[3]);
    double tr_longitude = atof(argv[4]);
    GPS_DD * gps_dd_ptr = new GPS_DD(tr_latitude, tr_longitude);
    transaction_ptr->where = gps_dd_ptr;

    IP_Address * ip_ptr = new IP_Address(inet_addr(argv[5]));
    transaction_ptr->srcIP = ip_ptr;
    transaction_ptr->when = getNowJvTime();

    /*
    FILE *jf = fopen(argv[6], "r");
    if (jf == NULL)
    {
        return -1;
    }
        
    Json::CharReaderBuilder builder;
    Json::CharReader* reader;
    std::string errors;
    bool parsingSuccessful;

    // *** process JSON File ***
    long lSize;
    size_t lresult;

    // obtain file size:
    fseek(jf, 0 , SEEK_END);
    lSize = ftell(jf);
    rewind(jf);

    // allocate memory to contain the whole file:
    char *jf_ptr = (char *) malloc(sizeof(char)*lSize);
    // copy the file into the buffer:
    lresult = fread(jf_ptr, 1, lSize, jf);
    if (lresult != lSize)
    {
        fputs("Reading error", stderr);
        exit (-3);
    }
        
    Json::Value myv;
    reader = builder.newCharReader();
    parsingSuccessful = reader->parse(jf_ptr, jf_ptr + lSize, &myv, &errors);
    delete reader;

    if (!parsingSuccessful)
    {
        std::cout << "Failed to parse the content of the first JSON, errors:" << std::endl;
        std::cout << errors << std::endl;
    }
    free(jf_ptr);
    fclose(jf);
        
    //Post * my_post_ptr = Json_2_Post(myv);
    Post * my_post_ptr = new Post((Person *) NULL, (Message *) NULL);
    if (my_post_ptr == NULL)
    {
        std::cout << "my post pointer NULL \n";
        return -4;
    }
        
    if (my_post_ptr->history == NULL)
    {
        my_post_ptr->history = new vector<Transaction *>();
        (*(my_post_ptr->history)).push_back(transaction_ptr);
    }
    std::cout << (*(my_post_ptr->dumpJ())).toStyledString() << std::endl;
     */
    
    std::cout << (*(transaction_ptr->dumpJ())).toStyledString() << std::endl;
    
    return 0;
}
