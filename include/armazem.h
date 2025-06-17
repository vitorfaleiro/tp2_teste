// include/armazem.h
#ifndef ARMAZEM_H
#define ARMAZEM_H

#include <string>
#include "pilha.h"          // Inclui a definição de Pilha e PacoteNaPilha
#include "lista_ligada.h"   // Inclui ListaLigada para uso em recuperaPacoteLIFO

// Declaração da classe Armazem
class Armazem {
private:
    std::string nome; // Nome do armazém (ex: "000", "001")
    Pilha* secoes;                  // Ponteiro para o array de pilhas (alocação dinâmica)
    std::string* secoesNomes;       // Ponteiro para o array de nomes de seção (alocação dinâmica)
    int numSecoes;                  // Número de seções ativas neste armazém
    int capacidadeSecoes;           // Capacidade atual dos arrays secoes e secoesNomes

    // Função auxiliar para redimensionar os arrays de seções
    void redimensionarSecoes(int novaCapacidade);

public:
    Armazem(); // Construtor padrão
    Armazem(const std::string& nome); // Construtor com nome
    ~Armazem(); // Destrutor para liberar a memória alocada

    // Adiciona uma nova seção para um destino específico
    void adicionarSecao(const std::string& destino);

    // Armazena um pacote em uma seção específica, registrando o tempo de chegada
    void armazenaPacote(int idPacote, const std::string& destino, double tempoAtual);

    // Recupera o pacote mais antigo de uma seção, simulando o LIFO (desempilha e re-empilha)
    // Retorna o ID do pacote recuperado e preenche rearmazenadosTemp com os pacotes temporariamente movidos.
    // O custo de manipulação é calculado externamente com base no tamanho de rearmazenadosTemp.
    int recuperaPacoteLIFO(const std::string& destino, ListaLigada<PacoteNaPilha>& rearmazenadosTemp);

    // Encontra o ID do pacote mais antigo em uma seção específica (apenas para busca, não remove)
    int encontrarPacoteMaisAntigoNaSecao(const std::string& destino);

    // Retorna o nome do armazém
    std::string getNome() const;

    // Obtém o índice de uma seção pelo nome de destino
    int getIndiceSecao(const std::string& destino) const;

    // Verifica se uma seção específica está vazia
    bool secaoVazia(const std::string& destino) const;
};

#endif // ARMAZEM_H