#ifndef ARMAZEM_H
#define ARMAZEM_H

#include <string>
#include "pilha.h"

class Armazem {
private:
    std::string nome;
    Pilha secoes[50];               // Até 50 seções por armazém
    std::string secoesNomes[50];    // Nome de cada seção
    int numSecoes;

public:
    Armazem();
    Armazem(const std::string& nome);

    void adicionarSecao(const std::string& destino);
    void armazenaPacote(int idPacote, const std::string& destino);
    int recuperaPacote(const std::string& destino);

    std::string getNome() const;
};

#endif
