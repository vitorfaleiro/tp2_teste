// src/pilha.cpp
#include "pilha.h"
#include "lista_ligada.h" // Para uso da ListaLigada em removerPacotePorId
#include <iostream>       // Para std::cerr

const int CAPACIDADE_INICIAL_PILHA = 10; // Capacidade inicial para cada pilha de seção

// Construtor da Pilha
Pilha::Pilha() : topo(-1), capacidade(CAPACIDADE_INICIAL_PILHA) {
    elementos = new PacoteNaPilha[capacidade]; // Aloca o array de elementos dinamicamente
}

// Destrutor da Pilha: libera a memória alocada
Pilha::~Pilha() {
    delete[] elementos;
}

// Função auxiliar para redimensionar a pilha
void Pilha::redimensionar(int novaCapacidade) {
    PacoteNaPilha* novosElementos = new PacoteNaPilha[novaCapacidade]; // Aloca novo array
    for (int i = 0; i <= topo; ++i) { // Copia até o topo atual
        novosElementos[i] = elementos[i];
    }
    delete[] elementos; // Libera o array antigo
    elementos = novosElementos; // Atualiza o ponteiro
    capacidade = novaCapacidade; // Atualiza a capacidade
}

// Empilha um pacote com seu ID e tempo de chegada na seção
void Pilha::empilhar(int idPacote, double tempoChegada) {
    if (topo + 1 == capacidade) { // Se o array estiver cheio, redimensiona
        redimensionar(capacidade * 2);
    }
    elementos[++topo] = PacoteNaPilha(idPacote, tempoChegada);
}

// Desempilha o elemento do topo da pilha e o retorna
PacoteNaPilha Pilha::desempilhar() {
    if (topo >= 0) {
        return elementos[topo--]; // Retorna o elemento e decrementa o topo
    }
    // Retorna um valor de erro se a pilha estiver vazia
    std::cerr << "Erro: Tentativa de desempilhar de uma pilha vazia." << std::endl;
    return PacoteNaPilha(-1, -1.0); // ID -1 e tempo -1.0 indicam erro/vazio
}

// Retorna o elemento do topo da pilha sem removê-lo
PacoteNaPilha Pilha::topoElemento() const {
    if (topo >= 0) {
        return elementos[topo];
    }
    // Retorna um valor de erro se a pilha estiver vazia
    return PacoteNaPilha(-1, -1.0); // ID -1 e tempo -1.0 indicam erro/vazio
}

// Verifica se a pilha está vazia
bool Pilha::vazia() const {
    return topo == -1;
}

// Retorna o número de elementos na pilha
int Pilha::getNumElementos() const {
    return topo + 1;
}

// Preenche uma ListaLigada com cópias dos elementos da pilha, do fundo para o topo.
// Isso é útil para buscar pacotes sem alterar a ordem da pilha.
void Pilha::obterElementosParaBusca(ListaLigada<PacoteNaPilha>& listaSaida) const {
    listaSaida.limpar(); // Garante que a lista de saída está limpa
    for (int i = 0; i <= topo; ++i) {
        listaSaida.inserir(elementos[i]); // Insere os elementos na ordem em que estão no array
    }
}

// Remove um pacote específico da pilha, simulando a lógica LIFO (Last-In First-Out) para remoção de itens "enterrados".
// Pacotes temporariamente removidos para acessar o alvo são adicionados à lista rearmazenadosTemp.
// Retorna true se o pacote foi encontrado e removido, false caso contrário.
bool Pilha::removerPacotePorId(int idPacote, ListaLigada<PacoteNaPilha>& rearmazenadosTemp) {
    if (vazia()) return false;

    Pilha tempPilha; // Pilha temporária para armazenar elementos desempilhados
    // A pilha temporária também precisa de capacidade inicial e redimensionamento,
    // mas para este cenário, ela geralmente não cresce tanto.
    // O construtor padrão de Pilha já aloca uma capacidade inicial.

    bool found = false;

    // Desempilha elementos da pilha original para a pilha temporária até encontrar o idPacote desejado
    while (!vazia()) {
        PacoteNaPilha current = desempilhar();
        if (current.id == idPacote) {
            found = true;
            // O pacote desejado foi encontrado e já foi desempilhado da pilha original.
            // Não o empilhamos na tempPilha.
            break;
        }
        // Se não for o pacote desejado, empilha na pilha temporária
        tempPilha.empilhar(current.id, current.tempoChegadaNaSecao);
    }

    if (found) {
        // Agora, re-empilha os pacotes da pilha temporária de volta para a pilha original.
        // Estes são os pacotes que foram temporariamente removidos para acessar o alvo.
        // Eles são também adicionados à lista de rearmazenadosTemp para log.
        while (!tempPilha.vazia()) {
            PacoteNaPilha pTemp = tempPilha.desempilhar();
            rearmazenadosTemp.inserir(pTemp); // Adiciona à lista de rearmazenados para logar
            empilhar(pTemp.id, pTemp.tempoChegadaNaSecao); // Re-empilha na pilha original
        }
        return true; // Pacote encontrado e removido
    } else {
        // Se o pacote não foi encontrado (a pilha original está agora vazia ou parcialmente vazia),
        // todos os elementos desempilhados foram para tempPilha. Precisamos restaurar a pilha original.
        while (!tempPilha.vazia()) {
            PacoteNaPilha pTemp = tempPilha.desempilhar();
            empilhar(pTemp.id, pTemp.tempoChegadaNaSecao); // Re-empilha para restaurar o estado original
        }
        return false; // Pacote não encontrado
    }
}