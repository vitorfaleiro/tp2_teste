// src/armazem.cpp
#include "armazem.h"
#include "util.h" // Para formatarNomeArmazem, desformatarNomeArmazem
#include <iostream>
#include <limits> // Para std::numeric_limits

// Construtor padrão do Armazem
Armazem::Armazem() : nome(""), secoes(nullptr), secoesNomes(nullptr), numSecoes(0), capacidadeSecoes(0) {}

// Construtor do Armazem com nome
Armazem::Armazem(const std::string& nome) : nome(nome), secoes(nullptr), secoesNomes(nullptr), numSecoes(0), capacidadeSecoes(0) {}

// Destrutor do Armazem para liberar memória alocada dinamicamente
Armazem::~Armazem() {
    delete[] secoes;       // Libera o array de objetos Pilha
    delete[] secoesNomes;  // Libera o array de strings
}

// Função auxiliar para redimensionar os arrays de seções
void Armazem::redimensionarSecoes(int novaCapacidade) {
    // Aloca novos arrays com a nova capacidade
    Pilha* novasSecoes = new Pilha[novaCapacidade];
    std::string* novosSecoesNomes = new std::string[novaCapacidade];

    // Copia os elementos existentes para os novos arrays
    for (int i = 0; i < numSecoes; ++i) {
        novasSecoes[i] = secoes[i]; // Cópia do objeto Pilha
        novosSecoesNomes[i] = secoesNomes[i];
    }

    // Libera a memória dos arrays antigos
    delete[] secoes;
    delete[] secoesNomes;

    // Atualiza os ponteiros e a capacidade
    secoes = novasSecoes;
    secoesNomes = novosSecoesNomes;
    capacidadeSecoes = novaCapacidade;
}

// Adiciona uma nova seção a este armazém para um destino específico
void Armazem::adicionarSecao(const std::string& destino) {
    // Verifica se a seção já existe
    for (int i = 0; i < numSecoes; ++i) {
        if (secoesNomes[i] == destino) {
            return; // Seção já existe, não adiciona novamente
        }
    }

    // Se não houver capacidade ou a capacidade estiver cheia, redimensiona
    if (numSecoes == capacidadeSecoes) {
        int novaCapacidade = (capacidadeSecoes == 0) ? 4 : capacidadeSecoes * 2; // Começa com 4, dobra depois
        redimensionarSecoes(novaCapacidade);
    }

    // Adiciona a nova seção
    secoesNomes[numSecoes] = destino;
    // A pilha para esta seção é inicializada automaticamente pelo construtor Pilha padrão
    ++numSecoes;
}

// Armazena um pacote em uma seção específica, registrando o tempo atual
void Armazem::armazenaPacote(int idPacote, const std::string& destino, double tempoAtual) {
    for (int i = 0; i < numSecoes; ++i) {
        if (secoesNomes[i] == destino) {
            secoes[i].empilhar(idPacote, tempoAtual);
            return;
        }
    }
    std::cerr << "Erro: Seção para " << destino << " não encontrada no armazém " << nome << std::endl;
}

// Encontra o ID do pacote mais antigo em uma seção específica
// Não remove o pacote, apenas retorna seu ID
int Armazem::encontrarPacoteMaisAntigoNaSecao(const std::string& destino) {
    for (int i = 0; i < numSecoes; ++i) {
        if (secoesNomes[i] == destino) {
            if (secoes[i].vazia()) {
                return -1; // Seção vazia
            }

            ListaLigada<PacoteNaPilha> elementosSecao;
            secoes[i].obterElementosParaBusca(elementosSecao);

            if (elementosSecao.vazia()) {
                return -1; // Lista de elementos vazia (erro lógico ou pilha vazia)
            }

            NoLista<PacoteNaPilha>* atualNo = elementosSecao.getCabeca();
            PacoteNaPilha maisAntigo = atualNo->dado;
            atualNo = atualNo->prox;

            while (atualNo != nullptr) {
                // Compara pelo tempo de chegada na seção
                if (atualNo->dado.tempoChegadaNaSecao < maisAntigo.tempoChegadaNaSecao) {
                    maisAntigo = atualNo->dado;
                }
                atualNo = atualNo->prox;
            }
            return maisAntigo.id;
        }
    }
    return -1; // Seção não existente
}

// Recupera o pacote mais antigo de uma seção, simulando o LIFO
// Os pacotes temporariamente desempilhados são adicionados a rearmazenadosTemp.
// Retorna o ID do pacote recuperado, ou -1 se não houver pacote ou seção inválida.
int Armazem::recuperaPacoteLIFO(const std::string& destino, ListaLigada<PacoteNaPilha>& rearmazenadosTemp) {
    for (int i = 0; i < numSecoes; ++i) {
        if (secoesNomes[i] == destino) {
            int pacoteIdParaRemover = encontrarPacoteMaisAntigoNaSecao(destino);

            if (pacoteIdParaRemover == -1) {
                return -1; // Seção vazia ou pacote não encontrado
            }

            // A função removerPacotePorId da Pilha já lida com a lógica LIFO
            // de desempilhar e re-empilhar, e preenche rearmazenadosTemp.
            if (secoes[i].removerPacotePorId(pacoteIdParaRemover, rearmazenadosTemp)) {
                return pacoteIdParaRemover;
            } else {
                return -1; // Pacote não encontrado na pilha (erro lógico)
            }
        }
    }
    return -1; // Seção não existente
}

// Retorna o nome do armazém
std::string Armazem::getNome() const {
    return nome;
}

// Retorna o índice da seção dado o nome do destino
int Armazem::getIndiceSecao(const std::string& destino) const {
    for (int i = 0; i < numSecoes; ++i) {
        if (secoesNomes[i] == destino) {
            return i;
        }
    }
    return -1; // Não encontrou
}

// Verifica se uma seção específica está vazia
bool Armazem::secaoVazia(const std::string& destino) const {
    for (int i = 0; i < numSecoes; ++i) {
        if (secoesNomes[i] == destino) {
            return secoes[i].vazia();
        }
    }
    return true; // Seção não existe, considerada vazia
}