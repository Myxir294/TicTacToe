#include "drzewo.hh"

Wezel_Konfiguracyjny::Wezel_Konfiguracyjny(unsigned int zadany_rozmiar)
{   
    rodzic = nullptr;
    rozmiar = zadany_rozmiar;
    konfiguracja = new char *[zadany_rozmiar]; //Inicjalizacja tablicy odpowiedzialnej za konfigurację
    for(unsigned int i = 0; i <zadany_rozmiar; i++)
    {
        konfiguracja[i] = new char[zadany_rozmiar];
    }
    Lista_dzieci = new Wezel_Konfiguracyjny*[zadany_rozmiar*zadany_rozmiar]; //Inicjalizacja listy dzieci
    for(unsigned int i = 0; i <(zadany_rozmiar*zadany_rozmiar); i++)
    {
        Lista_dzieci[i] = nullptr;
    }
}

Wezel_Konfiguracyjny::~Wezel_Konfiguracyjny()
{
    for(unsigned int i = 0; i <rozmiar; i++) //Dynamiczne czyszczenie konfiguracji
    {
        delete konfiguracja[i];
        konfiguracja[i] = nullptr;
    }
    delete konfiguracja;
    konfiguracja = nullptr;

    for(unsigned int i = 0; i < (rozmiar*rozmiar); i++) //Czyszczenie listy dzieci
    {   
        Lista_dzieci[i] = nullptr;
    }
    delete Lista_dzieci;
    Lista_dzieci = nullptr;
    rodzic = nullptr; //Usuwanie wskaznika na rodzica 
}

void Wezel_Konfiguracyjny::Kopiuj_Konfiguracje(char** Nowa_Konfiguracja)
{
    for (unsigned int i = 0; i < (this->rozmiar); ++i)
    {
        for (unsigned int j = 0; j < (this->rozmiar); ++j)
        {
            this->konfiguracja[i][j] = Nowa_Konfiguracja[i][j];
        }
    }      
}

void Wezel_Konfiguracyjny::Wyswietl_Konfiguracje() 
{
    for (unsigned int i = 0; i < (this->rozmiar); ++i)
    {
        for (unsigned int j = 0; j < (this->rozmiar); ++j)
        {
            std::cout << this->konfiguracja[i][j] << " ";
        }
    std::cout << std::endl;    
    }     
}

int Drzewo_Gry::Minimax(Wezel_Konfiguracyjny* pozycja, unsigned int glebia, int alfa, int beta, bool MaxGracz)
{   
    bool game_over = false;
    Wyznacz_Wynik_W_Pozycji(pozycja);
    int ocena_ruchu = pozycja->Podaj_Ocene(); //Zapisanie oceny badanej konfiguracji
    if (Czy_Jest_Mozliwosc_Dalszego_Ruchu(pozycja) == false || ocena_ruchu != 0)
    {
        game_over = true;
    }
    if (glebia == 0 || game_over)
    {
        return ocena_ruchu;
    } 

    if (MaxGracz == true) //Tura gracza "X", najlepszy mozliwy wynik do uzyskania to 1
    {
        int max_ocena = std::numeric_limits<int>::min();
        Dodaj_Mozliwe_Ruchy(pozycja, 'X'); //Dodanie do drzewa wszystkich możliwych ruchów X
        unsigned int i = 0;
        while (i < rozmiar_planszy*rozmiar_planszy && pozycja->Odczyt_Listy_Dzieci()[i] != nullptr)
        {   
            //Rekurencyjne ocenianie możliwych poddrzew powstałych w wyniku analizowanego ruchu
            ocena_ruchu = Minimax(pozycja->Odczyt_Listy_Dzieci()[i], glebia - 1, alfa, beta, false);
            max_ocena = std::max(max_ocena, ocena_ruchu);
            alfa = std::max(alfa, ocena_ruchu); //Alfa-beta cięcie
            if (alfa >= beta){break;}
            ++i;
        }
        pozycja->Ustaw_ocene(max_ocena);
        return max_ocena;            
    }
    else //Tura gracza "O", najlepszy mozliwy wynik do uzyskania to -1
    {   
        //Działania analogiczne do opisanych powyżej
        int min_ocena = std::numeric_limits<int>::max();
        Dodaj_Mozliwe_Ruchy(pozycja, 'O');
        unsigned int i = 0;
        while (i < rozmiar_planszy*rozmiar_planszy && pozycja->Odczyt_Listy_Dzieci()[i] != nullptr)
        {   
            ocena_ruchu = Minimax(pozycja->Odczyt_Listy_Dzieci()[i], glebia - 1, alfa, beta, true);
            min_ocena = std::min(min_ocena, ocena_ruchu);
            beta = std::min(beta, ocena_ruchu);
            if (alfa >= beta){break;}                
            ++i;
        }
        pozycja->Ustaw_ocene(min_ocena);
        return min_ocena;                
    }
}

