


#ifndef Sudoku_h
#define Sudoku_h

#include "Tablero.h"
#include "Celda.h"
#include <fstream>

struct tPosicion {
	int fila;
	int columna;
};

struct tBloqueos {
	int cont;
	tPosicion bloqueadas[DIM * DIM];
};

struct tValor {
	bool posible;
	int celdas_que_afectan;
};

struct tValores {
	int dimension;
	tValor valores[DIM][DIM][DIM];
};

struct tSudoku {
	tTablero tab;
	int cont_numeros;
	tBloqueos celdas_bloqueadas;
	tValores valores_celda;
};

typedef int arrValores[DIM];

void inicializaSudoku(tSudoku& sudoku, int dim);
void carga_sudoku(std::ifstream& archivo, tSudoku& sudoku);

int dame_dimension(const tSudoku& sudoku);
tCelda dame_celda(const tSudoku& sudoku, int f, int c);
bool terminado(const tSudoku& sudoku);
bool bloqueo(const tSudoku& sudoku);
int dame_num_celdas_bloqueadas(const tSudoku& sudoku);
void dame_celda_bloqueada(const tSudoku& sudoku, int p, int& f, int& c);
bool es_valor_posible(const tSudoku& sudoku, int f, int c, int v);
int dame_num_celdas_libres(const tSudoku& sudoku);
void numero_posibles_valores(const tSudoku& sudoku, arrValores& av);

bool operator<(const tSudoku& s1, const tSudoku& s2);
bool operator==(const tSudoku& s1, const tSudoku& s2);

bool pon_valor(tSudoku& sudoku, int f, int c, int v);
bool quita_valor(tSudoku& sudoku, int f, int c);
void reset(tSudoku& sudoku);
void autocompleta(tSudoku& sudoku);

bool resolver(tSudoku& sudoku);

#endif
