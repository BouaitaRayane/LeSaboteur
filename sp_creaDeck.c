#include "header.h"

/*
Sous-programme de création générale du deck
*/
void creationDeck(t_pioche *pioche, BITMAP *tabBIT[BIT], int *erreur)
{
    int i; //Permet le bon fonctionnement de la boucle d'empilement
    t_carte *deckInitial[69]; //Permet de stocker le deck contenant l'ensemble des cartes (Action / Terrain)

    initTabCartes(deckInitial, tabBIT, erreur); //Création des cartes dans le tableau dédié

    if(*erreur == 0) //Cas où l'initialisation a pu se faire
    {
        melangeDeckInitial(deckInitial); //Mélange des cartes dans le tableau

        initPioche(pioche, erreur); //Création de la pile devant accueillir le deck jouable

        if(*erreur == 0) //Cas où l'initialisation de la pile du deck a réussi
        {
            for(i=0 ; i<69 ; i++) //Boucle d'empilement de la pioche (constitution de la pioche)
            {
                if(*erreur == 0) //Processus se faisant seulement si aucune erreur détectée
                {
                    empilerPioche(pioche, deckInitial[i], erreur); //Appel du sous-programme d'empilement
                }
            }
        }

        liberationDeckInitial(deckInitial, 68); //Libération du tableau ayant permis la création de la pile du deck
    }
}


