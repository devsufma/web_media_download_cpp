#include "lista.h"

//Faz um novo node na lista
lista* novoNode(string url) {
    lista* node = new lista;

    if(node) {
        node->url = url;
        node->next = NULL;
    }
    return node;
}

void addNode(lista **head, string url) {
    lista *nNode = novoNode(url);

    if(head == NULL) {
        *head = nNode;
        return;
    }
    nNode->url = url;
    nNode->next = *head;
    *head = nNode;
}

void printNodes(lista *head) {
    lista *crawl = head;
    int i = 1;

    while(crawl) {
        i++;
        crawl=crawl->next;
    }
}

int countNodes(lista *head) {
    lista *crawl = head;
    int i = 0;
    while(crawl) {
        i++;
        crawl = crawl->next;
    }
    return i;
}


string getUrl(lista *head, int index) {
    lista *crawl = head;
    int i = 1;
    while(crawl) {
        if(i == index)
            return crawl->url;
        else
            i++;
            crawl = crawl->next;
    }
}

void apagarLista(lista **head) {
    lista *tmp = *head;

    while(tmp != NULL) {
        *head = tmp->next;
        delete [] tmp;
        tmp = *head;
    }
}
