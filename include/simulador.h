// include/simulador.h
#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <string>
#include "armazem.h"
#include "pacote.h"
#include "escalonador.h"
#include "transporte.h" // Para EventType

// Estrutura para armazenar as configurações da simulação
struct Config {
    int tempoTransporte;    // Tempo de transporte entre dois armazéns
    int tempoManipulacao;   // Tempo de manipulação de pacote em uma seção
    int tempoSimulacao;     // Tempo total de simulação (pode ser um limite para o relógio)
    int numTiposPacote;     // Não utilizado diretamente, mas lido do input
    int numArmazens;        // Número total de armazéns na rede
    int** grafo;            // Matriz de adjacência para representar as conexões entre armazéns

    // Construtor para inicializar grafo como nullptr
    Config() : tempoTransporte(0), tempoManipulacao(0), tempoSimulacao(0), numTiposPacote(0), numArmazens(0), grafo(nullptr) {}

    // Destrutor para liberar a memória do grafo
    ~Config() {
        if (grafo) {
            for (int i = 0; i < numArmazens; ++i) {
                delete[] grafo[i];
            }
            delete[] grafo;
            grafo = nullptr;
        }
    }
};

// Calcula a rota de um pacote usando BFS
void calcularRota(Pacote& pacote, int** grafo, int totalArmazens);

// Carrega os dados de entrada do arquivo
void carregarEntrada(const std::string& nomeArquivo,
                     Config& config,
                     Escalonador& escalonador,
                     Pacote** outPacotes, // Retorna os pacotes alocados
                     int& totalPacotes,   // Retorna o total de pacotes
                     Armazem** outArmazens, // Retorna os armazéns alocados
                     int& totalArmazens); // Retorna o total de armazéns

// Executa a simulação
void executarSimulacao(Escalonador& escalonador,
                       Pacote* pacotes,
                       int totalPacotes,
                       Armazem* armazens,
                       int totalArmazens,
                       int** grafo,
                       const Config& config);

// Função para logar eventos no formato especificado
void logEvento(int tempo, int pacoteId, const std::string& acao, const std::string& de, const std::string& para);

// Função auxiliar para encontrar um armazém pelo nome
Armazem* encontrarArmazem(const std::string& nome, Armazem armazens[], int totalArmazens);

#endif // SIMULADOR_H