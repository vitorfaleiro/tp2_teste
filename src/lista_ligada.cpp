#ifndef LISTA_LIGADA_CPP
#define LISTA_LIGADA_CPP

#include "lista_ligada.h"
#include <cstddef>
#include <string>

template<typename T>
ListaLigada<T>::ListaLigada() : cabeca(nullptr) {}

template<typename T>
ListaLigada<T>::~ListaLigada() {
    while (cabeca != nullptr) {
        removerPrimeiro();
    }
}

template<typename T>
bool ListaLigada<T>::vazia() const {
    return cabeca == nullptr;
}

template<typename T>
void ListaLigada<T>::inserir(const T& valor) {
    NoLista<T>* novo = new NoLista<T>(valor);
    if (cabeca == nullptr) {
        cabeca = novo;
    } else {
        NoLista<T>* atual = cabeca;
        while (atual->prox != nullptr) {
            atual = atual->prox;
        }
        atual->prox = novo;
    }
}

template<typename T>
void ListaLigada<T>::removerPrimeiro() {
    if (cabeca != nullptr) {
        NoLista<T>* temp = cabeca;
        cabeca = cabeca->prox;
        delete temp;
    }
}

template<typename T>
NoLista<T>* ListaLigada<T>::getCabeca() const {
    return cabeca;
}

// Instanciação explícita para string
template class ListaLigada<std::string>;

#endif
