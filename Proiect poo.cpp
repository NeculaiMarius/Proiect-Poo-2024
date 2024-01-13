#include <iostream>
#include<string>
#include<fstream>

using namespace std;


enum class CompanieAsiguratorie {
    AlianzTiriac,
    GeneraliRomania,
    Groupama,
    Omniasig,
    Nespecificat
};

ostream& operator<<(ostream& os, CompanieAsiguratorie ca)
{
    switch (ca)
    {
    case CompanieAsiguratorie::AlianzTiriac: os << "Alianz Tiriac Asigurari"; break;
    case CompanieAsiguratorie::Groupama: os << "Groupama Asigurari"; break;   
    case CompanieAsiguratorie::GeneraliRomania: os << "Generali Romania Asigurari"; break;    
    case CompanieAsiguratorie::Omniasig: os << "Omniasig "; break;
    default: os << "Nespecificat";
    }

    return os;
}



class Asigurare {
protected:
    string numeClient;
    CompanieAsiguratorie ca;
    int perioada;  
    
public:
    Asigurare(string numeClient, CompanieAsiguratorie ca, int perioada) {
        this->numeClient = numeClient;
        this->perioada = perioada;
        this->ca = ca;
    }
    Asigurare() {
        this->numeClient = "Fara nume";
        this->ca = CompanieAsiguratorie::Nespecificat;
        this->perioada = 0;
    }

    //gett-eri pentru membrii protected
    string getNumeClient() {
        return this->numeClient;
    }
    CompanieAsiguratorie getCompanieAsiguratorie() {
        return this->ca;
    }
    int getPerioada() {
        return this->perioada;
    }

    //Sett-eri
    void setNumeClient(string numeNou) {
        this->numeClient = numeNou;
    }
    void setCompanieAsiguratorie(CompanieAsiguratorie caNou){
        this->ca = caNou;
    }
    void setPerioada(int perioadaNoua) {
        if (perioadaNoua >= 0) {
            this->perioada = perioadaNoua;
        }
        else cerr <<endl<< "Perioada nu poate sa fie negativa"<<endl;
    }


    virtual void display(ostream& os) {
        os << " _______________________________________________________________________" << endl;
        os << "|" << "Nume clinet....................... " << this->numeClient << endl;
        os << "|" << "Companie Asiguratorie............. " << this->ca << endl;
        os << "|" << "Perioada(luni).................... " << this->perioada << endl;
    }
    

    friend ostream& operator<<(ostream& os, Asigurare* a) {
        a->display(os);
        return os;
    }

    //Afisare pentru obiecte care nu sunt declarate ca Asigurare*
    virtual void display_v2(ostream& os) {
        os <<"__________________________________________________________"<<endl;
        os <<"|"<< "Nume clinet....................... " << this->numeClient << endl;
        os <<"|"<< "Companie Asiguratorie............. " << this->ca << endl;
        os <<"|"<< "Perioada(luni)...................." << this->perioada << endl;
    }
    friend ostream& operator<<(ostream& os, Asigurare& a) {
        a.display_v2(os);
        return os;
    }


    virtual void citireTastatura(istream& is) {
        char nume_temp[100];
        cout << " --------------------INTRODUCERE DATE DE LA TASTATURA--------------------" << endl;
        cout <<" * " << "Dati nume client: "; cin.get(nume_temp, 100);
        this->numeClient = nume_temp;
        cout << " * " << "Introduceti varianta pentru compania asiguratorie: " << endl;
        cout << "            " << "(1) Alianz Tiriac Asigurari " << endl;
        cout << "            " << "(2) Groupama Asigurari " << endl;
        cout << "            " << "(3) Generali Romania Asigurari" << endl;
        cout << "            " << "(4) Omniasig " << endl;
        char temp[2];
        bool ok=1;
        while(ok==1)
        {
            cout << "  Alegeti varianta : "; is >> temp;
            if (strcmp(temp,"1")==0) {
                ok = 0;
                this->ca = CompanieAsiguratorie::AlianzTiriac;
            }
            else if (strcmp(temp, "2") == 0) {
                ok = 0;
                this->ca = CompanieAsiguratorie::Groupama;
            }
            else if (strcmp(temp, "3") == 0) {
                ok = 0;
                this->ca = CompanieAsiguratorie::GeneraliRomania;
            }
            else if (strcmp(temp, "4") == 0) {
                ok = 0;
                this->ca = CompanieAsiguratorie::Omniasig;
            }
            else 
            {
                ok = 1;
                cout << "*******************************************************"<<endl;
                cout << "* EROARE ---> Varianta invalida ----> Alegeti din nou *" << endl;
                cout << "*******************************************************"<<endl;
            }
        }
        cout << " * "<< "Dati perioada(luni): "; is >> this->perioada;
    }
    friend istream& operator>>(istream& is,Asigurare* a) {
        bool ok = 1;
        a->citireTastatura(is);
        return is;
    }


