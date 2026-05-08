


#include "ListaSudokus.h"
#include "checkML.h"
#include <iostream>

using namespace std;


void redimensionar(tListaSudokus& lista);
int buscar_posicion_insercion(const tListaSudokus& lista, const tSudoku& sudoku);

void redimensionar(tListaSudokus& lista) {
	int nuevaCapacidad = lista.capacidad * 2;
	tSudoku** nuevaLista = new tSudoku * [nuevaCapacidad];

	for (int i = 0; i < lista.cont; ++i) {
		nuevaLista[i] = lista.sudokus[i];
	}

	for (int i = lista.cont; i < nuevaCapacidad; ++i) {
		nuevaLista[i] = nullptr;
	}

	delete[] lista.sudokus;
	lista.sudokus = nuevaLista;
	lista.capacidad = nuevaCapacidad;
}


int buscar_posicion_insercion(const tListaSudokus& lista, const tSudoku& sudoku) {
	int ini = 0;
	int fin = lista.cont;

	while (ini < fin) {
		int mitad = (ini + fin) / 2;

		if (sudoku < *lista.sudokus[mitad]) {
			fin = mitad;
		}
		else {
			ini = mitad + 1;
		}
	}

	return ini;
}




void inicializa_lista_sudokus(tListaSudokus& lista) {
	lista.cont = 0;
	lista.capacidad = MAX_LISTA;
	lista.sudokus = new tSudoku * [lista.capacidad];

	for (int i = 0; i < lista.capacidad; ++i) {
		lista.sudokus[i] = nullptr;
	}
}


void destruye_lista_sudokus(tListaSudokus& lista) {
	for (int i = 0; i < lista.cont; ++i) {
		delete lista.sudokus[i];
		lista.sudokus[i] = nullptr;
	}

	delete[] lista.sudokus;
	lista.sudokus = nullptr;
	lista.cont = 0;
	lista.capacidad = 0;
}

int dame_num_elems(const tListaSudokus& lista) {
	return lista.cont;
}

tSudoku dame_sudoku(const tListaSudokus& lista, int idx) {
	return *lista.sudokus[idx];
}



void insertar(tListaSudokus& lista, const tSudoku& sudoku) {
	if (lista.cont >= lista.capacidad) {
		redimensionar(lista);
	}

	int pos = buscar_posicion_insercion(lista, sudoku);

	for (int i = lista.cont; i > pos; i--) {
		lista.sudokus[i] = lista.sudokus[i - 1];
	}

	lista.sudokus[pos] = new tSudoku(sudoku);
	lista.cont++;
}


void eliminar(tListaSudokus& lista, int pos) {
	if (0 <= pos && pos < lista.cont) {
		delete lista.sudokus[pos];

		for (int i = pos; i < lista.cont - 1; ++i) {
			lista.sudokus[i] = lista.sudokus[i + 1];
		}

		--lista.cont;
		lista.sudokus[lista.cont] = nullptr;
	}
}


void mostrar_lista_sudokus(const tListaSudokus& lista) {
	for (int i = 0; i < lista.cont; ++i) {
		arrValores av;
		tSudoku sudoku = dame_sudoku(lista, i);
		numero_posibles_valores(sudoku, av);
		int dim = dame_dimension(sudoku);

		cout << i + 1 << ".- Sudoku con " << dame_num_celdas_libres(sudoku) << " casillas vacias" << endl;

		for (int j = 0; j < dim; ++j) {
			cout << "   Celdas con " << j + 1 << " valores posibles: " << av[j] << endl;
		}

		cout << endl;
	}
}
