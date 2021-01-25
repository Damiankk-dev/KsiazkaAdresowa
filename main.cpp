#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;
struct kontakt {
    int id = 0;
    string imie = "";
    string nazwisko = "";
    string telefon = "";
    string email = "";
    string adres = "";
};
void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}
void wyswietlMenuGlowne(string header = "Ksiazka kadresowa") {
    cout << "\n" << header << "\n";
    cout << "____________________________\n\n";
    cout << "1. Wpisz nowy kontakt\n";
    cout << "2. Wyszukaj kontakt\n";
    cout << "3. Wyswietl wszystkie kontakty\n";
    cout << "0. Zamknij program\n";
    cout << "____________________________\n";
}
void zapiszKontaktDoPliku(kontakt &kontaktDoZapisania, fstream &plik) {
    plik << endl;
    plik << kontaktDoZapisania.id << "|";
    plik << kontaktDoZapisania.imie << "|";
    plik << kontaktDoZapisania.nazwisko << "|";
    plik << kontaktDoZapisania.telefon << "|";;
    plik << kontaktDoZapisania.email << "|";
    plik << kontaktDoZapisania.adres;
}
void zaktualizujPlik( std::vector<kontakt> &ksiazkaAdresowa) {
    fstream plik;
    plik.open("ksiazkaAdresowa_vec.txt", ios::out | ios::trunc);
    for (std::vector<kontakt>::iterator itr = ksiazkaAdresowa.begin(), finish = ksiazkaAdresowa.end();
            itr != finish; itr++) {
        kontakt biezacyKontakt = *itr;
        zapiszKontaktDoPliku(biezacyKontakt, plik);
    }
    plik.close();
}
void dodajNowyKontakt(std::vector<kontakt> &ksiazkaAdresowa, int &ostatnieId) {

    kontakt* nowyKontakt = new kontakt;
    ostatnieId++;
    nowyKontakt->id = ostatnieId;
    string temp;
    cout << "Podaj imie: ";
    getline(cin, nowyKontakt->imie);
    cout << "Podaj nazwisko: ";
    getline(cin, nowyKontakt->nazwisko);
    cout << "Podaj telefon: ";
    getline(cin, temp);
    nowyKontakt->telefon = temp;
    cout << "Podaj adres e-mail: ";
    getline(cin, nowyKontakt->email);
    cout << "Podaj adres: ";
    getline(cin, nowyKontakt->adres);
    ksiazkaAdresowa.push_back(*nowyKontakt);
    delete nowyKontakt;
}
void formatWyswietlania(kontakt* biezacyKontakt) {
    cout << "\nId \t\t\t"          << biezacyKontakt->id << endl;
    cout << "Imie: \t\t\t"         << biezacyKontakt->imie << endl;
    cout << "Nazwisko: \t\t"         << biezacyKontakt->nazwisko << endl;
    cout << "Nr telefonu: \t\t"      << biezacyKontakt->telefon << endl;
    cout << "Adres e-mail: \t\t"     << biezacyKontakt->email << endl;
    cout << "Adres zamieszkania: \t" << biezacyKontakt->adres << endl;
}
void wyswietlPodmenuSzukaj(string header = "Wyszukaj kontakt") {
    system("cls");
    cout << "\n" << header << "\n";
    cout << "____________________________\n\n";
    cout << "1. Podaj imie\n";
    cout << "2. Podaj nazwisko\n";
    cout << "0. Powrot do menu glownego\n";
    cout << "____________________________\n";
}
void wyswietlKontakty(std::vector<kontakt> &kontakty) {
    if (kontakty.empty()) wyswietlMenuGlowne("Ksiazka adresowa jest pusta!");
    else {
        for (std::vector<kontakt>::iterator itr = kontakty.begin(), finishItr = kontakty.end();
                itr != finishItr; itr++) {
            kontakt * biezacyKontakt = &(*itr);
            formatWyswietlania(biezacyKontakt);
        }
        cout << "Nacisnij dowolny przycisk, aby kontynuowac...";
        getch();
        wyswietlMenuGlowne();
    }
}
int wyswietlOpcjeEdycjiKontaktu() {
    std::cout << "1. Edytuj kontakt\n";
    std::cout << "2. Usun kontakt\n";
    std::cout << "0. Powrot\n";
    return getch();
}
int pobierzIndeksWybranegoKontaktu() {
    std::cout << "Podaj Id wybranego kontaktu:";
    char temp;
    temp = getch();
    return temp - '0' ;
}
std::vector<kontakt>::iterator wybierzKontakt (std::vector<std::vector<kontakt>::iterator> &kontakty) {
    int idWybranegoKontaktu = pobierzIndeksWybranegoKontaktu();
    std::vector<kontakt>::iterator itToReturn;
    for (unsigned int i = 0; i < kontakty.size(); i++) {
        kontakt * biezacyKontakt = &(*kontakty[i]);
        if (idWybranegoKontaktu == biezacyKontakt->id) {
            itToReturn = kontakty[i];
            break;
        } else itToReturn = kontakty[kontakty.size()];
    }
    return itToReturn;
}
void wyswietlPodmenuEdytuj(std::string header = "Edytuj parametr kontaktu:") {
    system("cls");
    std::cout << "\n" << header << "\n";
    std::cout << "____________________________\n\n";
    std::cout << "1. Imie\n";
    std::cout << "2. Nazwisko\n";
    std::cout << "3. Numer telefonu\n";
    std::cout << "4. Adres email\n";
    std::cout << "5. Adres\n";
    std::cout << "0. Powrot do menu glownego\n";
    std::cout << "____________________________\n";
}
void edytujKontakt(kontakt* kontaktDoEdycji) {
    std::string temp;
    wyswietlPodmenuEdytuj();
    char wybor;
    wybor = getch();
    switch (wybor) {
    case '1':
        std::cout << "Podaj imie:";
        getline(cin, temp);
        kontaktDoEdycji->imie = temp;
        break;
    case '2':
        std::cout << "Podaj nazwisko:";
        getline(cin, temp);
        kontaktDoEdycji->nazwisko = temp;
        break;
    case '3':
        std::cout << "Podaj numer telefonu:";
        getline(cin, temp);
        kontaktDoEdycji->telefon = temp;
        break;
    case '4':
        std::cout << "Podaj email:";
        getline(cin, temp);
        kontaktDoEdycji->email = temp;
        break;
    case '5':
        std::cout << "Podaj adres:";
        getline(cin, temp);
        kontaktDoEdycji->adres = temp;
        break;
    case '0':
        wyswietlPodmenuSzukaj();
        break;
    default:
        wyswietlPodmenuEdytuj("Wybierz poprawny parametr!");
        break;
    }
}
void wyszukajKontakt(std::vector<kontakt> &kontakty, string szukane, char wybor) {
    string znalezione;
    std::vector<std::vector<kontakt>::iterator> znalezioneKontakty;
    if (kontakty.empty()) wyswietlPodmenuSzukaj("Ksiazka adresowa jest pusta!");
    else {
        for (std::vector<kontakt>::iterator itr = kontakty.begin(), finishItr = kontakty.end();
                itr != finishItr; itr++) {
            kontakt * biezacyKontakt = &(*itr);
            if (wybor == '1') znalezione = biezacyKontakt->imie;
            else if (wybor == '2') znalezione = biezacyKontakt->nazwisko;
            if (znalezione == szukane) {
                cout << "Znaleziono kontakt: " << "ID: " << biezacyKontakt->id;
                std::cout <<" | " << szukane << endl;
                znalezioneKontakty.push_back(itr);
                formatWyswietlania(biezacyKontakt);
            }
        }
        std::vector<kontakt>::iterator znalezionyKontakt;
        if (!znalezioneKontakty.empty()) {
            if(znalezioneKontakty.size() == 1) {
                znalezionyKontakt = znalezioneKontakty[0];
            } else {
                znalezionyKontakt = wybierzKontakt(znalezioneKontakty);
            }
            kontakt * biezacyKontakt = &(*znalezionyKontakt);
            system("cls");
            formatWyswietlania(biezacyKontakt);

            char wyborEdycji = '9';
            while (wyborEdycji != '0') {
                wyborEdycji = wyswietlOpcjeEdycjiKontaktu();
                switch (wyborEdycji) {
                case '1':
                    edytujKontakt(biezacyKontakt);
                    formatWyswietlania(biezacyKontakt);
                    zaktualizujPlik(kontakty);
                    break;
                case '2': {
                    std::cout << "Aby usunac wybrany kontakt, wcisnij \"t\"\n";
                    char decyzja = getch();
                    if (decyzja == 't') {
                        kontakty.erase(znalezionyKontakt);
                        zaktualizujPlik(kontakty);
                    } else std::cout << "Kontakt nie zostal usuniety\n";
                    break;
                }
                case '0':
                    wyswietlPodmenuSzukaj();
                    break;
                default:
                    std::cout << "Wybierz poprawna opcje!\n";
                    wyswietlOpcjeEdycjiKontaktu();
                    break;
                }
            }
        } else {
            wyswietlPodmenuSzukaj("Nie znaleziono kontaktu: " + szukane + " \n");
        }
    }
}
void podmenuSzukaj(std::vector<kontakt> &ksiazkaAdresowa) {
    char podWybor = '9';
    string szukane;
    while(podWybor!= '0') {
        podWybor = getch();
        switch(podWybor) {
        case '1':
            cout <<"Podaj imie: ";
            getline(cin, szukane);
            wyszukajKontakt(ksiazkaAdresowa, szukane, podWybor);
            break;
        case '2':
            cout <<"Podaj nazwisko: ";
            getline(cin, szukane);
            wyszukajKontakt(ksiazkaAdresowa, szukane, podWybor);
            break;
        case '0':
            system("cls");
            wyswietlMenuGlowne();
            break;
        default:
            wyswietlPodmenuSzukaj("Wybierz poprawna opcje!");
        }
    }
}
kontakt wczytajKontaktZPliku(fstream &plik) {
    string temp;
    std::vector<string> linijkaKontaktu;
    kontakt wczytanyKontakt;
    getline(plik, temp);
    linijkaKontaktu = split(temp, '|');
    wczytanyKontakt.id = stoi(linijkaKontaktu[0]);
    wczytanyKontakt.imie = linijkaKontaktu[1];
    wczytanyKontakt.nazwisko = linijkaKontaktu[2];
    wczytanyKontakt.telefon = linijkaKontaktu[3];
    wczytanyKontakt.email = linijkaKontaktu[4];
    wczytanyKontakt.adres = linijkaKontaktu[5];
    return wczytanyKontakt;
}
int wczytajKsiazkeAdresowa(std::vector<kontakt> &ksiazkaAdresowa) {
    string temp;
    fstream plik;
    plik.open("ksiazkaAdresowa_vec.txt", ios::in);
    int lokalneId = 0;
    getline(plik, temp);
    while(!plik.eof()) {
        kontakt nowyKontakt = wczytajKontaktZPliku(plik);
        ksiazkaAdresowa.push_back(nowyKontakt);
        if (nowyKontakt.id > lokalneId) {
            lokalneId = nowyKontakt.id;
        }
    }
    plik.close();
    return lokalneId;
}
std::string pobierzImieOsatniegoKontaktu(std::vector<kontakt> ksiazkaAdresowa) {
    std::vector<kontakt>::reverse_iterator miejsceOstatniegoElementu = ksiazkaAdresowa.rbegin();
    kontakt* kontaktDoWyswietlenia = &(*miejsceOstatniegoElementu);
    return kontaktDoWyswietlenia->imie;
}
void usunKontakt(std::vector<kontakt>::iterator kontaktDoUsuniecia, std::vector<kontakt> &ksiazkaAdresowa) {
    ksiazkaAdresowa.erase(kontaktDoUsuniecia);
}
int main() {
    fstream plik;
    int wybor;
    int ostatnieId = 0;
    std::vector<kontakt> ksiazkaAdresowa;
    plik.open("ksiazkaAdresowa_vec.txt", ios::in);
    if (plik.good() == false) {
        ofstream plik("ksiazkaAdresowa_vec.txt");
        plik.open("ksiazkaAdresowa_vec.txt", ios::in);
        system("cls");
    }
    plik.close();
    ostatnieId = wczytajKsiazkeAdresowa(ksiazkaAdresowa);
    wyswietlMenuGlowne();
    while (wybor != '0') {
        wybor = getch();
        switch(wybor) {
        case '0':
            system("cls");
            cout << "Koniec programu" << endl;
            break;
        case '1': {
            std::vector<kontakt>::reverse_iterator ostatniElement;
            system("cls");
            dodajNowyKontakt(ksiazkaAdresowa, ostatnieId);
            ostatniElement = ksiazkaAdresowa.rbegin();
            plik.open("ksiazkaAdresowa_vec.txt", ios::out | ios::app);
            zapiszKontaktDoPliku(*ostatniElement, plik);
            plik.close();
            wyswietlMenuGlowne("Poprawnie dodano kontakt: " + pobierzImieOsatniegoKontaktu(ksiazkaAdresowa));
            break;
        }
        case '2':
            wyswietlPodmenuSzukaj();
            podmenuSzukaj(ksiazkaAdresowa);
            break;
        case '3':
            system("cls");
            wyswietlKontakty(ksiazkaAdresowa);
            break;
        default:
            wyswietlMenuGlowne("Wybierz poprawna opcje!");
        }
    }
    return 0;
}