/*
Sous-programme permettant la création du deck dans un tableau (deck initial)
*/
void initTabCartes(t_carte *deck[69], BITMAP *tabBIT[BIT], int *erreur)
{
    int i; //Permet le bon fonctionement de la boucle de parcours du tableau

    for (i=0; i<69; i++) //Parcours du tableau afin de le remplir
    {
        if(*erreur == 0) //Permet l'arrêt du processus en cas d'erreur
        {
           deck[i] = (t_carte*)malloc(sizeof(t_carte)); //Allocation dynamique de la structure principale d'une carte

            if (deck[i]==NULL) //Cas d'une erreur d'allocation mémoire
            {
                printf ("\n\nPROBLEME d'allocation memoire\n\n");
                *erreur = 1; //Incrémentation de l'indicateur d'erreur

                liberationDeckInitial(deck, i-1); //Appel du sous-programme permettant la libération du tableau en question en cas d'erreur
            }

            else //Cas où l'allocation a pu se faire
            {
                if(i <= 44) //Création des cartes de type Terrain
                {
                    deck[i]->typeTerrain = (t_carteTerrain*)malloc(sizeof(t_carteTerrain)); //Allocation dynamique de la structure d'une carte terrain

                    if (deck[i]->typeTerrain == NULL) //Cas d'une erreur d'allocation mémoire
                    {
                        printf ("\n\nPROBLEME d'allocation memoire\n\n");
                        *erreur = 1; //Incrémentation de l'indicateur d'erreur

                        free(deck[i]);
                        liberationDeckInitial(deck, i-1); //Appel du sous-programme permettant la libération du tableau en question en cas d'erreur
                    }

                    else //Cas où l'allocation a pu se faire
                    {
                        deck[i]->typeAction = NULL; //Initialisation du pointeur sur les données d'une carte type Action

                        deck[i]->typeAT ='t'; //Initialisation de l'identificateur de carte Action / Terrain

                        deck[i]->typeTerrain->visuelArriere = tabBIT[27];

                        if (i>=0 && i<=8) //Cas d'une carte T Haut
                        {
                            //Initialisation des jonctions possibles avec la carte
                            deck[i]->typeTerrain->haut = 1;
                            deck[i]->typeTerrain->bas = 0;
                            deck[i]->typeTerrain->gauche = 1;
                            deck[i]->typeTerrain->droite = 1;
                            strcpy(deck[i]->typeTerrain->typeJonction, "Th"); //Croisement T Haut
                            deck[i]->typeTerrain->visuelAvant = tabBIT[43];
                        }

                        else if (i>=9 && i<=17) //Cas d'une carte T Droite
                        {
                            //Initialisation des jonctions possibles avec la carte
                            deck[i]->typeTerrain->haut = 1;
                            deck[i]->typeTerrain->bas = 1;
                            deck[i]->typeTerrain->gauche = 0;
                            deck[i]->typeTerrain->droite = 1;
                            strcpy(deck[i]->typeTerrain->typeJonction, "Td"); //Croisement T Droite
                            deck[i]->typeTerrain->visuelAvant = tabBIT[44];
                        }

                        else if (i>=18 && i<=26) //Cas d'une Impasse
                        {
                            //Initialisation des jonctions possibles avec la carte
                            deck[i]->typeTerrain->haut = 0;
                            deck[i]->typeTerrain->bas = 0;
                            deck[i]->typeTerrain->gauche = 1;
                            deck[i]->typeTerrain->droite = 0;
                            strcpy(deck[i]->typeTerrain->typeJonction, "Im"); //Impasse
                            deck[i]->typeTerrain->visuelAvant = tabBIT[41];
                        }

                        else if (i>=27 && i<=35) //Cas d'un Carrefour
                        {
                            //Initialisation des jonctions possibles avec la carte
                            deck[i]->typeTerrain->haut = 1;
                            deck[i]->typeTerrain->bas = 1;
                            deck[i]->typeTerrain->gauche = 1;
                            deck[i]->typeTerrain->droite = 1;
                            strcpy(deck[i]->typeTerrain->typeJonction, "Ca"); //Carrefour
                            deck[i]->typeTerrain->visuelAvant = tabBIT[40];
                        }

                        else if (i>=36 && i<=44) //Cas d'un chemin Tout Droit
                        {
                            //Initialisation des jonctions possibles avec la carte
                            deck[i]->typeTerrain->haut = 0;
                            deck[i]->typeTerrain->bas = 0;
                            deck[i]->typeTerrain->gauche = 1;
                            deck[i]->typeTerrain->droite = 1;
                            strcpy(deck[i]->typeTerrain->typeJonction, "TD"); //Tout droit
                            deck[i]->typeTerrain->visuelAvant = tabBIT[42];
                        }
                    }
                }

                else //Création des cartes de type Action
                {
                    deck[i]->typeAction = (t_carteAction*)malloc(sizeof(t_carteAction)); //Allocation dynamique de la structure d'une carte terrain

                    if (deck[i]->typeAction == NULL) //Cas d'une erreur d'allocation mémoire
                    {
                        printf ("\n\nPROBLEME d'allocation memoire\n\n");
                        *erreur = 1; //incrémentation de l'indicateur d'erreur

                        free(deck[i]);
                        liberationDeckInitial(deck, i-1); //Appel du sous-programme permettant la libération du tableau en question en cas d'erreur
                    }

                    else //Cas où l'allocation a pu se faire
                    {
                        deck[i]->typeTerrain = NULL; //Initialisation du pointeur sur les données d'une carte de type Terrain

                        deck[i]->typeAT ='a'; //Initialisation de l'identificateur de carte Action / Terrain

                        deck[i]->typeAction->visuelArriere = tabBIT[27];

                        if (i==45 || i==46) //Carte "Traîneau en bois réparé"
                        {
                            //Initialisation des identificateurs de carte
                            deck[i]->typeAction->effet = 'r'; //'r' pour "réparation" et 'p' pour "pénalité"
                            deck[i]->typeAction->action = 'a'; //Inidicateur arbitraire
                            deck[i]->typeAction->visuelAvant = tabBIT[37];
                        }

                        else if (i==47 || i==48) //Carte "Canne à pêche réparée"
                        {
                            //Initialisation des identificateurs de carte
                            deck[i]->typeAction->effet = 'r';
                            deck[i]->typeAction->action = 'b';
                            deck[i]->typeAction->visuelAvant = tabBIT[33];
                        }

                        else if (i==49 || i==50) //Carte "Scie à glace réparée"
                        {
                            //Initialisation des identificateurs de carte
                            deck[i]->typeAction->effet = 'r';
                            deck[i]->typeAction->action = 'c';
                            deck[i]->typeAction->visuelAvant = tabBIT[35];
                        }

                        else if (i==51 || i==52) //Carte "Traîneau en bois et canne à pêche réparés"
                        {
                            //Initialisation des identificateurs de carte
                            deck[i]->typeAction->effet = 'r';
                            deck[i]->typeAction->action = 'd';
                            deck[i]->typeAction->visuelAvant = tabBIT[38];
                        }

                        else if (i==53 || i==54) //Carte "Canne à pêche et scie à glace réparées"
                        {
                            //Initialisation des identificateurs de carte
                            deck[i]->typeAction->effet = 'r';
                            deck[i]->typeAction->action = 'e';
                            deck[i]->typeAction->visuelAvant = tabBIT[36];
                        }

                        else if (i==55 || i==56) //Carte "Scie à glace et traîneau en bois réparés"
                        {
                            //Initialisation des identificateurs de carte
                            deck[i]->typeAction->effet = 'r';
                            deck[i]->typeAction->action = 'f';
                            deck[i]->typeAction->visuelAvant = tabBIT[39];
                        }

                        else if (i==57 || i==58) //Carte "Map"
                        {
                            //Initialisation des identificateurs de carte
                            deck[i]->typeAction->effet = 'r';
                            deck[i]->typeAction->action = 'g';
                            deck[i]->typeAction->visuelAvant = tabBIT[34];
                        }

                        else if (i==59 || i==60) //Carte "Traîneau en bois cassé"
                        {
                            //Initialisation des identificateurs de carte
                            deck[i]->typeAction->effet = 'p';
                            deck[i]->typeAction->action = 'h';
                            deck[i]->typeAction->visuelAvant = tabBIT[32];
                        }

                        else if (i==61 || i==62) //Carte "Canne à pêche cassée"
                        {
                            //Initialisation des identificateurs de carte
                            deck[i]->typeAction->effet = 'p';
                            deck[i]->typeAction->action = 'i';
                            deck[i]->typeAction->visuelAvant = tabBIT[28];
                        }

                        else if (i==63 || i==64) //Carte "Scie à glace cassée"
                        {
                            //Initialisation des identificateurs de carte
                            deck[i]->typeAction->effet = 'p';
                            deck[i]->typeAction->action = 'j';
                            deck[i]->typeAction->visuelAvant = tabBIT[30];
                        }

                        else if (i==65 || i==66) //Carte "Tous les outils sont cassés"
                        {
                            //Initialisation des identificateurs de carte
                            deck[i]->typeAction->effet = 'p';
                            deck[i]->typeAction->action = 'k';
                            deck[i]->typeAction->visuelAvant = tabBIT[31];
                        }

                        else if (i==67 || i==68) //Carte "Réchauffement climatique"
                        {
                            //Initialisation des identificateurs de carte
                            deck[i]->typeAction->effet = 'p';
                            deck[i]->typeAction->action = 'l';
                            deck[i]->typeAction->visuelAvant = tabBIT[29];
                        }
                    }
                }
            }
        }
    }
}


