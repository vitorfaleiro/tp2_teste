// include/escalonador.h
#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "minheap.h" // Inclui a definição de MinHeap

// Declaração da classe Escalonador
class Escalonador {
private:
    MinHeap heap;           // Heap de mínima para gerenciar os eventos
    double tempoAtual;      // Tempo atual da simulação

public:
    Escalonador(); // Construtor

    // Agenda um novo evento
    void agendarEvento(const Evento& e);

    // Retorna o próximo evento a ser processado (o de menor tempo)
    Evento proximoEvento();

    // Verifica se ainda há eventos pendentes na heap
    bool temEventos() const;

    // Avança o tempo atual da simulação para o tempo do próximo evento
    void avancarTempo(double novoTempo);

    // Retorna o tempo atual da simulação
    double getTempoAtual() const;
};

#endif // ESCALONADOR_H