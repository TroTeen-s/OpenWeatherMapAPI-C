#include <stdio.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <unistd.h>
#include <stdbool.h>

bool CallBackAPI();
const char * Configuration();
void json_parse();


int main(){

	char buffer[1024];
    json_parse();

    // const char * arrayDATA = Configuration();
    // printf("main=%s\n", arrayDATA);

    // int arrayDATALENGTH = json_object_array_length(arrayDATA);
    
    FILE *fp;
    if(CallBackAPI()){
     
        struct json_object *obj, *array, *array_obj, *array_obj_name;
        int arraylen, i;
        char name[10] = {0};
        static const char filename[] = "parsedData.json";
        obj = json_object_from_file(filename);
        array = json_object_object_get(obj, "weather");

        arraylen = json_object_array_length(array);

        for (i = 0; i < arraylen; i++) {
     
        array_obj = json_object_array_get_idx(array, i);
  
        array_obj_name = json_object_object_get(array_obj, "main");
    
        printf("main=%s\n", json_object_get_string(array_obj_name));

        }
    
    }
   
    return 0;

}


void json_parse() {

    const char * arrayDATA = Configuration();

    json_object * jobj = json_tokener_parse(arrayDATA);

  enum json_type type;
  json_object_object_foreach(jobj, key, val) {
    type = json_object_get_type(val);
    switch (type) {
      case json_type_array: printf("type: json_type_array, ");
                          jobj = json_object_object_get(jobj, key);

                          int arraylen = json_object_array_length(jobj);
                          printf("Array Length: %dn",arraylen);
                          int i;

                          json_object * jvalue;

                          for (i=0; i< arraylen; i++){
                            jvalue = json_object_array_get_idx(jobj, i);
                            printf("value[%d]: %sn",i, json_object_get_string(jvalue));
                          }

                          break;
    }
  }
}




const char * Configuration(){

    FILE *fp;
    
    struct json_object *obj, *array, *array_obj, *array_obj_name;
     int arraylen, i;
    static const char filename[] = "Configuration.json";
    obj = json_object_from_file(filename);
    array = json_object_object_get(obj, "configuration");

    arraylen = json_object_array_length(array);

    // printf("Configuration=%s\n", json_object_get_string(array));

    for (i = 0; i < arraylen; i++) {
     
        array_obj = json_object_array_get_idx(array, i);
    
        printf("main=%s\n", json_object_get_string(array_obj));

    }




    return json_object_get_string(array);

}

bool CallBackAPI(){

    CURL *hnd = curl_easy_init();
    char buf[10000];

    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(hnd, CURLOPT_URL, "https://community-open-weather-map.p.rapidapi.com/weather?q=lyon%2Cfr&lat=0&lon=0&id=2172797&lang=fr&units=metric&mode=JSON");

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "X-RapidAPI-Host: community-open-weather-map.p.rapidapi.com");
    headers = curl_slist_append(headers, "X-RapidAPI-Key: 1564dcd893mshf4a2056f73b2fccp13115bjsn4974baad00bd");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

    FILE * fp = fopen("parsedData.json", "w");

    curl_easy_setopt(hnd,  CURLOPT_WRITEDATA, fp);

    CURLcode ret = curl_easy_perform(hnd);

    fclose(fp);

    // Lets check if file is empty
    // SEEK_END searches the last byte of the file.

    fseek (fp, 0, SEEK_END);
    int size = ftell(fp);

    if (0 == size) {
        printf("file is empty\n");
        return false;
    }else{
        printf("Insertion done\n");
        return true;
    }



}





