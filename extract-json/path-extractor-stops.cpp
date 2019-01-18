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

	std::ofstream output("response-bus.json", std::ofstream::out);

	auto way_bus = jBus["features"][0]["geometry"]["coordinates"][0];
	auto features = jBus["features"];

	int nbCurrentLine = 0;
	std::string url = hostname + "/route/v1/driving/";

	for (int i = 0; i < way_bus.size(); i++) {

		bool is_stop = false;

		double lon = way_bus[i][0].get<double>();
		double lat = way_bus[i][1].get<double>();

		for (int j = 1; j < features.size(); j++) {
			auto coordsStop = features[j]["geometry"]["coordinates"];
			if (coordsStop[0].get<double>() == lon && coordsStop[1].get<double>() == lat) {
				is_stop = true;
				break;
			}
		}

		if (!is_stop) {
			continue;
		}

		if (nbCurrentLine >= 499) {
			url += ";" + std::to_string(lon) + "," + std::to_string(lat) + "?overview=full&steps=true&annotations=true";

			j_output.push_back(json::parse(curl_call(url)));

			url = hostname + "/route/v1/driving/";
			nbCurrentLine = 0;
		}
		if (nbCurrentLine > 0) {
			url += ";";
		}
		url += std::to_string(lon) + "," + std::to_string(lat);
		nbCurrentLine++;
	}
	url += "?overview=full&steps=true&annotations=true";

	j_output.push_back(json::parse(curl_call(url)));

	output << std::setw(4) << j_output;
}
