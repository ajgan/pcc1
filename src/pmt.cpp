using namespace std;
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define MAXSTATES 1000
#define MAXP 100
#define ASCLEN 127

int g [MAXSTATES][ASCLEN];

void print_alphabet(vector <char> alphabet){
    for(int i = 0; i<alphabet.size(); i++){
        cout << alphabet[i] << endl;
    }
}

struct Out_goto{
    int nxt;
    //int g [MAXSTATES][ASCLEN];
    vector < vector <int> > o;
};

struct Out_fail{
    vector <int> f;
    vector < vector <int> > o;
};


//-------------- Build Goto --------------------------------------------------------------

Out_goto build_go_to(vector <string> ps, vector <char> alphabet){
    //int g [MAXSTATES][ASCLEN];
    memset(g, -1, sizeof g);

    vector <vector <int> > o (MAXSTATES, vector<int> (0));
    int nxt = 0;

    for(int k = 0; k < ps.size(); k++){
        string pk = ps[k];
        int m = pk.size();
        int current = 0;
        int j = 0;

        while(j < m && g[current][pk[j]] != -1){
            current = g[current][pk[j]];
            j++;
        }
        while(j < m){
            nxt++;
            g[current][pk[j]] = nxt;
            current = nxt;
            j++;
        }
        //cout << " Current " << current << " " << k;
        o[current].push_back(k);
    }

    // cout << endl;

    for(int i = 0; i < alphabet.size(); i++){
        if(g[0][alphabet[i]] == -1){
            g[0][alphabet[i]] = 0;
        }
    }


    /*for(int i = 0; i < o.size(); i++){
        for(int j = 0; j < o[i].size(); j++){
            cout << o[i][j] << " ";
        }
        cout << endl;
    }*/

    Out_goto gt;
    gt.nxt = nxt + 1;
    gt.o = o;
    // gt.g = &g;
    return gt;
}
//-----------------------------------------------------------------------------------------

//-------------- Build Fail ---------------------------------------------------------------

Out_fail build_fail(vector <string> ps, vector <char> alphabet, int n, vector < vector<int> > o){
    vector <int> q;
    vector <int> f (n);
    int aux = 0;

    for (int i = 0; i < alphabet.size(); i++){
        if( g[0][alphabet[i]] != 0){
            q.push_back(g[0][alphabet[i]]);
            f[g[0][alphabet[i]]] = 0;
        }
    }

    while(q.size() > aux){
        int current = q[aux];
        aux++;

        for(int i = 0; i < alphabet.size(); i++){
            if(g[current][alphabet[i]] != -1){
                int nxt = g[current][alphabet[i]];
                q.push_back(nxt);
                int brd = f[current];
                while(g[brd][alphabet[i]] == -1){
                    brd = f[brd];
                }
                f[nxt] = g[brd][alphabet[i]];
                for(int k = 0; k<o[f[nxt]].size(); k++){
                    o[nxt].push_back(o[f[nxt]][k]);
                }
            }
        }
    }

    Out_fail s;
    s.f = f;
    s.o = o;
    return s;
}

//-----------------------------------------------------------------------------------------

//------------------- Build Finite State Machine ------------------------------------------

Out_fail build_fsm(vector <string> ps, vector <char> alphabet){
    Out_goto gt = build_go_to(ps, alphabet);
    Out_fail fsm = build_fail(ps, alphabet, gt.nxt, gt.o);

    return fsm;
}
//-----------------------------------------------------------------------------------------

//---------------------- Aho-Corasick Algorithm -------------------------------------------

vector < vector <int> > ahocorasick(string txt, vector <string> ps, vector <char> alphabet){
    int n = txt.size();
    vector <int> m (ps.size());
    Out_fail fsm = build_fsm(ps, alphabet);
    int current = 0;
    vector < vector <int> > occ (ps.size(), vector <int> (0));

    for(int i = 0; i<ps.size(); i++){
        m.push_back(ps[i].size());
    }

    for(int i = 0; i < n; i++){
        char a = txt[i];
        while(g[current][a] == -1){
            current = fsm.f[current];
        }
        current = g[current][a];
        //cout << "Current algor" << current << endl;
        //cout << "O size" << fsm.o[current].size() << endl;
        for(int j = 0; j < fsm.o[current].size(); j++){
            occ[fsm.o[current][j]].push_back(i-m[j]+1);
            //cout << j << " " << i-m[j] +1 << endl;
        }
    }


    return occ;
}

