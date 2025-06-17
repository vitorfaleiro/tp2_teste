// include/transporte.h
#ifndef TRANSPORTE_H
#define TRANSPORTE_H

// Enumeração para os tipos de evento
enum EventType {
    CHEGADA_PACOTE,             // Pacote chega no armazém (postagem ou após transporte)
    TRANSPORTE_PROGRAMADO,      // Evento que aciona o processo de transporte entre dois armazéns
    CHEGADA_APOS_TRANSPORTE     // Pacote chega ao próximo armazém em sua rota
};

// Estrutura para representar um evento no escalonador
struct Evento {
    double tempo;               // Tempo de ocorrência do evento
    EventType tipo;             // Tipo do evento
    int pacoteId;               // ID do pacote relacionado ao evento
    int origemArmazemId;        // ID do armazém de origem (usado em TRANSPORTE_PROGRAMADO)
    int destinoArmazemId;       // ID do armazém de destino (usado em TRANSPORTE_PROGRAMADO e CHEGADA_APOS_TRANSPORTE)

    // Construtor padrão
    Evento() : tempo(0.0), tipo(CHEGADA_PACOTE), pacoteId(-1), origemArmazemId(-1), destinoArmazemId(-1) {}

    // Construtor para eventos de pacote
    Evento(double t, EventType tp, int pId) : tempo(t), tipo(tp), pacoteId(pId), origemArmazemId(-1), destinoArmazemId(-1) {}

    // Construtor para eventos de transporte
    Evento(double t, EventType tp, int oId, int dId) : tempo(t), tipo(tp), pacoteId(-1), origemArmazemId(oId), destinoArmazemId(dId) {}
};

#endif // TRANSPORTE_H