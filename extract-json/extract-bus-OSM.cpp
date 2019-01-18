#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "curl/curl.h"

using json = nlohmann::json;

int main(int argc, char** argv){
	// read a JSON file
	std::ifstream iBus(argv[1]);
	json jBus;
	iBus >> jBus;

	auto stops = jBus["features"][0]["geometry"]["coordinates"];

	std::cout << "http://localhost:9966/?z=13&center=51.045819%2C2.338200&loc=";
	for (int i = 0; i < 1; i++){
		for (int j = 0; j < stops[i].size()-1; j+=25) {
			std::cout << "&loc=" << stops[i][j][1] << "%2C" << stops[i][j][0];
		}
	}
	std::cout << "&hl=en&alt=0\n";
}
