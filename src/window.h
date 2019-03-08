#ifndef WINDOW_H
#define WINDOW_H

#include <ncurses.h>
#include "downloader.h"

using namespace std;

void start();

//Cria uma janela, com altura, largura, e começando em (x,y)
WINDOW *create_win(int alt, int larg, int y, int x);

//preenche os menus
void makeMenu(WINDOW *menuSites, WINDOW *menuMedias, WINDOW *header, string siteURL);

//Faz o update da janela que informa o site toda vez que o site muda
void updateHeader(WINDOW *header, string site);

//Faz o update de todas as janelas
void refresh_wins(WINDOW *header, WINDOW *menuSites, WINDOW *menuMedias);

//Faz o menu dos sites atualizar quando o usuario vai pra cima ou pra baixo 
//com as teclas
void printSitesMenu(WINDOW *menuSites, lista *rootSite, int highlight);
#endif
