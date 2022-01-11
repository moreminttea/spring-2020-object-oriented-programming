#include "Post.h"

Post::Post
(Person * arg_author, Message * arg_msg)
{
    this->author = arg_author;
    this->msg = arg_msg;
    
    this->author = NULL;
    this->receivers = NULL;
    this->msg = NULL;
    this->links = NULL;
    this->actions = NULL;
    this->reactions = NULL;
    this->comments = NULL;
    this->keys = NULL;
    this->history = NULL;
    this->created = NULL;
    this->updated = NULL;
    this->is_published = false;
}

Json::Value * Post::dumpJ
(void)
{
    int i;

    Json::Value *result_ptr = new Json::Value();
    Json::Value * dumpjv_ptr;
    std::string l_id = this->profile_id + "_" + this->post_id;
    
    //"actions"
    if ((this->actions != NULL) && ((this->actions)->size() > 0))
    {
        Json::Value json_actions;
        Json::Value json_actions_array;
        for (i= 0; i < (this->actions)->size(); i++)
        {
            Json::Value *dumpjv_ptr = ((*(this->actions))[i])->dumpJ();
            json_actions[i] = (*dumpjv_ptr);
            delete dumpjv_ptr;
        }
        json_actions_array["data"] = json_actions;
        json_actions_array["count"] = ((int) (*(this->actions)).size());
        (*result_ptr)["actions"] = json_actions_array;
    }
    
    //"comments"
    Json::Value json_comments;
    Json::Value json_comments_array;
    if ((this->comments != NULL) && ((this->comments)->size() > 0))
    {
        for (i = 0; i < (this->comments)->size(); i++)
        {
               Json::Value * dumpjv_ptr = ((*(this->comments))[i])->dumpJ();
               json_comments[i] = (*dumpjv_ptr);
               delete dumpjv_ptr;
        }
        json_comments_array["data"] = json_comments;
        json_comments_array["count"] = ((int) (*(this->comments)).size());
        (*result_ptr)["comments"] = json_comments_array;
    }
    
    //"links"
    if (this->links != NULL)
    {
        Json::Value json_links;
        Json::Value json_links_array;
        for (i = 0; i < (this->links)->size(); i++)
        {
            Json::Value * dumpjv_ptr = ((*(this->links))[i])->dumpJ();
            json_links[i] = (*dumpjv_ptr);
            delete dumpjv_ptr;
        }
        json_links_array["data"] = json_links;
        json_links_array["count"] = ((int) (*(this->links)).size());
        (*result_ptr)["links"] = json_links_array;
    }
    
    // "message"
    if (this->msg != NULL)
    {
        (*result_ptr)["message"] = (this->msg)->content;
    }
    
    //"reactions"
    Json::Value json_c_reactions;
    Json::Value json_c_reactions_array;
    if ((this->reactions != NULL) && ((this->reactions)->size() > 0))
    {
        for (i = 0; i < (this->reactions)->size(); i++)
        {
            Json::Value * dumpjv_ptr = ((*(this->reactions))[i])->dumpJ();
            json_c_reactions[i] = (*dumpjv_ptr);
            delete dumpjv_ptr;
        }
        json_c_reactions_array["data"] = json_c_reactions;
        json_c_reactions_array["count"] = ((int) (*(this->reactions)).size());
        (*result_ptr)["reactions"] = json_c_reactions_array;
    }
    
    //"history"
    if ((this->history != NULL) && ((this->history)->size() > 0))
    {
        Json::Value json_history;
        Json::Value json_history_array;
        for (i = 0; i < (this->history)->size(); i++)
        {
            Json::Value * dumpjv_ptr = ((*(this->history))[i])->dumpJ();
            json_history[i] = (*dumpjv_ptr);
            delete dumpjv_ptr;
        }
        json_history_array["data"] = json_history;
        json_history_array["count"] = ((int) (*(this->history)).size());
        (*result_ptr)["history"] = json_history_array;
    }
    
    //"id"
    (*result_ptr)["id"] = l_id;
    
    // "from"
    if (this->author != NULL)
    {
        dumpjv_ptr = (this->author)->dumpJ();
        (*result_ptr)["from"] = (*dumpjv_ptr);
        delete dumpjv_ptr;
    }

    //"to"
    if (this->receivers != NULL)
    {
        Json::Value json_receivers;
        Json::Value json_receivers_array;
        for (i = 0; i < (this->receivers)->size(); i++)
        {
            Json::Value * dumpjv_ptr = ((*(this->receivers))[i])->dumpJ();
            json_receivers[i] = (*dumpjv_ptr);
            delete dumpjv_ptr;
        }
        json_receivers_array["data"] = json_receivers;
        json_receivers_array["count"] = ((int) (*(this->receivers)).size());
        (*result_ptr)["to"] = json_receivers_array;
    }
    
    // "created_time"
    if (this->created != NULL)
    {
        (*result_ptr)["created_time"] = *((this->created)->getTimeString());
    }

    // "updated_time"
    if (this->updated != NULL)
    {
        (*result_ptr)["updated_time"] = *((this->updated)->getTimeString());
    }

    // "is_published"
    (*result_ptr)["is_published"] = this->is_published;
    
#ifdef _ECS36B_DEBUG_
    std::cout << result_ptr->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */
    return result_ptr;
}

Json::Value * Post:: dumpJ_H_only
(void)
{
    int i;
    
    Json::Value * result_ptr = new Json::Value();
    Json::Value * dumpjv_ptr;
    
    // "history"
    if ((this->history != NULL) && ((this->history)->size() > 0))
    {
        Json::Value json_history;
        Json::Value json_history_array;
        for (i = 0; i < (this->history)->size(); i++)
        {
            dumpjv_ptr = ((*(this->history))[i])->dumpJ();
            json_history[i] = (*dumpjv_ptr);
            delete dumpjv_ptr;
        }
        json_history_array["data"] = json_history;
        json_history_array["count"] = ((int) (*(this->history)).size());
        (*result_ptr)["history"] = json_history_array;
    }
    
    // "id"
    (*result_ptr)["id"] = (this->profile_id + "_" + this->post_id);
    
#ifdef _ECS36B_DEBUG_
    std::cout << result_ptr->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */
    return result_ptr;
}
