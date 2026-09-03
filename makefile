all:
	g++ main-Linux.cpp -o sokoban
run:
	./sokoban

clean:
	rm -f sokoban