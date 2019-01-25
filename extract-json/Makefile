all: sensors-extractor.cpp path-extractor.cpp combiner.cpp
	g++ -Werror -Lcurl-7.63.0/lib/.libs sensors-extractor.cpp curl-call.cpp -lcurl -o sensors-extractor
	g++ -Werror -Lcurl-7.63.0/lib/.libs path-extractor.cpp curl-call.cpp -lcurl -o path-extractor
	g++ -Werror -Lcurl-7.63.0/lib/.libs path-extractor-stops.cpp curl-call.cpp -lcurl -o path-extractor-stops
	g++ -Werror route-response-to-geojson.cpp -o route-response-to-geojson
	g++ -Werror nearest-to-geojson.cpp -o nearest-to-geojson
	g++ -Werror combiner.cpp -o combiner
clean:
	rm sensors-extractor path-extractor combiner path-extractor-stops geojson-nearest route-response-to-geojson bus-sensors.geojson sensors-list.json route-response.json
