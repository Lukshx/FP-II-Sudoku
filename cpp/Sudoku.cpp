


#include "Sudoku.h"
#include <iostream>

using namespace std;

int raiz_cuadrada(int n);
bool pos_en_tablero(const tSudoku& s, int f, int c);
bool valor_valido(const tSudoku& s, int v);
bool misma_subcuadricula(int f1, int c1, int f2, int c2, int raiz);
bool celda_afectada(int f, int c, int filaValor, int colValor, int raiz);
void modifica_celdas_afectadas(tSudoku& s, int f, int c, int v, int cambio);
void annade_celdas_afectadas(tSudoku& s, int f, int c, int v);
void elimina_celdas_afectadas(tSudoku& s, int f, int c, int v);
bool tiene_algun_valor_posible(const tSudoku& s, int f, int c);
void guardar_celda_bloqueada(tSudoku& s, int f, int c);
void incorpora_celdas_bloqueadas(tSudoku& s, int f, int c);
void quita_celdas_bloqueadas(tSudoku& s);
bool autocompletable(const tSudoku& s, int f, int c, int& valor_autocompletar);
void coloca_original(tSudoku& s, int f, int c, int valor);

bool resolver(tSudoku& sudoku);
bool busca_celda_vacia(const tSudoku& sudoku, int& fila, int& columna);
bool resolver_backtracking(tSudoku& sudoku);


int raiz_cuadrada(int n) {
	int raiz = 1;

	while (raiz * raiz < n) {
		++raiz;
	}
	return raiz;
}


bool pos_en_tablero(const tSudoku& s, int f, int c) {
	int dim = dame_dimension(s);
	return 0 <= f && f < dim && 0 <= c && c < dim;
}


bool valor_valido(const tSudoku& s, int v) {
	return 1 <= v && v <= dame_dimension(s);
}


bool misma_subcuadricula(int f1, int c1, int f2, int c2, int raiz) {
	return f1 / raiz == f2 / raiz && c1 / raiz == c2 / raiz;
}


bool celda_afectada(int f, int c, int filaValor, int colValor, int raiz) {
	return f == filaValor || c == colValor || misma_subcuadricula(f, c, filaValor, colValor, raiz);
}


void modifica_celdas_afectadas(tSudoku& s, int f, int c, int v, int cambio) {
	int dim = dame_dimension(s);
	int raiz = raiz_cuadrada(dim);
	int idx = v - 1;

	for (int i = 0; i < dim; ++i) {
		for (int j = 0; j < dim; ++j) {
			if (celda_afectada(i, j, f, c, raiz)) {
				s.valores_celda.valores[i][j][idx].celdas_que_afectan += cambio;

				if (s.valores_celda.valores[i][j][idx].celdas_que_afectan <= 0) {
					s.valores_celda.valores[i][j][idx].celdas_que_afectan = 0;
					s.valores_celda.valores[i][j][idx].posible = true;
				}
				else {
					s.valores_celda.valores[i][j][idx].posible = false;
				}
			}
		}
	}
}

void annade_celdas_afectadas(tSudoku& s, int f, int c, int v) {
	modifica_celdas_afectadas(s, f, c, v, 1);
}

void elimina_celdas_afectadas(tSudoku& s, int f, int c, int v) {
	modifica_celdas_afectadas(s, f, c, v, -1);
}


bool es_valor_posible(const tSudoku& s, int f, int c, int v) {
	bool posible = false;

	if (pos_en_tablero(s, f, c) && valor_valido(s, v)) {
		posible = s.valores_celda.valores[f][c][v - 1].posible;
	}

	return posible;
}


bool tiene_algun_valor_posible(const tSudoku& s, int f, int c) {
	bool encontrado = false;
	int valor = 1;
	int dim = dame_dimension(s);

	while (valor <= dim && !encontrado) {
		if (es_valor_posible(s, f, c, valor)) {
			encontrado = true;
		}
		++valor;
	}

	return encontrado;
}


void guardar_celda_bloqueada(tSudoku& s, int f, int c) {
	if (!tiene_algun_valor_posible(s, f, c)) {
		bool encontrada = false;
		int i = 0;

		while (i < s.celdas_bloqueadas.cont && !encontrada) {
			if (s.celdas_bloqueadas.bloqueadas[i].fila == f &&
				s.celdas_bloqueadas.bloqueadas[i].columna == c) {
				encontrada = true;
			}
			++i;
		}

		if (!encontrada) {
			s.celdas_bloqueadas.bloqueadas[s.celdas_bloqueadas.cont].fila = f;
			s.celdas_bloqueadas.bloqueadas[s.celdas_bloqueadas.cont].columna = c;
			++s.celdas_bloqueadas.cont;
		}
	}
}


