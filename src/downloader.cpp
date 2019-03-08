#include "downloader.h"

using namespace std;

bool downloadPage(string page, string original, lista **rootSites,lista  **rootMedias) {

    string line;    //Linha atual
    string::size_type linePos; //Posição na linha

    string* url = new string;
    string newHtml;
    string fileName;

    int achouSrc, achouHref; //Diz se achou source ou href
    string::size_type tagBegin, tagEnd;

    fstream htmlfile;                   //o arquivo HTML aberto
    htmlfile.open(page.c_str());

    if (htmlfile.is_open()) {
        while(getline(htmlfile, line)) { //enquanto tiver linhas
            linePos = 0;
            while(linePos != string::npos) { //Passou de tag
                achouSrc = false, achouHref = false;

                if(line.find("src=\"", linePos+1) != string::npos) { //Se achou source
                    linePos = line.find("src=\"", linePos+1);
                    achouSrc = true;
                } else if(line.find("href=\"", linePos+1) != string::npos) {
                    linePos = line.find("href=\"", linePos+1);
                    achouHref = true;
                } else { //Se não achou nd
                    linePos = string::npos; //Passa pra proxima linha
                }

                if(achouSrc == true) {
                    //Faz a url da primeira aspas pra ultima aspas
                    tagBegin = line.find("\"", linePos+1);
                    tagEnd = line.find("\"", tagBegin+1);
                    *url = line.substr(tagBegin+1, tagEnd-tagBegin-1);

                    if(url->find(".jpg") != string::npos) {
                        fileName = getFileName(url);
                        downloadMedia("Images/", fileName, url, rootMedias);

                    } else if(url->find(".png") != string::npos) {
                        fileName = getFileName(url);
                        downloadMedia("Images/", fileName, url, rootMedias);

                    } else if(url->find(".css") != string::npos) {
                        fileName = getFileName(url);
                        downloadMedia("CSS/", fileName, url, rootMedias);
                    } else if(url->find(".js") != string::npos) {
                        fileName = getFileName(url);
                        downloadMedia("JavaScript/", fileName, url, rootMedias);
                    }

                } else if(achouHref == true) {
                    tagBegin = line.find("\"", linePos+1);
                    tagEnd = line.find("\"", tagBegin+1);
                    *url = line.substr(tagBegin+1, tagEnd-tagBegin-1);

                    if(url->find(".jpg") != string::npos) {
                        fileName = getFileName(url);
                        downloadMedia("Images/", fileName, url, rootMedias);

                    } else if(url->find(".png") != string::npos){
                        fileName = getFileName(url);
                        downloadMedia("Images/", fileName, url, rootMedias);

                    } else if(url->find(".css") != string::npos) {
                        fileName = getFileName(url);
                        downloadMedia("CSS/", fileName, url, rootMedias);

                    } else if(url->find(".js") != string::npos) {
                        fileName = getFileName(url);
                        downloadMedia("JavaScript/", fileName, url, rootMedias);

                    } else if(url->find("#") != string::npos) {

                    } else if(url->find("http") != string::npos) {
                        addNode(rootSites, *url);

                    }else {
                        newHtml = original+ "/"+ *url;
                        addNode(rootSites, newHtml);
                    }
                }
            }
        }
        htmlfile.close();
        delete url;
        return true;
    } else {
        delete url;
        return false;
    }
}

string getFileName(string *url) {
    size_t tagEnd, fileNameBegin;
    string fileName;

    //rfind => procura de tras pra frente a ocorrencia, e deixa
    //o ponteiro no promeiro char(nesses casos o '.')
    if(url->rfind(".css") != string::npos) {
        tagEnd = url->rfind(".css");
        tagEnd += 4; //ai move o ponteiro +4, pra ficar depois de '.css'
    }else if (url->rfind(".png") != string::npos){
        tagEnd = url->rfind(".png");
        tagEnd += 4;
    }else if (url->rfind(".jpg") != string::npos){
        tagEnd = url->rfind(".jpg");
        tagEnd += 4;
    }else if (url->rfind(".js") != string::npos){
        tagEnd = url->rfind(".js");
        tagEnd += 3;
    }

    *url = url->substr(0, tagEnd); //remove caracteres aleatorios do final
    //Faz o nome do  arquivo
    fileNameBegin = url->rfind("/");
    fileName = url->substr(fileNameBegin+1, tagEnd);

    return fileName;
}

//Faz o comando pra baixar a midia
void downloadMedia(string dir, string fileName, string *url, lista **rootMedia) {
    string cmd;
    cmd = "wget -c --adjust-extension -q -nc -O ./downloads/";
    //Anexa o diretorio, o nome do arquivo, e a url pra baixar
    cmd += dir+'/'+fileName+' '+*url;
    system(cmd.c_str()); //executa o comando
    addNode(rootMedia, fileName); //Acrescente a midia baixada a lista de midias
}


//Pega uma url, e transforma ela em um nome de arquivo pra ser salvo
//substitui as '/' por '_', pq não pode salvar arquivo com '/' no nome
string UrlToFile(string& input) {
    string cmdString;
    string file;
    string fileName = input;
    file.clear();

    //Se o input for um site
    if ((input.find("https://") != string::npos) || (input.find("http://") != string::npos) || (input.find("www.") != string::npos)) {
        //Muda todas as "/" por "_", porque nome de arquivo não pode ter "/"
        replace(fileName.begin(), fileName.end(), '/', '_');

        //se tiver "https:__" ou "http:__", remove
        if(fileName.find("https:__") != string::npos) {
            fileName.erase(0, 8);
        } else if (fileName.find("http:__") != string::npos) {
            fileName.erase(0, 7);
        }

        //Fazer o comando
        cmdString += "wget -c -q -nc -O ./downloads/HTML/" + fileName + ".html ";
        cmdString += input;
        //Executar o comando
        system(cmdString.c_str());
        //Fazer o nome do arquivo, e retornar ele
        file += "./downloads/HTML/"+fileName + ".html";
        return file;
    }
    //Se for um arquivo, so retorna o nome dele
    return input;
}

