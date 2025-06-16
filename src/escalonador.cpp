#include "escalonador.h"

Escalonador::Escalonador() : tempoAtual(0) {}

void Escalonador::agendarEvento(const Evento& e) {
    heap.inserir(e);
}

Evento Escalonador::proximoEvento() {
    return heap.removerMin();
}

bool Escalonador::temEventos() const {
    return !heap.vazio();
}

void Escalonador::avancarTempo(double novoTempo) {
    tempoAtual = novoTempo;
}

double Escalonador::getTempoAtual() const {
    return tempoAtual;
}
