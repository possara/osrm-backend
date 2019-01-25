#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using json = nlohmann::json;
json features = json::array();

void special_nodes_check(json nodes_sensor, json node) {

	if ((nodes_sensor["nodes"][0] == 0 &&  nodes_sensor["nodes"][1] == node)) {

		std::cout << "Capteurs" << nodes_sensor["name"] << "\n";
	}
}

void combine_single_response(json single_response, json nodesSensor) {

	auto legs = single_response["routes"][0]["legs"];

	for (int i = 0; i < legs.size(); i++){
		auto nodes = legs[i]["annotation"]["nodes"];

		int k = (i == 0) ? 0 : 2;

		for(; k < nodes.size()-1; k++){
			// std::cout << "PhantomNodes " << nodes[k] << "-" << nodes[k+1] << "\n";

			for (int j = 0; j < nodesSensor.size(); j++){

				// std::cout << "Capteurs" << nodesSensor[j]["nodes"][0] << " - " << nodesSensor[j]["nodes"][1] << " - " <<  nodes[k] << " - "<<  nodes[k+1] << "\n";

				if (( nodesSensor[j]["nodes"][0] == nodes[k] && nodesSensor[j]["nodes"][1] == nodes[k+1])){
					std::cout << "Capteurs" << nodesSensor[j]["name"] << "\n";
					json coordinates = nodesSensor[j]["coordinates"];
					features.push_back({
						      {"type", "Feature"},
						      {"properties", {
						        {"marker-color", "#ff0000"},
						        {"marker-size", "large"},
										{"Text", nodesSensor[j]["name"]}
						      }},
						      {"geometry", {
						        {"type", "Point"},
						        {"coordinates", coordinates}
						      }}
						    });
				}

				special_nodes_check(nodesSensor[j], nodes[k+1]);

			}

			// node exists in file 1
			// insert in list
		}
	}
}


int main(int argc, char** argv){

	if(argc != 4){
      std::cout << "Usage: " << argv[0] << " route-response.json sensors-list.json output.geojson\n";
      exit(0);
    }
	// read a JSON file
	std::ifstream iRoute(argv[1]);
	json jRoute;
	iRoute >> jRoute;

	std::ifstream iSensor(argv[2]);
	json jSensor;
	iSensor >> jSensor;

	auto nodesSensor = jSensor;

	if (jRoute.is_array()) {
		for (size_t i = 0; i < jRoute.size(); ++i ){
			combine_single_response(jRoute[i], nodesSensor);
		}
	} else {
		combine_single_response(jRoute, nodesSensor);
	}

	json geojson_output = { {"type", "FeatureCollection"}, {"features", features} };
	std::ofstream output(argv[3], std::ofstream::out);
	output << std::setw(4) << geojson_output;

	// return list*/
}
