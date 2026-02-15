#include <iostream>
#include <fstream>  // do pobierania i zapisywania  danych z txt
#include <string>   // do stringa 
#include <vector>   // do wektrów
#include <cstdlib>  // do liczb losowych
#include <ctime>    // do losowania innych liczb nie ciągle tych samych
#include <cctype>   // do sprawdzenia czy to znak czy liczba

using namespace std;

struct Gracz {
    string imie;
    int proby;
    int poziom;
};  // dane gracza

// funkcje pomocnicze

void wyczyscEkran() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
} // funkcja czyszczenia ekranu u Carli też 

void rysujLinie() {
    cout << "----------------------------------------" << endl;
}
// funckcja do rysowania lini ozdobnych


int wczytajLiczbe() {
    string linia; // zmienna tymczasowa na to co wpisze użytkownik
    while (true) {
        // Wczytujemy cala linie tekstu (nawet jak wpiszesz "asdf ghjk")
        if (!getline(cin, linia)) {
            cin.clear();
            continue;
        }

        // Jesli uzytkownik wcisnal tylko ENTER
        if (linia.empty()) continue;

        // Sprawdzamy, czy w napisie sa same cyfry
        bool czyToLiczba = true; // zakładamy że to co wpisał gracz to cyfra
        for (char znak : linia) {   // bierze każdy znak z tego co wpisał gracz
            if (!isdigit(znak)) { // funkcja isdigit sprawdza czy znak to 0-9
                czyToLiczba = false;
                break; // jeśli jednak  to nie jest cyfra wracamy na początek
            }
        }

        if (czyToLiczba) {
            return atoi(linia.c_str());
        }
        else {
            cout << "To nie jest liczba! Wpisz ponownie: ";
        }
    }
}

// funkcje gry

void zapiszDoPliku(string imie, int proby, int poziom) {
    ofstream plik;
    plik.open("wyniki.txt", ios::app);

    if (plik.good()) {
        plik << imie << " " << proby << " " << poziom << endl;
        plik.close();
    }
}
// zapis wyników do pliku

void pokazTop5() {
    wyczyscEkran();
    cout << "--- TABELA WYNIKOW ---" << endl;
    cout << "1. Latwy" << endl;
    cout << "2. Sredni" << endl;
    cout << "3. Trudny" << endl;
    cout << "Wybierz poziom: ";

    int szukanyPoziom = wczytajLiczbe();

    vector<Gracz> wszyscyGracze;
    ifstream plik;
    plik.open("wyniki.txt");

    if (plik.good()) {
        string wczytaneImie;
        int wczytaneProby;
        int wczytanyPoziom;

        while (plik >> wczytaneImie >> wczytaneProby >> wczytanyPoziom) {
            if (wczytanyPoziom == szukanyPoziom) {
                Gracz g;
                g.imie = wczytaneImie;
                g.proby = wczytaneProby;
                g.poziom = wczytanyPoziom;
                wszyscyGracze.push_back(g);
            }
        }
        plik.close();
    }
    else {
        cout << "Brak zapisanych wynikow lub blad pliku!" << endl;
        // Wersja bezpieczna pauzy
        cout << "Nacisnij ENTER aby wrocic...";
        cin.get();
        return;
    }

    // Sortowanie babelkowe
    for (size_t i = 0; i < wszyscyGracze.size(); i++) {
        for (size_t j = 0; j < wszyscyGracze.size() - 1; j++) {
            if (wszyscyGracze[j].proby > wszyscyGracze[j + 1].proby) {
                swap(wszyscyGracze[j], wszyscyGracze[j + 1]);
            }
        }
    }

    cout << endl << "TOP 5 GRACZY:" << endl;
    rysujLinie();

    // wyniki tylko 5 osób najlepsszych 
    int ileWyswietlic = 5;
    if (wszyscyGracze.size() < 5) {
        ileWyswietlic = wszyscyGracze.size();
    }

    if (wszyscyGracze.empty()) {
        cout << "Brak wynikow dla tego poziomu." << endl;
    }
    else {
        for (int i = 0; i < ileWyswietlic; i++) {
            cout << i + 1 << ". " << wszyscyGracze[i].imie
                << " - " << wszyscyGracze[i].proby << " prob" << endl;
        }
    }

    cout << endl;
    cout << "Nacisnij ENTER aby wrocic...";
    getchar(); // Czeka na enter
}

