// include/lista_ligada.h
#ifndef LISTA_LIGADA_H
#define LISTA_LIGADA_H

#include <cstddef> // Para nullptr
#include <string>  // Para std::string

// Declaração da estrutura do nó da lista ligada (template)
template<typename T>
struct NoLista {
    T dado;         // Dado armazenado no nó
    NoLista* prox;  // Ponteiro para o próximo nó na lista

    NoLista(const T& valor) : dado(valor), prox(nullptr) {} // Construtor
};

// Declaração da classe ListaLigada (template)
template<typename T>
class ListaLigada {
private:
    NoLista<T>* cabeca; // Ponteiro para o primeiro nó da lista

public:
    ListaLigada();  // Construtor
    ~ListaLigada(); // Destrutor (libera memória)

    bool vazia() const; // Verifica se a lista está vazia

    // Insere um novo valor no final da lista
    void inserir(const T& valor);

    // Remove o primeiro elemento da lista
    void removerPrimeiro();

    // Retorna o ponteiro para o primeiro nó
    NoLista<T>* getCabeca() const;

    // Remove todos os elementos da lista
    void limpar();
};

// REMOVIDA A INCLUSÃO DE "../src/lista_ligada.cpp" AQUI.
// A implementação do template será instanciada explicitamente em lista_ligada.cpp
// e os .h de quem usa a lista ligada devem incluir apenas este arquivo.

#endif // LISTA_LIGADA_H