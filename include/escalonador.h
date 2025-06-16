#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "minheap.h"

class Escalonador {
private:
    MinHeap heap;
    double tempoAtual;

public:
    Escalonador();

    void agendarEvento(const Evento& e);
    Evento proximoEvento();
    bool temEventos() const;
    void avancarTempo(double novoTempo);
    double getTempoAtual() const;
};

#endif
