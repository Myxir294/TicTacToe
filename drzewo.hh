/*
 * Plik zawiera deklaracje klas związanych z drzewem gry oraz z modelowaniem
 * sztucznej inteligencji.
 */

#include <iostream>
#include <limits>
#include "ruch.hh"


/*
 * Klasa modeluje pojęcie wezla przechowującego konfigurację planszy oraz
 * dane ruchu z nią powiązane. Ponadto zawiera metody umożliwiające modyfikację
 * przechowywanej wartości, wyświetlenie ich lub dostanie się do sąsiednich węzłów,
 * oznaczanych jako rodzic lub dzieci.
 */

class Wezel_Konfiguracyjny
{
    //Definicje pól prywatnych
    private:
    Ruch ruch; //Pomocnicza struktura odpowiadająca ruchowi, jaki należy wykonać,
               //aby osiągnąć przechowywaną konfigurację z pozycji rodzica węzła.
    int ocena; //1 - wygrywa X, -1 - wygrywa O, 0 - remis
    char** konfiguracja; //Zalążek tablicy dwuwymiarowej, do której zapisana zostanie konfiguracja
    unsigned int rozmiar; //Rozmiar boku planszy gry

    Wezel_Konfiguracyjny** Lista_dzieci; //Wskaznik na liste dzieci wezla
    Wezel_Konfiguracyjny* rodzic; //Wskaznik na rodzica wezla


    // Definicje metod publicznych
    public:
    Wezel_Konfiguracyjny(unsigned int zadany_rozmiar); //Konstruktor wykorzystujący wskazany rozmiar planszy
    ~Wezel_Konfiguracyjny();
 
    const int  Podaj_Ocene() const {return ocena;}    
    const Ruch Podaj_Ruch() const {return ruch;}

    const Wezel_Konfiguracyjny* Podaj_Rodzica() const {return rodzic;}
    void Ustaw_Rodzica(Wezel_Konfiguracyjny *Nowy_Rodzic) {rodzic = Nowy_Rodzic;}    

    Wezel_Konfiguracyjny** Odczyt_Listy_Dzieci() const {return Lista_dzieci;}
    Wezel_Konfiguracyjny** Modyfikacja_Listy_Dzieci() {return Lista_dzieci;}

    bool Czy_Root() const {return(rodzic == nullptr);} //Sprawdzenie, czy węzeł jest korzeniem 
    bool Czy_Zewnetrzny() const {return(Lista_dzieci[0] == nullptr);} //Sprawdzenie, czy węzeł jest liściem 


    /*!
    * Metoda umozliwiajaca podstawienie pod konfigurację bieżącego węzła innej konfiguracji
    * Warunki:
    *      Podany argument musi odpowiadać poprawnej konfiguracji o odpowiednim rozmiarze
    * Argumenty:
    *      Nowa_Konfiguracja - tablica dwuwymiarowa zawierająca konfigurację, która zostanie
    *                          zapisana do węzła.
    * Zwraca:
    *      nic - efekt działania metody widoczny jest w zmianie konfiguracji węzła   
    */    
    void Kopiuj_Konfiguracje(char** Nowa_Konfiguracja); 


    char ** Wsk_Na_Konfiguracje() {return konfiguracja;}
    void Wyswietl_Konfiguracje(); //Wyswietla konfiguracje jako tablice znakow rozmiar x rozmiar

    void Ustaw_ocene(const int& Nowa_Ocena) {this->ocena = Nowa_Ocena;}
    void Ustaw_ruch(const Ruch& Zadany_Ruch) {this->ruch = Zadany_Ruch;}


};

/*
 * Klasa modeluje pojęcie drzewa gry, składającego się z węzłów konfiguracyjnych.
 * Zawiera metody umożliwiające między innymi niezbędne przejścia po drzewie, przechowywanie 
 * obecnego stanu gry oraz wykonanie optymalnego ruchu przez przeciwnika komputerowego.
 */

class Drzewo_Gry
{   
    //Definicje metod i pól prywatnych
    private:
    unsigned int rozmiar_drzewa; //Wartość pomocnicza w celu kontrolowania poprawności usuwania elementów
    unsigned int rozmiar_planszy; //Zarówno rozmiar planszy jak i ilość znaków do wygranej podawana jest przez użytkownika
    unsigned int ilosc_znakow_do_wygranej; 
    Wezel_Konfiguracyjny* korzen;
    Wezel_Konfiguracyjny* obecny_stan; //Wskaźnik na obecnie badany stan planszy; domyślnie inicjalizowany jest tak,
                                       //aby wskazywał na korzeń, tj. element początkowy.


