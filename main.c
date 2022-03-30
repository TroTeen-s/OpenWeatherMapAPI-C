#include <stdio.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <unistd.h>
#include <stdbool.h>

bool CallBackAPI();

int main(){

	char buffer[1024];
    
    FILE *fp;
    if(CallBackAPI()){
     
    struct json_object *parsed_json;

    struct json_object *name;
	struct json_object *age;
	struct json_object *friends;
	struct json_object *friend;

    fp = fopen("parsedData.json","r");
	fread(buffer, 1024, 1, fp);
	fclose(fp);

    parsed_json = json_tokener_parse(buffer);

    


    }
   



    return 0;



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
        return true;
    }

}




