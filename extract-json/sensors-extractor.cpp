#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <map>
#include "curl-call.hpp"

using json = nlohmann::json;

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

PhantomNodes osrm_nearest_request(double lon, double lat){
    std::string url = localhost + "/nearest/v1/driving/" + std::to_string(lon) + "," + std::to_string(lat) + "?number=1";
    std::string data = curl_call(url);

    json j = json::parse(data);
    try{
      auto nodes = j.at("waypoints")[0].at("nodes");
      // std::cout << "Noeud renvoyé par nearest : " << nodes[0] << "," << nodes[1] << "\n\n";
      PhantomNodes node(nodes[0], nodes[1]);

      return node;
    }
    catch(std::exception e){
      std::cout << data << "\n\n";
    }
}

int main(int argc, char** argv){
    if(argc != 3){
      std::cout << "Usage: " << argv[0] << " input-file.geojson hostname\n";
      exit(0);
    }

    // read a JSON file
    std::ifstream input(argv[1]);

    localhost = argv[2];

    json j_input;
    input >> j_input;

    std::ofstream output("sensors_list.json", std::ofstream::out);
    json j_output = json::array();

    auto features = j_input["features"];
    size_t size = j_input["features"].size();

    for (size_t i = 0; i < size; i++){
      double lon = features[i]["geometry"]["coordinates"][0].get<double>();
      double lat = features[i]["geometry"]["coordinates"][1].get<double>();
      // std::cout << features[i]["properties"]["Text"] << " à la coordonnée " << features[i]["geometry"]["coordinates"][0] << "," << features[i]["geometry"]["coordinates"][1] << "\n";
      // std::cout << "curl 'http://router.project-osrm.org/nearest/v1/driving/" << features[i]["geometry"]["coordinates"][0] << "," << features[i]["geometry"]["coordinates"][1] << "?number=3&bearings=0,20' \n";

      PhantomNodes node = osrm_nearest_request(lon, lat);
      std::string name = features[i]["properties"]["Text"].get<std::string>();

      json nodes = json::array({(node.getSource()) , (node.getTarget())});
      j_output.push_back( { {"nodes", nodes}, {"name", name }} );
    }

    output << std::setw(4) << j_output;

}