vector < vector <int> > ahocorasick(string txt, vector <string> ps, vector <char> alphabet, Out_fail fsm){
    int n = txt.size();
    vector <int> m (ps.size());
    int current = 0;
    vector < vector <int> > occ (ps.size(), vector <int> (0));

    for(int i = 0; i<ps.size(); i++){
        m.push_back(ps[i].size());
    }

    for(int i = 0; i < n; i++){
        char a = txt[i];
        while(g[current][a] == -1){
            current = fsm.f[current];
        }
        current = g[current][a];
        // cout << "Current algor" << current << endl;
        // cout << "O size" << fsm.o[current].size() << endl;
        for(int j = 0; j < fsm.o[current].size(); j++){
            occ[fsm.o[current][j]].push_back(i-m[j]+1);
            //cout << j << " " << i-m[j] +1 << endl;
        }
    }


    return occ;
}

void getPsArray(char* pat, int* psArray) {

    int m = strlen(pat);

    //maior sufixo que é prefixo
    int ps = 0;

    //tamanho do maior sufixo que é prefixo na posição 0 é zero
    psArray[0] = ps;

    //começa a análise da posição 1 do padrão, já que no 0 é zero
    int i = 1;

    while (i < m) {
        if (pat[i] == pat[ps]) { //match da posição atual com a posição relativa ao último tamanho de prefixo que é sufixo
            //dando match, significa que o maior sufixo que é prefixo nessa posição considera também a posição atual, então incrementa 1
            ps++;
            psArray[i] = ps;
            i++;
        }
        else { //não deu match na posição atual
            if (ps == 0) {
                //não tem sufixo que é prefixo nessa posição do padrão
                psArray[i] = 0;
                i++;
            }
            else {
                //atualiza o tamanho do maior sufixo que é prefixo pra saber se ao considerar um range menor, vai dar match nessa posição
                //não atualiza o i, até que se ache um match, ou um mismatch total(ps == 0)
                ps = psArray[ps - 1];
            }
        }
    }
}

int kmp(char* pat, char* txt, int cont) {
    int m = strlen(pat);
    int n = strlen(txt);

    //array de tamanhos dos maiores prefixos que são sufixos em cada posição do padrão
    //serve para otimizar os deslocamentos de janelas
    int psArray[m];
    getPsArray(pat, psArray);

    int i = 0; //texto
    int j = 0; //padrão

    while (i < n) {
        if (pat[j] == txt[i]) { //match entre caractere do padrao e do texto, andamos no texto(i++) e no padrão(j++)
            j++;
            i++;
        }

        if (j == m) { //deu match e está na última posição do padrão
            // add no contador de matches
            cont ++;
            //j volta pro maior sufixo que é prefixo da posição no momento
            j = psArray[j - 1];
        }

        else if (i < n && pat[j] != txt[i]) { //mismatch
            if (j != 0)
                //não deu match na posição mas até o momento o padrão estava em match
                //o i continua o mesmo pois ainda não se anda no texto, precisamos recomparar o caractere do texto considerando um novo range(ps) no padrão
                //atribui a j o valor do maior sufixo que é prefixo no último match
                j = psArray[j - 1];
            else
                //não deu match na posição e o padrão não estava em match
                //j já é zero(e não existe match pra nenhum prefixo que é sufixo no padrão), anda no i
                i ++;
        }
    }

    return cont;
}

