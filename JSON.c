#include <stdio.h>
#include <json.h>
int main()
{
    const  char* str = "{\"name\":\"LiuYuJi\",\"age\":18}";
    json_object *obj = json_tokener_parse(str);
    printf("%s\n",json_object_to_json_string(obj));
    printf("Hello world\n");
    return 0;
}

