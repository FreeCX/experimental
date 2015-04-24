CFLAGS=-Wall -std=c++11

debug:
	$(CXX) $(CFLAGS) -g database.cpp list.cpp -o list
release:
	$(CXX) $(CFLAGS) -O3 database.cpp list.cpp -o list
clean:
	$(RM) list
