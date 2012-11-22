seq: simulate sokoban.h node_b.h simulate.h
	g++ sokoban_seq.cpp simulate.o board.o position.o -o sokoban_seq

simulate: simulate.h board
	g++ -c simulate.cpp

board: board.h node_p.h position
	g++ -c board.cpp

position: position.h
	g++ -c position.cpp

clean:
	rm -f *.o sokoban_seq
