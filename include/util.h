// include/util.h
#ifndef UTIL_H
#define UTIL_H

#include <string> // Para std::string

// Formata um ID de armazém para uma string com 3 dígitos (ex: 0 -> "000")
std::string formatarNomeArmazem(int id);

// Converte um nome de armazém formatado ("000", "001") de volta para int
int desformatarNomeArmazem(const std::string& nome);

#endif // UTIL_H