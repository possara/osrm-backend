#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <map>
#include "curl-call.hpp"

using json = nlohmann::json;

std::string localhost;

int main(int argc, char** argv){
    if(argc != 4){
      std::cout << "Usage: " << argv[0] << " input-file.geojson hostname output.json\n";
      exit(0);
    }

    // read a JSON file
    std::ifstream input(argv[1]);

    localhost = argv[2];

    json j_input;
    input >> j_input;

    std::ofstream output(argv[3], std::ofstream::out);
    json j_output = json::array();

    auto features = j_input["features"];
    size_t size = j_input["features"].size();

    for (size_t i = 0; i < size; i++){
      double lon = features[i]["geometry"]["coordinates"][0].get<double>();
      double lat = features[i]["geometry"]["coordinates"][1].get<double>();

      std::string url = localhost + "/nearest/v1/driving/" + std::to_string(lon) + "," + std::to_string(lat) + "?number=1";
      std::string data = curl_call(url);

      json j = json::parse(data);

      auto nodes = j.at("waypoints")[0].at("nodes");
      std::string name = features[i]["properties"]["Text"].get<std::string>();
      auto coordinates = json::array({lon, lat});
      auto nearest_coordinates = j.at("waypoints")[0].at("location");

      j_output.push_back( { {"nodes", nodes}, {"name", name }, {"coordinates", coordinates}, {"nearest_coordinates", nearest_coordinates} } );
    }
    output << std::setw(4) << j_output;
}
