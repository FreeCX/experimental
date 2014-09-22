PROG=token
CFLAGS=-Wall -Ofast
RUSTFLAGS=--opt-level 3 --debuginfo 0 -Z no-landing-pads -Z lto
RUST=rustc
all:
	$(CC) -std=c11 $(CFLAGS) $(PROG).c -o $(PROG)_c
	$(CXX) -std=c++11 $(CFLAGS) $(PROG).cpp -o $(PROG)_c++
	$(RUST) $(RUSTFLAGS) $(PROG).rs -o $(PROG)_rust
clean:
	$(RM) $(PROG)_*