    /*!
    * Metoda umozliwiajaca realizację algorytmu Minimax, wykonując odpowiednie przejścia,
    * dodając nowe możliwe konfigurację oraz dokonując szeregu porównań. Dokładna zasada pracy algorytmu
    * została opisana w sprawozdaniu dołączonym do kodu źródłowego.
    * Argumenty:
    *      pozycja - wskaźnik na pierwszy badany element; jeżeli nie powoduje on końca gry, to zostanie
    *                w nim zapisany najgorszy lub najlepszy możliwy do osiągnięcia wynik z jego pozycji.
    *                W innym przypadku odpowiada on rezultatowi końcowemu.
    *       glebia - określa ilość przewidywanych do przodu optymalnych ruchów obu graczy.
    *                W celu poprawnej symulacji myślenia komputera powinna wynosić co najmniej 1.
    *                Bot wykonuje tym lepsze ruchy, im jej wartość jest większa.
    *    alfa/beta - wartości pomocnicze wykorzystywane do alfa-beta cięć
    *     MaxGracz - określa, czy pożądany przez nas wynik ma być wynikiem maksymalnym (tura X),
    *                czy minimalnym (tura O).   
    * Zwraca:
    *      obiekt int - najlepszy/najgorszy do uzyskania wynik gry, począwszy od pozycji bieżącej   
    */                                       
    int Minimax(Wezel_Konfiguracyjny* pozycja, unsigned int glebia, int alfa, int beta, bool MaxGracz);

    void Usuwanie_Post_Order(Wezel_Konfiguracyjny* pozycja); //Usuwanie elementów drzewa wykorzystująć przejście post-order


    //Metoda umożliwiająca dodanie węzła jako dziecka wskazanego rodzica, wykorzystując w tym celu
    //strukturę informującą o wykonywanym (lub symulowanym) ruchu.
    //W przedstawionym programie nie przewiduje się ręcznego wykorzystania metody,
    //zatem zakwalifikowano ją jako prywatną. 
    void Dodaj_Do(Wezel_Konfiguracyjny* element, Ruch wykonany_ruch); 

    //Definicje metod i pól publicznych    
    public:

    //Konstruktor w odpowiedni sposób przekazujący programowi dane wskazane przez użytkownika oraz
    //automatycznie dodający do drzewa korzeń z konfiguracją początkową planszy.
    Drzewo_Gry(unsigned int zadany_rozmiar_planszy, unsigned int dlugosc_rzedu);

    //Destruktor wykorzystujący przejście post order
    ~Drzewo_Gry(){Usuwanie_Post_Order(korzen); if (rozmiar_drzewa == 0) {std::cout << "Zwolniono pamiec" << std::endl;}}


    unsigned int Podaj_Rozmiar() const {return rozmiar_drzewa;}
    
    Wezel_Konfiguracyjny* Podaj_Korzen() const {return korzen;}
    Wezel_Konfiguracyjny* Podaj_Obecny_Stan() const {return obecny_stan;}
    void Ustaw_Obecny_Stan(Wezel_Konfiguracyjny* stan){obecny_stan = stan;}


    /*!
    * Metoda umożliwiająca dodanie do listy dzieci wskazanego węzła wszystkich możliwych
    * konfiguracji osiągalnych pojedynczym ruchem.
    * Warunki:
    *      Metoda nie robi nic, jeżeli węzeł ma już przynajmniej jedno dziecko, lub odpowiada
    *      całkowitemu zapełnieniu planszy do gry.
    * Argumenty:
    *      pozycja - wskaznik na rodzica, któremu zostaną utworzone dzieci
    *      znak_gracza - informacja o tym, który gracz wykonuje teraz ruch
    * Zwraca:
    *      nic - efekt działania metody widoczny jest w zmianie zawartości drzewa gry   
    */ 
    void Dodaj_Mozliwe_Ruchy(Wezel_Konfiguracyjny* pozycja, char znak_gracza);

    //Poniższa metoda w praktyce sprawdza, czy co najmniej jedno pole planszy jest nadal wolne.
    //Nie jest brane pod uwagę to, czy gra jest już rozstrzygnięta.
    bool Czy_Jest_Mozliwosc_Dalszego_Ruchu(Wezel_Konfiguracyjny* pozycja);


    //Metoda zapisuje wynik gry na podstawie konfiguracji badanego węzła. 
    //W przypadku nierozsztrzygnięcia zapisywana jest wartość 0.
    void Wyznacz_Wynik_W_Pozycji(Wezel_Konfiguracyjny* pozycja);


    //Metoda stanowi zasadniczą część programu związaną ze sztuczną inteligencją.
    //W celu wykonania optymalnego ruchu korzysta z algorytmu minimax oraz informacji
    //o tym, jaką figurą gra bot.
    //Po wywołaniu metody wskażnik obecny_stan zaczyna wskazywać na optymalny
    //ruch przeciwnika komputerowego, możliwy do osiągnięcia z danej pozycji.
    void Wykonaj_optymalny_ruch(unsigned int glebia, char znak_bota);
};
