#include <stdio.h>
#include <json.h>
int main()
{
    json_object  *obj = json_object_new_object();

    json_object_object_add(obj,"name",json_object_new_string("LiuYuJi"));
    json_object_object_add(obj,"age",json_object_new_int(18));

    
    printf("%s\n",json_object_to_json_string(obj));
    
    //解析json
    //第一步 根据键名解析出对应的json对象
    json_object *json;
    json_object_object_get_ex(obj,"name",&json);
    //第二步 根据json对象类型转换成对应的数据
    //先获取json对象类型
    json_type type = json_object_get_type(json);
    if(type == json_type_string){
        printf("%s\n",json_object_get_string(json));
    }
    return 0;
}

