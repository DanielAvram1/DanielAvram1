#include <iostream>
#include <fstream>
#include <queue>
using namespace std;



///clasa nod
class Nod{
private:
    friend class ABC;
    int info;
    Nod* st;
    Nod* dr;
    int inaltime;
public:
    Nod();
    Nod(int info, Nod* st, Nod* dr);
    ~Nod();

    friend ostream& preordine(ostream &out, Nod* r);
    friend ostream& inordine(ostream &out, Nod* r);
    friend ostream& postordine(ostream &out, Nod* r);
};

Nod::Nod() {
    info;
    st = NULL;
    dr = NULL;
    inaltime = 0;
}

Nod::Nod(int info, Nod* st, Nod* dr) {
    this->info = info;
    this->st = st;
    this->dr = dr;
}

Nod::~Nod(){
    if(st)
        delete st;

    if(dr)
        delete dr;
    //cout<<"Nod distrus\n";
}


ostream &preordine(ostream &out, Nod* r){
    if(r){
        out<<r->info<<' ';
        preordine(out,r->st);
        preordine(out,r->dr);
    }
    return out;

}

ostream& inordine(ostream &out,Nod* r){
    if(r){

        inordine(out,r->st);
        out<<r->info<<' ';
        inordine(out,r->dr);
    }
    return out;

}

ostream& postordine(ostream &out, Nod* r){
    if(r){
        postordine(out,r->st);
        postordine(out,r->dr);
        out<<r->info<<' ';
    }
    return out;
}

///clasa arbore

class ABC{
private:
    Nod* radacina;
public:
    ABC();
    void insert(int a);
    void erase(int a);
    void afisare_frunze();
    ABC operator +(int a);
    friend ostream &operator <<(ostream &out, ABC &arb);
    friend istream &operator>>(istream &in, ABC &arb);
    int inaltime();
};

ABC::ABC(){
    radacina = NULL;
}

/// functia de inserare
void ABC::insert(int a){
    Nod* curr = radacina;

    ///daca arborele e gol, cream radacina si initializam cu elementul inserat
    if(radacina == NULL){
        radacina = new Nod(0,NULL,NULL);
        radacina->info = a;
        return;
    }

    ///cautam locul potrivit pentru elementul adaugat
    while(curr){
        if(a < curr->info){
            if(curr->st == NULL){
                curr->st = new Nod(0,NULL,NULL);
                curr = curr->st;
                break;
            }
            else {
                curr = curr->st;
            }
        }
        else {
            if(curr->dr == NULL){
                curr->dr = new Nod(0,NULL,NULL);
                curr = curr->dr;
                break;
            }
            else {
                curr = curr->dr;
            }
        }
    }
    curr->info = a;
    return;
}


///functia de stergere (iterativa)     (cel mai mare cancer)
void ABC::erase(int a){

    Nod* curr = radacina;

    ///gasire elementului
    Nod* coada = curr;
    Nod* pred;
    while(curr != NULL){

        if(curr->info == a)
            break;
        coada = curr;
        if(curr->info > a ){

            curr = curr->st;
        }
        else{
            curr = curr->dr;
        }
    }

    ///daca nu am gasit elementul
    if(curr == NULL){
        cout<<a<<" nu este in arbore\n";
        return;
    }

    ///gasire inlocuire
    bool spre_dreapta;
    if(curr->dr){
        ///daca nodul care trebuie sters are fiu drept, coboram in el ca apoi sa cautam cel mai MIC element din subarborele DREPT
        pred = curr;
        coada = curr;
        curr = curr->dr;
        spre_dreapta = 0;
    }
    else if(curr->st){
        ///daca nodul care trebuie sters nu fiu drept dar are stang, coboram in el ca apoi sa cautam cel mai MARE element din subarborele STANG
        pred = curr;
        coada = curr;
        curr = curr->st;
        spre_dreapta = 1;
    }
    else {
        ///daca nodul nu are fii, atunci acesta este o frunza si putem direct sa il stergem
        if(coada->st == curr)
            coada->st = NULL;
        else coada->dr = NULL;
        delete curr;
        return;
    }
    if(!spre_dreapta){
        ///aici am mers in fiul DREPT, deci trebuie sa gasim cel mai MIC element care va fi inlocuirea lui pred
        while(1){
            bool cob = 0;
            while(1){
                ///coboram pe stanga cat mai putem
                if(curr->st){
                    cob = 1;
                    coada = curr;
                    curr = curr->st;
                }
                else break;
            }
            /// daca dupa coborare am dat de frunza, atunci il stergem ATENT (nu uitam da parintii sai) si ii introducem valoarea in pred->info
            if(curr->dr == NULL){
                pred->info = curr->info;
                if(cob){
                    coada->st = NULL;
                    coada->dr = NULL;
                }
                else {
                    if(coada->dr == curr)
                        coada->dr = NULL;
                    else coada->st = NULL;
                }
                delete curr;
                return;
            }
            ///daca nu e fiu, inseamna ca are un fiu drept, coboram cu o treapta in jos pe dreapta si incepem din nou cautarea
            else {
                pred->info = curr->info;
                pred = curr;
                coada = curr;
                curr = curr->dr;
            }
        }
    }
    else {
        ///aici am mers in fiul STANG, deci trebuie sa gasim cel mai MARE element care va fi inlocuirea lui pred
        ///repetam aceiasi pasi ca si mai sus, doar ca schimbam cu locul st cu dr
        while(1){
            bool cob = 0;
            while(1){
                if(curr->dr){
                    cob = 1;
                    coada = curr;
                    curr = curr->dr;
                }
                else break;
            }
            if(curr->st == NULL){
                pred->info = curr->info;
                if(cob){
                    coada->st = NULL;
                    coada->dr = NULL;
                }
                else {
                    if(coada->dr == curr)
                        coada->dr = NULL;
                    else coada->st = NULL;
                }
                delete curr;
                return;
            }
            else {
                pred->info = curr->info;
                pred = curr;
                coada = curr;
                curr = curr->st;
            }
        }
    }
    return;

}

