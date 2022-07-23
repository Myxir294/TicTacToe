#include "drzewo.hh"
#include "funkcje_pomocnicze.hh"
using namespace std;

/*Program umozliwia rozegranie z komputerem gry w kolko i krzyzyk,
wykorzystujac algorytm MiniMax.

 W przyjętym rozwiązaniu gracz zawsze zaczyna jako pierwszy.
 Przed rozpoczęciem meczu następuje wybór figury, rozmiaru planszy,
 długości znaków w rzędzie oraz trudności przeciwnika komputerowego*/

int main()
{

    cout << "Kolko i krzyzyk przeciwko SI" << endl;


    cout << "Wybierz symbol (1 - kolko, 2 - krzyzyk): ";
    char znak_gracza = Wybierz_Znak_Gracza();
    char znak_bota;
    if (znak_gracza == 'X'){znak_bota = 'O';} //Bot gra przeciwną figurą
    else if (znak_gracza == 'O'){znak_bota = 'X';}


    cout << "Wybierz rozmiar planszy (min 3, max 5): ";
    unsigned int rozmiar_planszy = Wybierz_Rozmiar_Planszy();

    cout << "Wybierz ilosc znakow do wygranej (min 3, max rozmiar planszy): ";
    unsigned int ilosc_znakow = Wybierz_Dlugosc_Rzedu(rozmiar_planszy);

    cout << "Wybierz poziom trudnosci (1 - latwy, 2 - sredni, 3 - trudny): ";
    unsigned int glebia = Wybierz_Trudnosc(rozmiar_planszy);

    Drzewo_Gry* wsk = new Drzewo_Gry(rozmiar_planszy,ilosc_znakow); //Utworzenie i inicjalizacja drzewa gry
    wsk->Podaj_Korzen()->Wyswietl_Konfiguracje(); //Pozycja wejściowa - pusta plansza

    //Parametry pomocnicze do przekazywania i przetwarzania ruchów gracza
    unsigned int wiersz = 8; 
    unsigned int kolumna = 8;
    Ruch Nowy_Ruch; //Struktura umożliwiająca powiązanie współrzędnych z figurą
    
    //Parametry pomocnicze do sterowania pętlą
    unsigned int liczba_wykonanych_ruchow = 0;
    bool czy_gracz_wykonal_poprawny_ruch = false;
    bool tura_gracza = true;
    int wynik_gry = 0;

    while(wynik_gry == 0 && liczba_wykonanych_ruchow != rozmiar_planszy*rozmiar_planszy) //Dopóki gra trwa
    {
        if (tura_gracza)
        {
            while (!czy_gracz_wykonal_poprawny_ruch) //Jeżeli gracz nadal nie wykonał poprawnego ruchu
            {   
                wiersz = 8; //8 jest większe niż dozwolone 5 więc początkowo ruch jest niepoprawny
                kolumna = 8;
                cout << "Podaj wspolrzedne swojego ruchu (indeksowanie od 0)" << endl;
                cout << "Wiersz: ";
                while (wiersz > rozmiar_planszy) //Dopóki wartość jest niepoprawna
                {
                    cin  >> wiersz; //Wybór wiersza
                    cout << endl;
                    if (wiersz > rozmiar_planszy)
                    {
                        cout << "Blad rozmiaru, wybierz jeszcze raz: ";    
                    }
                    cin.clear();
                    cin.ignore(1000, '\n');   
                }    
                cout << "Kolumna: ";
                while (kolumna > rozmiar_planszy)
                {
                    cin  >> kolumna; //Wybór kolumny
                    cout << endl;
                    if (kolumna > rozmiar_planszy)
                    {
                        cout << "Blad rozmiaru, wybierz jeszcze raz: ";    
                    }
                    cin.clear();
                    cin.ignore(1000, '\n');   
                }

                //Weryfikacja, czy podane pole jest obecnie puste
                if (wsk->Podaj_Obecny_Stan()->Wsk_Na_Konfiguracje()[wiersz][kolumna] != '_')
                {
                    cout << "To pole jest juz zajete, sprobuj jeszcze raz" << endl;     
                }
                else
                {   
                    if (liczba_wykonanych_ruchow == 0) //Pierwszy ruch modyfikuje root - nie dodaje nowych elementów do drzewa gry
                    {
                        wsk->Podaj_Obecny_Stan()->Wsk_Na_Konfiguracje()[wiersz][kolumna] = znak_gracza; //Modyfikacja pustej planszy
                    }

                    else
                    {   
                        Nowy_Ruch.wiersz = wiersz; //Zapisanie podanych przez gracza wartosci do struktury pomocniczej
                        Nowy_Ruch.kolumna = kolumna;
                        Nowy_Ruch.znak_gracza = znak_gracza;
                        bool znaleziono_ruch = false;
                        wsk->Dodaj_Mozliwe_Ruchy(wsk->Podaj_Obecny_Stan(),znak_gracza); //Dodanie do drzewa mozliwych ruchów gracza                        
                        for (unsigned int i = 0; znaleziono_ruch == false; i++) //Przechodzenie po pętli tak długo, aż napotka się wybrany ruch
                        {   
                            if (wsk->Podaj_Obecny_Stan()->Odczyt_Listy_Dzieci()[i]->Podaj_Ruch() == Nowy_Ruch) //Jeżeli ruch odpowiada wybranemu
                            {   
                                wsk->Ustaw_Obecny_Stan(wsk->Podaj_Obecny_Stan()->Odczyt_Listy_Dzieci()[i]); //Ustawienie obecnej pozycji gry
                                znaleziono_ruch = true;    
                            }
                        }
                    }
                    cout << "Stan planszy po ruchu gracza " << endl;
                    std::cout << "---------------------" << std::endl;  
                    wsk->Podaj_Obecny_Stan()->Wyswietl_Konfiguracje();
                    std::cout << "---------------------" << std::endl;

                    liczba_wykonanych_ruchow++; //Modyfikacja zmiennych pomocniczych
                    czy_gracz_wykonal_poprawny_ruch = true; 
                    tura_gracza = false;                    
                }
            }            
        }
        else
        {
            cout << "Stan planszy po ruchu komputera " << endl;
            std::cout << "---------------------" << std::endl;     
            wsk->Wykonaj_optymalny_ruch(glebia, znak_bota); //Wykonanie optymalnego ruchu przez bota oraz wyświetlenie jego efektu
            std::cout << "---------------------" << std::endl; 
            liczba_wykonanych_ruchow++;
            tura_gracza = true;
            czy_gracz_wykonal_poprawny_ruch = false;            
        }

        wsk->Wyznacz_Wynik_W_Pozycji(wsk->Podaj_Obecny_Stan()); //Sprawdzenie, czy gra dobiegła końca
        wynik_gry = (wsk->Podaj_Obecny_Stan()->Podaj_Ocene());   
             
    }

    Wyswietl_Rezultat_Meczu(wynik_gry,znak_gracza); //Podanie wyniku meczu

    delete wsk; //Zwolnienie pamięci
    wsk = nullptr;
    return 0;
}