    Asigurare& operator=(Asigurare& a) {
        this->numeClient = a.numeClient;
        this->perioada = a.perioada;
        this->ca = a.ca;
        return *this;
    }
    

    virtual void serialize(ostream& os) const
    {
        os.write(numeClient.c_str(), numeClient.size() + 1);
        os.write((char*)&this->ca, sizeof(CompanieAsiguratorie));
        os.write((char*)&this->perioada, sizeof(int));
    }

    static Asigurare* deserialize(istream& is)
    {
        string numeClient;
        char ch;
        while (is.get(ch) && ch != '\0')
            numeClient += ch;

        CompanieAsiguratorie ca;
        is.read((char*)&ca, sizeof(CompanieAsiguratorie));


        int perioada;
        is.read((char*)&perioada, sizeof(int));

        return new Asigurare(numeClient,ca,perioada);
    }

    ~Asigurare() {
    }

};

//================================================================================================================================
//================================================================================================================================

class AsigurareAuto:public Asigurare {
private:
    int anFabricatie;
    float capacitateCilindrica;
    float pretAsigurareAuto; 

public:
    AsigurareAuto(
        string numeClient,
        CompanieAsiguratorie ca,
        int perioada,
        int anFabricatie,
        float capacitateCilindrica
    ) :Asigurare(numeClient, ca, perioada) {
        this->anFabricatie = anFabricatie;
        this->capacitateCilindrica = capacitateCilindrica;
        this->pretAsigurareAuto = this->calculeazaPretAsigurareAuto();
    }

    AsigurareAuto(): Asigurare() {
        this->anFabricatie = 0;
        this->capacitateCilindrica = 0;
        this->pretAsigurareAuto = this->calculeazaPretAsigurareAuto();
    }

    //Gett-eri
    int getAnFabricatie() {
        return this->anFabricatie;
    }
    float getCapacitateCilindrica() {
        return this->capacitateCilindrica;
    }
    float getPretAsigurareAuto() {
        return this->pretAsigurareAuto;
    }

    //Sett-eri
    void setAnFabricatie(int anFabricatieNou) {
        if (anFabricatieNou > 1771 && anFabricatieNou < 2024)
        {
            this->anFabricatie = anFabricatieNou;
            this->pretAsigurareAuto = calculeazaPretAsigurareAuto();
        }
        else cerr << endl << "An fabricatie invalid"<<endl;
    }
    void setCapacitateCilindrica(float capacitateCilindricaNoua) {
        if (capacitateCilindricaNoua > 0)
        {
            this->capacitateCilindrica = capacitateCilindricaNoua;
            this->pretAsigurareAuto = calculeazaPretAsigurareAuto();
        }
        else cerr << endl << "Capacitatea cilindrica introdusa este invalida" << endl;
    }


    void display(ostream& os) override {
        Asigurare::display(os);
        os << " -----------------------------------------------------------------------" << endl;
        os << "|" << "    ASIGURARE MASINA" << endl;
        os << " -----------------------------------------------------------------------" << endl;
        os << "|" << "An fabricatie..................... " << this->anFabricatie << endl;
        os << "|" << "Capacitate cilindirca............. " << this->capacitateCilindrica*1000<<" cm^3" << endl;
        os << "|" << "Pret asigurare.................... " << this->pretAsigurareAuto <<" RON" << endl;
        os << " -----------------------------------------------------------------------" << endl;

    }
    void display_v2(ostream& os) override {
        Asigurare::display_v2(os);
        os << "An fabricatie: " << this->anFabricatie << endl;
        os << "Capacitate cilindirca: " << this->capacitateCilindrica << endl;
        os << "Pret asigurare: " << this->pretAsigurareAuto << endl;
    }

    void citireTastatura(istream& is) override {
        Asigurare::citireTastatura(is);
        cout << "        ASIGURARE AUTO " << endl;
        cout << " *" << "Dati an fabricatie: "; is >> this->anFabricatie;
        cout << " *" << "Dati capacitate cilindrica (litri): "; is >> this->capacitateCilindrica;
        this->pretAsigurareAuto = calculeazaPretAsigurareAuto();
        system("CLS");
    }

    AsigurareAuto& operator=(AsigurareAuto& a) {
        Asigurare::operator=(a);
        this->anFabricatie = a.anFabricatie;
        this->capacitateCilindrica = a.capacitateCilindrica;
        this->pretAsigurareAuto = a.pretAsigurareAuto;
        return *this;
    }

