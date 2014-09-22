PROG=token
CFLAGS=-Wall -Ofast
RUSTFLAGS=--opt-level 3 --debuginfo 0 -Z no-landing-pads -Z lto
RUST=rustc
all:
	$(CC) $(CFLAGS) $(PROG).c -o $(PROG)_c
	$(CXX) $(CFLAGS) $(PROG).cpp -o $(PROG)_c++
	$(RUST) $(RUSTFLAGS) $(PROG).rs -o $(PROG)_rust
clean:
	$(RM) $(PROG)_*