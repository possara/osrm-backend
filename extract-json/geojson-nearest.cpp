#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <map>

using json = nlohmann::json;
json coordinates = json::array();

json pointGeoJson(json coordinates, std::string color, std::string name) {
  json result = {
          {"type", "Feature"},
          {"properties", {
            {"marker-color", color},
            {"marker-size", "large"},
            {"Text", name}
          }},
          {"geometry", {
            {"type", "Point"},
            {"coordinates", coordinates}
          }}
        };
  return result;
}

json lineGeoJson(json coordinates) {
  json result = {
      {"type", "Feature"},
      {"properties", {}},
      {"geometry", {
        {"type", "LineString"},
        {"coordinates", coordinates}
      }}
    };
  return result;
}

int main(int argc, char** argv){

  if(argc != 3){
    std::cout << "Usage: " << argv[0] << " sensors_list.json sensors.geojson\n";
    exit(0);
  }
      // read a JSON file
  std::ifstream input(argv[1]);

  json j_input;
  input >> j_input;

  json features = json::array();

  size_t size = j_input.size();

  for (int i = 0; i < size; i++){
    auto sensor = j_input[i];
    features.push_back(pointGeoJson(sensor["coordinates"], "#ff0000", sensor["name"]));
    features.push_back(pointGeoJson(sensor["nearest_coordinates"], "#00ff00", sensor["name"]));
    features.push_back(lineGeoJson(json::array({sensor["coordinates"], sensor["nearest_coordinates"]})));
  }

  json geojson_output = { {"type", "FeatureCollection"}, {"features", features} };

  std::ofstream output(argv[2], std::ofstream::out);
  output << std::setw(4) << geojson_output;

}