void incorpora_celdas_bloqueadas(tSudoku& s, int f, int c) {
	int dim = dame_dimension(s);
	int raiz = raiz_cuadrada(dim);

	for (int i = 0; i < dim; ++i) {
		for (int j = 0; j < dim; ++j) {
			if (celda_afectada(i, j, f, c, raiz) && es_vacia(dame_celda(s, i, j))) {
				guardar_celda_bloqueada(s, i, j);
			}
		}
	}
}



void quita_celdas_bloqueadas(tSudoku& s) {
	int i = 0;

	while (i < s.celdas_bloqueadas.cont) {
		int f = s.celdas_bloqueadas.bloqueadas[i].fila;
		int c = s.celdas_bloqueadas.bloqueadas[i].columna;

		if (tiene_algun_valor_posible(s, f, c)) {
			s.celdas_bloqueadas.bloqueadas[i] = s.celdas_bloqueadas.bloqueadas[s.celdas_bloqueadas.cont - 1];
			--s.celdas_bloqueadas.cont;
		}
		else {
			++i;
		}
	}
}


bool autocompletable(const tSudoku& s, int f, int c, int& valor_autocompletar) {
	int cont = 0;
	int valor = 1;
	int dim = dame_dimension(s);

	while (valor <= dim && cont < 2) {
		if (es_valor_posible(s, f, c, valor)) {
			++cont;
			if (cont == 1) {
				valor_autocompletar = valor;
			}
		}
		++valor;
	}

	return cont == 1;
}


void coloca_original(tSudoku& s, int f, int c, int valor) {
	tCelda celda;

	if (valor == 0) {
		inicializaCelda(celda);
	}
	else {
		inicializaCelda(celda, valor, ORIGINAL);
		++s.cont_numeros;
		annade_celdas_afectadas(s, f, c, valor);
	}

	pon_elemento(s.tab, f, c, celda);
}


void inicializaSudoku(tSudoku& sudoku, int dim) {
	inicializaTablero(sudoku.tab, dim);
	sudoku.cont_numeros = 0;
	sudoku.celdas_bloqueadas.cont = 0;
	sudoku.valores_celda.dimension = dim;

	for (int i = 0; i < dim; ++i) {
		for (int j = 0; j < dim; ++j) {
			for (int k = 0; k < dim; ++k) {
				sudoku.valores_celda.valores[i][j][k].posible = true;
				sudoku.valores_celda.valores[i][j][k].celdas_que_afectan = 0;
			}
		}
	}
}


void carga_sudoku(ifstream& archivo, tSudoku& sudoku) {
	int dimension;
	archivo >> dimension;

	inicializaSudoku(sudoku, dimension);

	for (int i = 0; i < dimension; ++i) {
		for (int j = 0; j < dimension; ++j) {
			int valor;
			archivo >> valor;
			coloca_original(sudoku, i, j, valor);
		}
	}
}

int dame_dimension(const tSudoku& sudoku) {
	return dame_dimension(sudoku.tab);
}

tCelda dame_celda(const tSudoku& sudoku, int f, int c) {
	return dame_elem(sudoku.tab, f, c);
}



bool pon_valor(tSudoku& sudoku, int f, int c, int v) {
	bool ok = false;

	if (pos_en_tablero(sudoku, f, c) && valor_valido(sudoku, v) &&
		es_vacia(dame_celda(sudoku, f, c)) && es_valor_posible(sudoku, f, c, v)) {
		tCelda celda;
		inicializaCelda(celda, v, OCUPADA);
		pon_elemento(sudoku.tab, f, c, celda);
		++sudoku.cont_numeros;
		annade_celdas_afectadas(sudoku, f, c, v);
		incorpora_celdas_bloqueadas(sudoku, f, c);
		ok = true;
	}

	return ok;
}


bool quita_valor(tSudoku& sudoku, int f, int c) {
	bool ok = false;

	if (pos_en_tablero(sudoku, f, c)) {
		tCelda celda = dame_celda(sudoku, f, c);

		if (!es_vacia(celda) && !es_original(celda)) {
			int valor = dame_valor(celda);
			inicializaCelda(celda);
			pon_elemento(sudoku.tab, f, c, celda);
			--sudoku.cont_numeros;
			elimina_celdas_afectadas(sudoku, f, c, valor);
			quita_celdas_bloqueadas(sudoku);
			ok = true;
		}
	}

	return ok;
}


void reset(tSudoku& sudoku) {
	int dim = dame_dimension(sudoku);

	for (int f = 0; f < dim; ++f) {
		for (int c = 0; c < dim; ++c) {
			tCelda celda = dame_celda(sudoku, f, c);

			if (es_ocupada(celda)) {
				quita_valor(sudoku, f, c);
			}
		}
	}

	sudoku.celdas_bloqueadas.cont = 0;
}


