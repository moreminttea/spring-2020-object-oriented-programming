// JSON_Post.cpp
// #define _ECS36B_DEBUG_

#include <arpa/inet.h>
#include "Post.h"

std::map<std::string, OKey *> All_Keys;
Post * my_post_ptr = new Post((Person *) NULL, (Message *) NULL);

std::vector<Post *> *
getPostbyKey
(std::string key)
{
    if (All_Keys.find(key) == All_Keys.end())
    {
        return NULL;
    }
    
    OKey *okey_ptr = All_Keys[key];
    return okey_ptr->posts;
}

OKey *
getOKey
(std::string key)
{
    if (All_Keys.find(key) != All_Keys.end())
    {
        return All_Keys[key];
    }
    
    OKey *new_okey_ptr = new OKey(key);
    new_okey_ptr->posts = new std::vector<Post *>();
    All_Keys[key] = new_okey_ptr;
    return new_okey_ptr;
}

std::string
Json_2_PostID
(Json::Value myv)
{
    return ((myv["id"]).asString());
}
 
int
Json_Post_Common
(Json::Value myv, Post *my_post_ptr)
{
    int i, j;

    //Post * my_post_ptr = new Post((Person *) NULL, (Message *) NULL);
    bzero(my_post_ptr, sizeof(class Post));

    char idstr[1024];

    // "actions"
    my_post_ptr->actions = new vector<Action *>();
    if ((myv["actions"]["count"]).asInt() > 0)
    {
        for (i = 0; i < (myv["actions"]["count"]).asInt(); i++)
        {
            Json::Value l_jv = myv["actions"]["data"][i];
            Action * action_ptr = new Action((l_jv["class_id"]).asString(), (l_jv["host_url"]).asString(), (l_jv["name"]).asString(), (l_jv["object_id"]).asString(), (l_jv["owner_vsID"]).asString());
            (*(my_post_ptr->actions)).push_back(action_ptr);
        }
    }
    
    // "comments"
    if ((myv["comments"]["count"]).asInt() > 0)
    {
        if (my_post_ptr->comments == NULL)
        {
            my_post_ptr->comments = new vector<Comment *>();
        }
        for (i = 0; i < (myv["comments"]["count"]).asInt(); i++)
        {
            vector<Reaction *> * vec_c_reactions_ptr;
            vector<Tag *> * vec_tag_ptr;
            
            Json::Value l_jv = myv["comments"]["data"][i];
            
            char buf_profile[1024];
            char buf_post[1024];
            char buf_comment[1024];
            bzero(buf_profile, 1024);
            bzero(buf_post, 1024);
            bzero(buf_comment, 1024);
            sscanf((l_jv["id"].asString()).c_str(), "%[^_]_%[^_]_%s", buf_profile, buf_post, buf_comment);
            std::string s_profile = { buf_profile };
            std::string s_post = { buf_post };
            std::string s_comment = { buf_comment };
            
            Person * p_ptr = new Person((l_jv["from"]["avatar_name"]).asString(), (l_jv["from"]["avatar_name"]).asString());
            Message *m_ptr = new Message((l_jv["message"]).asString());
            JvTime *cr_time_ptr = new JvTime(((l_jv["created_time"]).asString()).c_str());
            Comment *comment_ptr = new Comment(s_profile, s_post, s_comment, p_ptr, m_ptr, cr_time_ptr);
            
            //adding reactions part
            if(((l_jv["reactions"]).isNull() != true) && ((l_jv["reactions"]["count"]).asInt() > 0))
            {
                if (my_post_ptr->reactions == NULL)
                {
                    vec_c_reactions_ptr = new vector<Reaction *>();
                }
                for (j = 0; j < (l_jv["reactions"]["count"]).asInt(); j++)
                {
                    Json::Value l_jv_react = l_jv["reactions"]["data"][j];
                    Person *react_p_ptr = new Person((l_jv_react["avatar_name"]).asString(), (l_jv_react["vsID"]).asString());
                    JvTime *react_cr_time_ptr = new JvTime(((l_jv_react["created_time"]).asString()).c_str());
                    Reaction *react_ptr = new Reaction((l_jv_react["type"]).asString(), react_p_ptr, react_cr_time_ptr);
                    vec_c_reactions_ptr->push_back(react_ptr);
                }
            }
            comment_ptr->reactions = vec_c_reactions_ptr;
            
            // adding the tags part
            Tag *tags_ptr;
            if(((l_jv["tags"]).isNull() != true) && ((l_jv["tags"]["count"]).asInt() > 0))
            {
                if (comment_ptr->tags == NULL)
                {
                    vec_tag_ptr = new vector<Tag *>();
                }
                for (j = 0; j < (l_jv["tags"]["count"]).asInt(); j++)
                {
                    Json::Value l_jv_tag = l_jv["tags"][j];
                    tags_ptr->index = (l_jv_tag["data"]["index"]).asInt();
                    tags_ptr->avatar_name = (l_jv_tag["data"]["avatar_name"]).asString();
                    tags_ptr->comment = (l_jv_tag["data"]["comment"]).asString();
                    tags_ptr->vsID = (l_jv_tag["data"]["vsID"]).asString();
                    Tag *tags_ptr = new Tag(tags_ptr->index, tags_ptr->avatar_name, tags_ptr->comment, tags_ptr->vsID);
                    vec_tag_ptr->push_back(tags_ptr);
                }
            }
            comment_ptr->tags = vec_tag_ptr;
            
            // done with one comment, now push back into the comments vector
            (*(my_post_ptr->comments)).push_back(comment_ptr);
        }
    }
    
    // "keys"
    if (((myv["keys"]).isNull() != true) && ((myv["keys"]["count"]).asInt() > 0))
    {
        if (my_post_ptr->keys == NULL)
        {
            my_post_ptr->keys = new vector<OKey *>();
        }
        for (i = 0; i < (myv["keys"]["count"]).asInt(); i++)
        {
            Json::Value l_jv = myv["keys"]["data"][i];
            if (l_jv.isNull() != true)
            {
                OKey * okey_ptr = getOKey(l_jv.asString());
                //adding keyword object
                (*(okey_ptr->posts)).push_back(my_post_ptr);
                (*(my_post_ptr->keys)).push_back(okey_ptr);
            }
        }
    }
    
    // "links"
    if (((myv["links"]).isNull() != true) && ((myv["links"]["count"]).asInt() > 0))
    {
        if (my_post_ptr->links == NULL)
        {
            my_post_ptr->links = new vector<Link *>();
        }
        for (i = 0; i < (myv["links"]["count"]).asInt(); i++)
        {
            Json::Value l_jv = myv["links"]["data"][i];
            Link * link_ptr = new Link((l_jv["url"]).asString(), (l_jv["class_id"]).asString(), (l_jv["object_id"]).asString());
            (*(my_post_ptr->links)).push_back(link_ptr);
        }
    }
    
    // "message"
    Message *msg_ptr;
    if ((myv["message"]).isNull() != true)
    {
        msg_ptr = new Message(myv["message"].asString());
        my_post_ptr->msg = msg_ptr;
    }
    
    // "reactions"
    if (((myv["reactions"]).isNull() != true) && ((myv["reactions"]["count"]).asInt() > 0))
    {
        if (my_post_ptr->reactions == NULL)
        {
            my_post_ptr->reactions = new vector<Reaction *>();
        }
        JvTime *react_cr_time_ptr;
        for (i = 0; i < (myv["reactions"]["count"]).asInt(); i++)
        {
            Json::Value l_jv = myv["reactions"]["data"][i];
            if (((l_jv["avatar_name"]).isNull() != true) && ((l_jv["vsID"]).isNull() != true) && ((l_jv["created_time"]).isNull() != true) && ((l_jv["type"]).isNull() != true))
            {
                Person *react_p_ptr = new Person((l_jv["avatar_name"]).asString(), (l_jv["vsID"]).asString());
                JvTime *react_cr_time_ptr = new JvTime(((l_jv["created_time"]).asString()).c_str());
                Reaction *react_ptr = new Reaction(l_jv["type"].asString(), react_p_ptr, react_cr_time_ptr);
                (*(my_post_ptr->reactions)).push_back(react_ptr);
            }
        }
    }
    
    // "history"
    Transaction *transaction_ptr;
    if (((myv["history"]).isNull() != true) && ((myv["history"]["data"]).isNull() != true) && ((myv["history"]["data"]).isArray() == true))
    {
        if (my_post_ptr->history == NULL)
        {
            my_post_ptr->history = new vector<Transaction *>();
        }
        for (i = 0; i < (myv["history"]["data"]).size(); i++)
        {
            Json::Value l_jv = myv["history"]["data"][i];
            Transaction * transaction_ptr = Json_Transaction_Common(l_jv);
            if (transaction_ptr != NULL)
            {
                //check if Transaction already exists
                vector<Transaction *>::iterator my_it_hs;
                int flag_hs = 0;
                for (my_it_hs = (*(my_post_ptr->history)).begin(); my_it_hs < (*(my_post_ptr->history)).end(); my_it_hs++)
                {
                    if (((*((*my_it_hs)->agent)) == (*(transaction_ptr->agent))) && ((*((*my_it_hs)->when)) == (*(transaction_ptr->when))))
                    {
                        flag_hs = 1;
                    }
                }
                if (flag_hs == 0)
                {
                    return HW7_ERROR_POST_ID_MISMATCHED;
                }
            }
            else
            {
                std::cerr << "No Post ID presented for the second JSON" << std::endl;
                return HW7_ERROR_JSON_POST_NO_ID;
            }
        }
        //now they match so merge
        return Json_Post_Common(myv, my_post_ptr);
    }
    
    // "from"
    Person *author_ptr;
    if (((myv["from"]).isNull() != true) && ((myv["from"]["count"]).asInt() > 0))
    {
        author_ptr = new Person((myv["from"]["avatar_name"]).asString(), (myv["from"]["vsID"]).asString());
        my_post_ptr->author = author_ptr;
    }
    
    // "to"
    Person *to_ptr;
    if (((myv["to"]).isNull() != true) && ((myv["to"]["count"]).asInt() > 0))
    {
        if (my_post_ptr->receivers == NULL)
        {
            my_post_ptr->receivers = new vector<Person *>();
        }
        for (i = 0; i < (myv["to"]["count"]).asInt(); i++)
        {
            Json::Value l_jv = myv["to"]["data"][i];
            if (((l_jv["avatar_name"]).isNull() != true) && ((l_jv["vsID"]).isNull() != true))
            {
                to_ptr = new Person((l_jv["avatar_name"]).asString(), (l_jv["vsID"]).asString());
                (*(my_post_ptr->receivers)).push_back(to_ptr);
            }
        }
    }
    
    // "created_time"
    if ((myv["created_time"]).isNull() != true)
    {
        my_post_ptr->created = new JvTime(((myv["created_time"]).asString()).c_str());
    }
    
    // "updated_time"
    if ((myv["updated_time"]).isNull() != true)
    {
        my_post_ptr->updated = new JvTime(((myv["updated_time"]).asString()).c_str());
    }
    
    // "is_published"
    my_post_ptr->is_published = (myv["is_published"]).asBool();
    
    return 0;
}

