/*Plik zawiera deklarację funkcji pomocniczych, stosowanych
podczas inicjalizacji gry w kółko i krzyżyk*/

#include <iostream>

/*!
 * Funkcja umozliwiajaca wybór znaku gracza
 * Argumenty:
 *      Brak 
 * Zwraca:
 *      obiekt char - 'X' lub 'O', w zależności od wyboru  
 */
char Wybierz_Znak_Gracza();

/*!
 * Funkcja umozliwiajaca wybór rozmiaru boku planszy
 * Argumenty:
 *      Brak 
 * Zwraca:
 *      obiekt unsigned int - wybrany rozmiar z przedziału [3,5]  
 */
unsigned int Wybierz_Rozmiar_Planszy();

/*!
 * Funkcja umozliwiajaca wybór długości rzędu znaków, który zapewnia zwycięstwo
 * Argumenty:
 *      rozmiar_planszy - pomocniczy, w celu uniemożliwienia wyboru wartości
 *                        niemożliwych do uzyskania dla wcześniej wybranego
 *                        rozmiaru planszy
 * Zwraca:
 *      obiekt unsigned int - wybrana dlugosc rzedzu z przedzialu [3, rozmiar_planszy] 
 */
unsigned int Wybierz_Dlugosc_Rzedu(unsigned int rozmiar_planszy);

/*!
 * Funkcja umozliwiajaca wybór trudności gry, związany z głębią algorytmu MiniMax
 * Argumenty:
 *      rozmiar_planszy - pomocniczy, w celu dostosowania maksymalnej glebi do dlugosci
 *                        rozmiaru planszy tak, aby ograniczyc czas oczekiwania na ruch bota
 * Zwraca:
 *      obiekt unsigned int - wartosc glebii (max 10 dla 3x3, 8 dla 4x4, 5 dla 5x5) 
 */
unsigned int Wybierz_Trudnosc(unsigned int rozmiar_planszy);

/*!
 * Funkcja umozliwiajaca wyswietlenie rezultatu meczu
 * Argumenty:
 *      wynik_gry - -1, jezeli wygralo 'O', 0 dla remisu oraz 1 dla wygranej 'X'
 *      znak_gracza - pomocniczy, w celu okreslenia czy wygrala strona gracza czy bota 
 * Zwraca:
 *      Nic - efekt dzialania funkcji widoczny jest poprzez podawany komunikat   
 */
void Wyswietl_Rezultat_Meczu(int wynik_gry, char znak_gracza);
