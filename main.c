#include <stdio.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <mysql/mysql.h>

#define MAX 100

bool CallBackAPI(const char *City, const char *Format, const char *Language, const char *Units);
const char Configuration();
void json_parse();
void FetchDATA(struct json_object *city);
char *concatenate(const char *a, const char *b, const char *c);


int main(){
    
    Configuration();
   
    return 0;

}

const char Configuration(){

    FILE *fp;
    
    struct json_object *obj, *array, *array_obj, *obj_City, *obj_units, *obj_language, *obj_format;
    int arraylengh, i;
    const char * stock[10];
    static const char filename[] = "Configuration.json";
    obj = json_object_from_file(filename);
    array = json_object_object_get(obj, "configuration");

    json_object *new_obj = json_tokener_parse(json_object_get_string(array));

    obj_City = json_object_object_get(new_obj,"city");
    obj_units = json_object_object_get(new_obj,"units");
    obj_language = json_object_object_get(new_obj,"language");
    obj_format = json_object_object_get(new_obj, "format");

  const char *City = json_object_get_string(obj_City);
  const char *Units = json_object_get_string(obj_units);
  const char *Language = json_object_get_string(obj_language);
  const char *Format = json_object_get_string(obj_format);

//   printf("Name : %s\n",City);
//   printf("Id : %s\n",Format);

    if(CallBackAPI(City, Format, Language, Units)){

        FILE *fp = NULL;

        fp = fopen("DataFETCH.json","w+");
        fclose(fp);

        FetchDATA(obj_City);

    }
    

}

void FetchDATA(struct json_object *city){


      FILE *fp = NULL;

    struct json_object *obj,*obj_second, *array, *array_obj, *array_second, *array_three, *array_four;
    struct json_object *main_temp,*main_feels_like,*main_temp_min,*main_temp_max,*main_pressure,*main_humidity;
    struct json_object *localisation,
                        *weather_description,
                        *wind_speed,
                        *city_data,
                        *country_data;
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

    arraylengh = json_object_array_length(array_three);

    for(i=0;i < arraylengh; i++){

        array_obj = json_object_array_get_idx(array_three, i);
        weather_description = json_object_object_get(array_obj, "description");
        printf("Description : %s\n",json_object_get_string(weather_description));

    }



    array_four = json_object_object_get(obj, "sys");
    json_object *new_obj_four = json_tokener_parse(json_object_get_string(array_four));


        country_data = json_object_object_get(new_obj_four,"country");
        printf("Country : %s\n",json_object_get_string(country_data));
        printf("City : %s\n",json_object_get_string(city));


        const char *City = json_object_get_string(city);
        const char *Country = json_object_get_string(country_data);
        const char *temp_min = json_object_get_string(main_temp_min);
        const char *temp_max = json_object_get_string(main_temp_max);
        const char *temp = json_object_get_string(main_temp);
        const char *feels_like = json_object_get_string(main_feels_like);
        const char *description = json_object_get_string(weather_description);
        const char *wind = json_object_get_string(wind_speed);
        const char *pressure = json_object_get_string(main_pressure);
        const char *humidity = json_object_get_string(main_humidity);



// INSERT TO BDD

        /////////////////////////////

    MYSQL * mysql;
    MYSQL_RES * result = NULL;
    MYSQL_ROW row;

    char * Server = "51.254.117.247";
    char * Utilisateur = "matthias"; // yuki
    char * MotDePasse = "aulyma"; // azerty
    char * BaseDeDonnee = "EZ_TROT"; // projet
    char * requete = malloc(256);

   mysql = mysql_init(NULL);

   if (mysql == NULL) {
        printf("Error %u %s\n", mysql_errno(mysql), mysql_error(mysql));
        exit(1);
    }

 /*Connexion a la base de donnée*/

    if (!mysql_real_connect(mysql, Server, Utilisateur, MotDePasse, BaseDeDonnee, 3306, NULL, 0)) {
        printf("Connexion error : %s", mysql_error(mysql));
    } else {
         sprintf(requete, "insert into weather(description,temp,feels_like,temp_min,temp_max,pressure,humidity,city,country) Values('%s','%s','%s','%s','%s','%s','%s','%s','%s');", 
         description, temp, feels_like,temp_min,temp_max,pressure,humidity,City,Country );
        mysql_query(mysql, requete);
        mysql_close(mysql);    
        puts("insertion SQL DONE");
    }

    free(requete);

}


char *concatenate(const char *a, const char *b, const char *c) {
    size_t CharOne = strlen(a);
    size_t CharTwo = strlen(b);
    size_t CharThree = strlen(c);
    char *res = malloc(CharOne + CharTwo + CharThree + 1);
    if (res) {
        memcpy(res, a, CharOne);
        memcpy(res + CharOne, b, CharTwo);
        memcpy(res + CharOne + CharTwo, c, CharThree + 1);
    }
    return res;
}

bool CallBackAPI(const char *City, const char *Format, const char *Language, const char *Units){

    CURL *hnd = curl_easy_init();
    char buf[MAX];
    char buffer_units[MAX];
    char buffer_language[MAX];
    char buffer_format[MAX];

    char bu[MAX];
    char bu2[MAX];


    printf("City : %s\n",City);
    printf("Format : %s\n",Units);

    const char* str1 = "http://api.openweathermap.org/data/2.5/weather?q=";
    const char* str2 = "&APPID=dcdf6042d6b1009056865694d9f97846&lang=";
    const char* str3 = "&units=";

    strcat(strcpy(buf, str1), City);
    strcat(strcpy(buffer_language, str2), Language);
    strcat(strcpy(buffer_units,str3), Units);

    const char *result = concatenate(buf,buffer_language,buffer_units);
    printf("%s\n",result);

    curl_easy_setopt(hnd, CURLOPT_URL, result);

      const char* str4 = "Data.";

    strcat(strcpy(buffer_format, str4), Format);

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




