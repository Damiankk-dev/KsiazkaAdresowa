#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>

//using namespace std;
unsigned int userId = 0;
struct kontakt {
    int id = 0;
    std::string imie = "";
    std::string nazwisko = "";
    std::string telefon = "";
    std::string email = "";
    std::string adres = "";
};
struct User {
    int id = 0;
    std::string username = "", password = "";
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
void deleteFile(std::string fileName) {
    if( remove( fileName.c_str() ) != 0 )
        perror( "Error deleting file" );
    else
        puts( "File successfully deleted" );
}
void wyswietlMenuGlowne(std::string header = "Ksiazka kadresowa") {
    std::cout << "\n" << header << "\n";
    std::cout << "____________________________\n\n";
    std::cout << "1. Wpisz nowy kontakt\n";
    std::cout << "2. Wyszukaj kontakt\n";
    std::cout << "3. Wyswietl wszystkie kontakty\n";
    std::cout << "0. Zamknij aplikacje\n";
    std::cout << "____________________________\n";
}
void zapiszKontaktDoPliku(kontakt &kontaktDoZapisania, std::fstream &plik, unsigned int & signedInUserId) {
    plik << std::endl;
    plik << kontaktDoZapisania.id << "|";
    plik << signedInUserId << "|";
    plik << kontaktDoZapisania.imie << "|";
    plik << kontaktDoZapisania.nazwisko << "|";
    plik << kontaktDoZapisania.telefon << "|";;
    plik << kontaktDoZapisania.email << "|";
    plik << kontaktDoZapisania.adres;
}
void createFileIfNotExist(std::string fileName) {
    std::fstream file;
    file.open(fileName, std::ios::in);
    if (file.good() == false) {
        std::ofstream file(fileName);
        file.open(fileName, std::ios::in);
        system("cls");
        file.close();
    }
    file.close();
}
kontakt wczytajKontaktZPliku(std::fstream &plik, unsigned int &registeredUserId) {
    std::string temp;
    std::vector<std::string> linijkaKontaktu;
    kontakt wczytanyKontakt;
    getline(plik, temp);
    linijkaKontaktu = split(temp, '|');
    registeredUserId = stoi(linijkaKontaktu[1]);
    wczytanyKontakt.id = stoi(linijkaKontaktu[0]);
    wczytanyKontakt.imie = linijkaKontaktu[2];
    wczytanyKontakt.nazwisko = linijkaKontaktu[3];
    wczytanyKontakt.telefon = linijkaKontaktu[4];
    wczytanyKontakt.email = linijkaKontaktu[5];
    wczytanyKontakt.adres = linijkaKontaktu[6];
    return wczytanyKontakt;
}
void zaktualizujPlik( std::vector<kontakt> &ksiazkaAdresowa ) {
    std::fstream plikTymczasowy;
    std::string fileName = "ksiazkaAdresowa_vec_temp.txt";
    createFileIfNotExist(fileName);
    std::fstream baseFile;
    std::string baseFileName = "ksiazkaAdresowa_vec.txt";
    std::string lineFromBaseFile = "";
    kontakt baseContact;
    unsigned int addressId = 0;
    plikTymczasowy.open(fileName, std::ios::out | std::ios::trunc);
    baseFile.open(baseFileName, std::ios::in);
    getline(baseFile, lineFromBaseFile);

    while (!baseFile.eof()) {
        baseContact = wczytajKontaktZPliku(baseFile, addressId);
        if (addressId != userId) {
            zapiszKontaktDoPliku(baseContact, plikTymczasowy, addressId);
        } else {
            for (std::vector<kontakt>::iterator itr = ksiazkaAdresowa.begin(), finishItr = ksiazkaAdresowa.end();
                    itr != finishItr; itr++) {
                if (baseContact.id == itr->id) zapiszKontaktDoPliku(*itr, plikTymczasowy, userId);
            }
        }
    }
    plikTymczasowy.close();
    baseFile.close();
    deleteFile(baseFileName);
    rename(fileName.c_str(), baseFileName.c_str());
}
void dodajNowyKontakt(std::vector<kontakt> &ksiazkaAdresowa, int &ostatnieId) {

    kontakt* nowyKontakt = new kontakt;
    ostatnieId++;
    nowyKontakt->id = ostatnieId;
    std::string temp;
    std::cout << "Podaj imie: ";
    getline(std::cin, nowyKontakt->imie);
    std::cout << "Podaj nazwisko: ";
    getline(std::cin, nowyKontakt->nazwisko);
    std::cout << "Podaj telefon: ";
    getline(std::cin, temp);
    nowyKontakt->telefon = temp;
    std::cout << "Podaj adres e-mail: ";
    getline(std::cin, nowyKontakt->email);
    std::cout << "Podaj adres: ";
    getline(std::cin, nowyKontakt->adres);
    ksiazkaAdresowa.push_back(*nowyKontakt);
    delete nowyKontakt;
}
void formatWyswietlania(kontakt* biezacyKontakt) {
    std::cout << "\nId \t\t\t"          << biezacyKontakt->id << std::endl;
    std::cout << "Imie: \t\t\t"         << biezacyKontakt->imie << std::endl;
    std::cout << "Nazwisko: \t\t"         << biezacyKontakt->nazwisko << std::endl;
    std::cout << "Nr telefonu: \t\t"      << biezacyKontakt->telefon << std::endl;
    std::cout << "Adres e-mail: \t\t"     << biezacyKontakt->email << std::endl;
    std::cout << "Adres zamieszkania: \t" << biezacyKontakt->adres << std::endl;
}
void wyswietlPodmenuSzukaj(std::string header = "Wyszukaj kontakt") {
    system("cls");
    std::cout << "\n" << header << "\n";
    std::cout << "____________________________\n\n";
    std::cout << "1. Podaj imie\n";
    std::cout << "2. Podaj nazwisko\n";
    std::cout << "0. Powrot do menu glownego\n";
    std::cout << "____________________________\n";
}
void wyswietlKontakty(std::vector<kontakt> &kontakty) {
    if (kontakty.empty()) wyswietlMenuGlowne("Ksiazka adresowa jest pusta!");
    else {
        for (std::vector<kontakt>::iterator itr = kontakty.begin(), finishItr = kontakty.end();
                itr != finishItr; itr++) {
            kontakt * biezacyKontakt = &(*itr);
            formatWyswietlania(biezacyKontakt);
        }
        std::cout << "Nacisnij dowolny przycisk, aby kontynuowac...";
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
        getline(std::cin, temp);
        kontaktDoEdycji->imie = temp;
        break;
    case '2':
        std::cout << "Podaj nazwisko:";
        getline(std::cin, temp);
        kontaktDoEdycji->nazwisko = temp;
        break;
    case '3':
        std::cout << "Podaj numer telefonu:";
        getline(std::cin, temp);
        kontaktDoEdycji->telefon = temp;
        break;
    case '4':
        std::cout << "Podaj email:";
        getline(std::cin, temp);
        kontaktDoEdycji->email = temp;
        break;
    case '5':
        std::cout << "Podaj adres:";
        getline(std::cin, temp);
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
void wyszukajKontakt(std::vector<kontakt> &kontakty, std::string szukane, char wybor) {
    std::string znalezione;
    std::vector<std::vector<kontakt>::iterator> znalezioneKontakty;
    if (kontakty.empty()) wyswietlPodmenuSzukaj("Ksiazka adresowa jest pusta!");
    else {
        for (std::vector<kontakt>::iterator itr = kontakty.begin(), finishItr = kontakty.end();
                itr != finishItr; itr++) {
            kontakt * biezacyKontakt = &(*itr);
            if (wybor == '1') znalezione = biezacyKontakt->imie;
            else if (wybor == '2') znalezione = biezacyKontakt->nazwisko;
            if (znalezione == szukane) {
                std::cout << "Znaleziono kontakt: " << "ID: " << biezacyKontakt->id;
                std::cout <<" | " << szukane << std::endl;
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
    std::string szukane;
    while(podWybor!= '0') {
        podWybor = getch();
        switch(podWybor) {
        case '1':
            std::cout <<"Podaj imie: ";
            getline(std::cin, szukane);
            wyszukajKontakt(ksiazkaAdresowa, szukane, podWybor);
            break;
        case '2':
            std::cout <<"Podaj nazwisko: ";
            getline(std::cin, szukane);
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
int wczytajKsiazkeAdresowa(std::vector<kontakt> &ksiazkaAdresowa) {
    std::string temp;
    std::fstream plik;
    plik.open("ksiazkaAdresowa_vec.txt", std::ios::in);
    int lokalneId = 0;
    unsigned int otherUser = 0;
    getline(plik, temp);
    while(!plik.eof()) {
        kontakt nowyKontakt = wczytajKontaktZPliku(plik, otherUser);
        if (nowyKontakt.id > lokalneId) {
            lokalneId = nowyKontakt.id;
        }
        if (otherUser == userId) {
            ksiazkaAdresowa.push_back(nowyKontakt);
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
void showMenuSignedIn(std::string header = "Poprawne logowanie!\n") {
    std::cout << "\n" << header << "\n";
    std::cout << "____________________________\n\n";
    std::cout << "1. Zmiana hasla\n";
    std::cout << "2. Wylogowanie\n";
    std::cout << "3. Przejscie do aplikacji\n";
    std::cout << "____________________________\n";
}
void maitainAdressBookMenu(unsigned int &userId) {
    std::fstream plik;
    int wybor;
    int ostatnieId = 0;
    std::vector<kontakt> ksiazkaAdresowa;
    plik.open("ksiazkaAdresowa_vec.txt", std::ios::in);
    if (plik.good() == false) {
        std::ofstream plik("ksiazkaAdresowa_vec.txt");
        plik.open("ksiazkaAdresowa_vec.txt", std::ios::in);
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
            showMenuSignedIn("Zamknieto ksiazke adresowa");
            break;
        case '1': { //dodaj Nowy kontakt
            std::vector<kontakt>::reverse_iterator ostatniElement;
            system("cls");
            dodajNowyKontakt(ksiazkaAdresowa, ostatnieId);
            ostatniElement = ksiazkaAdresowa.rbegin();
            plik.open("ksiazkaAdresowa_vec.txt", std::ios::out | std::ios::app);
            zapiszKontaktDoPliku(*ostatniElement, plik, userId);
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

}
void showMainMenu(std::string header = "MENU GLOWNE") {
    std::cout << "\n" << header << "\n";
    std::cout << "____________________________\n\n";
    std::cout << "1. Logowanie\n";
    std::cout << "2. Rejestracja\n";
    std::cout << "0. Zamknij program\n";
    std::cout << "____________________________\n";
}
std::string getUsername() {
    std::string inputUsername;
    std::cout << "Podaj login\n";
    getline(std::cin, inputUsername);
    return inputUsername;
}
bool isInsertedUsernameCorrect(std::map<std::string, std::string>& registeredUsers, std::string& username) {
    std::pair<std::map<std::string,std::string>::iterator,bool> ret;
    ret = registeredUsers.insert(
              std::pair<std::string, std::string>(username, "") );
    return ret.second;
}
std::string getPassword() {
    std::string inputPass;
    std::cout << "Podaj haslo\n";
    getline(std::cin, inputPass);
    return inputPass;
}
bool isPasswordCorrect(std::string& password) {
    int passLength = password.length();
    if (passLength >= 4) return true;
    else return false;
}
std::string createCorrectUsername(std::map<std::string, std::string>& registeredUsers) {
    bool confirmUsername = false;
    do {
        std::string newUsername = getUsername();
        confirmUsername = isInsertedUsernameCorrect(registeredUsers, newUsername);
        if (confirmUsername == true) {
            std::cout << "Poprawnie dodano uzytkownika\n";
            userId = registeredUsers.size();
            return newUsername;
        } else {
            std::cout << "Taki uzytkownik juz istnieje\n";
            std::cout << "Podaj inny login\n";
        }
    } while(confirmUsername == false);
    return "";
}
std::string getCorrectPassword() {
    unsigned int wrongPasswordApproach = 0;
    std::string inputPass ;
    do {
        wrongPasswordApproach++;
        inputPass = getPassword();
        if (isPasswordCorrect(inputPass)) {
            return inputPass;
        } else {
            std::cout << "Haslo musi posiadac przynajmniej 4 znaki\n";
        }
    } while (isPasswordCorrect(inputPass) == false);
    return "";
}
void saveUserData2File(std::map<std::string, std::string>& registeredUsers, std::string &user) {
    std::string fileName = "zarejestrowani_uzytkownicy.txt";
    std::fstream file;
    file.open(fileName, std::ios::out | std::ios::app);
    std::map<std::string, std::string>::iterator userItr;
    userItr = registeredUsers.find(user);
    file << registeredUsers.size() << '|';
    file << userItr->first << '|';
    file << userItr->second << '\n';
    file.close();
}
std::string createUser(std::map<std::string, std::string>& registeredUsers) {
    std::string newUser;
    newUser = createCorrectUsername(registeredUsers);
    registeredUsers[newUser] = getCorrectPassword();
    saveUserData2File(registeredUsers, newUser);
    return newUser;
}
std::map<std::string, std::string>::iterator getRegisteredUser(std::map<std::string, std::string>& registeredUsers) {
    std::map<std::string, std::string>::iterator usernameItr;
    do {
        std::string userToLogIn = getUsername();
        usernameItr = registeredUsers.find(userToLogIn);
        if (usernameItr == registeredUsers.end()) {
            std::cout << "Uzytkownik o podanym loginie nie istnieje, podaj wlasciwy login\n";
        }
    } while (usernameItr == registeredUsers.end());
    return usernameItr;
}
bool isPasswordMatch(std::map<std::string, std::string>::iterator &usernameItr) {
    std::string password = getPassword();
    if (usernameItr->second == password) return true;
    else return false;
}
bool isPasswordVerified(std::map<std::string, std::string>::iterator &usernameItr) {
    unsigned int wrongPasswordAttempt = 0;
    while (isPasswordMatch(usernameItr) == false ) {
        wrongPasswordAttempt++;
        if (wrongPasswordAttempt >= 3) {
            std::cout << "Blednie wpisano haslo 3 razy, nalezy odczekac 5 sekund\n";
            wrongPasswordAttempt = 0;
            Sleep(5000);
        }
    }
    return true;
}
std::map<std::string, std::string>::iterator getSignedInUser (std::map<std::string, std::string>& registeredUsers) {
    std::map<std::string, std::string>::iterator usernameItr;
    usernameItr = getRegisteredUser(registeredUsers);
    if (usernameItr != registeredUsers.end() ) {
        if (isPasswordVerified(usernameItr) ) return usernameItr;
    } else return registeredUsers.end();
}
void changePassword(std::map<std::string, std::string>::iterator &usernameItr) {
    std::string newPassword = getCorrectPassword();
    usernameItr->second = newPassword;
}
void maitainMenuSignedIn(std::map<std::string, std::string> &registeredUsers, std::map<std::string, std::string>::iterator &usernameItr) {
    char choice = '\0';
    while(choice != '2') {
        choice = getch();
        switch(choice) {
        case '1':
            system("cls");
            changePassword(usernameItr);
            showMenuSignedIn("Poprawnie zmieniono haslo");
            break;
        case '2':
            system("cls");
            showMainMenu("Wylogowano uzytkownika");
            break;
        case '3':

            maitainAdressBookMenu(userId);
//            choice = '2';
            break;
        default:
            break;
        }

    }

}
std::vector<std::string> loadRegisteredUsers(std::map<std::string, std::string>& registeredUsers) {
    std::string temp;
    std::fstream file;
    std::vector<std::string> registeredUsersIds;
    std::vector<std::string> userDataLine;
    file.open("zarejestrowani_uzytkownicy.txt", std::ios::in);
    getline(file, temp);
    while(!file.eof()) {
        userDataLine = split(temp, '|');
        std::string loadedUser = userDataLine[1];
        registeredUsersIds.push_back(loadedUser);
        registeredUsers[loadedUser] = userDataLine[2];
        getline(file, temp);
    }
    file.close();
    return registeredUsersIds;
}
void maitainSignInMenu(std::map<std::string, std::string> &registeredUsers) {
    std::vector<std::string> registeredUsersIds;
    std::fstream registeredUsersFile;
    std::string registeredUsersFileName = "zarejestrowani_uzytkownicy.txt";
    createFileIfNotExist(registeredUsersFileName);
    registeredUsersIds = loadRegisteredUsers(registeredUsers);
    if (registeredUsers.size() != 0) userId = registeredUsers.size();
    char choice = '\0';
    showMainMenu();
    while(choice != '0') {
        choice = getch();
        switch(choice) {
        case '1': {
            std::map<std::string, std::string>::iterator signedUser = getSignedInUser(registeredUsers);
            if ( signedUser != registeredUsers.end() ) {
//                for (std::vector<std::string>::iterator itr = registeredUsersIds.begin(), finishItr = registeredUsersIds.end();
//                     itr != finishItr; itr++){
//                        if
//                     }
                for (unsigned int i = 0; i < registeredUsersIds.size(); i++){
                    if (registeredUsersIds[i] == signedUser->first) userId = i+1;
                }
                showMenuSignedIn();
                maitainMenuSignedIn(registeredUsers, signedUser);
            }
            break;
        }
        case '2': {
            system("cls");
            std::string createdUser = createUser(registeredUsers);
            showMainMenu("Poprawnie dodano uzytkownika: " + createdUser);
            registeredUsersIds.push_back(createdUser);
            break;
        }
        case '0':
            system("cls");
            std::cout << "Koniec programu\n";
            break;
        default:
            break;
        }

    }

}
int main() {
    std::map<std::string, std::string> registeredUsers;
    maitainSignInMenu(registeredUsers);
    return 0;
}
