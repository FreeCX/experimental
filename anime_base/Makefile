CFLAGS=-Wall -std=c++11

debug:
	$(CXX) $(CFLAGS) database.cpp list.cpp -o list
release:
	$(CXX) $(CFLAGS) -Ofast database.cpp list.cpp -o list
clean:
	$(RM) list
