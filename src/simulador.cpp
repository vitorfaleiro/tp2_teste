// src/simulador.cpp
#include "simulador.h"
#include "util.h" // Para formatarNomeArmazem, desformatarNomeArmazem
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip> // Para std::setfill, std::setw
// #include <queue>   // Para BFS, mas usando array como fila para aderir às restrições

// Calcula a rota mais curta para um pacote usando Busca em Largura (BFS)
void calcularRota(Pacote& pacote, int** grafo, int totalArmazens) {
    int origem = desformatarNomeArmazem(pacote.getOrigem());
    int destino = desformatarNomeArmazem(pacote.getDestino());

    // Verifica se origem ou destino são inválidos para evitar acessos fora do limite do grafo
    if (origem < 0 || origem >= totalArmazens || destino < 0 || destino >= totalArmazens) {
        std::cerr << "Erro: Origem ou destino inválidos para o pacote " << pacote.getId() << std::endl;
        // Limpa a rota para indicar falha no cálculo
        pacote.rota.limpar();
        return;
    }


    // Arrays para a BFS
    int* anterior = new int[totalArmazens]; // Armazena o nó anterior no caminho mais curto
    bool* visitado = new bool[totalArmazens]; // Marca nós visitados
    int* fila = new int[totalArmazens];       // Implementa a fila para BFS

    // Inicializa arrays
    for (int i = 0; i < totalArmazens; i++) {
        anterior[i] = -1; // -1 indica que não há predecessor
        visitado[i] = false; // Nenhum nó visitado inicialmente
    }

    int inicioFila = 0, fimFila = 0; // Ponteiros para o início e fim da fila
    fila[fimFila++] = origem;     // Adiciona o nó de origem à fila
    visitado[origem] = true;      // Marca a origem como visitada

    // Loop principal da BFS
    while (inicioFila < fimFila) {
        int atual = fila[inicioFila++]; // Remove o nó atual da fila
        if (atual == destino) {
            break; // Destino alcançado
        }

        // Explora vizinhos do nó atual
        for (int viz = 0; viz < totalArmazens; viz++) {
            // Se há uma conexão e o vizinho não foi visitado
            if (grafo[atual][viz] == 1 && !visitado[viz]) {
                anterior[viz] = atual;  // Define o predecessor do vizinho
                visitado[viz] = true;   // Marca o vizinho como visitado
                fila[fimFila++] = viz;  // Adiciona o vizinho à fila
            }
        }
    }

    // Reconstroi o caminho do destino para a origem
    pacote.rota.limpar(); // Limpa qualquer rota existente

    // Verifica se o destino foi realmente alcançado pela BFS
    if (!visitado[destino]) {
        std::cerr << "Erro: Sem caminho de " << pacote.getOrigem() << " até " << pacote.getDestino()
                  << " para o pacote " << pacote.getId() << ". Rota vazia." << std::endl;
        // A rota já está limpa, então é só retornar
        
        // Libera a memória alocada para os arrays da BFS
        delete[] anterior;
        delete[] visitado;
        delete[] fila;
        return;
    }

    // Usaremos uma lista temporária para construir o caminho inverso e depois inserí-lo
    // na rota do pacote na ordem correta.
    ListaLigada<std::string> caminhoInversoTemp;
    int curr = destino;
    while (curr != -1) {
        caminhoInversoTemp.inserir(formatarNomeArmazem(curr));
        if (curr == origem) { // Se chegamos na origem, paramos
            break;
        }
        curr = anterior[curr]; // Move para o nó anterior
    }

    // Se por algum motivo o loop acima não conseguiu rastrear até a origem (o que não deve acontecer
    // se visitado[destino] for true e a lógica BFS estiver correta), isso seria um problema.
    // Mas a verificação de visitado[destino] já trata o caso de "não há caminho".

    // Agora, insere os nós da caminhoInversoTemp (que está destino -> origem)
    // na rota do pacote (que deve ser origem -> destino).
    // Percorre a lista inversa e insere no início da rota final, ou empilha e desempilha.
    // A forma mais fácil é simplesmente adicionar na ordem inversa do que foi coletado.
    NoLista<std::string>* noTemp = caminhoInversoTemp.getCabeca();
    
    // Para inserir na ordem correta (origem -> destino) usando ListaLigada.inserir (que insere no final),
    // precisaríamos de um array temporário ou uma pilha auxiliar (já que std::stack não pode).
    // Ou podemos coletar em um array e depois inserir.

    // Coleter em um array temporário e então inserir na ordem correta
    std::string* rotaArrayFinal = new std::string[totalArmazens];
    int idxFinal = 0;
    // Percorrendo caminhoInversoTemp (destino -> origem) para preencher rotaArrayFinal (origem -> destino)
    while(noTemp != nullptr) {
        rotaArrayFinal[idxFinal++] = noTemp->dado;
        noTemp = noTemp->prox;
    }

    // Agora, inserimos em Pacote.rota na ordem inversa do rotaArrayFinal
    for (int k = idxFinal - 1; k >= 0; --k) {
        pacote.rota.inserir(rotaArrayFinal[k]);
    }
    delete[] rotaArrayFinal; // Libera o array temporário

    // Libera a memória alocada para os arrays da BFS
    delete[] anterior;
    delete[] visitado;
    delete[] fila;
}