///afisarea frunzelor utilizand BFS, nimic special
void ABC::afisare_frunze(){
    if(this->radacina == NULL){
        cout<<"arbore gol\n";
        return;
    }
    cout<<"Afisare frunze:\n";
    queue<Nod*> q;
    q.push(this->radacina);
    while(!q.empty()){
        Nod* curr = q.front();
        //cout<<curr->getInfo()<<' ';
        q.pop();
        if(curr->st == NULL && curr->dr == NULL){
            cout<<curr->info<<' ';
        }
        else{
            if(curr->st){
                q.push(curr->st);
            }
            if(curr->dr){
                q.push(curr->dr);
            }
        }
    }
    cout<<'\n';
    return;
}

///supraincarcarea operatorului + utilizand functia insert
ABC ABC::operator +(int a){
    this->insert(a);
    return *this;
}

///afisarea arborelui prin supraincarcarea operatorului <<.
///afiseaza elementele arborelui in cele 3 "ordini"
ostream &operator <<(ostream &out, ABC &arb){
    /*if(*(abc.radacina) == NULL){
        out<<"Arbore gol\n";
        return out;
    }*/

    ///pentru afisare, folosim functiile friend recursive din Nod
    out<<"Preordine:(";
    preordine(out, arb.radacina);
    out<<")\n";
    out<<"Inordine:(";
    inordine(out, arb.radacina);
    out<<")\n";
    out<<"Postordine:(";
    postordine(out, arb.radacina);
    out<<")\n";
    return out;

}

///supraincararea operatorului de citire >>. se citeste mai intai un n, urmat de n numere care sunt inserate pe rand in arbore
istream &operator>>(istream &in, ABC &arb){
    ///mai intai, stergem arborele precedent
    delete arb.radacina;
    arb.radacina = NULL;
    int n;
    in>>n;
    while(n--){
        int a;
        in>>a;
        arb.insert(a);
    }
    return in;
}

///prin BFS, se afla inaltimea "maxima" a arborelui
int ABC::inaltime(){
    Nod* curr = this->radacina;
    if(curr == NULL)
        return 0;
    queue < Nod* > q;
    int mx = 0;
    q.push(curr);
    while(!q.empty()){
        curr = q.front();
        q.pop();
        mx = max(mx, curr->inaltime);
        if(curr->st){
            (curr->st)->inaltime = curr->inaltime + 1;
            q.push(curr->st);
        }
        if(curr->dr){
            (curr->dr)->inaltime = curr->inaltime + 1;
            q.push(curr->dr);
        }

    }
    return mx + 1;

}


int main()
{
   // ifstream cin("main.in");
   // ofstream cout("main.out");

    ABC arbore;
    cout<<arbore;
    cin>>arbore;
    arbore.afisare_frunze();
    arbore.insert(40);
    arbore.insert(20);
    arbore.insert(60);
    arbore.insert(6);
    arbore.insert(30);
    arbore.insert(20);
    arbore.insert(50);
    arbore.insert(55);
    arbore = arbore + 56 + 34;
    arbore.afisare_frunze();
    cout<<"Inaltimea: "<<arbore.inaltime()<<'\n';
    cout<<arbore;
    arbore.erase(40);
    cout<<arbore;
    arbore.erase(6);
    cout<<arbore;
    arbore.erase(30);
    cout<<arbore;
    arbore.erase(20);
    cout<<arbore;
    arbore.erase(6);
    arbore.erase(6);
    arbore.erase(6);
    cout<<arbore;
    arbore.afisare_frunze();
    cout<<arbore;
    cin>>arbore;
    cout<<arbore;
    cout<<"Inaltimea: "<<arbore.inaltime();



    return 0;
}