/*
Sous-programme de libération du deck en cas de problème mémoire lors de la génération de ce dernier
*/
void liberationDeckInitial(t_carte *deck[69], int position)
{
    while(position >= 0) //Boucle de parcours inversé du tableau contenant le deck initial
    {
        //Libération des éléments précédemment créés
        if(deck[position]->typeAT == 't') //Cas d'une carte Terrain
        {
            free(deck[position]->typeTerrain);
            free(deck[position]);
        }
        else //Cas d'une carte Action
        {
            free(deck[position]->typeAction);
            free(deck[position]);
        }

        position--; //Diminution de la position pour continuer le parcours
    }
}


/*
Sous-programme de mélange du deck initial (mélange de Fisher-Yates)
*/
void melangeDeckInitial(t_carte *deck[69])
{
    int i; //Permet le bon fonctionnement de la boucle de mélange
    int j; //Permet l'éhcnage entre deux cases du tableau
    t_carte *stockage; //Permet l'échange de deux cases du tableau contenant le deck initial

    for(i=68 ; i>=1 ; i--) //Boucle de mélange
    {
        j = rand()%(i+1); //Récupération d'une case aléatoire

        //Echange des deux cases à mélanger
        stockage = deck[j];
        deck[j] = deck[i];
        deck[i] = stockage;
    }
}


/*
Sous-programme permettant l'initialisation de la pioche (pile)
*/
void initPioche(t_pioche *pioche, int *erreur)
{
    int i; //Permet le bon fonctionnement de la boucle d'allocation des éléments dynamiques de la pile

    pioche->indiceNbPlaces = 69; //Initialisation du nombre de places dans la pile
    pioche->indiceSommet = 0; //Initialisation de l'indice du sommet de la pile

    for(i=0 ; i<69 ; i++) //Boucle d'allocation des éléments dynamiques de la pile
    {
        if(*erreur == 0) //Processus se réalisant seulement lorsqu'aucune erreur d'allocation n'a eu lieu
        {
            pioche->deck[i] = (t_carte*)malloc(sizeof(t_carte)); //Allocation dynamique
            if(pioche->deck[i] == NULL) //Cas où l'allocation dynamique n'a pas pu se faire
            {
                *erreur = 1; //Incrémentation de l'indicateur d'erreur

                liberationPile(pioche, i-1, 0); //Appel du sous-programme de libération de la pioche
            }
        }
    }
}


