seq: simulate sokoban.h node_b.h simulate.h
	g++ -std=c++0x sokoban_seq.cpp simulate.o board.o position.o -o sokoban_seq

simulate: simulate.h board
	g++ -std=c++0x -c simulate.cpp

board: board.h node_p.h position
	g++ -std=c++0x -c board.cpp

position: position.h
	g++ -std=c++0x -c position.cpp

clean:
	rm -f *.o sokoban_seq
