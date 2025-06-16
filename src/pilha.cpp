#include "pilha.h"

Pilha::Pilha() : topo(-1) {}

void Pilha::empilhar(int valor) {
    if (topo < 999) {
        elementos[++topo] = valor;
    }
}

int Pilha::desempilhar() {
    if (topo >= 0) {
        return elementos[topo--];
    }
    return -1; // Pilha vazia
}

int Pilha::topoElemento() const {
    if (topo >= 0) {
        return elementos[topo];
    }
    return -1; // Pilha vazia
}

bool Pilha::vazia() const {
    return topo == -1;
}
