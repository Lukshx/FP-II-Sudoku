


#ifndef ListaSudokus_h
#define ListaSudokus_h
#include "Sudoku.h"

const int MAX_LISTA = 2;

struct tListaSudokus {
	int cont;
	int capacidad;
	tSudoku** sudokus;
};

void inicializa_lista_sudokus(tListaSudokus& lista);
void destruye_lista_sudokus(tListaSudokus& lista);

int dame_num_elems(const tListaSudokus& lista);
tSudoku dame_sudoku(const tListaSudokus& lista, int idx);

void insertar(tListaSudokus& lista, const tSudoku& sudoku);
void eliminar(tListaSudokus& lista, int pos);
void mostrar_lista_sudokus(const tListaSudokus& lista);

#endif