// Carrega os dados de entrada do arquivo, aloca memória para pacotes e armazéns
// e agenda os eventos iniciais.
void carregarEntrada(const std::string& nomeArquivo,
                     Config& config,
                     Escalonador& escalonador,
                     Pacote** outPacotes, // Retorno: ponteiro para o array de Pacotes
                     int& totalPacotes,   // Retorno: número total de pacotes
                     Armazem** outArmazens, // Retorno: ponteiro para o array de Armazéns
                     int& totalArmazens)  // Retorno: número total de armazéns
{
    std::ifstream arq(nomeArquivo);
    if (!arq.is_open()) { // Usa is_open() para verificar se o arquivo abriu
        std::cerr << "Erro ao abrir arquivo: " << nomeArquivo << std::endl;
        // Não aloca nada se o arquivo não abrir, então não precisa desalocar.
        totalPacotes = 0;
        totalArmazens = 0;
        *outPacotes = nullptr;
        *outArmazens = nullptr;
        return;
    }

    // Lê as configurações iniciais
    arq >> config.tempoTransporte
        >> config.tempoManipulacao
        >> config.tempoSimulacao
        >> config.numTiposPacote
        >> config.numArmazens;

    totalArmazens = config.numArmazens; // Atualiza o parâmetro de saída

    // Aloca a matriz de adjacência (grafo)
    config.grafo = new int*[totalArmazens];
    for (int i = 0; i < totalArmazens; ++i) {
        config.grafo[i] = new int[totalArmazens];
    }

    // Lê os dados do grafo
    for (int i = 0; i < totalArmazens; ++i) {
        for (int j = 0; j < totalArmazens; ++j) {
            arq >> config.grafo[i][j];
        }
    }

    // Aloca o array de armazéns e os inicializa
    *outArmazens = new Armazem[totalArmazens];
    for (int i = 0; i < totalArmazens; ++i) {
        (*outArmazens)[i] = Armazem(formatarNomeArmazem(i));
    }

    // Adiciona as seções para cada armazém com base no grafo
    for (int i = 0; i < totalArmazens; ++i) {
        for (int j = 0; j < totalArmazens; ++j) {
            if (config.grafo[i][j] == 1) { // Se há uma conexão
                (*outArmazens)[i].adicionarSecao(formatarNomeArmazem(j));
            }
        }
    }

    int numPacotesLidos;
    arq >> numPacotesLidos;
    arq.ignore(); // Consome o restante da linha após o número de pacotes

    totalPacotes = numPacotesLidos; // Atualiza o parâmetro de saída
    *outPacotes = new Pacote[totalPacotes]; // Aloca o array de pacotes

    // Loop para ler e processar cada pacote
    for (int i = 0; i < numPacotesLidos; ++i) {
        std::string linha;
        std::getline(arq, linha);
        std::istringstream ss(linha);

        double tempoChegada; // Tempo de postagem do pacote
        std::string dummy;   // Para "pac", "org", "dst"
        int idPacote, origemId, destinoId;

        // Formato: <tempo> pac <id> org <origem> dst <destino>
        ss >> tempoChegada >> dummy >> idPacote >> dummy >> origemId >> dummy >> destinoId;

        std::string origemNome = formatarNomeArmazem(origemId);
        std::string destinoNome = formatarNomeArmazem(destinoId);

        // Inicializa o objeto Pacote no array
        (*outPacotes)[idPacote] = Pacote(idPacote, "Rem" + std::to_string(idPacote),
                                        "Dest" + std::to_string(idPacote),
                                        origemNome, destinoNome, "Normal");

        // Calcula a rota para o pacote
        calcularRota((*outPacotes)[idPacote], config.grafo, config.numArmazens);

        // Agenda o evento de chegada inicial do pacote
        escalonador.agendarEvento(Evento(tempoChegada, CHEGADA_PACOTE, idPacote));
    }

    arq.close(); // Fecha o arquivo

    // Agenda os eventos de TRANSPORTE_PROGRAMADO iniciais para todas as conexões existentes
    // Cada conexão (origem, destino) terá um evento de transporte programado periodicamente.
    for (int i = 0; i < totalArmazens; ++i) {
        for (int j = 0; j < totalArmazens; ++j) {
            if (config.grafo[i][j] == 1) { // Se há uma ligação entre i e j
                // O primeiro evento de transporte pode ser agendado em tempo 0 ou tempoTransporte
                // Usaremos tempoTransporte como um intervalo inicial para que os transportes
                // comecem a ocorrer após um tempo de "configuração" ou "dia".
                // De acordo com o pseudo-código (pg. 6), "Escalona evento de transporte" para cada ligação.
                // E "Uma vez por dia pacotes são transportados".
                // Consideraremos que `config.tempoTransporte` define este "dia" para agendamento periódico.
                escalonador.agendarEvento(Evento(config.tempoTransporte, TRANSPORTE_PROGRAMADO, i, j));
            }
        }
    }
}


