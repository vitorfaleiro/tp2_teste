// src/simulador.cpp
#include "simulador.h" // Agora inclui RawPacoteData via simulador.h
#include "util.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

// Calcula a rota mais curta para um pacote usando Busca em Largura (BFS)
void calcularRota(Pacote& pacote, int** grafo, int totalArmazens) {
    int origem = desformatarNomeArmazem(pacote.getOrigem());
    int destino = desformatarNomeArmazem(pacote.getDestino());

    if (origem < 0 || origem >= totalArmazens || destino < 0 || destino >= totalArmazens) {
        std::cerr << "Erro: Origem ou destino inválidos para o pacote " << pacote.getId() << std::endl;
        pacote.rota.limpar();
        return;
    }

    int* anterior = new int[totalArmazens];
    bool* visitado = new bool[totalArmazens];
    int* fila = new int[totalArmazens];

    for (int i = 0; i < totalArmazens; i++) {
        anterior[i] = -1;
        visitado[i] = false;
    }

    int inicioFila = 0, fimFila = 0;
    fila[fimFila++] = origem;
    visitado[origem] = true;

    while (inicioFila < fimFila) {
        int atual = fila[inicioFila++];
        if (atual == destino) {
            break;
        }

        for (int viz = 0; viz < totalArmazens; viz++) {
            if (grafo[atual][viz] == 1 && !visitado[viz]) {
                anterior[viz] = atual;
                visitado[viz] = true;
                fila[fimFila++] = viz;
            }
        }
    }

    pacote.rota.limpar();

    if (!visitado[destino]) {
        std::cerr << "Erro: Sem caminho de " << pacote.getOrigem() << " até " << pacote.getDestino()
                  << " para o pacote " << pacote.getId() << ". Rota vazia." << std::endl;
        
        delete[] anterior;
        delete[] visitado;
        delete[] fila;
        return;
    }

    std::string* rotaArrayTemp = new std::string[totalArmazens];
    int idxTemp = 0;
    int curr = destino;
    while (curr != -1) {
        rotaArrayTemp[idxTemp++] = formatarNomeArmazem(curr);
        if (curr == origem) {
            break;
        }
        curr = anterior[curr];
    }

    for (int k = idxTemp - 1; k >= 0; --k) {
        pacote.rota.inserir(rotaArrayTemp[k]);
    }
    delete[] rotaArrayTemp;

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
                     int& totalPacotes,   // Retorno: número total de pacotes (passará a ser max_id + 1)
                     Armazem** outArmazens, // Retorno: ponteiro para o array de Armazéns
                     int& totalArmazens)  // Retorno: número total de armazéns
{
    std::cerr << "Tentando abrir arquivo: " << nomeArquivo << std::endl;
    std::ifstream arq(nomeArquivo);

    if (!arq.is_open()) {
        std::cerr << "Erro FATAL: Nao foi possivel abrir o arquivo: " << nomeArquivo << std::endl;
        totalPacotes = 0;
        totalArmazens = 0;
        *outPacotes = nullptr;
        *outArmazens = nullptr;
        return;
    }
    std::cerr << "Arquivo '" << nomeArquivo << "' aberto com sucesso." << std::endl;

    if (arq.peek() == std::ifstream::traits_type::eof()) {
        std::cerr << "Erro: O arquivo de entrada '" << nomeArquivo << "' esta vazio." << std::endl;
        arq.close();
        totalPacotes = 0;
        totalArmazens = 0;
        *outPacotes = nullptr;
        *outArmazens = nullptr;
        return;
    }
    std::cerr << "Arquivo nao esta vazio. Prosseguindo com a leitura." << std::endl;

    // --- Primeira parte: Leitura das configurações e grafo ---
    arq >> config.tempoTransporte
        >> config.tempoManipulacao
        >> config.tempoSimulacao
        >> config.numTiposPacote
        >> config.numArmazens;

    if (arq.fail()) {
        std::cerr << "Erro: Falha na leitura inicial das configuracoes do arquivo. O formato pode estar incorreto." << std::endl;
        arq.close();
        totalPacotes = 0; totalArmazens = 0; *outPacotes = nullptr; *outArmazens = nullptr;
        return;
    }
    std::cerr << "Configuracoes lidas: TT=" << config.tempoTransporte << ", TM=" << config.tempoManipulacao
              << ", TS=" << config.tempoSimulacao << ", NT=" << config.numTiposPacote << ", NA=" << config.numArmazens << std::endl;

    totalArmazens = config.numArmazens;

    config.grafo = new int*[totalArmazens];
    for (int i = 0; i < totalArmazens; ++i) {
        config.grafo[i] = new int[totalArmazens];
    }

    for (int i = 0; i < totalArmazens; ++i) {
        for (int j = 0; j < totalArmazens; ++j) {
            arq >> config.grafo[i][j];
            if (arq.fail()) {
                std::cerr << "Erro: Falha ao ler grafo na posicao [" << i << "][" << j << "]. Arquivo malformado?" << std::endl;
                arq.close();
                for (int k = 0; k < i; ++k) delete[] config.grafo[k];
                delete[] config.grafo; config.grafo = nullptr;
                totalPacotes = 0; totalArmazens = 0; *outPacotes = nullptr; *outArmazens = nullptr;
                return;
            }
        }
    }
    std::cerr << "Grafo lido com sucesso." << std::endl;

    *outArmazens = new Armazem[totalArmazens];
    for (int i = 0; i < totalArmazens; ++i) {
        (*outArmazens)[i] = Armazem(formatarNomeArmazem(i));
    }
    std::cerr << "Armazens inicializados." << std::endl;

    for (int i = 0; i < totalArmazens; ++i) {
        for (int j = 0; j < totalArmazens; ++j) {
            if (config.grafo[i][j] == 1) {
                (*outArmazens)[i].adicionarSecao(formatarNomeArmazem(j));
            }
        }
    }
    std::cerr << "Secoes adicionadas aos armazens." << std::endl;


    int numPacotesReaisLidos; // Quantidade de pacotes no arquivo (count)
    arq >> numPacotesReaisLidos;
    if (arq.fail()) {
        std::cerr << "Erro: Falha na leitura do numero de pacotes. Arquivo malformado?" << std::endl;
        arq.close();
        delete[] *outArmazens; *outArmazens = nullptr;
        totalPacotes = 0; totalArmazens = 0; *outPacotes = nullptr;
        return;
    }
    arq.ignore(); // Consome o restante da linha após o número de pacotes
    std::cerr << "Numero TOTAL de pacotes a serem processados: " << numPacotesReaisLidos << std::endl;
    config.totalPacotesIniciais = numPacotesReaisLidos; // Armazena o total real de pacotes para a condição de parada


    // --- Segunda parte: Determinar o maior ID de pacote ---
    // Precisamos de uma lista temporária para armazenar os dados brutos dos pacotes
    // porque vamos precisar do max_id_pacote antes de alocar o array final.
    ListaLigada<RawPacoteData> rawPacotesList; // Usando ListaLigada, que é permitida
    int maxIdPacote = -1;

    // Salva a posição atual no arquivo para rebobinar depois
    std::streampos pacotesStartPos = arq.tellg(); 

    for (int i = 0; i < numPacotesReaisLidos; ++i) {
        std::string linha;
        if (!std::getline(arq, linha)) {
            std::cerr << "Erro: Falha ao ler linha do pacote " << i << " durante a primeira passagem. Arquivo terminou inesperadamente." << std::endl;
            arq.close(); // Fecha o arquivo para evitar problemas
            // Realocar / limpar memória previamente alocada.
            delete[] *outArmazens; *outArmazens = nullptr;
            totalPacotes = 0; totalArmazens = 0; *outPacotes = nullptr;
            return;
        }
        std::istringstream ss(linha);

        RawPacoteData rawData;
        std::string dummy;
        
        ss >> rawData.tempo >> dummy >> rawData.id >> dummy >> rawData.origemId >> dummy >> rawData.destinoId;
        rawData.linhaOriginal = linha;

        if (ss.fail()) {
            std::cerr << "Erro: Falha ao parsear dados do pacote na linha: '" << linha << "' durante a primeira passagem. Formato incorreto." << std::endl;
            arq.close();
            delete[] *outArmazens; *outArmazens = nullptr;
            totalPacotes = 0; totalArmazens = 0; *outPacotes = nullptr;
            return;
        }

        if (rawData.id > maxIdPacote) {
            maxIdPacote = rawData.id;
        }
        rawPacotesList.inserir(rawData);
    }
    std::cerr << "Primeira passagem completa. Maior ID de pacote encontrado: " << maxIdPacote << std::endl;

    // --- Alocar o array de Pacotes com base no maior ID ---
    totalPacotes = maxIdPacote + 1; // O tamanho do array será max_id + 1
    // Se não há pacotes ou maxIdPacote é -1, definimos um tamanho mínimo para evitar alocação de 0 ou -1.
    if (totalPacotes <= 0) {
        totalPacotes = 1; // Garante um array de pelo menos tamanho 1 se não houver pacotes.
                          // Embora a simulação não ocorra, evita alocações de tamanho 0.
    }
    *outPacotes = new Pacote[totalPacotes]; // Aloca o array de pacotes com o tamanho correto
    std::cerr << "Array 'pacotes' alocado com tamanho: " << totalPacotes << std::endl;


    // --- Terceira parte: Reiniciar a leitura dos pacotes e preencher o array ---
    // Rebobinar o arquivo para a posição onde os pacotes começam
    arq.clear(); // Limpa quaisquer flags de erro do stream
    arq.seekg(pacotesStartPos); // Volta para o início da seção de pacotes
    std::cerr << "Rebobinando arquivo para ler pacotes novamente." << std::endl;


    // Agora, itera sobre a lista de RawPacoteData para preencher o array Pacote
    NoLista<RawPacoteData>* currentRawPacote = rawPacotesList.getCabeca();
    while (currentRawPacote != nullptr) {
        RawPacoteData& rawData = currentRawPacote->dado;

        std::string origemNome = formatarNomeArmazem(rawData.origemId);
        std::string destinoNome = formatarNomeArmazem(rawData.destinoId);

        // Preenche o objeto Pacote no array na posição do seu ID
        // Não é mais necessário verificar idPacote < 0 || idPacote >= totalPacotes aqui,
        // pois totalPacotes foi definido com base no maxIdPacote.
        (*outPacotes)[rawData.id] = Pacote(rawData.id, "Rem" + std::to_string(rawData.id),
                                        "Dest" + std::to_string(rawData.id),
                                        origemNome, destinoNome, "Normal");
        std::cerr << "Pacote " << rawData.id << " inicializado no indice " << rawData.id << "." << std::endl;


        calcularRota((*outPacotes)[rawData.id], config.grafo, config.numArmazens);
        std::cerr << "Rota calculada para pacote " << rawData.id << ". Rota: ";
        NoLista<std::string>* rotaNo = (*outPacotes)[rawData.id].rota.getCabeca();
        while(rotaNo) {
            std::cerr << rotaNo->dado << " ";
            rotaNo = rotaNo->prox;
        }
        std::cerr << std::endl;

        escalonador.agendarEvento(Evento(rawData.tempo, CHEGADA_PACOTE, rawData.id));
        std::cerr << "Evento de chegada agendado para pacote " << rawData.id << " no tempo " << rawData.tempo << std::endl;
        
        currentRawPacote = currentRawPacote->prox;
    }
    rawPacotesList.limpar(); // Libera a memória da lista temporária de dados brutos
    std::cerr << "Todos os pacotes lidos, inicializados e eventos de chegada agendados." << std::endl;

    arq.close(); // Fecha o arquivo
    std::cerr << "Arquivo de entrada fechado." << std::endl;


    // Agenda os eventos de TRANSPORTE_PROGRAMADO iniciais para todas as conexões existentes
    // Modificado para usar config.tempoSimulacao como ponto de partida para o primeiro dia de transporte,
    // e config.tempoTransporte como o intervalo diário subsequente.
    for (int i = 0; i < totalArmazens; ++i) {
        for (int j = 0; j < totalArmazens; ++j) {
            if (config.grafo[i][j] == 1) { // Se há uma ligação entre i e j
                // O primeiro transporte para cada rota ocorre no tempo_simulacao.
                escalonador.agendarEvento(Evento(config.tempoSimulacao, TRANSPORTE_PROGRAMADO, i, j));
                std::cerr << "Evento TRANSPORTE_PROGRAMADO inicial agendado para " << formatarNomeArmazem(i)
                          << " para " << formatarNomeArmazem(j) << " no tempo " << config.tempoSimulacao << std::endl;
            }
        }
    }
    std::cerr << "Eventos de transporte programados iniciais agendados." << std::endl;
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
    return nullptr;
}