/*
Sous-programme de libération de la pioche (pile)
*/
void liberationPile(t_pioche *pioche, int etatActuel, int cas)
{
    int i; //Permet le bon fonctionnement de la boucle de libération de la pile
    int termine = 0;


    for(i=0 ; i<etatActuel ; i++)
    {
        if(termine == 0)
        {
            if(pioche->deck[i] != NULL)
            {
                if(cas == 1)
                {
                    if(pioche->deck[i]->typeAction == NULL) //Cas où il s'agit d'une carte Terrain
                    {

                        free(pioche->deck[i]->typeTerrain); //Libération de la mémoire dédiée aux informations de la carte Terrain
                    }
                    else if(pioche->deck[i]->typeTerrain == NULL) //Cas où il s'agit d'une carte Action
                    {
                        free(pioche->deck[i]->typeAction); //Libération de la mémoire didée aux informations de la carte Action
                    }
                }
                free(pioche->deck[i]); //Libération de la mémoire dédiée à la structure de la carte
            }
            else
            {
                termine = 1;
            }
        }
    }
}


/*
Sous-programme d'empilement de la pioche (pile)
*/
void empilerPioche(t_pioche *pileCarte, t_carte *nouvo, int *erreur)
{
    if (estPleine(pileCarte) == 1) //Cas où la pioche est pleine et qu'on ne peut pas empiler plus
    {
        printf ("Impossible d'empiler, la pioche est pleine.\n");
    }

    else //Cas où la pile n'est pas pleine et qu'on peut empiler
    {

        if(nouvo->typeAT == 't') //Cas d'une carte Terrain
        {
            pileCarte->deck[pileCarte->indiceSommet]->typeAction = NULL;
            pileCarte->deck[pileCarte->indiceSommet]->typeTerrain = (t_carteTerrain*)malloc(sizeof(t_carteTerrain)); //Allocation mémoire de la place pour une carte Terrain
            if(pileCarte->deck[pileCarte->indiceSommet]->typeTerrain == NULL) //Cas où l'allocation a échoué
            {
                printf ("\n\nPROBLEME d'allocation memoire\n\n");
                *erreur = 1; //Incrémentation de l'indicateur d'erreur

                liberationPile(pileCarte, 68, 1);
            }

            else //Cas où l'allocation a pu se faire
            {
                //Récupération de l'ensemble des informations de la carte Terrain
                pileCarte->deck[pileCarte->indiceSommet]->etat = 'm';
                pileCarte->deck[pileCarte->indiceSommet]->typeAT = nouvo->typeAT; //Type de la carte ('t')
                strcpy(pileCarte->deck[pileCarte->indiceSommet]->typeTerrain->typeJonction, nouvo->typeTerrain->typeJonction); //Jonction
                pileCarte->deck[pileCarte->indiceSommet]->typeTerrain->haut = nouvo->typeTerrain->haut; //Haut
                pileCarte->deck[pileCarte->indiceSommet]->typeTerrain->bas = nouvo->typeTerrain->bas; //Bas
                pileCarte->deck[pileCarte->indiceSommet]->typeTerrain->gauche = nouvo->typeTerrain->gauche; //Gauche
                pileCarte->deck[pileCarte->indiceSommet]->typeTerrain->droite = nouvo->typeTerrain->droite; //Droite

                //Récupération des visuels de la carte
                pileCarte->deck[pileCarte->indiceSommet]->typeTerrain->visuelAvant = nouvo->typeTerrain->visuelAvant;
                pileCarte->deck[pileCarte->indiceSommet]->typeTerrain->visuelArriere = nouvo->typeTerrain->visuelArriere;
            }
        }

        else //Cas d'une carte Action
        {
            pileCarte->deck[pileCarte->indiceSommet]->typeTerrain = NULL;
            pileCarte->deck[pileCarte->indiceSommet]->typeAction = (t_carteAction*)malloc(sizeof(t_carteAction)); //Allocation mémoire de la place pour une carte Action
            if(pileCarte->deck[pileCarte->indiceSommet]->typeAction == NULL) //Cas où l'allocation a échoué
            {
                printf ("\n\nPROBLEME d'allocation memoire\n\n");
                *erreur = 1; //Incrémentation de l'indicateur d'erreur

                //Libération de la pile
            }
            else //Cas où l'allocation a pu se faire
            {
                //Récupération de l'ensemble des informations de la carte Action
                pileCarte->deck[pileCarte->indiceSommet]->etat = 'm';
                pileCarte->deck[pileCarte->indiceSommet]->typeAT = nouvo->typeAT; //Type de la carte ('a')
                pileCarte->deck[pileCarte->indiceSommet]->typeAction->action = nouvo->typeAction->action; //Action
                pileCarte->deck[pileCarte->indiceSommet]->typeAction->effet = nouvo->typeAction->effet; //Effet

                //Récupération des visuels de la carte
                pileCarte->deck[pileCarte->indiceSommet]->typeAction->visuelAvant = nouvo->typeAction->visuelAvant;
                pileCarte->deck[pileCarte->indiceSommet]->typeAction->visuelArriere = nouvo->typeAction->visuelArriere;
            }
        }

        (pileCarte->indiceSommet)++; //Incrémentation de l'indice de sommet de la pile
    }
}


