#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "curl/curl.h"

using json = nlohmann::json;

std::string data; //will hold the url's contents

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up)
{ //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer

    for (int c = 0; c<size*nmemb; c++)
    {
        data.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}

void curl_call(double lon, double lat){

    std::string url = "http://router.project-osrm.org/nearest/v1/driving/" + std::to_string(lon) + "," + std::to_string(lat) + "?number=1&bearings=0,20";
    // std::cout << url << "\n";

    CURL* curl; //our curl object

    curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress

    curl_easy_perform(curl);

    //std::cout << data << "\n";

    json l = json::parse(data);

    try{
      auto nodes = l.at("waypoints")[0].at("nodes");
      std::cout << "Noeud renvoyé par nearest : " << nodes << "\n\n";
    }
    catch(std::exception e){
      std::cout << data << "\n\n";
    }

    data = "";

    curl_easy_cleanup(curl);
    curl_global_cleanup();
}


int main(){
    // read a JSON file
    std::ifstream i("capteurs.geojson");
    json j;
    i >> j;


    auto features = j["features"];
    size_t size = j["features"].size();

    for (int i = 0; i < size; i++){
      std::cout << features[i]["properties"]["Text"] << " à la coordonnée " << features[i]["geometry"]["coordinates"][0] << "," << features[i]["geometry"]["coordinates"][1] << "\n";
      // std::cout << "curl 'http://router.project-osrm.org/nearest/v1/driving/" << features[i]["geometry"]["coordinates"][0] << "," << features[i]["geometry"]["coordinates"][1] << "?number=3&bearings=0,20' \n";

      curl_call(features[i]["geometry"]["coordinates"][0].get<double>(), features[i]["geometry"]["coordinates"][1].get<double>());


    }



    // std::cout << data << "\n";
    // std::cin.get();



}
