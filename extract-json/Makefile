all: sensors-extractor path-extractor path-extractor-stops combiner json-to-geojson geojson-nearest

sensors-extractor:  sensors-extractor.cpp curl-call.cpp
	g++ -Werror -Lcurl-7.63.0/lib/.libs sensors-extractor.cpp curl-call.cpp -lcurl -o sensors-extractor

path-extractor: path-extractor.cpp curl-call.cpp 
	g++ -Werror -Lcurl-7.63.0/lib/.libs path-extractor.cpp curl-call.cpp -lcurl -o path-extractor

path-extractor-stops: path-extractor-stops.cpp curl-call.cpp
	g++ -Werror -Lcurl-7.63.0/lib/.libs path-extractor-stops.cpp curl-call.cpp -lcurl -o path-extractor-stops

json-to-geojson: json-to-geojson.cpp
	g++ -Werror json-to-geojson.cpp -o json-to-geojson

geojson-nearest: geojson-nearest.cpp
	g++ -Werror geojson-nearest.cpp -o geojson-nearest

combiner: combiner.cpp
	g++ -Werror combiner.cpp -o combiner

clean:
	rm sensors-extractor path-extractor combiner path-extractor-stops geojson-nearest json-to-geojson 