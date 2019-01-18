#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "curl/curl.h"
#include <map>

using json = nlohmann::json;

json convert_single_response(json j_input) {

  json j_output = json::array();

  auto legs = j_input["routes"][0]["legs"];
  size_t size = legs.size();

  for (int i = 0; i < size; i++){

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

  return j_output;

  
}

int main(int argc, char** argv){


  if(argc != 3){
    std::cout << "Usage: " << argv[0] << " route-response.json json-to-geojson.geojson\n";
    exit(0);
  }
      // read a JSON file
  std::ifstream input(argv[1]);

  json j_input;
  input >> j_input;

  std::ofstream output(argv[2], std::ofstream::out);
  json j_output;      

  if (j_input.is_array()) {
    for ( size_t i = 0; i < j_input.size(); ++i ){

      j_output.push_back(convert_single_response(j_input[i]));
    }
  } else {
    j_output.push_back(convert_single_response(j_input));
  }

  json tmp = { {"type", "FeatureCollection"},
  {"name", "capteurs"},{"features", j_output} };

  output << std::setw(4) << tmp;

  

}
