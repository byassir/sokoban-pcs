make: omp threads seq

omp: simulate sokoban.h node_b.h
	g++ -std=c++0x -fopenmp sokoban_omp.cpp simulate.o board.o position.o -o sokoban_omp

threads: simulate sokoban.h node_b.h
	g++ -std=c++0x -pthread sokoban_threads.cpp simulate.o board.o position.o -o sokoban_threads

seq: simulate sokoban.h node_b.h
	g++ -std=c++0x sokoban_seq.cpp simulate.o board.o position.o -o sokoban_seq

simulate: simulate.h board
	g++ -std=c++0x -c simulate.cpp

board: board.h node_p.h position
	g++ -std=c++0x -c board.cpp

position: position.h
	g++ -std=c++0x -c position.cpp

clean:
	rm -f *.o sokoban_seq sokoban_threads sokoban_omp
