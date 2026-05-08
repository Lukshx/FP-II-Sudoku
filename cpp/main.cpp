


#include "inputOutput.h"
#include "ListaSudokus.h"
#include "checkML.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void solicitar_posicion(const tSudoku& sudoku, int& f, int& c);
void solicitar_valor(int& v);
void mostrar_valores_posibles(const tSudoku& sudoku, int f, int c);
void mostrar_mensaje_victoria();
void mostrar_bloqueos(const tSudoku& sudoku);
char elegir_partida();
int elegir_sudoku(const tListaSudokus& lista);
bool posicion_valida(const tSudoku& sudoku, int f, int c);
bool cargar_lista_sudokus_nuevos(const string& nombre_fichero, tListaSudokus& ls);
bool cargar_lista_sudokus_comenzados(const string& nombre_fichero, tListaSudokus& ls);
void guardar_lista_sudokus_comenzados(const string& nombre_fichero, const tListaSudokus& ls);
bool juega(tSudoku& sudoku);
void jugar_sudoku_nuevo(tListaSudokus& listaNuevas, tListaSudokus& listaContinuar);
void jugar_sudoku_comenzado(tListaSudokus& listaNuevas, tListaSudokus& listaContinuar);

#ifdef _WIN32
#include <windows.h>
#endif


int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#ifdef _WIN32
	SetConsoleOutputCP(437);
#endif

	const string ficheroNuevos = "lista_sudokus.txt";
	const string ficheroComenzados = "lista_partidas.txt";
	tListaSudokus listaNuevas;
	tListaSudokus listaContinuar;
	bool listasCargadas = false;
	bool salir = false;

	if (cargar_lista_sudokus_nuevos(ficheroNuevos, listaNuevas)) {
		if (cargar_lista_sudokus_comenzados(ficheroComenzados, listaContinuar)) {
			listasCargadas = true;
		}
		else {
			destruye_lista_sudokus(listaNuevas);
			destruye_lista_sudokus(listaContinuar);
			cout << "No se pudo cargar la lista de sudokus comenzados." << endl;
		}
	}
	else {
		destruye_lista_sudokus(listaNuevas);
		cout << "No se pudo cargar la lista de sudokus nuevos." << endl;
	}

	while (listasCargadas && !salir) {
		char opcion = elegir_partida();

		if (opcion == 'N') {
			jugar_sudoku_nuevo(listaNuevas, listaContinuar);
		}
		else if (opcion == 'C') {
			jugar_sudoku_comenzado(listaNuevas, listaContinuar);
		}
		else {
			salir = true;
		}
	}

	if (listasCargadas) {
		guardar_lista_sudokus_comenzados(ficheroComenzados, listaContinuar);
		destruye_lista_sudokus(listaNuevas);
		destruye_lista_sudokus(listaContinuar);
	}

	return 0;
}


void solicitar_posicion(const tSudoku& sudoku, int& f, int& c) {
	cout << "Fila y columna entre 1..." << dame_dimension(sudoku) << ": ";
	cin >> f >> c;
	--f;
	--c;
}

void solicitar_valor(int& v) {
	cout << "Valor: ";
	cin >> v;
}

bool posicion_valida(const tSudoku& sudoku, int f, int c) {
	int dim = dame_dimension(sudoku);
	return 0 <= f && f < dim && 0 <= c && c < dim;
}




void mostrar_valores_posibles(const tSudoku& sudoku, int f, int c) {
	if (!posicion_valida(sudoku, f, c)) {
		cout << "Posicion introducida fuera del tablero" << endl;
	}
	else if (!es_vacia(dame_celda(sudoku, f, c))) {
		cout << "La celda introducida no esta vacia" << endl;
	}
	else {
		cout << "Los valores posibles para la celda son: { ";

		for (int i = 1; i <= dame_dimension(sudoku); ++i) {
			if (es_valor_posible(sudoku, f, c, i)) {
				cout << i << ' ';
			}
		}

		cout << "}" << endl;
	}
}

