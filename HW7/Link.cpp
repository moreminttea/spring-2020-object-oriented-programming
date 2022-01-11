#include "Link.h"

Link::Link
(std::string arg_url, std::string arg_class_id, std::string arg_object_id)
{
  this->url = arg_url;
  this->class_id = arg_class_id;
  this->object_id = arg_object_id;
}

Json::Value * Link::dumpJ
(void)
{
    Json::Value *result_ptr = new Json::Value();
    
    (*result_ptr)["url"] = url;
    (*result_ptr)["class_id"] = class_id;
    (*result_ptr)["object_id"] = object_id;
    
#ifdef _ECS36B_DEBUG_
    std::cout << result_ptr->toStyledString() << std::endl;
#endif /* _ECS36B_DEBUG_ */
    return result_ptr;
}
