// src/lista_ligada.cpp
#ifndef LISTA_LIGADA_CPP
#define LISTA_LIGADA_CPP

#include "lista_ligada.h" // Inclui o cabeçalho da própria classe
#include "pilha.h"        // Agora inclui pilha.h para que PacoteNaPilha seja conhecida

// Construtor da ListaLigada
template<typename T>
ListaLigada<T>::ListaLigada() : cabeca(nullptr) {}

// Destrutor da ListaLigada: libera todos os nós
template<typename T>
ListaLigada<T>::~ListaLigada() {
    limpar(); // Chama a função limpar para liberar todos os nós
}

// Verifica se a lista está vazia
template<typename T>
bool ListaLigada<T>::vazia() const {
    return cabeca == nullptr;
}

// Insere um novo valor no final da lista
template<typename T>
void ListaLigada<T>::inserir(const T& valor) {
    NoLista<T>* novo = new NoLista<T>(valor); // Cria um novo nó
    if (cabeca == nullptr) {
        cabeca = novo; // Se a lista estiver vazia, o novo nó é a cabeça
    } else {
        NoLista<T>* atual = cabeca;
        while (atual->prox != nullptr) {
            atual = atual->prox; // Percorre a lista até o último nó
        }
        atual->prox = novo; // Adiciona o novo nó ao final
    }
}

// Remove o primeiro elemento da lista
template<typename T>
void ListaLigada<T>::removerPrimeiro() {
    if (cabeca != nullptr) { // Verifica se a lista não está vazia
        NoLista<T>* temp = cabeca; // Salva o nó a ser removido
        cabeca = cabeca->prox;     // O próximo nó se torna a nova cabeça
        delete temp;               // Libera a memória do nó removido
    }
}

// Retorna o ponteiro para o primeiro nó (cabeça da lista)
template<typename T>
NoLista<T>* ListaLigada<T>::getCabeca() const {
    return cabeca;
}

// Remove todos os elementos da lista e libera a memória
template<typename T>
void ListaLigada<T>::limpar() {
    while (cabeca != nullptr) {
        removerPrimeiro(); // Remove o primeiro repetidamente até esvaziar
    }
}

// Instanciações explícitas para os tipos usados no projeto
// Isso é necessário quando a definição de um template está em um arquivo .cpp separado
template class ListaLigada<std::string>;
template class ListaLigada<PacoteNaPilha>;

#endif // LISTA_LIGADA_CPP
