all: board.cc line.cc board.h test/board_test.cc test/line_test.cc
	g++ -c test/board_test.cc
	g++ -o test/board.test board_test.o gtest/gtest_main.o gtest/gtest-all.o
	rm board_test.o

	g++ -c test/line_test.cc
	g++ -o test/line.test line_test.o gtest/gtest_main.o gtest/gtest-all.o
	rm line_test.o

	g++ -c test/brain_test.cc
	g++ -o test/brain.test brain_test.o gtest/gtest_main.o gtest/gtest-all.o
	rm brain_test.o

	./test/board.test
	./test/line.test
	./test/brain.test	

board: board.cc board.h test/board_test.cc
	g++ -c test/board_test.cc
	g++ -o test/board.test board_test.o gtest/gtest_main.o gtest/gtest-all.o
	rm board_test.o
	./test/board.test

line: line.cc board.h test/line_test.cc
	g++ -c test/line_test.cc
	g++ -o test/line.test line_test.o gtest/gtest_main.o gtest/gtest-all.o
	rm line_test.o
	./test/line.test

brain: brain.cc brain.h test/brain_test.cc
	g++ -c test/brain_test.cc
	g++ -o test/brain.test brain_test.o gtest/gtest_main.o gtest/gtest-all.o
	rm brain_test.o
	./test/brain.test

setup: gtest/gtest-all.cc gtest/gtest_main.cc
	g++ -I./ -c gtest/gtest-all.cc
	g++ -I./ -c gtest/gtest_main.cc
	mv gtest*.o gtest

gomocup: gomocup.cc gomocup.h board.cc line.cc board.h brain.cc brain.h
	g++ -c board.cc
	g++ -c line.cc
	g++ -c gomocup.cc
	g++ -c brain.cc
	g++ -o gomocup board.o line.o gomocup.o brain.o
	rm board.o
	rm line.o
	rm brain.o
	rm gomocup.o
