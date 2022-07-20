#include "crow_all.h"
#include <curl/curl.h>
#include "json.hpp"
#include <time.h>
#include <chrono>
using json = nlohmann::json;


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


void fetchData(std::string &readBuffer, std::string url){
    CURL* curl = curl_easy_init();
    CURLcode res;
    curl = curl_easy_init();
    if(curl) {
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
  
      res = curl_easy_perform(curl);
      if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

      curl_easy_cleanup(curl);
    }
}

int main()
{
    crow::SimpleApp app;
    std::string url = "https://data.covid19.go.id/public/api/update.json";
    
    
    // General Cumulative Covid19 Cases in Indonesia

    CROW_ROUTE(app, "/")
    ([url]{
      std::string readBuffer;
      fetchData(readBuffer, url);
      json j = json::parse(readBuffer.c_str());
      json data = {
        {"total_positive", j["update"]["total"]["jumlah_positif"]},
        {"total_recovered", j["update"]["total"]["jumlah_sembuh"]}, 
        {"total_deaths", j["update"]["total"]["jumlah_meninggal"]},
        {"total_active", j["update"]["total"]["jumlah_dirawat"]},
        {"new_positive", j["update"]["penambahan"]["jumlah_positif"]},
        {"new_recovered", j["update"]["penambahan"]["jumlah_sembuh"]}, 
        {"new_deaths", j["update"]["penambahan"]["jumlah_meninggal"]},
        {"new_active", j["update"]["penambahan"]["jumlah_dirawat"]}
        };
      crow::json::wvalue resData;
      resData["ok"] = "true";
      resData["data"] = crow::json::load(data.dump());
      resData["message"] = "Success";
      return resData;
    });

    // Cumulative Covid19 Cases for each year in Indonesia
    CROW_ROUTE(app, "/yearly/<int>") // example : /yearly/2020 (Year 2020)
    ([url](int year){
      std::string readBuffer;
      fetchData(readBuffer, url);
      json j = json::parse(readBuffer.c_str());
      json data = {
        {"positive", 0},
        {"recovered", 0}, 
        {"deaths", 0},
        {"active", 0}
        };
      int totalDataFound = 0;
      for(auto a : j["update"]["harian"]){
        time_t timeInSeconds = (time_t)a["key"]/(time_t)1000; // Convert ms to seconds
        struct tm *tm = localtime(&timeInSeconds); 
        int dataYear = tm->tm_year + 1900; // tm_year = The numbers years since 1900
        if(year == dataYear){
          data["positive"] = (long)data["positive"] + (long)a["jumlah_positif"]["value"];
          data["recovered"] = (long)data["recovered"] + (long)a["jumlah_sembuh"]["value"];
          data["deaths"] = (long)data["deaths"] + (long)a["jumlah_meninggal"]["value"];
          data["active"] = (long)data["active"] + (long)a["jumlah_dirawat"]["value"];
          totalDataFound++;
        }
      }
      crow::json::wvalue resData;
      resData["ok"] = true;
      resData["data"] = crow::json::load(data.dump());
      resData["message"] = "Success";
      if(!totalDataFound){
        resData["ok"] = false;
        resData["message"] = "Data not found!";
      }
      return resData;
    });

    
    CROW_ROUTE(app, "/monthly/<int>/<int>") // example : /monthly/2022/02 (Feb 2022)
    ([url](int year, int month){
      std::string readBuffer;
      fetchData(readBuffer, url);
      json j = json::parse(readBuffer.c_str());
      json data = {
        {"positive", 0},
        {"recovered", 0}, 
        {"deaths", 0},
        {"active", 0}
        };
      int totalDataFound = 0;
      for(auto a : j["update"]["harian"]){
        time_t timeInSeconds = (time_t)a["key"]/(time_t)1000; // Convert ms to seconds
        struct tm *tm = localtime(&timeInSeconds); 
        int dataYear = tm->tm_year + 1900; // tm_year = The number of years since 1900
        int dataMonth = tm->tm_mon + 1;  // tm_year = The number of months since Jan 
        if(year == dataYear && month == dataMonth){
          data["positive"] = (long)data["positive"] + (long)a["jumlah_positif"]["value"];
          data["recovered"] = (long)data["recovered"] + (long)a["jumlah_sembuh"]["value"];
          data["deaths"] = (long)data["deaths"] + (long)a["jumlah_meninggal"]["value"];
          data["active"] = (long)data["active"] + (long)a["jumlah_dirawat"]["value"];
          totalDataFound++;
        }
      }
      crow::json::wvalue resData;
      resData["ok"] = true;
      resData["data"] = crow::json::load(data.dump());
      resData["message"] = "Success";
      if(!totalDataFound){
        resData["ok"] = false;
        resData["message"] = "Data not found!";
      }
      return resData;
    });


    CROW_ROUTE(app, "/daily/<int>/<int>/<int>") // example : /daily/2022/02/03 (03 Feb 2022)
    ([url](int year, int month, int day){
      std::string readBuffer;
      fetchData(readBuffer, url);
      json j = json::parse(readBuffer.c_str());
      json data = {
        {"positive", 0},
        {"recovered", 0}, 
        {"deaths", 0},
        {"active", 0}
        };
      int totalDataFound = 0;
      for(auto a : j["update"]["harian"]){
        time_t timeInSeconds = (time_t)a["key"]/(time_t)1000; // Convert ms to seconds
        struct tm *tm = localtime(&timeInSeconds); 
        int dataYear = tm->tm_year + 1900; // tm_year = The number of years since 1900
        int dataMonth = tm->tm_mon + 1;  // tm_year = The number of months since Jan 
        int dataDay = tm->tm_mday;  // tm_mday = The day of the month 
        if(year == dataYear && month == dataMonth && day == dataDay){
          data["positive"] = (long)data["positive"] + (long)a["jumlah_positif"]["value"];
          data["recovered"] = (long)data["recovered"] + (long)a["jumlah_sembuh"]["value"];
          data["deaths"] = (long)data["deaths"] + (long)a["jumlah_meninggal"]["value"];
          data["active"] = (long)data["active"] + (long)a["jumlah_dirawat"]["value"];
          totalDataFound++;
        }
      }
      crow::json::wvalue resData;
      resData["ok"] = true;
      resData["data"] = crow::json::load(data.dump());
      resData["message"] = "Success";
      if(!totalDataFound){
        resData["ok"] = false;
        resData["message"] = "Data not found!";
      }
      return resData;
    });

    app.port(3000).multithreaded().run();

}