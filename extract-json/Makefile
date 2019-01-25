all: sensors-extractor path-extractor path-extractor-stops combiner route-response-to-geojson nearest-to-geojson

sensors-extractor:  sensors-extractor.cpp curl-call.cpp
	g++ -Werror -Lcurl-7.63.0/lib/.libs sensors-extractor.cpp curl-call.cpp -lcurl -o sensors-extractor

path-extractor: path-extractor.cpp curl-call.cpp
	g++ -Werror -Lcurl-7.63.0/lib/.libs path-extractor.cpp curl-call.cpp -lcurl -o path-extractor

path-extractor-stops: path-extractor-stops.cpp curl-call.cpp
	g++ -Werror -Lcurl-7.63.0/lib/.libs path-extractor-stops.cpp curl-call.cpp -lcurl -o path-extractor-stops

route-response-to-geojson: route-response-to-geojson.cpp
	g++ -Werror route-response-to-geojson.cpp -o route-response-to-geojson

nearest-to-geojson: nearest-to-geojson.cpp
	g++ -Werror nearest-to-geojson.cpp -o nearest-to-geojson

combiner: combiner.cpp
	g++ -Werror combiner.cpp -o combiner

clean:
	rm sensors-extractor path-extractor combiner path-extractor-stops nearest-to-geojson route-response-to-geojson