void mostrar_mensaje_victoria() {
	cout << "Enhorabuena: Has ganado ......" << endl;
}


void mostrar_bloqueos(const tSudoku& sudoku) {
	if (bloqueo(sudoku)) {
		cout << "Sudoku bloqueado..... Las casillas bloqueadas son: ";

		for (int i = 0; i < dame_num_celdas_bloqueadas(sudoku); ++i) {
			int f;
			int c;
			dame_celda_bloqueada(sudoku, i, f, c);
			cout << "(" << f + 1 << "," << c + 1 << ") ";
		}

		cout << endl;
	}
}


char elegir_partida() {
	char opcion;

	cout << "Partida nueva (N), continuar partida (C) o abandonar la aplicacion (A)? ";
	cin >> opcion;
	opcion = toupper(opcion);

	while (opcion != 'N' && opcion != 'C' && opcion != 'A') {
		cout << "Opcion incorrecta. Introduce N, C o A: ";
		cin >> opcion;
		opcion = toupper(opcion);
	}

	return opcion;
}


int elegir_sudoku(const tListaSudokus& lista) {
	int pos;

	cout << "Elige un sudoku entre 1 y " << dame_num_elems(lista) << ": ";
	cin >> pos;

	while (pos < 1 || pos > dame_num_elems(lista)) {
		cout << "Posicion incorrecta. Elige un sudoku entre 1 y " << dame_num_elems(lista) << ": ";
		cin >> pos;
	}

	return pos - 1;
}


bool cargar_lista_sudokus_nuevos(const string& nombre_fichero, tListaSudokus& ls) {
	bool cargado = false;
	ifstream archivoLista(nombre_fichero);
	inicializa_lista_sudokus(ls);

	if (archivoLista.is_open()) {
		int numSudokus;
		archivoLista >> numSudokus;
		cargado = true;

		for (int i = 0; i < numSudokus; ++i) {
			string nombreSudoku;
			archivoLista >> nombreSudoku;
			ifstream archivoSudoku(nombreSudoku);

			if (archivoSudoku.is_open()) {
				tSudoku sudoku;
				carga_sudoku(archivoSudoku, sudoku);
				insertar(ls, sudoku);
				archivoSudoku.close();
			}
			else {
				cargado = false;
			}
		}

		archivoLista.close();
	}

	return cargado;
}


bool cargar_lista_sudokus_comenzados(const string& nombre_fichero, tListaSudokus& ls) {
	bool cargado = false;
	ifstream archivo(nombre_fichero);
	inicializa_lista_sudokus(ls);

	if (archivo.is_open()) {
		int numSudokus;
		archivo >> numSudokus;
		cargado = true;

		for (int i = 0; i < numSudokus; ++i) {
			tSudoku sudoku;
			carga_sudoku(archivo, sudoku);

			int fila;
			archivo >> fila;

			while (fila != -1) {
				int columna;
				int valor;
				archivo >> columna >> valor;
				pon_valor(sudoku, fila, columna, valor);
				archivo >> fila;
			}

			insertar(ls, sudoku);
		}

		archivo.close();
	}

	return cargado;
}