void Drzewo_Gry::Usuwanie_Post_Order(Wezel_Konfiguracyjny *pozycja)
{   
    unsigned int i = 0;

    //Rekurencyjne usuwanie poddrzew do momentu opróżnienia listy dzieci pozycji wywołania
    while (i < rozmiar_planszy*rozmiar_planszy && pozycja->Modyfikacja_Listy_Dzieci()[i] != nullptr)
    {   
        Usuwanie_Post_Order(pozycja->Odczyt_Listy_Dzieci()[i]);
        ++i;
    }
    delete pozycja; //Usunięcie pozycji wywołania
    pozycja = nullptr;
    rozmiar_drzewa--;
}

Drzewo_Gry::Drzewo_Gry(unsigned int zadany_rozmiar_planszy, unsigned int dlugosc_rzedu)
{   
    rozmiar_drzewa = 1;
    rozmiar_planszy = zadany_rozmiar_planszy;

    //Sprawdzenie, czy zadane wartości nie wykluczają się wzajemnie
    if (dlugosc_rzedu > zadany_rozmiar_planszy || dlugosc_rzedu < 2) 
    {
        ilosc_znakow_do_wygranej = zadany_rozmiar_planszy; //Maksymalna możliwa długość rzędu
    }
    else
    {
        ilosc_znakow_do_wygranej = dlugosc_rzedu;    
    }
    korzen = new Wezel_Konfiguracyjny(zadany_rozmiar_planszy); //Dodanie korzenia
    for (unsigned int i = 0; i < (zadany_rozmiar_planszy); ++i) //Inicjalizacja konfiguracji korzenia
    {
        for (unsigned int j = 0; j < (zadany_rozmiar_planszy); ++j)
        {
            korzen->Wsk_Na_Konfiguracje()[i][j] = '_';
        }
    }
    obecny_stan = korzen; //Inicjalizacja obecnego stanu rozgrywki
    std::cout << "Przydzielono pamiec" << std::endl;         
}


void Drzewo_Gry::Dodaj_Do(Wezel_Konfiguracyjny* element, Ruch wykonany_ruch)
{   
    unsigned int i = 0;

    //Zapewnienie, że obecnie dodawany element trafia na poprawne miejsce
    while (element->Odczyt_Listy_Dzieci()[i] != nullptr) 
    {
        i++;
        if (i > ((rozmiar_planszy*rozmiar_planszy) - 1)){throw -1;}
    }
    element->Modyfikacja_Listy_Dzieci()[i] = new Wezel_Konfiguracyjny(rozmiar_planszy); //Utworzenie węzła
    element->Odczyt_Listy_Dzieci()[i]->Kopiuj_Konfiguracje(element->Wsk_Na_Konfiguracje()); //Skopiowanie konfiguracji

    //Modyfikacja konfiguracji
    element->Odczyt_Listy_Dzieci()[i]->Wsk_Na_Konfiguracje()[wykonany_ruch.wiersz][wykonany_ruch.kolumna] = wykonany_ruch.znak_gracza;

    element->Odczyt_Listy_Dzieci()[i]->Ustaw_ruch(wykonany_ruch); //Zapisanie informacji na temat ruchu
    element->Odczyt_Listy_Dzieci()[i]->Ustaw_Rodzica(element); //Przydzielenie rodzica
    rozmiar_drzewa++;
}

