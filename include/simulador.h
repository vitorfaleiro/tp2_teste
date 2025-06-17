// include/simulador.h
#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <string>
#include "armazem.h"
#include "pacote.h"
#include "escalonador.h"
#include "transporte.h" // Para EventType
#include "lista_ligada.h" // Para ListaLigada (para a assinatura das funções, mas a instanciação é no .cpp)
#include "raw_pacote_data.h" // NOVO: Inclui a definição de RawPacoteData

// Estrutura para armazenar as configurações da simulação
struct Config {
    int tempoTransporte;
    int tempoManipulacao;
    int tempoSimulacao;
    int numTiposPacote;
    int numArmazens;
    int** grafo;

    int totalPacotesIniciais;
    int pacotesEntreguesCount;

    Config() : tempoTransporte(0), tempoManipulacao(0), tempoSimulacao(0), numTiposPacote(0), numArmazens(0), grafo(nullptr), totalPacotesIniciais(0), pacotesEntreguesCount(0) {}

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

// REMOVIDA DAQUI: struct RawPacoteData { ... };

// (Restante das declarações de funções em simulador.h permanecem as mesmas)
void calcularRota(Pacote& pacote, int** grafo, int totalArmazens);

void carregarEntrada(const std::string& nomeArquivo, 
                     Config& config, 
                     Escalonador& escalonador, 
                     Pacote** outPacotes, 
                     int& totalPacotes, 
                     Armazem** outArmazens, 
                     int& totalArmazens);
                     
void executarSimulacao(Escalonador& escalonador, 
                       Pacote* pacotes, 
                       int totalPacotes, 
                       Armazem* armazens, 
                       int totalArmazens, 
                       int** grafo, 
                       Config& config);

void logEvento(int tempo, 
               int pacoteId, 
               const std::string& acao, 
               const std::string& de, 
               const std::string& para);

            
Armazem* encontrarArmazem(const std::string& nome, Armazem armazens[], int totalArmazens);

#endif // SIMULADOR_H