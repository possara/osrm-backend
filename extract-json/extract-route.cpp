#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "curl/curl.h"

using json = nlohmann::json;

int main(int argc, char** argv){
	// read a JSON file
	std::ifstream iRoute(argv[1]);
	json jRoute;
	iRoute >> jRoute;

	std::ifstream iSensor("sensors_list.json");
	json jSensor;
	iSensor >> jSensor;

	auto legs = jRoute["routes"][0]["legs"];

	auto nodesSensor = jSensor;

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

	// return list
}