/*
Sous-programme permettant de savoir si la pioche (pile) est pleine
*/
int estPleine(t_pioche *PileCarte)
{
    if (PileCarte->indiceNbPlaces==PileCarte->indiceSommet) //Cas où la pioche est pleine
    {
        return 1;
    }
    else //Cas où la pioche n'est pas pleine
    {
        return 0;
    }
}


    ////////////////////////////////////
    // SOUS-PROGRAMMES DE MAINTENANCE //
    ////////////////////////////////////

/*
Sous-programme permettant l'affichage du tableau contenant le deck initial
*/
void affichageDeckInitial(t_carte *deck[69])
{
    int i; //Permet le bon fonctionnement de la boucle de parcours du deck

    for(i=0 ; i<69 ; i++) //Boucle de parcours du tableau
    {
        if(deck[i]->typeAT == 't') //Cas d'une carte Terrain
        {
            printf("Type de carte : %c | Haut : %d | Bas : %d | Gauche : %d | Droite : %d\n", deck[i]->typeAT, deck[i]->typeTerrain->haut, deck[i]->typeTerrain->bas, deck[i]->typeTerrain->gauche, deck[i]->typeTerrain->droite); //Affichage des informations de la carte de type Terrain
        }
        else //Cas d'une carte Action
        {
            printf("Type de carte : %c | Type d'action : %c | Effet : %c\n", deck[i]->typeAT, deck[i]->typeAction->action, deck[i]->typeAction->effet); //Affichage de la carte de type Action
        }
    }
}


/*
Sous-programme d'affichage de la pioche (pile)
*/
void affichagePioche(t_pioche *pioche, int *erreur)
{
    t_carte *carteRecup;

    carteRecup = (t_carte*)malloc(sizeof(t_carte));
    if(carteRecup == NULL)
    {
        printf ("\n\nPROBLEME d'allocation memoire\n\n");
        *erreur = 1;
    }
    else
    {
        printf("\n\n");
        while(estVide(pioche) == 0)
        {
            carteRecup = pioche->deck[(pioche->indiceSommet) - 1];

            if(pioche->deck[(pioche->indiceSommet) - 1]->typeAT == 't') //Cas d'une carte Terrain
            {
                printf("Type de carte : %c | Haut : %d | Bas : %d | Gauche : %d | Droite : %d\n", pioche->deck[(pioche->indiceSommet) - 1]->typeAT, pioche->deck[(pioche->indiceSommet) - 1]->typeTerrain->haut, pioche->deck[(pioche->indiceSommet) - 1]->typeTerrain->bas, pioche->deck[(pioche->indiceSommet) - 1]->typeTerrain->gauche, pioche->deck[(pioche->indiceSommet) - 1]->typeTerrain->droite); //Affichage des informations de la carte de type Terrain
                printf("Type de carte : %c | Haut : %d | Bas : %d | Gauche : %d | Droite : %d\n", carteRecup->typeAT, carteRecup->typeTerrain->haut, carteRecup->typeTerrain->bas, carteRecup->typeTerrain->gauche, carteRecup->typeTerrain->droite); //Affichage des informations de la carte de type Terrain
                printf("\n");
            }
            else //Cas d'une carte Action
            {
                printf("Type de carte : %c | Type d'action : %c | Effet : %c\n", pioche->deck[(pioche->indiceSommet) - 1]->typeAT, pioche->deck[(pioche->indiceSommet) - 1]->typeAction->action, pioche->deck[(pioche->indiceSommet) - 1]->typeAction->effet); //Affichage de la carte de type Action
                printf("Type de carte : %c | Type d'action : %c | Effet : %c\n", carteRecup->typeAT, carteRecup->typeAction->action, carteRecup->typeAction->effet); //Affichage de la carte de type Action
                printf("\n");
            }

            (pioche->indiceSommet)--;
        }
    }
}
