#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <string>
#include "armazem.h"
#include "pacote.h"
#include "escalonador.h"
#include "transporte.h"
#include "lista_ligada.h" // Para ListaLigada
// #include "raw_pacote_data.h" // <--- Descomentar se você precisa deste struct

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

void calcularRota(Pacote& pacote, int** grafo, int totalArmazens);
void carregarEntrada(const std::string& nomeArquivo, Config& config, Escalonador& escalonador, Pacote** outPacotes, int& totalPacotes, Armazem** outArmazens, int& totalArmazens);
void executarSimulacao(Escalonador& escalonador, Pacote* pacotes, int totalPacotes, Armazem* armazens, int totalArmazens, int** grafo, Config& config);
void logEvento(int tempo, int pacoteId, const std::string& acao, const std::string& de, const std::string& para);

// MUDANÇA IMPORTANTE: Assinatura para Armazem* armazens
Armazem* encontrarArmazem(const std::string& nome, Armazem* armazens, int totalArmazens);


#endif // SIMULADOR_H