#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "curl/curl.h"

using json = nlohmann::json;

int main(){
	// read a JSON file
	std::ifstream i("response.json");
	json j;
	i >> j;

	auto legs = j["routes"][0]["legs"];

	for (int i = 0; i < legs.size(); i++){
		auto nodes = legs[i]["annotation"]["nodes"];

		int k = (i == 0) ? 0 : 2;

		for(; k < nodes.size()-1; k++){
			std::cout << "PhantomNodes " << nodes[k] << "-" << nodes[k+1] << "\n";
			// node exists in file 1
			// insert in list
		}
	}

	// return list
}
