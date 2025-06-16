#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <string>
#include "armazem.h"
#include "pacote.h"
#include "escalonador.h"

#define MAX_ARMAZENS 50
#define MAX_PACOTES 1000

struct Config {
    int tempoTransporte;
    int tempoManipulacao;
    int tempoSimulacao;
    int numTiposPacote;
    int numArmazens;
    int grafo[MAX_ARMAZENS][MAX_ARMAZENS];
};

void calcularRota(Pacote& pacote, int grafo[MAX_ARMAZENS][MAX_ARMAZENS], int totalArmazens);
void carregarEntrada(const std::string& nomeArquivo, Config& config, Armazem armazens[], int& totalArmazens, Pacote pacotes[], int& totalPacotes, Escalonador& escalonador);
void executarSimulacao(Escalonador& escalonador, Pacote pacotes[], int totalPacotes, Armazem armazens[], int totalArmazens, int grafo[MAX_ARMAZENS][MAX_ARMAZENS], const Config& config);
void logEvento(int tempo, int pacoteId, const std::string& acao, const std::string& de, const std::string& para);

#endif