void Drzewo_Gry::Dodaj_Mozliwe_Ruchy(Wezel_Konfiguracyjny* pozycja, char znak_gracza)
{   
    if (pozycja->Odczyt_Listy_Dzieci()[0] == nullptr)
    {
        Ruch Nowy_Ruch;
        Nowy_Ruch.znak_gracza = znak_gracza;
        for (unsigned int i = 0; i < this->rozmiar_planszy; i++)
        {
            for (unsigned int j = 0; j < this->rozmiar_planszy; j++)
            {
                if (pozycja->Wsk_Na_Konfiguracje()[i][j] == '_') //Puste pole oznacza możliwość ruchu
                {   
                    Nowy_Ruch.wiersz = i;
                    Nowy_Ruch.kolumna = j;
                    Dodaj_Do(pozycja, Nowy_Ruch); //Dodanie ruchu (dziecka) zapełniającego puste pole
                }
            }
        }
    }
}

bool Drzewo_Gry::Czy_Jest_Mozliwosc_Dalszego_Ruchu(Wezel_Konfiguracyjny* pozycja)
{   
    int pom = 0;
    for (unsigned int i = 0; i < this->rozmiar_planszy; i++)
    {
        for (unsigned int j = 0; j < this->rozmiar_planszy; j++)
        {
            if (pozycja->Wsk_Na_Konfiguracje()[i][j] == '_')
            {                     
                pom++;
            }
        }
    }
    return (pom > 0);
}

void Drzewo_Gry::Wyznacz_Wynik_W_Pozycji(Wezel_Konfiguracyjny* pozycja)
{   
    bool wygralX = false;
    bool wygralO = false;
    unsigned int punkty_X = 1; 
    unsigned int punkty_O = 1;
    int a = 1;

    //Sprawdzenie konfiguracji sąsiadów każdego z pól, do momentu ich wyczerpania lub wykrycia zwycięzcy
    for (unsigned int i = 0; i < this->rozmiar_planszy && wygralO == false && wygralX == false; i++)
    {
        for (unsigned int j = 0; j < this->rozmiar_planszy && wygralO == false && wygralX == false; j++)
        {   
            if (pozycja->Wsk_Na_Konfiguracje()[i][j] == 'X')
            {   
                //lewo-prawo
                while ( j+a < this->rozmiar_planszy && pozycja->Wsk_Na_Konfiguracje()[i][j + a] == 'X'){a++; punkty_X++;}
                a = 1;
                while ( j-a >= 0 && j-a < j && pozycja->Wsk_Na_Konfiguracje()[i][j - a] == 'X'){a++; punkty_X++;}
                a = 1;
                if (punkty_X == this->ilosc_znakow_do_wygranej){wygralX = true;}
                else {punkty_X = 1;}                   

                //gora-dol
                while ( i+a < this->rozmiar_planszy && pozycja->Wsk_Na_Konfiguracje()[i + a][j] == 'X'){a++; punkty_X++;}
                a = 1;
                while ( i-a >= 0 && i-a < i && pozycja->Wsk_Na_Konfiguracje()[i - a][j] == 'X'){a++; punkty_X++;}
                a = 1;
                if (punkty_X == this->ilosc_znakow_do_wygranej){wygralX = true;}
                else {punkty_X = 1;}  

                //na ukos w gore od lewej
                while ( j+a < this->rozmiar_planszy && i+a < this->rozmiar_planszy && pozycja->Wsk_Na_Konfiguracje()[i + a][j + a] == 'X'){a++; punkty_X++;}
                a = 1;
                while ( j-a < this->rozmiar_planszy && i-a < this->rozmiar_planszy && pozycja->Wsk_Na_Konfiguracje()[i - a][j - a] == 'X'){a++; punkty_X++;}
                a = 1;
                if (punkty_X == this->ilosc_znakow_do_wygranej){wygralX = true;}
                else {punkty_X = 1;}

                //na ukos w dol od prawej
                while ( j+a < this->rozmiar_planszy && i-a < this->rozmiar_planszy && pozycja->Wsk_Na_Konfiguracje()[i - a][j + a] == 'X'){a++; punkty_X++;}
                a = 1;
                while ( j-a < this->rozmiar_planszy && i+a < this->rozmiar_planszy && pozycja->Wsk_Na_Konfiguracje()[i + a][j - a] == 'X'){a++; punkty_X++;}
                a = 1;
                if (punkty_X == this->ilosc_znakow_do_wygranej){wygralX = true;}
                else {punkty_X = 1;}                    
            }

            if (pozycja->Wsk_Na_Konfiguracje()[i][j] == 'O')
            {   
                //lewo-prawo
                while ( j+a < this->rozmiar_planszy && pozycja->Wsk_Na_Konfiguracje()[i][j + a] == 'O'){a++; punkty_O++;}
                a = 1;
                while ( j-a >= 0 && j-a < j && pozycja->Wsk_Na_Konfiguracje()[i][j - a] == 'O'){a++; punkty_O++;}
                a = 1;
                if (punkty_O == this->ilosc_znakow_do_wygranej){wygralO = true;}
                else {punkty_O = 1;}                   

                //gora-dol
                while ( i+a < this->rozmiar_planszy && pozycja->Wsk_Na_Konfiguracje()[i + a][j] == 'O'){a++; punkty_O++;}
                a = 1;
                while ( i-a >= 0 && i-a < i && pozycja->Wsk_Na_Konfiguracje()[i - a][j] == 'O'){a++; punkty_O++;}
                a = 1;
                if (punkty_O == this->ilosc_znakow_do_wygranej){wygralO = true;}
                else {punkty_O = 1;}  

                //na ukos w gore od lewej
                while ( j+a < this->rozmiar_planszy && i+a < this->rozmiar_planszy && pozycja->Wsk_Na_Konfiguracje()[i + a][j + a] == 'O'){a++; punkty_O++;}
                a = 1;
                while ( j-a < this->rozmiar_planszy && i-a < this->rozmiar_planszy && pozycja->Wsk_Na_Konfiguracje()[i - a][j - a] == 'O'){a++; punkty_O++;}
                a = 1;
                if (punkty_O == this->ilosc_znakow_do_wygranej){wygralO = true;}
                else {punkty_O = 1;}
                //na ukos w dol od prawej
                while ( j+a < this->rozmiar_planszy && i-a < this->rozmiar_planszy && pozycja->Wsk_Na_Konfiguracje()[i - a][j + a] == 'O'){a++; punkty_O++;}
                a = 1;
                while ( j-a < this->rozmiar_planszy && i+a < this->rozmiar_planszy && pozycja->Wsk_Na_Konfiguracje()[i + a][j - a] == 'O'){a++; punkty_O++;}
                a = 1;
                if (punkty_O == this->ilosc_znakow_do_wygranej){wygralO = true;}
                else {punkty_O = 1;}                    
            }                
        }
    }
    if (wygralO) {pozycja->Ustaw_ocene(-1);}
    else if (wygralX) {pozycja->Ustaw_ocene(1);}
    else {pozycja->Ustaw_ocene(0);}
}

