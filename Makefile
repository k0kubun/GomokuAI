all: board.cc line.cc board.h test/board_test.cc test/line_test.cc
	g++ -c test/board_test.cc
	g++ -o test/board.test board_test.o gtest/gtest_main.o gtest/gtest-all.o
	rm board_test.o

	g++ -c test/line_test.cc
	g++ -o test/line.test line_test.o gtest/gtest_main.o gtest/gtest-all.o
	rm line_test.o

	./test/board.test
	./test/line.test

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
