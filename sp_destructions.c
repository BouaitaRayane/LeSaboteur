#include "header.h"

/*
Sous-programme de libération des polices et des BITMAPs
*/
void liberationPolicesBIT(FONT *tabPolices[POLICES], BITMAP *tabBIT[BIT])
{
    liberationPolices(tabPolices, POLICES-1); //Libération des polices
    liberationBITMAPS(tabBIT, BIT-1); //Libération des BITMAPs
}


/*
Sous-programme de libération des polices
*/
void liberationPolices(FONT *tabPolices[POLICES], int etatActuel)
{
    int i; //Permet le bon fonctionnement de la boucle de suppression

    for(i=etatActuel ; i>=0 ; i--) //Boucle de suppression
    {
        destroy_font(tabPolices[i]); //Destruction de chaque police
    }
}


/*
Sous-programme de libération des BITMAPs
*/
void liberationBITMAPS(BITMAP *tabBIT[BIT], int etatActuel)
{
    int i; //Permet le bon fonctionnement de la boucle de suppression

    for(i=etatActuel ; i>=0 ; i--) //Boucle de suppression
    {
        destroy_bitmap(tabBIT[i]); //Destruction de chaque BITMAP
    }
}
