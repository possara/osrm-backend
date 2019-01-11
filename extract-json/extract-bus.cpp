#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "curl/curl.h"

using json = nlohmann::json;

int main(){
	// read a JSON file
	std::ifstream iBus("bus.json");
	json jBus;
	iBus >> jBus;

	auto stops = jBus["features"][0]["geometry"]["coordinates"];

	std::cout << "http://localhost:5000/route/v1/driving/";
	int nbCurrentLine = 0;
	for (int i = 0; i < stops.size(); i++){
		for (int j = 0; j < stops[i].size(); j++) {
			if (nbCurrentLine >= 499) {
				std::cout << ";" << stops[i][j][0] << "," << stops[i][j][1];
				std::cout << "?overview=full&steps=true&annotations=true\n\n\n";
				std::cout << "http://localhost:5000/route/v1/driving/";
				nbCurrentLine = 0;
			}
			if (nbCurrentLine > 0) {
				std::cout << ";";
			}
			std::cout << stops[i][j][0] << "," << stops[i][j][1];
			nbCurrentLine++;
		}
	}
	std::cout << "?overview=full&steps=true&annotations=true\n";
}
