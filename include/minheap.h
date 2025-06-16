#ifndef MINHEAP_H
#define MINHEAP_H

#include "transporte.h"

class MinHeap {
private:
    Evento eventos[10000]; // Tamanho suficiente para os testes do TP
    int tamanho;

    void subir(int i);
    void descer(int i);

public:
    MinHeap();
    void inserir(const Evento& e);
    Evento removerMin();
    bool vazio() const;
};

#endif
