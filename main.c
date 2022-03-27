#include <stdio.h>
#include <curl/curl.h>

size_t write_data(void *buffer)
{
// printf ("%s",(char *)buffer);
}

int main(){

    CURL *hnd = curl_easy_init();
    char buf[10000];

    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(hnd, CURLOPT_URL, "https://community-open-weather-map.p.rapidapi.com/weather?q=Lyon%2Cfr&lat=0&lon=0&id=2172797&lang=null&units=imperial&mode=xml");

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "X-RapidAPI-Host: community-open-weather-map.p.rapidapi.com");
    headers = curl_slist_append(headers, "X-RapidAPI-Key: 1564dcd893mshf4a2056f73b2fccp13115bjsn4974baad00bd");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

     FILE * fp = fopen("test.html", "w");

    curl_easy_setopt(hnd,  CURLOPT_WRITEDATA, fp); 
	curl_easy_setopt(hnd,  CURLOPT_WRITEFUNCTION, write_data);

    CURLcode ret = curl_easy_perform(hnd);

        printf("Extraction in JSON done in test.html\n");

    // sleep(1);
    return 0;



}