int sellers(char* pat, char* txt, int cont, int err) {

    int m = strlen(pat);
    int n = strlen(txt);

    //matriz de distâncias entre os dois padrões [m+1 x n+1]
    //apenas duas linhas são necessárias para fazer todos os cálculos e chegar na distância final
    int D[2][m+1];

    for (int i = 0; i < m+1; i++) {
        D[0][i] = i;
        D[1][i] = i;
    }
    int last = 1;
    int prev = 0;

    for (int j = 0; j < n; j++) {//o for vai percorrer todas as posições do texto, operando sobre cada posição do padrão

        D[last][0] = 0;
        for (int i = 1; i < m+1; i++) {
            /* De acordo com Sellers, o valor da distância é o mínimo entre os 3 valores seguintes:
            1 - valor da linha anterior da matriz na mesma posição mais 1 => e(i-1, j) + d(Ai , 1)
            2 - valor da linha anterior da matriz na posição anterior mais a diferença entre padrão e texto => e(i-1, j-1) + d(Ai, Bj)
            3 - valor da linha atual da matriz na posição anterior mais 1 => e(i, j-1) + d(1, Bj)
            Numa matriz desenhada isso seria o mínimo entre a casa da esquerda(+1), a casa da diagonal(+diff) e a casa de cima(+1)
            */
            int partmin = 0;
            partmin = min (D[prev][i]+1, D[prev][i-1] + int(pat[i-1] != txt[j]));
            D[last][i] = min (partmin , D[last][i-1]+1);
            //o uso da variável partmin é apenas pra simplicar, já que o método min só aceita dois parâmetros por vez

        }
        if (D[last][m] <= err) {/* se na última posição da linha atual da matriz o erro for menor que o limiar,
            significa que houve match do padrão com a posição j do texto */
            cont ++;
        }
        //inverte as linhas para que se opere na outra, olhando pro valor da anterior
        //isso tudo é apenas pra minimizar o custo com espaço de memória e não ser necessária uma matriz (m+1 x n+1)
        last = (last+1) % 2;
        prev = (prev+1) % 2;
    }
    return cont;
}

int brute (char* pat, char* txt, int cont) {
    int m = strlen(pat);
    int n = strlen(txt);

    for (int i = 0; i < n; i++) {
        int match = 0;
        for (int j = 0; j < m; j++) {
            if (pat[j] == txt[i+match]) {
                match++;
            }
            else {
                break;
            }
            if(match == m) {
                cont ++;
            }
        }
    }

    return cont;

}

char* treat(char *txt) {
    int n = strlen(txt);
    for (int i = 0; i < n; i++) {
        if(int(txt[i]) < 0) {
            txt[i] = ' ';
        }
    }
    return txt;
}