// Função para logar eventos no formato especificado no documento.
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

// Encontra um armazém pelo nome (ex: "000") no array de armazéns
Armazem* encontrarArmazem(const std::string& nome, Armazem armazens[], int totalArmazens) {
    for (int i = 0; i < totalArmazens; ++i) {
        if (armazens[i].getNome() == nome) {
            return &armazens[i];
        }
    }
    return nullptr; // Armazém não encontrado
}


// Executa a simulação de eventos discretos
void executarSimulacao(Escalonador& escalonador,
                       Pacote* pacotes,
                       int totalPacotes,
                       Armazem* armazens,
                       int totalArmazens,
                       int** grafo,
                       const Config& config) {

    // Loop principal da simulação: continua enquanto houver eventos na heap
    while (escalonador.temEventos()) {
        Evento e = escalonador.proximoEvento(); // Pega o próximo evento
        escalonador.avancarTempo(e.tempo);     // Avança o relógio para o tempo do evento

        int tempoAtual = static_cast<int>(e.tempo); // Tempo atual formatado como inteiro
        
        // Processa o evento com base em seu tipo
        switch (e.tipo) {
            case CHEGADA_PACOTE: { // Evento de chegada de um pacote (postagem ou após transporte)
                int idPacote = e.pacoteId;                  // ID do pacote envolvido
                Pacote& p = pacotes[idPacote]; // Referência ao pacote

                // Verifica se a rota do pacote é válida
                if (p.rota.vazia()) {
                    // Se a rota estiver vazia, o pacote já chegou ao destino final
                    logEvento(tempoAtual, idPacote, "entregue", p.getDestino(), "");
                    p.mudarEstado(ENTREGUE);
                    continue;
                }

                NoLista<std::string>* noAtual = p.rota.getCabeca(); // Armazém atual na rota
                std::string armazemAtualNome = noAtual->dado;
                
                // O pacote acaba de chegar ao armazém 'armazemAtualNome'.
                // O próximo destino na rota (se houver) define a seção onde ele será armazenado.
                NoLista<std::string>* proximoNoRota = noAtual->prox;

                Armazem* armazemPtr = encontrarArmazem(armazemAtualNome, armazens, totalArmazens);
                if (!armazemPtr) {
                    std::cerr << "Erro: Armazém " << armazemAtualNome << " não encontrado." << std::endl;
                    continue;
                }

                if (!proximoNoRota) { // Se não há próximo nó, o pacote chegou ao destino final
                    logEvento(tempoAtual, idPacote, "entregue", armazemAtualNome, "");
                    p.mudarEstado(ENTREGUE);
                } else {
                    std::string proximoArmazemNome = proximoNoRota->dado;
                    // Armazena o pacote na seção correta do armazém atual
                    armazemPtr->armazenaPacote(idPacote, proximoArmazemNome, tempoAtual);
                    logEvento(tempoAtual, idPacote, "armazenado", armazemAtualNome, proximoArmazemNome);
                    p.mudarEstado(ARMAZENADO);
                }
                break;
            }

            case TRANSPORTE_PROGRAMADO: { // Evento de transporte programado entre dois armazéns
                int origemArmazemId = e.origemArmazemId;
                int destinoArmazemId = e.destinoArmazemId;

                Armazem* origemArmazem = &armazens[origemArmazemId];
                std::string destinoNomeSecao = formatarNomeArmazem(destinoArmazemId);

                // Re-agenda o próximo evento de TRANSPORTE_PROGRAMADO para esta rota
                escalonador.agendarEvento(Evento(tempoAtual + config.tempoTransporte, TRANSPORTE_PROGRAMADO, origemArmazemId, destinoArmazemId));

                // Seção está vazia, não há pacotes para transportar agora
                if (origemArmazem->secaoVazia(destinoNomeSecao)) {
                    continue;
                }

                // Encontra o pacote mais antigo nesta seção para transporte
                int pacoteIdParaMover = origemArmazem->encontrarPacoteMaisAntigoNaSecao(destinoNomeSecao);
                if (pacoteIdParaMover == -1) {
                    continue; // Não encontrou pacote para mover
                }

                Pacote& pacoteParaMover = pacotes[pacoteIdParaMover];
                ListaLigada<PacoteNaPilha> rearmazenadosTemp; // Lista para pacotes temporariamente movidos

                // Tenta remover o pacote mais antigo da pilha LIFO da seção,
                // simulando o desempilhamento e re-empilhamento dos pacotes que estão em cima.
                if (origemArmazem->recuperaPacoteLIFO(destinoNomeSecao, rearmazenadosTemp)) {
                    // Loga os pacotes que foram rearmazenados (movidos temporariamente)
                    NoLista<PacoteNaPilha>* noRearmazenado = rearmazenadosTemp.getCabeca();
                    while (noRearmazenado != nullptr) {
                        // Estes pacotes são logados como "rearmazenado" no tempo atual do transporte
                        logEvento(tempoAtual, noRearmazenado->dado.id, "rearmazenado", origemArmazem->getNome(), destinoNomeSecao);
                        noRearmazenado = noRearmazenado->prox;
                    }

                    // Agora o pacote desejado está 'disponível' e é movido
                    logEvento(tempoAtual, pacoteIdParaMover, "removido", origemArmazem->getNome(), destinoNomeSecao);
                    logEvento(tempoAtual, pacoteIdParaMover, "em transito", origemArmazem->getNome(), destinoNomeSecao);
                    pacoteParaMover.mudarEstado(ALOCADO_TRANSPORTE);

                    // Agenda o evento de chegada do pacote no próximo armazém
                    escalonador.agendarEvento(Evento(tempoAtual + config.tempoTransporte, CHEGADA_APOS_TRANSPORTE, pacoteIdParaMover));
                }
                break;
            }

            case CHEGADA_APOS_TRANSPORTE: { // Evento de chegada de um pacote após ter sido transportado
                int idPacote = e.pacoteId;                  // ID do pacote envolvido
                Pacote& p = pacotes[idPacote]; // Referência ao pacote

                // Remove o armazém atual da rota (onde ele acabou de chegar)
                p.rota.removerPrimeiro();
                NoLista<std::string>* proximoNoRota = p.rota.getCabeca(); // Próximo armazém na rota

                if (!proximoNoRota) { // Se a rota está vazia, o pacote chegou ao destino final
                    logEvento(tempoAtual, idPacote, "entregue", p.getDestino(), "");
                    p.mudarEstado(ENTREGUE);
                } else {
                    std::string armazemAtualNome = proximoNoRota->dado; // O pacote está agora neste armazém
                    NoLista<std::string>* proximoDestinoNaRota = proximoNoRota->prox;

                    Armazem* armazemPtr = encontrarArmazem(armazemAtualNome, armazens, totalArmazens);
                    if (!armazemPtr) {
                        std::cerr << "Erro: Armazém " << armazemAtualNome << " não encontrado." << std::endl;
                        continue;
                    }

                    if (!proximoDestinoNaRota) { // Chegou ao último armazém da rota (destino final)
                        logEvento(tempoAtual, idPacote, "entregue", armazemAtualNome, "");
                        p.mudarEstado(ENTREGUE);
                    } else {
                        // O pacote está em um armazém intermediário, precisa ser armazenado novamente.
                        std::string proximoArmazemNome = proximoDestinoNaRota->dado;
                        armazemPtr->armazenaPacote(idPacote, proximoArmazemNome, tempoAtual);
                        logEvento(tempoAtual, idPacote, "armazenado", armazemAtualNome, proximoArmazemNome);
                        p.mudarEstado(ARMAZENADO);
                    }
                }
                break;
            }
        }
    }
}