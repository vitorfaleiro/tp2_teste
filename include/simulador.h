#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <string>
#include "armazem.h"
#include "pacote.h"
#include "escalonador.h"


struct Config {
    int tempoTransporte;
    int tempoManipulacao;
    int tempoSimulacao;
    int numTiposPacote;
    int numArmazens;
    int** grafo;  // ✅ Agora é ponteiro para ponteiro, para permitir alocação dinâmica
};


void calcularRota(Pacote& pacote, int** grafo, int totalArmazens);
void carregarEntrada(const std::string& nomeArquivo, Config& config, Escalonador& escalonador);
void executarSimulacao(Escalonador& escalonador,
    Pacote pacotes[],
    int totalPacotes,
    Armazem armazens[],
    int totalArmazens,
    int** grafo,
    const Config& config);
void logEvento(int tempo, int pacoteId, const std::string& acao, const std::string& de, const std::string& para);

#endif
