#ifndef PACOTE_H
#define PACOTE_H

#include <string>
#include "lista_ligada.h" // Inclui ListaLigada para a rota do pacote

enum Estado {
    NAO_POSTADO,
    CHEGADA_ESCALONADA,
    CHEGOU_NAO_ARMAZENADO,
    ARMAZENADO,
    ALOCADO_TRANSPORTE,
    ENTREGUE
};

class Pacote {
private:
    int id;                 // ID SEQUENCIAL (0, 1, 2...) para indexacao interna e logs
    int originalLabelId;    // NOVO: ID original lido do arquivo (e.g., 1101)
    std::string remetente;
    std::string destinatario;
    std::string origem;
    std::string destino;
    std::string tipo;
    Estado estado;

public:
    ListaLigada<std::string> rota;

    Pacote();
    // Construtor com parametro para o ID sequencial e o label original
    Pacote(int id_seq, int original_label, const std::string& rem, const std::string& dest, const std::string& origem, const std::string& destino, const std::string& tipo);

    int getId() const;              // Retorna o ID SEQUENCIAL
    int getOriginalLabelId() const; // NOVO: Retorna o ID original (e.g., 1101)
    std::string getOrigem() const;
    std::string getDestino() const;
    Estado getEstado() const;
    void mudarEstado(Estado novoEstado);
};

#endif // PACOTE_H