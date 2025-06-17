// src/minheap.cpp
#include "minheap.h"
#include <iostream> // Para std::cerr

const int CAPACIDADE_INICIAL_MINHEAP = 10; // Capacidade inicial para a MinHeap

// Construtor da MinHeap
MinHeap::MinHeap() : tamanho(0), capacidade(CAPACIDADE_INICIAL_MINHEAP) {
    eventos = new Evento[capacidade]; // Aloca o array de eventos dinamicamente
}

// Destrutor da MinHeap: libera a memória alocada
MinHeap::~MinHeap() {
    delete[] eventos;
}

// Função auxiliar para redimensionar o array de eventos
void MinHeap::redimensionar(int novaCapacidade) {
    Evento* novosEventos = new Evento[novaCapacidade]; // Aloca novo array
    for (int i = 0; i < tamanho; ++i) {
        novosEventos[i] = eventos[i]; // Copia os elementos existentes
    }
    delete[] eventos; // Libera o array antigo
    eventos = novosEventos; // Atualiza o ponteiro
    capacidade = novaCapacidade; // Atualiza a capacidade
}

// Função auxiliar para subir um elemento na heap até sua posição correta
void MinHeap::subir(int i) {
    while (i > 0) {
        int pai = (i - 1) / 2; // Calcula o índice do pai
        // Compara o tempo do evento atual com o do pai
        if (eventos[i].tempo < eventos[pai].tempo) {
            // Troca os elementos se o filho for menor que o pai
            Evento temp = eventos[i];
            eventos[i] = eventos[pai];
            eventos[pai] = temp;
            i = pai; // Atualiza o índice para continuar subindo
        } else {
            break; // A propriedade da heap foi restaurada
        }
    }
}

// Função auxiliar para descer um elemento na heap até sua posição correta
void MinHeap::descer(int i) {
    int menor = i; // Assume que o elemento atual é o menor
    int esquerda = 2 * i + 1; // Índice do filho esquerdo
    int direita = 2 * i + 2;  // Índice do filho direito

    // Verifica se o filho esquerdo existe e é menor
    if (esquerda < tamanho && eventos[esquerda].tempo < eventos[menor].tempo) {
        menor = esquerda;
    }

    // Verifica se o filho direito existe e é menor que o atual menor
    if (direita < tamanho && eventos[direita].tempo < eventos[menor].tempo) {
        menor = direita;
    }

    // Se o menor não for o elemento atual, troca e desce recursivamente
    if (menor != i) {
        Evento temp = eventos[i];
        eventos[i] = eventos[menor];
        eventos[menor] = temp;
        descer(menor);
    }
}

// Insere um novo evento na heap
void MinHeap::inserir(const Evento& e) {
    if (tamanho == capacidade) { // Se o array estiver cheio, redimensiona
        redimensionar(capacidade * 2);
    }
    eventos[tamanho] = e; // Adiciona o evento no final
    subir(tamanho);       // Ajusta a posição do novo evento
    tamanho++;            // Incrementa o tamanho da heap
}

// Remove e retorna o evento com o menor tempo (o elemento na raiz da heap)
Evento MinHeap::removerMin() {
    if (vazio()) {
        // Retorna um evento "vazio" ou lança uma exceção se a heap estiver vazia
        std::cerr << "Erro: Tentativa de remover de uma MinHeap vazia." << std::endl;
        return Evento(); // Retorna um evento padrão (id -1, tempo 0.0)
    }

    Evento min = eventos[0];          // Salva o evento de menor tempo
    eventos[0] = eventos[--tamanho]; // Move o último elemento para a raiz
    descer(0);                        // Ajusta a nova raiz para manter a propriedade da heap
    return min;                       // Retorna o evento removido
}

// Verifica se a heap está vazia
bool MinHeap::vazio() const {
    return tamanho == 0;
}