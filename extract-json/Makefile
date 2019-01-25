all: sensors-extractor.cpp path-extractor.cpp combiner.cpp
	g++ -Werror -Lcurl-7.63.0/lib/.libs sensors-extractor.cpp curl-call.cpp -lcurl -o sensors-extractor
	g++ -Werror -Lcurl-7.63.0/lib/.libs path-extractor.cpp curl-call.cpp -lcurl -o path-extractor
	g++ -Werror -Lcurl-7.63.0/lib/.libs path-extractor-stops.cpp curl-call.cpp -lcurl -o path-extractor-stops
	g++ -Werror json-to-geojson.cpp -o json-to-geojson
	g++ -Werror geojson-nearest.cpp -o geojson-nearest
	g++ -Werror combiner.cpp -o combiner
clean:
	rm sensors-extractor path-extractor combiner path-extractor-stops geojson-nearest json-to-geojson bus-sensors.geojson sensors-list.json route-response.json
