using namespace std;
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
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

    cout << endl;

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
        //cout << "Current algor" << current << endl;
        //cout << "O size" << fsm.o[current].size() << endl;
        for(int j = 0; j < fsm.o[current].size(); j++){
            occ[fsm.o[current][j]].push_back(i-m[j]+1);
            //cout << j << " " << i-m[j] +1 << endl;
        }
    }
    
    
    return occ;
}

//-----------------------------------------------------------------------------------------


//--------------------------- Main --------------------------------------------------------

int main(){
    vector <string> ps;         // padrões
    vector <char> alphabet;     // alfabeto ASC
    vector < vector <int> > occ;

    for(char symbol = 0; symbol < 127; symbol++){
        alphabet.push_back(symbol);
    }

    vector <string> txt = {"worklaksdlkasd lask dword,l ., . ams jl alsm f vjds voioioioasmd",
                           "worklaksdlkasd lask dword,l ., . ams jl alsm f vjds voioioioiasmd",
                           "workworkworkwokewordwoird"};
    ps.push_back("word");
    ps.push_back("work");
    ps.push_back("oi");

    Out_goto t = build_go_to(ps, alphabet);
    cout << "States " << t.nxt << endl;

    Out_fail fsm = build_fsm(ps, alphabet);

    for(int j = 0; j<txt.size(); j++){
        occ = ahocorasick(txt[j], ps, alphabet, fsm);

        cout << occ.size() << endl;

        for(int i = 0; i < ps.size(); i++){
            cout << ps[i] << " -> " << occ[i].size() << endl;
        }
    }

    return 0;
}
