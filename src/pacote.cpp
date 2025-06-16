#include "pacote.h"

Pacote::Pacote() : id(0), remetente(""), destinatario(""), origem(""), destino(""), tipo(""), estado(ARMAZENADO) {}

Pacote::Pacote(int id, const std::string& rem, const std::string& dest, const std::string& origem, const std::string& destino, const std::string& tipo)
    : id(id), remetente(rem), destinatario(dest), origem(origem), destino(destino), tipo(tipo), estado(ARMAZENADO) {}

int Pacote::getId() const {
    return id;
}

std::string Pacote::getOrigem() const {
    return origem;
}

std::string Pacote::getDestino() const {
    return destino;
}

Estado Pacote::getEstado() const {
    return estado;
}

void Pacote::mudarEstado(Estado novoEstado) {
    estado = novoEstado;
}
