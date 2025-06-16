#ifndef PILHA_H
#define PILHA_H

class Pilha {
private:
    int elementos[1000];
    int topo;

public:
    Pilha();
    void empilhar(int valor);
    int desempilhar();
    int topoElemento() const;
    bool vazia() const;
};

#endif
