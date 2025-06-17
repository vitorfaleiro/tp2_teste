// include/pilha.h
#ifndef PILHA_H
#define PILHA_H

#include "lista_ligada.h" // Inclui ListaLigada para PacoteNaPilha

// Estrutura para armazenar o ID do pacote e o tempo de chegada na seção da pilha
struct PacoteNaPilha {
    int id;
    double tempoChegadaNaSecao;

    // Construtor padrão
    PacoteNaPilha() : id(-1), tempoChegadaNaSecao(-1.0) {}
    // Construtor com parâmetros
    PacoteNaPilha(int _id, double _tempo) : id(_id), tempoChegadaNaSecao(_tempo) {}
};

// Declaração da classe Pilha
class Pilha {
private:
    PacoteNaPilha* elementos; // Ponteiro para o array de elementos (alocação dinâmica)
    int topo;                 // Índice do elemento no topo da pilha
    int capacidade;           // Capacidade atual do array

    // Função auxiliar para redimensionar a pilha
    void redimensionar(int novaCapacidade);

public:
    Pilha(); // Construtor
    ~Pilha(); // Destrutor para liberar a memória alocada

    // Empilha um pacote com seu tempo de chegada na seção
    void empilhar(int idPacote, double tempoChegada);

    // Desempilha e retorna o pacote do topo
    PacoteNaPilha desempilhar();

    // Retorna o pacote do topo sem removê-lo
    PacoteNaPilha topoElemento() const;

    // Verifica se a pilha está vazia
    bool vazia() const;

    // Retorna o número de elementos na pilha
    int getNumElementos() const;

    // Preenche uma ListaLigada com cópias dos elementos da pilha (do fundo para o topo)
    void obterElementosParaBusca(ListaLigada<PacoteNaPilha>& listaSaida) const;

    // Remove um pacote específico da pilha, simulando a lógica LIFO (re-empilhando os de cima)
    // Os pacotes temporariamente removidos são adicionados à lista rearmazenadosTemp.
    // Retorna true se o pacote foi encontrado e removido, false caso contrário.
    bool removerPacotePorId(int idPacote, ListaLigada<PacoteNaPilha>& rearmazenadosTemp);
};

#endif // PILHA_H