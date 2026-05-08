


#include "Tablero.h"




void inicializaTablero(tTablero& tab, int dim) {
	for (int i = 0; i < dim; ++i) {
		for (int j = 0; j < dim; ++j) {
			inicializaCelda(tab.matriz[i][j]);
		}
	}
	tab.dimension = dim;
}


int dame_dimension(const tTablero& tab) {
	return tab.dimension;
}

tCelda dame_elem(const tTablero& tab, int f, int c) {
	return tab.matriz[f][c];
}




void pon_elemento(tTablero& tab, int f, int c, const tCelda& celda) {
	tab.matriz[f][c] = celda;
}