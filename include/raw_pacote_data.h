// include/raw_pacote_data.h
#ifndef RAW_PACOTE_DATA_H
#define RAW_PACOTE_DATA_H

#include <string> // Para std::string dentro da struct

// Estrutura temporária para armazenar dados brutos de pacotes durante a primeira passagem
struct RawPacoteData {
    double tempo;
    int id;
    int origemId;
    int destinoId;
    std::string linhaOriginal; // Opcional, para depuração
    // Construtor padrão para garantir inicialização de string
    RawPacoteData() : tempo(0.0), id(-1), origemId(-1), destinoId(-1), linhaOriginal("") {}
};

#endif // RAW_PACOTE_DATA_H