#include "Tag.h"

Tag::Tag
(int arg_index, std::string arg_avatar_name, std::string arg_vsID, std::string arg_comment)
{
    this->index = arg_index;
    this->avatar_name = arg_avatar_name;
    this->vsID = arg_vsID;
    this->comment = arg_comment;
}

Json::Value * Tag::dumpJ
(void)
{
    Json::Value *result_ptr = new Json::Value();
    
    (*result_ptr)["index"] = index;
    (*result_ptr)["avatar_name"] = avatar_name;
    (*result_ptr)["vsID"] = vsID;
    (*result_ptr)["comment"] = comment;
    
#ifdef _ECS36B_DEBUG_
    std::cout << result_ptr->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */
    return result_ptr;
}
