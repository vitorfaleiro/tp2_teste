#include "armazem.h"
#include <iostream>

Armazem::Armazem() : nome(""), numSecoes(0) {}

Armazem::Armazem(const std::string& nome) : nome(nome), numSecoes(0) {}

void Armazem::adicionarSecao(const std::string& destino) {
    secoesNomes[numSecoes] = destino;
    ++numSecoes;
}

void Armazem::armazenaPacote(int idPacote, const std::string& destino) {
    for (int i = 0; i < numSecoes; ++i) {
        if (secoesNomes[i] == destino) {
            secoes[i].empilhar(idPacote);
            return;
        }
    }
    std::cerr << "Erro: Seção para " << destino << " não encontrada no armazém " << nome << std::endl;
}

int Armazem::recuperaPacote(const std::string& destino) {
    for (int i = 0; i < numSecoes; ++i) {
        if (secoesNomes[i] == destino && !secoes[i].vazia()) {
            return secoes[i].desempilhar();
        }
    }
    return -1; // Seção vazia ou não existente
}

std::string Armazem::getNome() const {
    return nome;
}
