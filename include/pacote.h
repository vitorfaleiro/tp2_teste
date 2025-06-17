// include/pacote.h
#ifndef PACOTE_H
#define PACOTE_H

#include <string>
#include "lista_ligada.h" // Inclui ListaLigada para a rota do pacote

// Enumeração para os estados de um pacote
enum Estado {
    NAO_POSTADO,            // Não usado diretamente na simulação, mas definido
    CHEGADA_ESCALONADA,     // Não usado diretamente na simulação, mas definido
    CHEGOU_NAO_ARMAZENADO,  // Não usado diretamente na simulação, mas definido
    ARMAZENADO,             // Pacote está armazenado em um armazém
    ALOCADO_TRANSPORTE,     // Pacote foi removido e está em trânsito
    ENTREGUE                // Pacote chegou ao destino final
};

// Declaração da classe Pacote
class Pacote {
private:
    int id;                 // ID único do pacote
    std::string remetente;  // Nome do remetente
    std::string destinatario; // Nome do destinatário
    std::string origem;     // Armazém de origem inicial (string formatada)
    std::string destino;    // Armazém de destino final (string formatada)
    std::string tipo;       // Tipo do pacote (e.g., "Normal")
    Estado estado;          // Estado atual do pacote

public:
    ListaLigada<std::string> rota; // Rota do pacote como uma lista ligada de nomes de armazéns

    Pacote(); // Construtor padrão
    // Construtor com parâmetros para inicializar um pacote
    Pacote(int id, const std::string& rem, const std::string& dest, const std::string& origem, const std::string& destino, const std::string& tipo);

    // Métodos getters para acessar as propriedades do pacote
    int getId() const;
    std::string getOrigem() const; // Origem inicial
    std::string getDestino() const; // Destino final
    Estado getEstado() const;

    // Método para mudar o estado do pacote
    void mudarEstado(Estado novoEstado);
};

#endif // PACOTE_H