all: sensors-extractor.cpp path-extractor.cpp combiner.cpp
	g++ -Werror -Lcurl-7.63.0/lib/.libs sensors-extractor.cpp curl-call.cpp -lcurl -o sensors-extractor
	g++ -Werror -Lcurl-7.63.0/lib/.libs path-extractor.cpp curl-call.cpp -lcurl -o path-extractor
	g++ -Werror combiner.cpp -o combiner