    float calculeazaPretAsigurareAuto() {
        //calculez preturi anuale pe care la final le impart la 12 si inmultesc cu numarul de luni pentru care este facuta asigurarea
        if (capacitateCilindrica <=1)
        {
            this->pretAsigurareAuto = 500 * this->capacitateCilindrica;
        }
        else if (this->capacitateCilindrica <= 2) {
            this->pretAsigurareAuto = 900 + 900 * (this->capacitateCilindrica - 1);
        }
        else if (this->capacitateCilindrica <= 4) {
            this->pretAsigurareAuto = 2000 + 1000 * (this->capacitateCilindrica - 2);
        }
        else if (this->capacitateCilindrica > 4) {
            this->pretAsigurareAuto = 6000 + 1000 * (this->capacitateCilindrica - 4);
        }

        if (this->anFabricatie < 2005) {
            this->pretAsigurareAuto = this->pretAsigurareAuto + 500;
        }

        this->pretAsigurareAuto = this->pretAsigurareAuto / 12 * this->perioada;
        return this->pretAsigurareAuto;
    }


    ~AsigurareAuto(){

    }
};

//================================================================================================================================
//================================================================================================================================


class AsigurareLocuinta :public Asigurare {
private:
    int anConstructie;
    float suprafataUtila;
    string adresa;
    float pretAsigurareLocuinta;
    
public:
    AsigurareLocuinta(
        string numeClient,
        CompanieAsiguratorie ca,
        int perioada,
        int anConstructie,
        float suprafataUtila,
        string adresa
    ) :Asigurare(numeClient, ca, perioada) {
        this->anConstructie = anConstructie;
        this->suprafataUtila = suprafataUtila;
        this->adresa = adresa;
        this->pretAsigurareLocuinta = calculeazaPretAsigurareLocuinta();
    }
    AsigurareLocuinta():Asigurare() {
        this->anConstructie = 0;
        this->suprafataUtila = 0;
        this->adresa = "Fara adresa"; 
        this->pretAsigurareLocuinta = 0;
    }

    //Gett-eri
    int getAnConstrunctie() {
        return this->anConstructie;
    }
    float getSuprafataUtila() {
        return this->suprafataUtila;
    }
    string getAdresa() {
        return this->adresa;
    }
    float getPretAsigurareLocuinta() {
        return this->pretAsigurareLocuinta;
    }

    //Sett-eri
    void setAnConstructie(int anConstructieNou) {
        if (anConstructieNou > 0){
            this->anConstructie = anConstructieNou;
            this->pretAsigurareLocuinta = calculeazaPretAsigurareLocuinta();
        }
        else cerr << "Nu putem inregistra asigurarea pentru o cladire construita inainte de Hristos";
    }
    void setSuprafataUtila(float suprafataUtilaNoua) {
        if (suprafataUtilaNoua > 10){
            this->pretAsigurareLocuinta = calculeazaPretAsigurareLocuinta();
            this->suprafataUtila = suprafataUtilaNoua;
        }
        else cerr << "Suprafata utila nu poate fi de " << suprafataUtilaNoua << " mp"<<endl;
    }
    void setAdresa(string adresaNoua) {
        this->adresa = adresaNoua;
    }

    void display(ostream& os) override {
        Asigurare::display(os);
        os << " -----------------------------------------------------------------------" << endl;
        os << "|" << "    ASIGURARE LOCUINTA" << endl;
        os << " -----------------------------------------------------------------------" << endl;
        os << "|" << "An constructie.................... " << this->anConstructie << endl;
        os << "|" << "Suprafata utila................... " << this->suprafataUtila <<" mp" << endl;
        os << "|" << "Adresa............................ " << this->adresa << endl;
        os << "|" << "Pret asigurare.................... " << this->pretAsigurareLocuinta <<" RON" << endl;
        os << " -----------------------------------------------------------------------" << endl;

    }
    void display_v2(ostream& os) override {
        Asigurare::display_v2(os);
        os << "An constructie: " << this->anConstructie << endl;
        os << "Suprafata utila: " << this->suprafataUtila <<" mp" << endl;
        os << "Adresa: " << this->adresa << endl;
        os << "Pret asigurare: " << this->pretAsigurareLocuinta <<" RON" << endl;
    }

    void citireTastatura(istream& is) override {
        Asigurare::citireTastatura(is);
        cout << "        ASIGURARE AUTO " << endl;
        cout << " *" << "Dati an constructie: "; is >> this->anConstructie;
        cout << " *" << "Dati suprafata utila (mp): "; is >> this->suprafataUtila;
        char adresa_temp[100];
        cout << " *" << "Dati Aderesa: "; getchar(); cin.get(adresa_temp, 100);
        this->adresa = adresa_temp;
        this->pretAsigurareLocuinta = calculeazaPretAsigurareLocuinta();
        system("CLS");
    }