void autocompleta(tSudoku& sudoku) {
	int dim = dame_dimension(sudoku);

	for (int i = 0; i < dim; ++i) {
		for (int j = 0; j < dim; ++j) {
			int valor_autocompletar = 0;

			if (es_vacia(dame_celda(sudoku, i, j)) && autocompletable(sudoku, i, j, valor_autocompletar)) {
				pon_valor(sudoku, i, j, valor_autocompletar);
			}
		}
	}
}

bool terminado(const tSudoku& sudoku) {
	int dim = dame_dimension(sudoku);
	return sudoku.cont_numeros == dim * dim;
}

bool bloqueo(const tSudoku& sudoku) {
	return sudoku.celdas_bloqueadas.cont > 0;
}

int dame_num_celdas_bloqueadas(const tSudoku& sudoku) {
	return sudoku.celdas_bloqueadas.cont;
}

void dame_celda_bloqueada(const tSudoku& sudoku, int p, int& f, int& c) {
	f = sudoku.celdas_bloqueadas.bloqueadas[p].fila;
	c = sudoku.celdas_bloqueadas.bloqueadas[p].columna;
}

int dame_num_celdas_libres(const tSudoku& sudoku) {
	int dim = dame_dimension(sudoku);
	return dim * dim - sudoku.cont_numeros;
}


void numero_posibles_valores(const tSudoku& sudoku, arrValores& av) {
	int dim = dame_dimension(sudoku);

	for (int i = 0; i < dim; ++i) {
		av[i] = 0;
	}

	for (int f = 0; f < dim; ++f) {
		for (int c = 0; c < dim; ++c) {
			if (es_vacia(dame_celda(sudoku, f, c))) {
				int posibles = 0;

				for (int v = 1; v <= dim; ++v) {
					if (es_valor_posible(sudoku, f, c, v)) {
						++posibles;
					}
				}

				if (posibles > 0) {
					++av[posibles - 1];
				}
			}
		}
	}
}

bool operator<(const tSudoku& s1, const tSudoku& s2) {
	bool menor = false;
	int libres1 = dame_num_celdas_libres(s1);
	int libres2 = dame_num_celdas_libres(s2);

	if (libres1 < libres2) {
		menor = true;
	}
	else if (libres1 == libres2) {
		arrValores av1;
		arrValores av2;
		numero_posibles_valores(s1, av1);
		numero_posibles_valores(s2, av2);

		int dim = dame_dimension(s1);
		int i = 0;
		bool decidido = false;

		while (i < dim && !decidido) {
			if (av1[i] > av2[i]) {
				menor = true;
				decidido = true;
			}
			else if (av1[i] < av2[i]) {
				decidido = true;
			}
			++i;
		}
	}

	return menor;
}


bool operator==(const tSudoku& s1, const tSudoku& s2) {
	bool iguales = false;

	if (dame_num_celdas_libres(s1) == dame_num_celdas_libres(s2)) {
		arrValores av1;
		arrValores av2;
		numero_posibles_valores(s1, av1);
		numero_posibles_valores(s2, av2);

		int dim = dame_dimension(s1);
		int i = 0;
		iguales = true;

		while (i < dim && iguales) {
			if (av1[i] != av2[i]) {
				iguales = false;
			}
			++i;
		}
	}

	return iguales;
}


bool busca_celda_vacia(const tSudoku& sudoku, int& fila, int& columna) {
	bool encontrada = false;
	int dim = dame_dimension(sudoku);
	int f = 0;

	while (f < dim && !encontrada) {
		int c = 0;

		while (c < dim && !encontrada) {
			if (es_vacia(dame_celda(sudoku, f, c))) {
				fila = f;
				columna = c;
				encontrada = true;
			}

			++c;
		}

		++f;
	}

	return encontrada;
}


bool resolver_backtracking(tSudoku& sudoku) {
	bool resuelto = false;
	int fila = 0;
	int columna = 0;

	if (!busca_celda_vacia(sudoku, fila, columna)) {
		resuelto = true;
	}

	else {
		int dim = dame_dimension(sudoku);
		int valor = 1;

		while (valor <= dim && !resuelto) {

			if (pon_valor(sudoku, fila, columna, valor)) {
				if (resolver_backtracking(sudoku)) {
					resuelto = true;
				}
				else {
					quita_valor(sudoku, fila, columna);
				}
			}
			++valor;
		}
	}

	return resuelto;
}

bool resolver(tSudoku& sudoku) {
	reset(sudoku);
	return resolver_backtracking(sudoku);
}