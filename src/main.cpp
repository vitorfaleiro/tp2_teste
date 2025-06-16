#include <iostream>
#include "simulador.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Uso correto: " << argv[0] << " <arquivo de entrada>" << std::endl;
        return 1;
    }

    std::string nomeArquivo = argv[1];

    Config config;

    Escalonador escalonador;

    carregarEntrada(nomeArquivo, config, escalonador);

    return 0;
}