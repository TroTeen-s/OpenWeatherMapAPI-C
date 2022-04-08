#include <stdio.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <mysql/mysql.h>

#define MAX 100

bool CallBackAPI(const char *City, const char *Format);
const char Configuration();
void json_parse();
void FetchDATA(struct json_object *city);


int main(){
    
    Configuration();
   
    return 0;

}

const char Configuration(){

    FILE *fp;
    
    struct json_object *obj, *array, *array_obj, *obj_City, *obj_format;
    int arraylengh, i;
    const char * stock[10];
    static const char filename[] = "Configuration.json";
    obj = json_object_from_file(filename);
    array = json_object_object_get(obj, "configuration");

    json_object *new_obj = json_tokener_parse(json_object_get_string(array));

    obj_City = json_object_object_get(new_obj,"city");
    obj_format = json_object_object_get(new_obj,"format");

  const char *City = json_object_get_string(obj_City);
  const char *Format = json_object_get_string(obj_format);

//   printf("Name : %s\n",City);
//   printf("Id : %s\n",Format);

    if(CallBackAPI(City, Format)){

        

        FILE *fp = NULL;

        fp = fopen("DataFETCH.json","w+");
        fclose(fp);

        FetchDATA(obj_City);

    }

}

void FetchDATA(struct json_object *city){


      FILE *fp = NULL;

    struct json_object *obj,*obj_second, *array, *array_obj, *array_second, *array_three;
    struct json_object *main_temp,*main_feels_like,*main_temp_min,*main_temp_max,*main_pressure,*main_humidity;
    struct json_object *localisation,
                        *weather_description,
                        *wind_speed,
                        *city_data;
    int arraylengh, i;
    const char * stock[10];
    static const char filename[] = "Data.json";


    obj = json_object_from_file(filename);
    array = json_object_object_get(obj, "main");
    json_object *new_obj = json_tokener_parse(json_object_get_string(array));

        main_temp = json_object_object_get(new_obj,"temp");
        main_feels_like = json_object_object_get(new_obj,"feels_like");
        main_temp_min = json_object_object_get(new_obj, "temp_min");
        main_temp_max = json_object_object_get(new_obj,"temp_max");
        main_pressure = json_object_object_get(new_obj,"pressure");
        main_humidity = json_object_object_get(new_obj,"humidity");

        printf("TEMP :%s\n",json_object_get_string(main_temp));


    obj_second = json_object_from_file(filename);
    array_second = json_object_object_get(obj_second, "wind");
    json_object *new_obj_second = json_tokener_parse(json_object_get_string(array_second));

        wind_speed = json_object_object_get(new_obj_second,"speed");
        printf("WIND : %s\n",json_object_get_string(wind_speed));


    array_three = json_object_object_get(obj, "weather");
    json_object *new_obj_three = json_tokener_parse(json_object_get_string(array_three));

        weather_description = json_object_object_get(new_obj_three,"description");
        printf("CITY : %s\n",json_object_get_string(city));


        /////////////////////////////

            // INSERT TO BDD

        /////////////////////////////


    MYSQL * mysql;
    MYSQL_RES * result = NULL;
    MYSQL_ROW row;

    char * Server = "51.254.117.247";
    char * Utilisateur = "esgi"; // yuki
    char * MotDePasse = "aulyma"; // azerty
    char * BaseDeDonnee = "EZ_TROT"; // projet
    char requete[300];

   mysql = mysql_init(NULL);

 /*Connexion a la base de donnée*/

    if (!mysql_real_connect(mysql, Server, Utilisateur, MotDePasse, BaseDeDonnee, 3306, NULL, 0)) {
        printf("Connexion error : %s", mysql_error(mysql));
    } else {
        puts("noooo");
    }

}

bool CallBackAPI(const char *City, const char *Format){

    CURL *hnd = curl_easy_init();
    char buf[MAX];
    char buffer[MAX];
    char buffer_final[MAX];
    char buffer_format[MAX];

    printf("City : %s\n",City);
    printf("Format : %s\n",Format);

    const char* str1 = "https://community-open-weather-map.p.rapidapi.com/weather?q=";
    const char* str2 = "%2Cfr&lat=0&lon=0&id=2172797&lang=fr&units=metric&mode=";

    strcat(strcpy(buf, str1), City);
    strcat(strcpy(buffer, str2), Format);
    strcat(strcpy(buffer_final, buf), buffer);

      printf("%s\n", buffer_final);

    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
    curl_easy_setopt(hnd, CURLOPT_URL, buffer_final);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "X-RapidAPI-Host: community-open-weather-map.p.rapidapi.com");
    headers = curl_slist_append(headers, "X-RapidAPI-Key: 1564dcd893mshf4a2056f73b2fccp13115bjsn4974baad00bd");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

      const char* str3 = "Data.";

    strcat(strcpy(buffer_format, str3), Format);

    FILE * fp = fopen(buffer_format, "w");

    curl_easy_setopt(hnd,  CURLOPT_WRITEDATA, fp);

    CURLcode ret = curl_easy_perform(hnd);

    fclose(fp);

    // Lets check if file is emptyconst 
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




