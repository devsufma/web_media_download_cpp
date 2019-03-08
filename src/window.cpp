#include "window.h"
#include "downloader.h"
#include <signal.h>

void start() {
    WINDOW *header; //Parte de cima da janela, que informa o site atual
    WINDOW *menuSites; //Parte que contém os sites e o menu
    WINDOW *menuSitesBorder; //borda de menuSites
    WINDOW *menuMedias; //Parte que contem as midias baixadas no site atual
    WINDOW *menuMediasBorder; //borda de menuMedias

    string input, file; //Site e Arquivo do site
    string primSite = "";
    char str[80];

    initscr(); //função que inicua nCurses
    signal(SIGWINCH, NULL); 
    /*quando a janela muda de tamanho, o programa
    recebe o sinal SIGWINCH, que tem que ser ignorado, se não ncurses
    sai de execução*/
    if(has_colors() == FALSE) { //Checa se terminal suporta cores
        endwin();
        cout << "Seu terminal não suporta cores.\n";
        exit(1);
    }
    start_color(); //Inicia cores

    //Inicia os pares de cores, sendo primeiro a cor das letras, e depois a cor do fundo
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);

    header = create_win(3, COLS, 0, 0); //Inicia a janela header
    box(header, '|', '-'); //Seta as bordas da janela


    //LINES e COLS é definido em curses.h, são a quantidade de linhas e colunas da janela do terminal todo
    menuSitesBorder = create_win(LINES-3, COLS/2, 3, 0);
    menuSites = create_win(LINES-5, COLS/2-2, 4, 1);
    box(menuSitesBorder, '|', '-');
    keypad(menuSites, TRUE); //mesma coisa de antes, só que pra janela menuSites

    menuMediasBorder = create_win(LINES-3, COLS/2, 3, COLS/2);
    menuMedias = create_win(LINES-5, COLS/2-2, 4, COLS/2+1);
    box(menuMediasBorder, '|', '-');

    //Refresh em todas as janelas(tem que fazer toda vez que quiser mostrar uma mudança)
    refresh_wins(header, menuSitesBorder, menuMediasBorder);
    refresh_wins(header, menuSites, menuMedias);

    //Input do site
    //getmaxx() return o numero de colunas da janela passada, ai divide por 6
    //pra "centralizar"
    //print na janela 'menuSites', linha '0', coluna 'getmaxx(menuSites)/6', a string
    mvwprintw(menuSites, 0, getmaxx(menuSites)/6, "%s", "Digite o site: ");
    //Get string (input), da janela 'menuSites', e botar na string 'str'
    wgetstr(menuSites, str);

    //Atualiza o site do header
    primSite = str;
    updateHeader(header, primSite);
    refresh_wins(header, menuSites, menuMedias);

    makeMenu(menuSites, menuMedias, header, primSite);
    refresh_wins(header, menuSites, menuMedias);
}

void makeMenu(WINDOW *menuSites, WINDOW *menuMedias, WINDOW *header, string siteURL) {
    //Linked Lists com as medias e os sites do site atual(td site tem uma lista propia)
    lista *rootSite = NULL;
    lista *rootMedia = NULL;

    int i = 0;
    string file = UrlToFile(siteURL);
    int largSites, largMedias;
    int highlight = 1; //Determina qual site destacar
    int c, choice = 0; //c é pra receber um char qualquer, choice é o site escolhido no menu

    //Limpa as janelas
    wclear(menuSites);
    wclear(menuMedias);

    if(downloadPage(file, siteURL, &rootSite, &rootMedia)) {
        mvwprintw(menuMedias, 0, 0, "%s", "Medias encontradas: ");
        int sizeMedias = countNodes(rootMedia);
        //Da print das medias
        for(i = 1; i < sizeMedias; i++) {
            wattrset(menuMedias, COLOR_PAIR(1) | A_BOLD);
            mvwprintw(menuMedias, i, 0, "%d: ", i);
            wattrset(menuMedias, COLOR_PAIR(2));
            mvwprintw(menuMedias, i, 3, "%s", getUrl(rootMedia, i).c_str());
        }
        mvwprintw(menuSites, 0, 0, "%s", "Sites encontrados: ");

    } else {
        mvwprintw(menuSites, 0, getmaxx(menuSites)/6 , "%s", "Erro ao abrir o arquivo.");
    }
    wrefresh(menuMedias);

    int sizeMenu = countNodes(rootSite);
    sizeMenu++;

    printSitesMenu(menuSites, rootSite, highlight);
    while(1) {
        c = wgetch(menuSites);
        switch(c) {
            //Se pressionar seta pra cima, faz destacar o item de cima
            case KEY_UP:
                if(highlight == 1)
                    highlight = sizeMenu+1;
                else
                    highlight--;
                break;
            //mesma coisa, só que pra baixo
            case KEY_DOWN:
                if(highlight == sizeMenu+1)
                    highlight = 1;
                else
                    highlight++;
                break;
            //character ascii pra enter
            case 10:
                choice = highlight;
                break;
        }
        printSitesMenu(menuSites, rootSite, highlight);
        if(choice != 0)
            break;
    }

    //se selecionou Sair no menu
    if(choice == sizeMenu+1) {
        delete rootSite;
        delete rootMedia;
        return;
    } else {
        //se selecionou outro site no menu
        updateHeader(header, getUrl(rootSite, choice-1));
        wrefresh(header);
        makeMenu(menuSites, menuMedias, header, getUrl(rootSite, choice-1)); //Faz um menu pro novo site
    }
    apagarLista(&rootSite);
    apagarLista(&rootMedia);
    delete rootSite;
    delete rootMedia;
}

//Faz o menu da janela de sites
void printSitesMenu(WINDOW *menuSites, lista *rootSite, int highlight) {
    int size = countNodes(rootSite);
    int i;

    //Imprimir o menu, com o item 'highlight' destacado
    for(i = 1; i <= size+1; i++) {
        if(i == size+1) {
            if((i+1) == highlight)
                wattron(menuSites, A_REVERSE);
            else
                wattroff(menuSites, A_REVERSE);

            wattron(menuSites, COLOR_PAIR(1) | A_BOLD);
            mvwprintw(menuSites, i, 0, "%d: ", i);
            wattroff(menuSites,A_BOLD);
            wattron(menuSites, COLOR_PAIR(2));
            mvwprintw(menuSites, i, 3, "%s", "Sair.");
        } else {
            if((i+1) == highlight)
                wattron(menuSites, A_REVERSE);
            else
                wattroff(menuSites, A_REVERSE);

            wattron(menuSites, COLOR_PAIR(1) | A_BOLD);
            mvwprintw(menuSites, i, 0, "%d: ", i);
            wattroff(menuSites,A_BOLD);
            wattron(menuSites, COLOR_PAIR(2));
            mvwprintw(menuSites, i, 3, "%s", getUrl(rootSite, i).c_str());
        }
    }

}

//Muda o site do header
void updateHeader(WINDOW *header, string site) {
    string p = "Site: "+site;
    mvwprintw(header, 1, 1, "%s", p.c_str());
}

WINDOW *create_win(int alt, int larg, int y, int x) {
    WINDOW *new_win;
    new_win = newwin(alt, larg, y, x);
    wrefresh(new_win);
    return new_win;
}

void refresh_wins(WINDOW *header, WINDOW *menuSites, WINDOW *menuMedias) {
    refresh();
    wrefresh(header);
    wrefresh(menuSites);
    wrefresh(menuMedias);
}

