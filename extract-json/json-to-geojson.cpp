#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "curl/curl.h"
#include <map>

using json = nlohmann::json;

std::string data; //will hold the url's contents
std::string localhost;

class PhantomNodes{
  private:
    unsigned int source;
    unsigned int target;
  public:
    PhantomNodes(){
    }
    PhantomNodes(int source, int target){
      this->source = source;
      this->target = target;
    }
    unsigned int getSource() const{
      return this->source;
    }
    unsigned int getTarget() const{
      return this->target;
    }
    bool operator<(const PhantomNodes &node) const{
      if(this->getSource() == node.getSource()){
        return this->getTarget() < node.getTarget();
      }
      else{
        return this->getSource() < node.getSource();
      }
    }
};

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

PhantomNodes curl_call(double lon, double lat){

    std::string url = localhost + "/nearest/v1/driving/" + std::to_string(lon) + "," + std::to_string(lat) + "?number=1";
    // std::cout << url << "\n";

    CURL* curl; //our curl object

    curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress

    curl_easy_perform(curl);

    // std::cout << data << "\n";

    json l = json::parse(data);

    try{
      auto nodes = l.at("waypoints")[0].at("nodes");
      // std::cout << "Noeud renvoyé par nearest : " << nodes[0] << "," << nodes[1] << "\n\n";
      PhantomNodes node(nodes[0], nodes[1]);

      data = "";

      curl_easy_cleanup(curl);
      curl_global_cleanup();

      return node;
    }
    catch(std::exception e){
      std::cout << data << "\n\n";
      data = "";
    }

}


int main(int argc, char** argv){
    // read a JSON file
    std::ifstream input(argv[1]);

    json j_input;
    input >> j_input;

    std::ofstream output("json-to-geojson.geojson", std::ofstream::out);
    json j_output = json::array();

    auto legs = j_input["routes"][0]["legs"];

    size_t size = legs.size();

    for (int i = 0; i < size; i++){
      // std::cout << features[i]["properties"]["Text"] << " à la coordonnée " << features[i]["geometry"]["coordinates"][0] << "," << features[i]["geometry"]["coordinates"][1] << "\n";
      // std::cout << "curl 'http://router.project-osrm.org/nearest/v1/driving/" << features[i]["geometry"]["coordinates"][0] << "," << features[i]["geometry"]["coordinates"][1] << "?number=3&bearings=0,20' \n";
      
      auto steps = legs[i]["steps"];
      size_t size_steps = steps.size();


      for (int j = 0; j < size_steps; j++){

        auto location = steps[j]["intersections"][0]["location"];

        j_output.push_back({ {"type", "Feature"},
                              { "properties", {
        {"Layer", "route"},
        {"SubClasses", NULL},
        {"ExtendedEntity", NULL},
        {"Linetype", NULL},
        {"EntityHandle", NULL}
       }}, {"geometry", { {"type", "Point"}, {"coordinates", location} } } });
      
      }

    }

    json tmp = { {"type", "FeatureCollection"},
                {"name", "capteurs"},
  {"crs", {
    {"type", "name"},
    {"properties", {
      {"name", "urn:ogc:def:crs:OGC:1.3:CRS84"}}}}},{"features", j_output} };

    output << std::setw(4) << tmp;

    // std::ifstream i("response-route.json");
    // json j;
    // i >> j;
    //
    // auto legs = j["routes"][0]["legs"];
    // size_t size = legs.size();
    // std::cout << "legs de taille " << size << "\n";
    //
    // for (int i = 0; i < size; i++){
    //   auto steps = legs[i]["steps"];
    //   size_t size_steps = steps.size();
    //   std::cout << "steps de taille " << size_steps << "\n";
    //   for(int j = 0; j < size_steps; j++){
    //     auto intersections = steps[j]["intersections"];
    //     size_t size_inter = intersections.size();
    //     std::cout << "intersections de taille " << size_inter << "\n";
    //     for(int k = 0; k < size_inter; k++){
    //       std::cout << intersections[k]["location"] << ",\n";
    //     }
    //   }
    // }
    //
    // std::cout << std::setw(4) << j << '\n';

}
