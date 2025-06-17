// src/util.cpp
#include "util.h"
#include <sstream> // Para std::ostringstream
#include <iomanip> // Para std::setfill, std::setw
#include <string>  // Para std::stoi

// Formata um ID de armazém inteiro para uma string de três dígitos com zeros à esquerda
// Ex: 0 -> "000", 1 -> "001", 10 -> "010"
std::string formatarNomeArmazem(int id) {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(3) << id;
    return oss.str();
}

// Converte um nome de armazém formatado ("000", "001") de volta para um inteiro
int desformatarNomeArmazem(const std::string& nome) {
    // std::stoi converte string para int. Ele ignorará zeros à esquerda.
    return std::stoi(nome);
}