int main(int argc, char** argv) {

    auto start = chrono::steady_clock::now();

    vector <string> mypats;         // padrões
    vector <char> alphabet;     // alfabeto ASC
    vector < vector <int> > occ;
    vector <string> files;
    string pats;

    for(char symbol = 0; symbol < 127; symbol++){
        alphabet.push_back(symbol);
    }

    // flag de impressão de contagem
    bool printCont = 0;
    // flag de erro
    int err = 0;
    // contador de matches
    int cont = 0;
    // flag de impessão do tempo
    bool printTime = 0;

    // bool para indicar se existe flag de algoritmo
    bool hasAlg = 0;
    char* alg;

    //bool pra indicar que o padrão já foi identificado no comando(padrão sempre vem antes do arquivo)
    bool patflag = 0;
    //bool que indica se o padrão virá de uma file
    bool patfile = 0;
    char *patpath;

    // array criado para identificação das flags fornecidas como entrada
    int markedPos[argc];

    //marca tudo como zero(para cada argumento de entrada)
    for (int a = 0; a < argc; a++) {
        markedPos[a] = 0;
    }
    //posição 0 é marcada com 1 pois não nos interessa(argv[0] == ./pmt)
    markedPos[0] = 1;

    for (int i = 1; i < argc; ++i) {
        //checa por flag de contagem
        if ((strncmp(argv[i],"-c",256) == 0) || (strncmp(argv[i],"--count",256) == 0)) {
            // manda imprimir contador no final
            printCont = 1;
            // diz que essa posição dos args já foi identificada
            markedPos[i] = 1;
        }
        else if ((strncmp(argv[i],"-e",256) == 0) || (strncmp(argv[i],"--edit",256) == 0)) {
            // atribui ao erro o valor seguinte fornecido nos args
            err = atoi(argv[i+1]);
            // diz que essa posição dos args já foi identificada
            markedPos[i] = 1;
            // diz que a posição seguinte dos args também já foi identificada(valor do erro)
            markedPos[i+1] = 1;
        }
        else if ((strncmp(argv[i],"-a",256) == 0) || (strncmp(argv[i],"--algorithm",256) == 0)) {
            // diz que essa posição dos args já foi identificada
            markedPos[i] = 1;
            // diz que a posição seguinte dos args também já foi identificada(nome do algoritmo)
            markedPos[i+1] = 1;
            alg = argv[i+1];
            hasAlg = 1;
        }
        if ((strncmp(argv[i],"-p",256) == 0) || (strncmp(argv[i],"--pattern",256) == 0)) {
            // indica que já identificou de onde vem o padrão e que vem de uma file
            patflag = 1;
            patfile = 1;
            patpath = argv[i+1];
            // diz que essa posição dos args já foi identificada
            markedPos[i] = 1;
            markedPos[i+1] = 1;
        }
        if ((strncmp(argv[i],"-h",256) == 0) || (strncmp(argv[i],"--help",256) == 0)) {
            //implementar
            cout << "help" << "\n";
            markedPos[i] = 1;
        }
        if ((strncmp(argv[i],"-t",256) == 0) || (strncmp(argv[i],"--time",256) == 0)) {
            printTime = 1;
            markedPos[i] = 1;
        }
    }

    // varre o markedPos para checar as posições do argv que não foram identificadas como nenhuma flag
    // a primeira ocorrência de um argv não-identifcado será o padrão e a segunda será o texto
    for (int j = 0; j < argc; j++) {
        if(markedPos[j] == 0 && (patflag == 0)) {
            patflag = 1;
            markedPos[j] = 1;
            mypats.push_back(argv[j]);
        }
        if((markedPos[j] == 0) && (patflag == 1)) {
            files.push_back(argv[j]);
            markedPos[j]  = 1;
        }
    }

    // flag de impressão de linhas
    bool printLine = !printCont;

    if (patfile == 1) {
        //abrir arquivo de padrões
        ifstream patsrc(patpath);
        while(getline(patsrc, pats)) { //vai checar a linha para cada padrão
            mypats.push_back(pats);
        }
    }

    Out_goto t = build_go_to(mypats, alphabet);
    Out_fail fsm = build_fsm(mypats, alphabet);
    //abrir o arquivo

    for (int i = 0; i < files.size(); i++) {
        ifstream src(files[i]);
        string line;
        while (getline(src, line)) {// percorre linha por linha
            //converter a linha para char*
            char *txt = &line[0u];
            int prevCont = cont;
            // cout << txt << "\n";

            for (int j = 0; j < mypats.size(); j++) {
                char *newpat = &mypats[j][0u];
                if(hasAlg) {
                    if (strncmp(alg,"kmp",256) == 0) {
                        cont = kmp(newpat, txt, cont);
                    }
                    else if (strncmp(alg,"sel",256) == 0) {
                        cont = sellers(newpat, txt, cont, err);
                    }
                    else if (strncmp(alg,"brt",256) == 0) {
                        cont = brute(newpat, txt, cont);
                    }
                    else if (strncmp(alg,"aho",256) == 0) {
                        txt = treat(txt);
                        // cout << txt << "\n";
                        occ = ahocorasick(txt, mypats, alphabet, fsm);

                        for(int k = 0; k < occ.size(); k++){
                            cont += occ[k].size();
                        }

                        break;
                    }
                    else {
                        cont = sellers(newpat, txt, cont, err);
                    }
                }
                else {
                    cont = sellers(newpat, txt, cont, err);
                }
            }

            if ((cont - prevCont > 0) && printLine) {
                cout << txt << "\n";
            }

        }
    }

    if (printCont) {
        cout << cont << " occurrences\n";
    }
    auto end = chrono::steady_clock::now();
    auto diff = end - start;

    if (printTime) {
        if (chrono::duration <double, milli> (diff).count() > 1000) {
            cout << chrono::duration <double> (diff).count() << " s" << endl;
        }
        else {
            cout << chrono::duration <double, milli> (diff).count() << " ms" << endl;
        }
    }


    return 0;
}
