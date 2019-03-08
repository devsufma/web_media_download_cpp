#ifndef LISTA_H
#define LISTA_H

#include <string>
#include <cstring>
#include <iostream>

using namespace std;

typedef struct _lista {
    std::string url;
    _lista *next;
} lista;

//retorna um novo node ja com a url preenchida
lista* novoNode(string url);
//Adiciona o node no final da lista
void addNode(lista **head, string url);
//Imprime todos os nodes
void printNodes(lista *head);
//Retorna quantos nodes a lista tem
int countNodes(lista *head);
//Pega a url do node index
string getUrl(lista *head, int index);
//Apaga a lista
void apagarLista(lista **head);

#endif
