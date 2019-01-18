#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "curl/curl.h"

using json = nlohmann::json;

void combine_single_response(json single_response, json nodesSensor ) {

	auto legs = single_response["routes"][0]["legs"];

	for (int i = 0; i < legs.size(); i++){
		auto nodes = legs[i]["annotation"]["nodes"];

		int k = (i == 0) ? 0 : 2;

		for(; k < nodes.size()-1; k++){
			// std::cout << "PhantomNodes " << nodes[k] << "-" << nodes[k+1] << "\n";

			for (int j = 0; j < nodesSensor.size(); j++){

				//std::cout << "Capteurs" << nodesSensor[j]["nodes"][0] << " - " << nodesSensor[j]["nodes"][1] << " - " <<  nodes[k] << " - "<<  nodes[k+1] << "\n";

				if (( nodesSensor[j]["nodes"][0] == nodes[k] && nodesSensor[j]["nodes"][1] == nodes[k+1])){

					std::cout << "Capteurs" << nodesSensor[j]["name"] << "\n";
				}

			}

			// node exists in file 1
			// insert in list
		}
	}
}


int main(int argc, char** argv){

	if(argc != 3){
      std::cout << "Usage: " << argv[0] << " route-response.json sensors-list.json\n";
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


	// return list
}