    AsigurareLocuinta& operator=(AsigurareLocuinta& a) {
        Asigurare::operator=(a);
        this->anConstructie = a.anConstructie;
        this->adresa = a.adresa;
        this->suprafataUtila = a.suprafataUtila;
        return *this;
    }

    float calculeazaPretAsigurareLocuinta() {
        if (this->suprafataUtila <= 30) {
            this->pretAsigurareLocuinta = 400 + 10 * (this->suprafataUtila - 10);
        }
        else if (this->suprafataUtila <= 50) {
            this->pretAsigurareLocuinta = 500 + 10 * (this->suprafataUtila - 30);
        }
        else if (this->suprafataUtila <= 80) {
            this->pretAsigurareLocuinta = 600 + 10 * (this->suprafataUtila - 50);
        }
        else{
            this->pretAsigurareLocuinta = 700 + 10 * (this->suprafataUtila - 80);
        }

        if (this->anConstructie >= 20 && this->anConstructie <= 40) {
            this->pretAsigurareLocuinta = this->pretAsigurareLocuinta + 200;
        }
        else if (this->anConstructie <= 60) {
            this->pretAsigurareLocuinta = this->pretAsigurareLocuinta + 500;
        }
        else if (this->anConstructie <= 80) {
            this->pretAsigurareLocuinta = this->pretAsigurareLocuinta + 800;
        }
        else {
            this->pretAsigurareLocuinta = this->pretAsigurareLocuinta + 1000;
        }

        this->pretAsigurareLocuinta = this->pretAsigurareLocuinta / 12 * this->perioada;
        
        return this->pretAsigurareLocuinta;
    }

    ~AsigurareLocuinta() {

    }
};

class ListaAsigurari
{
private:
    Asigurare** asigurari;
    int lungime;
public:
    ListaAsigurari()
    {
        this->lungime = 0;
        this->asigurari = new Asigurare * [0];
    }

    void operator += (Asigurare* as)
    {
        Asigurare** temp = new Asigurare * [this->lungime + 1];
        for (int i = 0; i < this->lungime; i++)
        {
            temp[i] = this->asigurari[i];
        }
        temp[this->lungime] = as;
        delete[] this->asigurari;
        this->asigurari = temp;
        this->lungime++;
    }

    friend ostream& operator << (ostream& os, const ListaAsigurari& lista)
    {
        for (int i = 0; i < lista.lungime; i++)
        {
            os << lista.asigurari[i] << endl << endl;
        }

        return os;
    }

    Asigurare* operator[](int index)
    {
        if (index >= this->lungime || index < 0)
            throw exception("Index out of bounds");
        else
            return this->asigurari[index];
    }


    void scriereAsigurareInFisierBinar()
    {
        ofstream wf("asigurari.dat", ios::out | ios::binary);

        if (!wf)
        {
            cerr << "Nu se poate deschide fisierul" << endl;
        }
        else
        {
            wf.write((char*)&lungime, sizeof(lungime));

            for (int i = 0; i < lungime; ++i)
            {
                asigurari[i]->serialize(wf);
            }

            wf.close();

            if (!wf.good())
            {
                cerr << "Eroare la scrierea in fisier" << endl;
            }
        }
    }

    void citireAngajatiDinFisierBinar()
    {
        ifstream rf("asigurari.dat", ios::in | ios::binary);
        if (!rf)
        {
            cerr << "Nu se poate deschide fisierul" << endl;
        }
        else
        {
            rf.read((char*)&lungime, sizeof(lungime));

            asigurari = new Asigurare * [lungime];

            for (int i = 0; i < lungime; ++i)
            {
                asigurari[i] = Asigurare::deserialize(rf);
            }

            rf.close();

            if (!rf.good())
            {
                cerr << "Eroare la citirea din fisier" << endl;
            }
        }
    }

};



    int main()
    {
        Asigurare* a1 = new AsigurareAuto("Marius", CompanieAsiguratorie::Groupama, 12, 2003, 6);
        Asigurare* a2 = new AsigurareLocuinta("Mihaela", CompanieAsiguratorie::GeneraliRomania, 24, 1980, 150, "florilor nr 23");
        Asigurare* a3 = new AsigurareAuto("David", CompanieAsiguratorie::AlianzTiriac, 23, 2013, 2.0);
        
       ListaAsigurari lista;
        lista += a1;
        lista += a2;
        lista += a3;

 

        lista.scriereAsigurareInFisierBinar();
        lista.citireAngajatiDinFisierBinar();


        cout << lista;

    }

