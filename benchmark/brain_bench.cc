#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../brain.h"

clock_t start, end;

void start_time() {
  start = clock();
}

void end_time(const char *str) {
  end = clock();
  printf("%s: %.8f sec\n", str, (double)(end-start)/CLOCKS_PER_SEC);
}

Board board_with_id(int id) {
  Board load_board;
  FILE *fp;
	char load_string[256];
  char id_string[16];
  StoneType stone;

	if ((fp = fopen("./benchmark/board_data.txt", "r")) == NULL) {
		printf("file open error!!\n");
		exit(EXIT_FAILURE);
	}

  sprintf(id_string, "#%03d\n", id);
	while (fgets(load_string, 256, fp) != NULL) {
    if (strcmp(id_string, load_string) == 0) {
      //printf("%s", load_string);
      for (int i = 0; i < kBoardSize; i++) {
        fgets(load_string, 256, fp);
        for (int j = 0; j < kBoardSize; j++) {
          if (load_string[j] == 'x') {
            stone = kStoneBlack;
          } else if (load_string[j] == 'o') {
            stone = kStoneWhite;
          } else {
            stone = kStoneBlank;
          }
          load_board.set_stone(kBoardSize - j, i, stone);
        }
      }
    }
	}
  //Brain(kStoneBlack).PrintBoard(load_board);
	fclose(fp);
  return load_board;
}

void print_bench_start() {
  // Start Comment
  printf("\n==================================\n");
  printf("    GomokuAI Bench Mark Test\n");
  printf("==================================\n");
}

void print_bench_end() {
  printf("\n==================================\n\n");
}

void bench_heuristic() {
  Board board;
  Brain brain(kStoneBlack);

  printf("\n*** Heuristic ***\n");

  board = board_with_id(400);
  brain = Brain(kStoneBlack);
  
  start_time();
  brain.Heuristic(board);
  end_time("Heuristic");
}

void bench_search() {
  Board board;
  Brain brain(kStoneBlack);
  
  printf("\n*** Algorithm (Easy) ***\n");
  board = board_with_id(1);
  brain = Brain(kStoneBlack);

  start_time();
  brain.GetPutPoint(board);
  end_time("Original");
  
  start_time();
  brain.GetSearchPoint(board, 0, false);
  end_time("MiniMaxSearch(0)");

  start_time();
  brain.GetSearchPoint(board, 1, false);
  end_time("MiniMaxSearch(1)");

  start_time();
  brain.GetSearchPoint(board, 2, false);
  end_time("MiniMaxSearch(2)");

  start_time();
  brain.GetSearchPoint(board, 3, false);
  end_time("MiniMaxSearch(3)");

  // start_time();
  // brain.GetSearchPoint(board, 1, true);
  // end_time("AlphaBeta(1)");
  
  start_time();
  brain.GetSearchPoint(board, 2, true);
  end_time("AlphaBeta(2)");

  start_time();
  brain.GetSearchPoint(board, 3, true);
  end_time("AlphaBeta(3)");
  /*
  printf("\n*** Algorithm (Difficult) ***\n");
  board = board_with_id(400);
  brain = Brain(kStoneBlack);

  start_time();
  brain.GetPutPoint(board);
  end_time("Original");
  
  start_time();
  brain.GetSearchPoint(board, 0, false);
  end_time("MiniMaxSearch(0)");
  
  start_time();
  brain.GetSearchPoint(board, 1, false);
  end_time("MiniMaxSearch(1)");

  start_time();
  brain.GetSearchPoint(board, 1, true);
  end_time("AlphaBeta(1)");

  start_time();
  brain.GetSearchPoint(board, 2, true);
  end_time("AlphaBeta(2)");
  */
}

int main() {
  print_bench_start();
  
  bench_heuristic();
  bench_search();

  print_bench_end();
  return 0;
}
