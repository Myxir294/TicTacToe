/* Plik zawiera definicje struktury definiującej pojęcie ruchu jako
zestawienie współrzednych oraz znaku gracza*/

struct Ruch
{
    int  wiersz;   
    int  kolumna;
    char znak_gracza;    /*! Możliwe X lub O. */
    Ruch& operator = (Ruch Skl1); /*Operator umożliwiający przypisanie jednemu ruchowi wartości innego*/
};

/*Operatory porównujące dwa ruchy*/
bool operator == (Ruch  Skl1, Ruch  Skl2); // True jeśli są takie same
bool operator != (Ruch  Skl1, Ruch  Skl2); // True jeśli nie są takie same
