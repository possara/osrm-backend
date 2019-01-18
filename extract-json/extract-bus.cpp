#include "nlohmann/json.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "curl-call.hpp"


using json = nlohmann::json;
std::string hostname;


int main(int argc, char** argv){
	if(argc != 3){
		std::cout << "Usage: " << argv[0] << " input-file.json hostname\n";
		exit(0);
	}

	// read a JSON file
	std::ifstream iBus(argv[1]);

	hostname = argv[2];

	json jBus;
	iBus >> jBus;
	json j_output = json::array();

	std::ofstream output("route-response.json", std::ofstream::out);

	auto stops = jBus["features"][0]["geometry"]["coordinates"];

	int nbCurrentLine = 0;

	std::string url = hostname + "/route/v1/driving/";
	for (int i = 0; i < stops.size(); i++){

		for (int j = 0; j < stops[i].size(); j++) {
			double lon = stops[i][j][0].get<double>();
			double lat = stops[i][j][1].get<double>();
			if (nbCurrentLine >= 499) {
				url += ";" + std::to_string(lon) + "," + std::to_string(lat) + "?overview=full&steps=true&annotations=true";

				std::string data = curl_call(url);
		    json j = json::parse(data);

				j_output.push_back(j);

				url = hostname + "/route/v1/driving/";
				nbCurrentLine = 0;
			}
			if (nbCurrentLine > 0) {
				url += ";";
			}
			url += std::to_string(lon) + "," + std::to_string(lat);
			nbCurrentLine++;
		}

	}
	url += "?overview=full&steps=true&annotations=true";

	std::string data = curl_call(url);
	json j = json::parse(data);

	j_output.push_back(j);
	output << std::setw(4) << j_output;
}
