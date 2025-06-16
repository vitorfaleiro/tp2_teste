#include "simulador.h"
#include "util.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

void calcularRota(Pacote& pacote, int** grafo, int totalArmazens) {
    int origem = std::stoi(pacote.getOrigem());
    int destino = std::stoi(pacote.getDestino());

    int* anterior = new int[totalArmazens];
    bool* visitado = new bool[totalArmazens];
    int* fila = new int[totalArmazens];

    for (int i = 0; i < totalArmazens; i++) {
        anterior[i] = -1;
        visitado[i] = false;
    }

    int inicio = 0, fim = 0;
    fila[fim++] = origem;
    visitado[origem] = true;

    while (inicio < fim) {
        int atual = fila[inicio++];
        for (int viz = 0; viz < totalArmazens; viz++) {
            if (grafo[atual][viz] && !visitado[viz]) {
                anterior[viz] = atual;
                visitado[viz] = true;
                fila[fim++] = viz;
            }
        }
    }

    // Reconstruir caminho
    int* caminho = new int[totalArmazens];
    int tam = 0;
    int atual = destino;

    while (atual != -1 && atual != origem) {
        caminho[tam++] = atual;
        atual = anterior[atual];
    }

    if (atual == origem) {
        caminho[tam++] = origem;
        for (int i = tam - 1; i >= 0; i--) {
            pacote.rota.inserir(formatarNomeArmazem(caminho[i]));
        }
    } else {
        std::cerr << "Erro: sem caminho de " << origem << " até " << destino << std::endl;
    }

    delete[] anterior;
    delete[] visitado;
    delete[] fila;
    delete[] caminho;
}


void carregarEntrada(const std::string& nomeArquivo,
    Config& config,
    Escalonador& escalonador) {
std::ifstream arq(nomeArquivo);
if (!arq) {
std::cerr << "Erro ao abrir arquivo: " << nomeArquivo << std::endl;
return;
}
Armazem* armazens;
Pacote* pacotes;
int numPacotes;
int totalArmazens;
int totalPacotes = 0;
arq >> config.tempoTransporte
>> config.tempoManipulacao
>> config.tempoSimulacao
>> config.numTiposPacote
>> config.numArmazens;

totalArmazens = config.numArmazens;
armazens = new Armazem[totalArmazens];
for (int i = 0; i < config.numArmazens; ++i)
for (int j = 0; j < config.numArmazens; ++j)
arq >> config.grafo[i][j];

for (int i = 0; i < config.numArmazens; ++i)
armazens[i] = Armazem(formatarNomeArmazem(i));

for (int i = 0; i < config.numArmazens; ++i)
for (int j = 0; j < config.numArmazens; ++j)
if (config.grafo[i][j])
armazens[i].adicionarSecao(formatarNomeArmazem(j));

arq >> numPacotes;
arq.ignore();
totalPacotes = numPacotes;
pacotes = new Pacote[numPacotes];
for (int i = 0; i < numPacotes; ++i) {
std::string linha;
std::getline(arq, linha);
std::istringstream ss(linha);

double tempo;
std::string dummy;
int id, origem, destino;

ss >> tempo >> dummy >> id >> dummy >> origem >> dummy >> destino;

std::string origemNome = formatarNomeArmazem(origem);
std::string destinoNome = formatarNomeArmazem(destino);

pacotes[id] = Pacote(id, "Rem" + std::to_string(id),
            "Dest" + std::to_string(id),
            origemNome, destinoNome, "Normal");

calcularRota(pacotes[id], config.grafo, config.numArmazens);

Evento e;
e.tempo = tempo;
e.tipo = 0;
e.pacoteId = id;
escalonador.agendarEvento(e);
}

arq.close();
executarSimulacao(escalonador, pacotes, totalPacotes, armazens, totalArmazens, config.grafo, config);
}

void logEvento(int tempo, int pacoteId, const std::string& acao, const std::string& de, const std::string& para) {
    std::cout << std::setfill('0') << std::setw(7) << tempo
              << " pacote " << std::setfill('0') << std::setw(3) << pacoteId
              << " " << acao;

    if (acao == "armazenado" || acao == "rearmazenado") {
        std::cout << " em " << de << " na secao " << para;
    } else if (acao == "removido") {
        std::cout << " de " << de << " na secao " << para;
    } else if (acao == "em transito") {
        std::cout << " de " << de << " para " << para;
    } else if (acao == "entregue") {
        std::cout << " em " << de;
    }

    std::cout << std::endl;
}


