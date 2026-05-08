


#include "inputOutput.h"
#include "colors.h"
#include <iostream>
#include <cmath>
using namespace std;

const int BARRA_HORIZONTAL = 196;
const int CRUZ = 197;
const int BARRA_VERTICAL = 179;
const int ANCHO_CELDA = 3;
const int BLANCO = 250;

void pinta_numeros_columna(const tSudoku& s, int size) {
    cout << "   ";
    for (int c = 1; c <= dame_dimension(s); c++) {
        cout << " " << c << " ";
        if (c % size == 0)
            cout << " ";
    }
    cout << endl;
}

void pinta_fila_submatriz(int fila, int col, int size, const tSudoku& s) {
    for (int n = 0; n < size; n++) {
        cout << " ";
        tCelda celda = dame_celda(s, fila, col);
        if (es_vacia(celda))
            cout << char(BLANCO);
        else {
            if (es_original(celda))
                cout << ORANGE;
            else
                cout << LGREEN;
            cout << dame_valor(celda) << RESET;
        }
        cout << " ";
        col++;
    }

}

void pinta_separador_horizontal(int size) {
    cout << "  " << char(CRUZ);
    for (int i = 0; i < size; i++) {
        int hasta = ANCHO_CELDA * size;
        for (int i = 0; i < hasta; i++)
            cout << char(BARRA_HORIZONTAL);
        cout << char(CRUZ);
    }
    cout << endl;
}

void pinta_fila_matriz(int fila, const tSudoku& s) {
    int size = sqrt(dame_dimension(s));
    cout << fila + 1 << " ";
    cout << char(BARRA_VERTICAL);
    int col = 0;
    for (int ns = 0; ns < size; ns++) {
        pinta_fila_submatriz(fila, col, size, s);
        col = col + size;
        cout << char(BARRA_VERTICAL);
    }
    cout << endl;
}

void mostrar_juego_consola(const tSudoku& s) {
    int size = sqrt(dame_dimension(s));
    cout << endl;
    pinta_numeros_columna(s, size);
    pinta_separador_horizontal(size);
    for (int f = 0; f < dame_dimension(s); f++) {
        pinta_fila_matriz(f, s);
        if ((f + 1) % size == 0)
            pinta_separador_horizontal(size);
    }
    cout << endl;
}

int menu() {
    int op;
    cout << "1.- poner valor" << endl;
    cout << "2.- quitar valor" << endl;
    cout << "3.- reset" << endl;
    cout << "4.- posibles valores de una celda vacia" << endl;
    cout << "5.- autocompletar celdas con valor unico" << endl;
    cout << "6.- resolver sudoku" << endl;
    cout << "7.- salir" << endl;

    cout << "Elija una opcion valida(1 - 7): ";
    cin >> op;

    while (op <= 0 || op > 7) {
        cout << "Opcion incorrecta. Elija una opcion valida(1 - 7): ";
        cin >> op;
    }

    return op;
}