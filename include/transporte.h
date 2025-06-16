#ifndef TRANSPORTE_H
#define TRANSPORTE_H

struct Evento {
    double tempo;    // Tempo do evento
    int tipo;        // Tipo: 0 - chegada, 1 - remoção, 2 - chegada após transporte
    int pacoteId;    // ID do pacote relacionado ao evento
};

#endif
