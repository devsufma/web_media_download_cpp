#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "lista.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstring>

using namespace std;

// Percorre um site, encontrando as href e src
// 'page' é o arquivo .html no sistema, exemplo: "www.google.com.html"
// 'original' é o link, exemplo: "https://www.google.com"
bool downloadPage(string page, string original, lista **rootSite, lista **rootMedias);

//Recebe 'url', e remove caracteres inuteis, etc
//formatando o nome, pra ser salva no sistema
string getFileName(string *url);

//Baixa a midia em 'url', e bota em 'dir'
void downloadMedia(string dir, string fileName, string *url, lista **rootMedia);

//Transforma um site em um arquivo .html e baixa o arquivo
string UrlToFile(string& input);

#endif
