#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <map>

using json = nlohmann::json;
json coordinates = json::array();

void convert_single_response(json j_input) {
  auto legs = j_input["routes"][0]["legs"];
  size_t size = legs.size();

  for (int i = 0; i < size; i++){
    auto steps = legs[i]["steps"];
    size_t size_steps = steps.size();

    for (int j = 0; j < size_steps; j++){
      coordinates.push_back(steps[j]["intersections"][0]["location"]);
    }
  }
}

int main(int argc, char** argv){

  if(argc != 3){
    std::cout << "Usage: " << argv[0] << " route-response.json output.geojson\n";
    exit(0);
  }
      // read a JSON file
  std::ifstream input(argv[1]);

  json j_input;
  input >> j_input;

  if (j_input.is_array()) {
    for ( size_t i = 0; i < j_input.size(); ++i ){
      convert_single_response(j_input[i]);
    }
  } else {
    convert_single_response(j_input);
  }

  json json_output = {
      {"type", "Feature"},
      {"properties", {
        {"stroke", "#0000ff"},
        {"stroke-width", 2},
        {"stroke-opacity", 3}
      }},
      {"geometry", {
        {"type", "LineString"},
        {"coordinates", coordinates}
      }}
  };

  std::ofstream output(argv[2], std::ofstream::out);
  output << std::setw(4) << json_output;

}
