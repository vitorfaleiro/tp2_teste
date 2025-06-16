#include "minheap.h"

MinHeap::MinHeap() : tamanho(0) {}

void MinHeap::subir(int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;
        if (eventos[i].tempo < eventos[pai].tempo) {
            Evento temp = eventos[i];
            eventos[i] = eventos[pai];
            eventos[pai] = temp;
            i = pai;
        } else {
            break;
        }
    }
}

void MinHeap::descer(int i) {
    int menor = i;
    int esquerda = 2 * i + 1;
    int direita = 2 * i + 2;

    if (esquerda < tamanho && eventos[esquerda].tempo < eventos[menor].tempo)
        menor = esquerda;

    if (direita < tamanho && eventos[direita].tempo < eventos[menor].tempo)
        menor = direita;

    if (menor != i) {
        Evento temp = eventos[i];
        eventos[i] = eventos[menor];
        eventos[menor] = temp;
        descer(menor);
    }
}

void MinHeap::inserir(const Evento& e) {
    eventos[tamanho] = e;
    subir(tamanho);
    tamanho++;
}

Evento MinHeap::removerMin() {
    Evento min = eventos[0];
    eventos[0] = eventos[--tamanho];
    descer(0);
    return min;
}

bool MinHeap::vazio() const {
    return tamanho == 0;
}
