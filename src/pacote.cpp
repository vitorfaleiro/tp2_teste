#include "pacote.h"

// Construtor padrão do Pacote
Pacote::Pacote() : id(0), originalLabelId(0), remetente(""), destinatario(""), origem(""), destino(""), tipo(""), estado(ARMAZENADO) {}

// Construtor com parâmetros para inicializar um Pacote
Pacote::Pacote(int id_seq, int original_label, const std::string& rem, const std::string& dest, const std::string& origem, const std::string& destino, const std::string& tipo)
    : id(id_seq), originalLabelId(original_label), remetente(rem), destinatario(dest), origem(origem), destino(destino), tipo(tipo), estado(ARMAZENADO) {}

// Retorna o ID SEQUENCIAL do pacote
int Pacote::getId() const {
    return id;
}

// Retorna o ID original/label do pacote lido do arquivo
int Pacote::getOriginalLabelId() const {
    return originalLabelId;
}

// Retorna o armazém de origem inicial do pacote
std::string Pacote::getOrigem() const {
    return origem;
}

// Retorna o armazém de destino final do pacote
std::string Pacote::getDestino() const {
    return destino;
}

// Retorna o estado atual do pacote
Estado Pacote::getEstado() const {
    return estado;
}

// Altera o estado do pacote
void Pacote::mudarEstado(Estado novoEstado) {
    estado = novoEstado;
}