


#include "Celda.h"


void inicializaCelda(tCelda& c) {
	c.estado = VACIA;
	c.valor = 0;
}
void inicializaCelda(tCelda& c, int v, tEstado e) {
	c.estado = e;
	c.valor = v;
}


bool es_vacia(const tCelda& c) {
	return c.estado == VACIA;
}
bool es_original(const tCelda& c) {
	return c.estado == ORIGINAL;
}
bool es_ocupada(const tCelda& c) {
	return c.estado == OCUPADA;
}
int dame_valor(const tCelda& c) {
	return c.valor;
}


void pon_valor(tCelda& c, int v) {
	c.valor = v;
}
void pon_vacia(tCelda& c) {
	c.estado = VACIA;
}
void pon_original(tCelda& c) {
	c.estado = ORIGINAL;
}
void pon_ocupada(tCelda& c) {
	c.estado = OCUPADA;
}