void guardar_lista_sudokus_comenzados(const string& nombre_fichero, const tListaSudokus& ls) {
	ofstream archivo(nombre_fichero);

	if (archivo.is_open()) {
		archivo << dame_num_elems(ls) << endl;

		for (int i = 0; i < dame_num_elems(ls); ++i) {
			tSudoku sudoku = dame_sudoku(ls, i);
			int dim = dame_dimension(sudoku);
			int filas[DIM * DIM];
			int columnas[DIM * DIM];
			int valores[DIM * DIM];
			int contOcupadas = 0;

			archivo << dim << endl;

			for (int f = 0; f < dim; ++f) {
				for (int c = 0; c < dim; ++c) {
					tCelda celda = dame_celda(sudoku, f, c);

					if (es_original(celda)) {
						archivo << dame_valor(celda) << ' ';
					}
					else {
						archivo << 0 << ' ';

						if (es_ocupada(celda)) {
							filas[contOcupadas] = f;
							columnas[contOcupadas] = c;
							valores[contOcupadas] = dame_valor(celda);
							++contOcupadas;
						}
					}
				}

				archivo << endl;
			}

			for (int j = 0; j < contOcupadas; ++j) {
				archivo << filas[j] << ' ' << columnas[j] << ' ' << valores[j] << endl;
			}

			archivo << -1 << endl;
		}

		archivo.close();
	}
}


bool juega(tSudoku& sudoku) {
	bool abandonado = false;
	int op;

	do {
		int f;
		int c;
		int v;

		mostrar_juego_consola(sudoku);
		op = menu();

		if (op == 1) {
			solicitar_posicion(sudoku, f, c);

			if (!posicion_valida(sudoku, f, c)) {
				cout << endl << "Posicion introducida fuera del tablero" << endl << endl;
			}
			else {
				solicitar_valor(v);

				if (pon_valor(sudoku, f, c, v)) {
					mostrar_bloqueos(sudoku);
				}
				else {
					cout << endl << "Los datos introducidos son incorrectos..." << endl << endl;
				}
			}
		}
		else if (op == 2) {
			solicitar_posicion(sudoku, f, c);

			if (!posicion_valida(sudoku, f, c)) {
				cout << endl << "Posicion introducida fuera del tablero" << endl << endl;
			}
			else {
				if (quita_valor(sudoku, f, c)) {
					mostrar_bloqueos(sudoku);
				}
				else {
					cout << endl << "Los datos introducidos son incorrectos..." << endl << endl;
				}
			}
		}
		else if (op == 3) {
			reset(sudoku);
		}
		else if (op == 4) {
			solicitar_posicion(sudoku, f, c);
			mostrar_valores_posibles(sudoku, f, c);
		}
		else if (op == 5) {
			autocompleta(sudoku);
			mostrar_bloqueos(sudoku);
		}
		else if (op == 6) {
			if (resolver(sudoku)) {
				cout << endl << "Sudoku resuelto correctamente." << endl << endl;
			}
			else {
				cout << endl << "No se ha podido resolver el Sudoku." << endl << endl;
			}
		}

	} while (op != 7 && !terminado(sudoku));

	if (terminado(sudoku)) {
		mostrar_juego_consola(sudoku);
		mostrar_mensaje_victoria();
	}
	else {
		abandonado = true;
	}

	return abandonado;
}



void jugar_sudoku_nuevo(tListaSudokus& listaNuevas, tListaSudokus& listaContinuar) {
	mostrar_lista_sudokus(listaNuevas);

	if (dame_num_elems(listaNuevas) > 0) {
		int pos = elegir_sudoku(listaNuevas);
		tSudoku sudoku = dame_sudoku(listaNuevas, pos);
		bool abandonado = juega(sudoku);

		if (abandonado) {
			insertar(listaContinuar, sudoku);
		}
	}
}


void jugar_sudoku_comenzado(tListaSudokus& listaNuevas, tListaSudokus& listaContinuar) {
	if (dame_num_elems(listaContinuar) == 0) {
		cout << "No hay sudokus comenzados. Debes elegir un sudoku nuevo." << endl;
		jugar_sudoku_nuevo(listaNuevas, listaContinuar);
	}
	else {
		mostrar_lista_sudokus(listaContinuar);
		int pos = elegir_sudoku(listaContinuar);
		tSudoku sudoku = dame_sudoku(listaContinuar, pos);
		eliminar(listaContinuar, pos);

		if (juega(sudoku)) {
			insertar(listaContinuar, sudoku);
		}
	}
}
