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

	std::cout << "http://localhost:9966/?z=13&center=51.045819%2C2.338200&loc=";
	int nbCurrentLine = 0;
	for (int i = 0; i < stops.size(); i++){
		for (int j = 1; j < stops[i].size()-1; j+=25) {
			if (nbCurrentLine >= 5000) {
				std::cout << "&loc=" << stops[i][j][1] << "%2C" << stops[i][j][0];
				std::cout << "?overview=full&steps=true&annotations=true\n\n\n";
				std::cout << "http://localhost:5000/route/v1/driving/";
				nbCurrentLine = 0;
			}
			if (nbCurrentLine > 0) {
				std::cout << "&loc=";
			}
			std::cout << stops[i][j][1] << "%2C" << stops[i][j][0];
			nbCurrentLine++;
		}
	}
	std::cout << "&hl=en&alt=0\n";
}
