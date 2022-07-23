#include "funkcje_pomocnicze.hh"


char Wybierz_Znak_Gracza()
{   
    int wybor_znaku = -1;
    char znak;
    while (wybor_znaku != 1 && wybor_znaku != 2)
    {
        std::cin >> wybor_znaku;
        std::cout << std::endl;
        if (wybor_znaku != 1 && wybor_znaku != 2)
        {
            std::cout << "Niepoprawny wybor, wybierz jeszcze raz: ";    
        }
        if (wybor_znaku == 1)
        {
            std::cout << "Wybrano kolko" << std::endl << std::endl;            
            znak = 'O';    
        }
        if (wybor_znaku == 2)
        {
            std::cout << "Wybrano krzyzyk" << std::endl << std::endl;
            znak = 'X';
        }
        std::cin.clear();
        std::cin.ignore(1000, '\n');   
    }
    return znak;    
}


unsigned int Wybierz_Rozmiar_Planszy()
{
    unsigned int rozmiar_planszy = 2;
    while (rozmiar_planszy < 3 || rozmiar_planszy > 5) //Limit 5
    {
        std::cin  >> rozmiar_planszy;
        std::cout << std::endl;
        if (rozmiar_planszy < 3 || rozmiar_planszy > 5)
        {
            std::cout << "Niepoprawny wybor, wybierz jeszcze raz: ";    
        }
        else
        {
            std::cout << "Wybrano rozmiar " << rozmiar_planszy << std::endl << std::endl;
        }
        std::cin.clear();
        std::cin.ignore(1000, '\n');  
    }
    return rozmiar_planszy;
}

unsigned int Wybierz_Dlugosc_Rzedu(unsigned int rozmiar_planszy)
{
    unsigned int ilosc_znakow = 2;
    while (ilosc_znakow < 3 || ilosc_znakow > rozmiar_planszy) //Fizyczny limit
    {
        std::cin  >> ilosc_znakow;
        std::cout << std::endl;
        if (ilosc_znakow < 3 || ilosc_znakow > rozmiar_planszy)
        {
            std::cout << "Niepoprawny wybor, wybierz jeszcze raz: ";    
        }
        else
        {
            std::cout << "Wybrano " << ilosc_znakow << std::endl << std::endl;
        }
        std::cin.clear();
        std::cin.ignore(1000, '\n');   
    }
    return ilosc_znakow;    
}


unsigned int Wybierz_Trudnosc(unsigned int rozmiar_planszy)
{
    unsigned int wybor = 0;
    unsigned int glebia = 0;
    while (wybor != 1 && wybor != 2 && wybor != 3)
    {
        std::cin  >> wybor;
        std::cout << std::endl;
        if (wybor != 1 && wybor != 2 && wybor != 3)
        {
            std::cout << "Niepoprawny wybor, wybierz jeszcze raz: ";    
        }
        else if (wybor == 1)
        {   
            std::cout << "Wybrano latwy" << std::endl << std::endl;
            glebia = 1;
        }
        else if (wybor == 2)
        {   
            std::cout << "Wybrano sredni" << std::endl << std::endl;
            glebia = 3;           
        }
        else 
        {   
            std::cout << "Wybrano trudny" << std::endl << std::endl;
            if (rozmiar_planszy == 3){glebia = 10;} //Dostosowanie głębi do rozmiaru planszy
            if (rozmiar_planszy == 4){glebia = 8;}  //Maksymalne wartości dobrano eksperymentalnie
            if (rozmiar_planszy == 5){glebia = 5;}
        }
        std::cin.clear();
        std::cin.ignore(1000, '\n'); 
    }
    std::cout << "Glebia wynosi: " << glebia << std::endl << std::endl;
    return glebia;
}

void Wyswietl_Rezultat_Meczu(int wynik_gry, char znak_gracza)
{
    if (znak_gracza == 'X')
    {
        if (wynik_gry == 1){std::cout << "Wygrales" << std::endl;}
        if (wynik_gry == -1){std::cout << "Przegrales" << std::endl;}        
    }
    if (znak_gracza == 'O')
    {
        if (wynik_gry == -1){std::cout << "Wygrales" << std::endl;}
        if (wynik_gry == 1){std::cout << "Przegrales" << std::endl;}        
    }
    if(wynik_gry == 0){std::cout << "Remis" << std::endl;} //Remis niezależnie od tego kim grał gracz    
}