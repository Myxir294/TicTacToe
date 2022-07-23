#include "ruch.hh"

Ruch& Ruch::operator = (Ruch Skl1)
{
    this->wiersz = Skl1.wiersz; 
    this->kolumna = Skl1.kolumna;
    this->znak_gracza = Skl1.znak_gracza;
    return *this;
}

bool operator == (Ruch  Skl1, Ruch Skl2)
{
    if ((Skl1.wiersz == Skl2.wiersz) && (Skl1.kolumna == Skl2.kolumna) && (Skl1.znak_gracza == Skl2.znak_gracza)) {return true;}
    else {return false;} 
}

bool operator != (Ruch  Skl1, Ruch Skl2)
{
    if ((Skl1.wiersz != Skl2.wiersz) || (Skl1.kolumna != Skl2.kolumna) || (Skl1.znak_gracza == Skl2.znak_gracza)) {return true;}
    else {return false;} 
}