void Drzewo_Gry::Wykonaj_optymalny_ruch(unsigned int glebia, char znak_bota)
{
    int najlepsza_ocena;
    bool znaleziono_ruch = false;
    if (znak_bota == 'X') //Wykorzystanie algorytmu minimax do znalezienia najlepszego osiągalnego wyniku X
    {
        najlepsza_ocena = Minimax(obecny_stan, glebia, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), true);

        //Procedura pomocnicza, przeciwdziałająca przedwczesnemu poddawaniu się bota, jeżeli przy samych optymalnych przewidywanych
        //ruchach ludzki przeciwnik ma gwarantowane zwycięstwo.
        while (najlepsza_ocena == -1 && glebia > 1) 
        {   
            --glebia;
            najlepsza_ocena = Minimax(obecny_stan, glebia, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), true);    
        }
    }
    else if (znak_bota == 'O') //Wykorzystanie algorytmu minimax do znalezienia najlepszego osiągalnego wyniku O
    {
        najlepsza_ocena = Minimax(obecny_stan, glebia, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false);
        while (najlepsza_ocena == 1 && glebia > 1)
        {   
            --glebia;
            najlepsza_ocena = Minimax(obecny_stan, glebia, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false);    
        }            
    }

    //Znalezienie ruchu, który umożliwia przejście do konfiguracji zapewniającej otrzymanie najlepszego osiągalnego wyniku
    for (unsigned int i = 0; i < rozmiar_planszy*rozmiar_planszy && znaleziono_ruch == false && obecny_stan->Odczyt_Listy_Dzieci()[i] != nullptr; i++)
    {       
        if (obecny_stan->Odczyt_Listy_Dzieci()[i]->Podaj_Ocene() == najlepsza_ocena)
        {
            obecny_stan = obecny_stan->Odczyt_Listy_Dzieci()[i];
            obecny_stan->Wyswietl_Konfiguracje(); //Wyświetlenie konfiguracji - graficzna informacja o podjętym ruchu bota
            znaleziono_ruch = true;
        }
    }
}