Armazem* encontrarArmazem(const std::string& nome, Armazem armazens[], int totalArmazens) {
    for (int i = 0; i < totalArmazens; ++i) {
        if (armazens[i].getNome() == nome) {
            return &armazens[i];
        }
    }
    return nullptr;
}


void executarSimulacao(Escalonador& escalonador,
    Pacote* pacotes,
    int totalPacotes,
    Armazem* armazens,
    int totalArmazens,
    int** grafo,
    const Config& config) {
    while (escalonador.temEventos()) {
        Evento e = escalonador.proximoEvento();
        escalonador.avancarTempo(e.tempo);

        int tempoAtual = static_cast<int>(e.tempo);
        int idPacote = e.pacoteId;
        Pacote& p = pacotes[idPacote];

        switch (e.tipo) {
            case 0: { // Evento Chegada Inicial
                NoLista<std::string>* noAtual = p.rota.getCabeca();
                if (!noAtual) continue;

                std::string armazemAtual = noAtual->dado;
                NoLista<std::string>* proximoNo = noAtual->prox;
                std::string proximoArmazem = proximoNo ? proximoNo->dado : "";

                Armazem* armazemPtr = encontrarArmazem(armazemAtual, armazens, totalArmazens);
                if (!armazemPtr) continue;

                if (!proximoNo) {
                    logEvento(tempoAtual, idPacote, "entregue", armazemAtual, "");
                    p.mudarEstado(ENTREGUE);
                } else {
                    armazemPtr->armazenaPacote(idPacote, proximoArmazem);
                    logEvento(tempoAtual, idPacote, "armazenado", armazemAtual, proximoArmazem);
                    p.mudarEstado(ARMAZENADO);

                    Evento novaRemocao;
                    novaRemocao.tempo = tempoAtual + config.tempoManipulacao;
                    novaRemocao.tipo = 1;
                    novaRemocao.pacoteId = idPacote;
                    escalonador.agendarEvento(novaRemocao);
                }
                break;
            }

            case 1: { // Evento Remocao/Início Transporte
                NoLista<std::string>* noAtual = p.rota.getCabeca();
                if (!noAtual || !noAtual->prox) continue;

                std::string origem = noAtual->dado;
                std::string destino = noAtual->prox->dado;

                Armazem* armazemPtr = encontrarArmazem(origem, armazens, totalArmazens);
                if (!armazemPtr) continue;

                int pacoteRecuperado = armazemPtr->recuperaPacote(destino);
                if (pacoteRecuperado == -1) continue;

                logEvento(tempoAtual, idPacote, "removido", origem, destino);
                logEvento(tempoAtual, idPacote, "em transito", origem, destino);
                p.mudarEstado(ALOCADO_TRANSPORTE);

                Evento chegadaTransporte;
                chegadaTransporte.tempo = tempoAtual + config.tempoTransporte;
                chegadaTransporte.tipo = 2;
                chegadaTransporte.pacoteId = idPacote;
                escalonador.agendarEvento(chegadaTransporte);
                break;
            }

            case 2: { // Evento Chegada após Transporte
                NoLista<std::string>* noAtual = p.rota.getCabeca();
                if (!noAtual) continue;

                std::string anterior = noAtual->dado;
                p.rota.removerPrimeiro();
                NoLista<std::string>* proximoNo = p.rota.getCabeca();

                if (!proximoNo) {
                    logEvento(tempoAtual, idPacote, "entregue", anterior, "");
                    p.mudarEstado(ENTREGUE);
                } else {
                    std::string atual = proximoNo->dado;
                    std::string proximoDestino = proximoNo->prox ? proximoNo->prox->dado : "";

                    Armazem* armazemPtr = encontrarArmazem(atual, armazens, totalArmazens);
                    if (!armazemPtr) continue;

                    if (!proximoDestino.empty()) {
                        armazemPtr->armazenaPacote(idPacote, proximoDestino);
                        logEvento(tempoAtual, idPacote, "rearmazenado", atual, proximoDestino);
                        p.mudarEstado(ARMAZENADO);

                        Evento novaRemocao;
                        novaRemocao.tempo = tempoAtual + config.tempoManipulacao;
                        novaRemocao.tipo = 1;
                        novaRemocao.pacoteId = idPacote;
                        escalonador.agendarEvento(novaRemocao);
                    } else {
                        logEvento(tempoAtual, idPacote, "entregue", atual, "");
                        p.mudarEstado(ENTREGUE);
                    }
                }
                break;
            }
        }
    }
}
