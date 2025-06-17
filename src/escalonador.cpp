// src/escalonador.cpp
#include "escalonador.h"

// Construtor do Escalonador
Escalonador::Escalonador() : tempoAtual(0) {}

// Agenda um evento inserindo-o na MinHeap
void Escalonador::agendarEvento(const Evento& e) {
    heap.inserir(e);
}

// Retorna o próximo evento a ser processado
Evento Escalonador::proximoEvento() {
    return heap.removerMin();
}

// Verifica se há eventos na heap
bool Escalonador::temEventos() const {
    return !heap.vazio();
}

// Avança o tempo atual do simulador
void Escalonador::avancarTempo(double novoTempo) {
    tempoAtual = novoTempo;
}

// Retorna o tempo atual do simulador
double Escalonador::getTempoAtual() const {
    return tempoAtual;
}