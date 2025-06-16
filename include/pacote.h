#ifndef PACOTE_H
#define PACOTE_H

#include <string>
#include "lista_ligada.h"

enum Estado {
    ARMAZENADO,
    ALOCADO_TRANSPORTE,
    ENTREGUE
};

class Pacote {
private:
    int id;
    std::string remetente;
    std::string destinatario;
    std::string origem;
    std::string destino;
    std::string tipo;
    Estado estado;

public:
    ListaLigada<std::string> rota;

    Pacote();
    Pacote(int id, const std::string& rem, const std::string& dest, const std::string& origem, const std::string& destino, const std::string& tipo);

    int getId() const;
    std::string getOrigem() const;
    std::string getDestino() const;
    Estado getEstado() const;
    void mudarEstado(Estado novoEstado);
};

#endif
