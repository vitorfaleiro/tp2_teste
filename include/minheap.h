// include/minheap.h
#ifndef MINHEAP_H
#define MINHEAP_H

#include "transporte.h" // Inclui a definição de Evento

// Declaração da classe MinHeap para gerenciar eventos por tempo
class MinHeap {
private:
    Evento* eventos; // Ponteiro para o array de eventos (alocação dinâmica)
    int tamanho;     // Número atual de eventos na heap
    int capacidade;  // Capacidade atual do array

    // Funções auxiliares para manter a propriedade da heap
    void subir(int i); // Move um elemento para cima na heap
    void descer(int i); // Move um elemento para baixo na heap

    // Função auxiliar para redimensionar a heap
    void redimensionar(int novaCapacidade);

public:
    MinHeap(); // Construtor
    ~MinHeap(); // Destrutor para liberar a memória alocada

    // Insere um novo evento na heap
    void inserir(const Evento& e);

    // Remove e retorna o evento com o menor tempo (o próximo evento a ocorrer)
    Evento removerMin();

    // Verifica se a heap está vazia
    bool vazio() const;
};

#endif // MINHEAP_H