#ifndef LISTA_LIGADA_H
#define LISTA_LIGADA_H

template<typename T>
struct NoLista {
    T dado;
    NoLista* prox;

    NoLista(const T& valor) : dado(valor), prox(nullptr) {}
};

template<typename T>
class ListaLigada {
private:
    NoLista<T>* cabeca;

public:
    ListaLigada();
    ~ListaLigada();

    bool vazia() const;
    void inserir(const T& valor);
    void removerPrimeiro();
    NoLista<T>* getCabeca() const;
};

#include "../src/lista_ligada.cpp"  // Inclusão da implementação genérica

#endif