Post *
Json_2_Post
(Json::Value myv)
{
    if (myv.isArray() == true)
    {
        //wrong one
        return NULL;
    }
    
    Post *my_post_ptr = new Post((Person*) NULL, (Message *) NULL);
    
    //"id"
    if ((myv["id"]).isNull() != true)
    {
        char idstr[1024];
        bzero(idstr, 1024);
        sprintf(idstr, "%s", ((myv["id"]).asString()).c_str());
        if (idstr[0] != '\0')
        {
            char c_prof_buf[256];
            char c_post_buf[256];
            bzero(c_prof_buf, 256);
            bzero(c_post_buf, 256);
            
            sscanf(idstr, "%[^_]_%s", c_prof_buf, c_post_buf);
            
            std::string profile_id = { c_prof_buf };
            std::string post_id = { c_post_buf };
            
            //put id attributes to post
            my_post_ptr->profile_id = profile_id;
            my_post_ptr->post_id = post_id;
        }
        else
        {
            std::cerr << "No Post ID presented" << std::endl;
            exit(-1);
        }
    }
    else
    {
        std::cerr << "No Post ID presented" << std::endl;
        exit(-2);
    }
    
    int rc = Json_Post_Common(myv, my_post_ptr);
    if (rc != 0)
    {
        std::cerr << "Error Json_Post_Common " << rc << std::endl;
        exit(-3);
    }
    return my_post_ptr;
}

int
Json_Post_Merging
(Json::Value myv, Post *my_post_ptr)
{
    char idstr[1024];
    bzero(idstr, 1024);
    
    if ((myv).isArray() == true)
    {
        return -3;
    }
    
    if ((myv["id"]).isNull() == true)
    {
        return -1;
    }
    
    // compare ID first
    // "id" (Let us check first if they share the same ID)
    sprintf(idstr, "%s", ((myv["id"]).asString()).c_str());

    if (idstr[0] != '\0')
    {
        char c_prof_buf[256];
        char c_post_buf[256];
        bzero(c_prof_buf, 256);
        bzero(c_post_buf, 256);
        
        sscanf(idstr, "%[^_]_%s", c_prof_buf, c_post_buf);
        
        std::string profile_id = { c_prof_buf };
        std::string post_id = { c_post_buf };

        if ((my_post_ptr->profile_id != profile_id) || (my_post_ptr->post_id != post_id))
        {
            std::cerr << "Post IDs mismatched!!" << std::endl;
            return -1;
        }
    }
    else
    {
        std::cerr << "No Post ID presented for the second JSON" << std::endl;
        return -2;
    }
    //they match, now merge
    return Json_Post_Common(myv, my_post_ptr);
}
