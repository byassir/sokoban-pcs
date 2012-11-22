seq: board sokoban_seq.h
	g++ sokoban_seq.cpp board.o position.o -o sokoban_seq

board: board.h node.h position
	g++ -c board.cpp

position: position.h
	g++ -c position.cpp

clean:
	rm -f *.o sokoban_seq
