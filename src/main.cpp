#include <iostream>
#include "simulador.h" // Inclui o cabeçalho do simulador

int main(int argc, char* argv[]) {
    // Verifica se o número de argumentos da linha de comando está correto
    if (argc != 2) {
        std::cerr << "Uso correto: " << argv[0] << " <arquivo de entrada>" << std::endl;
        return 1; // Retorna 1 para indicar erro
    }

    std::string nomeArquivo = argv[1]; // Obtém o nome do arquivo de entrada

    Config config;                  // Objeto para armazenar as configurações da simulação
    Escalonador escalonador;        // Objeto para gerenciar os eventos
    Pacote* pacotes = nullptr;      // Ponteiro para o array de pacotes (será alocado dinamicamente)
    int totalPacotes = 0;           // Número total de pacotes (passado por referência para carregarEntrada)
    Armazem* armazens = nullptr;   // Ponteiro para o array de armazéns (será alocado dinamicamente)
    int totalArmazens = 0;          // Número total de armazéns (passado por referência para carregarEntrada)

    // Carrega os dados de entrada do arquivo. Os arrays pacotes e armazens
    // serão alocados dentro de carregarEntrada e seus ponteiros e tamanhos
    // serão passados de volta via referência.
    carregarEntrada(nomeArquivo, config, escalonador, &pacotes, totalPacotes, &armazens, totalArmazens);

    // Inicializa o contador total de pacotes na Config após o carregamento
    config.totalPacotesIniciais = totalPacotes; // <= Nova linha aqui

    // Verifica se o carregamento dos dados foi bem-sucedido
    if (pacotes == nullptr || armazens == nullptr || totalPacotes == 0 || totalArmazens == 0) {
        std::cerr << "Falha ao carregar dados de entrada. Encerrando." << std::endl;
        // O destrutor de Config já lida com a desalocação de config.grafo.
        return 1;
    }

    // Executa a simulação
    executarSimulacao(escalonador, pacotes, totalPacotes, armazens, totalArmazens, config.grafo, config); // Passa config por referência

    // Desaloca a memória alocada dinamicamente para pacotes e armazens
    delete[] pacotes;
    delete[] armazens;
    // O destrutor de Config já lida com a desalocação de config.grafo.

    return 0; // Retorna 0 para indicar sucesso
}