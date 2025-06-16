#include <iostream>
#include "simulador.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso correto: " << argv[0] << " <arquivo de entrada>" << std::endl;
        return 1;
    }

    std::string nomeArquivo = argv[1];

    Config config;

    Armazem armazens[MAX_ARMAZENS];
    int totalArmazens = 0;

    Pacote pacotes[MAX_PACOTES];
    int totalPacotes = 0;

    Escalonador escalonador;

    carregarEntrada(nomeArquivo, config, armazens, totalArmazens, pacotes, totalPacotes, escalonador);

    executarSimulacao(escalonador, pacotes, totalPacotes, armazens, totalArmazens, config.grafo, config);

    return 0;
}