// Executa a simulação de eventos discretos
void executarSimulacao(Escalonador& escalonador,
                       Pacote* pacotes,
                       int totalPacotes, // Mantido, mas totalPacotesIniciais em Config é o que controla
                       Armazem* armazens,
                       int totalArmazens,
                       int** grafo, // Mantido, mas não usado diretamente aqui
                       Config& config) { // Agora `config` é uma referência mutável

    // Loop principal da simulação: continua enquanto houver eventos na heap
    // E a quantidade de pacotes entregues for menor que o total de pacotes a serem entregues
    while (escalonador.temEventos() && config.pacotesEntreguesCount < config.totalPacotesIniciais) {
        Evento e = escalonador.proximoEvento(); // Pega o próximo evento
        escalonador.avancarTempo(e.tempo);     // Avança o relógio para o tempo do evento

        int tempoAtual = static_cast<int>(e.tempo);
        
        switch (e.tipo) {
            case CHEGADA_PACOTE: { // Evento de chegada de um pacote (postagem ou após transporte)
                int idPacote = e.pacoteId;
                Pacote& p = pacotes[idPacote]; // Referência ao pacote

                // A rota deve ter pelo menos um nó (o armazém atual)
                NoLista<std::string>* noAtual = p.rota.getCabeca();
                if (!noAtual) {
                    std::cerr << "Aviso: Pacote " << idPacote << " chegou mas rota esta vazia. Entregue antecipadamente." << std::endl;
                    logEvento(tempoAtual, idPacote, "entregue", p.getDestino(), "");
                    p.mudarEstado(ENTREGUE);
                    config.pacotesEntreguesCount++; // Incrementa o contador de entregues
                    continue;
                }

                std::string armazemAtualNome = noAtual->dado;
                
                // O próximo destino na rota (se houver) define a seção onde ele será armazenado.
                NoLista<std::string>* proximoNoRota = noAtual->prox;

                Armazem* armazemPtr = encontrarArmazem(armazemAtualNome, armazens, totalArmazens);
                if (!armazemPtr) {
                    std::cerr << "Erro: Armazém " << armazemAtualNome << " não encontrado para pacote " << idPacote << std::endl;
                    continue;
                }

                if (!proximoNoRota) { // Se não há próximo nó, o pacote chegou ao destino final
                    logEvento(tempoAtual, idPacote, "entregue", armazemAtualNome, "");
                    p.mudarEstado(ENTREGUE);
                    config.pacotesEntreguesCount++; // Incrementa o contador de entregues
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
                // AGORA: Re-agenda para tempoAtual + config.tempoTransporte (como intervalo diário)
                escalonador.agendarEvento(Evento(tempoAtual + config.tempoTransporte, TRANSPORTE_PROGRAMADO, origemArmazemId, destinoArmazemId));
                
                // Loop para tentar remover e transportar TODOS os pacotes elegíveis desta seção
                // no tempo atual. A "capacidade do transporte" é considerada ilimitada para este tick,
                // mas apenas um pacote é movido por vez para aplicar o tempoManipulacao.
                // Se houvesse capacidade definida (ex: 3 pacotes por dia), seria um loop `for (int k=0; k < capacidade && !secaoVazia; k++)`.
                // Como não há, vamos mover APENAS O MAIS ANTIGO por TRANSPORTE_PROGRAMADO.
                // A interpretação do "Remove os pacotes mais antigos até a capacidade do transporte" pode ser que um evento move um,
                // e outro evento move outro, ou um evento move vários. O output parece indicar que um evento move 1 mais antigo.
                // Vamos tentar com 1 por evento para evitar a explosão de logs.

                // Verificação se a seção tem pacotes armazenados para este destino
                if (!origemArmazem->secaoVazia(destinoNomeSecao)) {
                    int pacoteIdParaMover = origemArmazem->encontrarPacoteMaisAntigoNaSecao(destinoNomeSecao);
                    
                    if (pacoteIdParaMover == -1) {
                        // Se encontrarPacoteMaisAntigoNaSecao retornou -1, a seção está logicamente vazia.
                        break; // Sai do case TRANSPORTE_PROGRAMADO
                    }

                    Pacote& pacoteParaMover = pacotes[pacoteIdParaMover];
                    
                    // O pacote mais antigo encontrado na pilha deve estar no estado ARMAZENADO para ser movido.
                    if (pacoteParaMover.getEstado() != ARMAZENADO) {
                         std::cerr << "Aviso: Pacote " << pacoteIdParaMover << " encontrado como 'mais antigo' em " 
                                  << origemArmazem->getNome() << " secao " << destinoNomeSecao
                                  << " mas estado eh " << pacoteParaMover.getEstado() << " (nao ARMAZENADO). Pulando e nao tentando mover mais nesta rota neste tick." << std::endl;
                        break; // Sai do case TRANSPORTE_PROGRAMADO para esta rota neste tick.
                    }

                    ListaLigada<PacoteNaPilha> rearmazenadosTemp; // Lista para pacotes temporariamente movidos

                    // Tenta remover o pacote mais antigo da pilha LIFO da seção.
                    // Esta função preencherá 'rearmazenadosTemp' com os pacotes que estavam acima do alvo.
                    if (origemArmazem->recuperaPacoteLIFO(destinoNomeSecao, rearmazenadosTemp)) {
                        // Calcular o tempo de manipulação
                        // Tempo total de manipulação = (número de pacotes rearmazenados + o próprio pacote a ser movido) * tempoManipulacao
                        // Correção: A especificação do problema diz: "Tempo de manipulação de pacote na respectiva sala do armazém."
                        // Isso é por pacote. O output de exemplo parece indicar que todos os custos são somados ao tempo de saída.
                        
                        int numRearmazenados = 0;
                        NoLista<PacoteNaPilha>* tempNode = rearmazenadosTemp.getCabeca();
                        while(tempNode != nullptr) {
                            numRearmazenados++;
                            tempNode = tempNode->prox;
                        }
                        // Custo de manipulacao total é (pacotes rearmazenados + pacote alvo) * custo por pacote
                        double custoManipulacaoTotal = (numRearmazenados + 1) * config.tempoManipulacao; 

                        // Loga os pacotes que foram rearmazenados (movidos temporariamente para acessar o alvo)
                        // Estes são logados no tempo atual do evento TRANSPORTE_PROGRAMADO
                        NoLista<PacoteNaPilha>* noRearmazenado = rearmazenadosTemp.getCabeca();
                        while (noRearmazenado != nullptr) {
                            logEvento(tempoAtual, noRearmazenado->dado.id, "rearmazenado", origemArmazem->getNome(), destinoNomeSecao);
                            noRearmazenado = noRearmazenado->prox;
                        }
                        
                        // O tempo em que o pacote alvo *realmente* sai do armazém é afetado pelo tempo de manipulação.
                        // Adicionamos o custo de manipulação total ao tempo atual do evento de transporte.
                        double tempoSaidaReal = tempoAtual + custoManipulacaoTotal;

                        // O pacote 'pacoteIdParaMover' foi efetivamente removido da pilha neste ponto.
                        logEvento(static_cast<int>(tempoSaidaReal), pacoteIdParaMover, "removido", origemArmazem->getNome(), destinoNomeSecao);
                        logEvento(static_cast<int>(tempoSaidaReal), pacoteIdParaMover, "em transito", origemArmazem->getNome(), destinoNomeSecao);
                        pacoteParaMover.mudarEstado(ALOCADO_TRANSPORTE);

                        // Agenda o evento de chegada do pacote no próximo armazém
                        // Tempo de chegada = tempo de saída real + duração do transporte (config.tempoTransporte)
                        escalonador.agendarEvento(Evento(tempoSaidaReal + config.tempoTransporte, CHEGADA_APOS_TRANSPORTE, pacoteIdParaMover));
                        
                    } else {
                        std::cerr << "Erro: Falha ao recuperar pacote " << pacoteIdParaMover << " da seção " 
                                  << destinoNomeSecao << " no armazém " << origemArmazem->getNome() << std::endl;
                    }
                } // Fim do if !secaoVazia
                break;
            }

            case CHEGADA_APOS_TRANSPORTE: { // Evento de chegada de um pacote após ter sido transportado
                int idPacote = e.pacoteId;
                Pacote& p = pacotes[idPacote];

                // Remove o armazém atual da rota (onde ele acabou de chegar).
                // Se a rota estiver vazia aqui, indica um problema lógico na rota ou no agendamento.
                if (p.rota.vazia()) {
                    std::cerr << "Aviso: Pacote " << idPacote << " chegou ao armazem, mas rota vazia inesperadamente. Entregue." << std::endl;
                    logEvento(tempoAtual, idPacote, "entregue", p.getDestino(), ""); // Usa o destino final original
                    p.mudarEstado(ENTREGUE);
                    config.pacotesEntreguesCount++; // Incrementa o contador de entregues
                    continue;
                }
                
                p.rota.removerPrimeiro(); // O primeiro nó da rota (o armazém de onde o pacote veio) é removido.
                                         // Agora, a cabeça da rota aponta para o armazém atual do pacote.

                NoLista<std::string>* proximoNoRota = p.rota.getCabeca(); // Este é o *novo* armazém atual

                if (!proximoNoRota) { // Se a rota está vazia após remover o nó anterior, chegou ao destino final
                    logEvento(tempoAtual, idPacote, "entregue", p.getDestino(), ""); // Usa o destino final original
                    p.mudarEstado(ENTREGUE);
                    config.pacotesEntreguesCount++; // Incrementa o contador de entregues
                } else {
                    std::string armazemAtualNome = proximoNoRota->dado; // O pacote está agora neste armazém
                    NoLista<std::string>* proximoDestinoNaRota = proximoNoRota->prox; // O próximo destino a partir deste armazém

                    Armazem* armazemPtr = encontrarArmazem(armazemAtualNome, armazens, totalArmazens);
                    if (!armazemPtr) {
                        std::cerr << "Erro: Armazém " << armazemAtualNome << " não encontrado para pacote " << idPacote << " apos transporte." << std::endl;
                        continue;
                    }

                    if (!proximoDestinoNaRota) { // Se não há próximo destino na rota, chegou ao armazém final
                        logEvento(tempoAtual, idPacote, "entregue", armazemAtualNome, "");
                        p.mudarEstado(ENTREGUE);
                        config.pacotesEntreguesCount++; // Incrementa o contador de entregues
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