// głowna instrukcja 
void nowaGra() {
    wyczyscEkran();
    cout << "WYBIERZ POZIOM TRUDNOSCI:" << endl;
    cout << "1. Latwy (1-50)" << endl;
    cout << "2. Sredni (1-100)" << endl;
    cout << "3. Trudny (1-250)" << endl;
    cout << "Wybor: ";

    int poziom = wczytajLiczbe();

    int maxLiczba = 50;
    if (poziom == 2) maxLiczba = 100;
    if (poziom == 3) maxLiczba = 250;

    cout << "Czy chcesz tryb zakladu? (1-Tak, 0-Nie): ";
    int czyZaklad = wczytajLiczbe();

    int maxProb = 1000;
    if (czyZaklad == 1) {
        cout << "Podaj limit prob: ";
        maxProb = wczytajLiczbe();
    }

    int wylosowana = rand() % maxLiczba + 1;
    int licznikProb = 0;
    int strzal;
    bool czyWygral = false;

    string tekstyZaMalo[3] = { "Za malo!", "Wiecej!", "Liczba jest wieksza!" };
    string tekstyZaDuzo[3] = { "Za duzo!", "Mniej!", "Liczba jest mniejsza!" };

    wyczyscEkran();
    cout << "ZGADNIJ LICZBE (1 - " << maxLiczba << ")" << endl;
    rysujLinie();

    // petla główna gry
    while (true) {
        if (licznikProb >= maxProb) {
            cout << "PRZEGRALES ZAKLAD! Wykorzystales limit prob." << endl;
            break;
        }

        licznikProb++;
        cout << "Proba nr " << licznikProb << ": ";

        strzal = wczytajLiczbe(); // Uzywamy bezpiecznego wczytywania

        if (strzal == wylosowana) {
            cout << "GRATULACJE! Zgadles!" << endl;
            czyWygral = true;
            break;
        }
        else if (strzal < wylosowana) {
            int los = rand() % 3;
            cout << ">> " << tekstyZaMalo[los] << endl;
        }
        else {
            int los = rand() % 3;
            cout << ">> " << tekstyZaDuzo[los] << endl;
        }
    }

    // jak wygrana, to pyta o imię i zapisuje wynik.
    if (czyWygral) {
        cout << "Podaj imie do rankingu (bez spacji): ";
        // UWAGA: Tu tez zmieniamy na getline, zeby nie psulo bufora
        string imie;
        getline(cin, imie);

        // Zabezpieczenie jakby imie bylo puste (przez enter)
        if (imie.empty()) getline(cin, imie);

        zapiszDoPliku(imie, licznikProb, poziom);
    }

    cout << "Nacisnij ENTER aby kontynuowac...";
    getchar(); 
}

// główna pętla startująca gre 
int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));
    bool czyZagrano = false;

    while (true) {
        wyczyscEkran();
        rysujLinie();
        cout << "GRA: ZGADNIJ LICZBE" << endl;
        rysujLinie();

        cout << "1. Nowa Gra" << endl;
        if (czyZagrano == true) {
            cout << "2. Top 5 Wynikow" << endl;
            cout << "3. Koniec" << endl;
        }
        else {
            cout << "2. Koniec" << endl;
        }

        cout << "Wybierz: ";
        int wybor = wczytajLiczbe();
        if (wybor == 1) {
            nowaGra();
            czyZagrano = true; 
        }
        else if (czyZagrano == true) {
            if (wybor == 2) {
                pokazTop5();
            }
            else if (wybor == 3) {
                break; 
            }
            else {
                cout << "Nie ma takiej opcji! (1-3)" << endl;
                getchar();
            }
        }
        else {
            if (wybor == 2) {
                break; 
            }
            else {
                cout << "Nie ma takiej opcji! (1-2)" << endl;
                cout << "Musisz najpierw zagrac, zeby zobaczyc wyniki!" << endl;
                cout << "Nacisnij ENTER..." << endl;
                getchar();
            }
        }
    }

    return 0;
}