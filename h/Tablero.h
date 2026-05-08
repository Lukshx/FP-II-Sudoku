


#ifndef Tablero_h
#define Tablero_h

#include "Celda.h"

const int DIM = 16;

struct tTablero {
	int dimension;
	tCelda matriz[DIM][DIM];
};


void inicializaTablero(tTablero& tab, int dim);


int dame_dimension(const tTablero& tab);
tCelda dame_elem(const tTablero& tab, int f, int c);


void pon_elemento(tTablero& tab, int f, int c, const tCelda& celda);

#endif