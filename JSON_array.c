#include <stdio.h>
#include <json.h>
int main()
{
    json_object *obj = json_object_new_object();
    json_object_object_add(obj,"name",json_object_new_string("jack"));

    //创建数组对象
    json_object *array = json_object_new_array();
    json_object_array_add(array,json_object_new_int(100));
    json_object_array_add(array,json_object_new_int(90));
    json_object_array_add(array,json_object_new_int(80));
    //添加json对象
    json_object_object_add(obj,"score",array);


    printf("%s\n",json_object_to_json_string(obj));
    //获取json数组
    json_object *json;
    json_object_object_get_ex(obj,"score",&json);
    if(json_object_get_type(json) == json_type_array)
    {
        int size = json_object_array_length(json);
        for(int i = 0;i < size; ++i){
            json_object *j = json_object_array_get_idx(json,i);
            printf("%d\n",json_object_get_int(j));
        }
    }
    return 0;
}

