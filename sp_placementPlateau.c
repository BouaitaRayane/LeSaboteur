#include "header.h"

/*
Sous-programme permettant de r�cup�rer les coordonn�es de la case vis�e
*/
void recupCasePlateau(int posActuX, int posActuY, int *caseX, int *caseY)
{
    int i; //Permet le bon fonctionnement des boucles de parcours vertical et horizontal
    int divEucX, divEucY; //Permet la division euclidienne des X et des Y
    int xTrouve = 0; //Indicateur que la case en X a �t� trouv�e
    int yTrouve = 0; //Indicateur que la case en Y a �t� trouv�e

    posActuX -= 445;
    posActuY -= 174;

    //Recherche de la case en X
    for(i=0 ; i<7 ; i++)
    {
        if(xTrouve == 0) //Tant que la case en X n'a pas �t� trouv�e
        {
            divEucX = posActuX - (i+1)*85; //Division euclidienne
            if(divEucX < 0) //Case trouv�e
            {
                xTrouve = 1; //Indication que la case en X a �t� trouv�e
                *caseX = i; //R�cup�ration de la valeur de la case en X
            }
        }
    }

    //Recherche de la case en Y
    for(i=0 ; i<5 ; i++)
    {
        if(yTrouve == 0) //Tant que la case en Y n'a pas �t� trouv�e
        {
            divEucY = posActuY - (i+1)*115; //Division euclidienne
            if(divEucY < 0) //Case trouv�e
            {
                yTrouve = 1; //Indication que la case en Y a �t� trouv�e
                *caseY = i; //R�cup�ration de la valeur de la case en Y
            }
        }
    }
}


/*
Sous-programme permettant l'illumination des cases concern�es par le r�chauffement climatique
*/
void illuminationRechauffementClimatique(int *dirRechauffement, int caseX, int caseY, BITMAP *doubleBuffer, FONT *policeTexte)
{
    //Affichage de l'indication de sens du r�chauffement climatique
    textprintf_centre_ex(doubleBuffer, policeTexte, 1312, 400, makecol(255, 255, 255), -1, "Appuyez sur ");
    textprintf_centre_ex(doubleBuffer, policeTexte, 1312, 425, makecol(255, 255, 255), -1, "les fleches directionnelles");

    if(key[KEY_LEFT]) //Fl�che gauche
    {
        *dirRechauffement = 0; //Le r�chauffement climatique vise � gauche
    }
    else if(key[KEY_UP]) //Fl�che haute
    {
        *dirRechauffement = 1; //Le r�chauffement climatique vise en haut
    }
    else if(key[KEY_RIGHT]) //Fl�che droite
    {
        *dirRechauffement = 2; //Le r�chauffement climatique vise � droite
    }
    else if(key[KEY_DOWN]) //Fl�che basse
    {
        *dirRechauffement = 3; //Le r�chauffement climatique vise en bas
    }

    //Illumination en rouge de la case vis�e par le r�chauffement climatique
    if(*dirRechauffement == 0 && caseX > 0) //Le r�chauffement climatique vise � gauche
    {
        rectfill(doubleBuffer, 444 + (caseX-1)*85, 174 + caseY*115, 444 + caseX*85, 174 + (caseY+1)*115, makecol(255, 0, 0)); //Rectangle rouge
    }

    else if(*dirRechauffement == 1 && caseY > 0) //Le r�chauffement climatique vise en haut
    {
        rectfill(doubleBuffer, 444 + caseX*85, 174 + (caseY-1)*115, 444 + 85 + caseX*85, 174 + caseY*115, makecol(255, 0, 0)); //Rectangle rouge
    }

    else if(*dirRechauffement == 2 && caseX < 6) //Le r�chauffement climatique vise � droite
    {
        rectfill(doubleBuffer, 444 + caseX*85, 174 + caseY*115, 444 + 85 + (caseX+1)*85, 174 + (caseY+1)*115, makecol(255, 0, 0)); //Rectangle rouge
    }

    else if(*dirRechauffement == 3 && caseY < 4) //Le r�chauffement climatique vise en bas
    {
        rectfill(doubleBuffer, 444 + caseX*85, 174 + (caseY+1)*115, 444 + 85 + caseX*85, 174 + (caseY+2)*115, makecol(255, 0, 0)); //Rectangle rouge
    }

    //Illumination de la case o� se placerait techniquement le r�chauffement climatique
    rectfill(doubleBuffer, 444 + caseX*85, 174 + caseY*115, 444 + (caseX+1)*85, 174 + (caseY+1)*115, makecol(255, 255, 0));
}


/*
Sous-programme de placement ou non de la carte s�lectionn�e sur le plateau de jeu
*/
int placementCarte(t_carteTerrain *carteAPoser, t_carteTerrain *plateau[7][5], int caseX, int caseY, int reverseCarte, BITMAP *tabBIT[BIT])
{
    int plateauVide = 1; //Permet de d�terminer si le plateau est vide ou non
    int i, j; //Permet le bon fonctionnement des boucles de parcours des deux dimensions du plateau de jeu
    int carteMalPlacee = 1; //Permet de d�terminer si la carte est finalement bien plac�e ou non
    int erreur = 0; //Permet de d�terminer si une jonction est mauvaise
    int carteSeule = 0; //Permet de d�terminer si la carte n'est pas plac�e sans carte autour
    int lien = 0; //Permet de d�terminer que la carte est bien connect� � une autre par une jonction

    //D�termination si le plateau est vide ou non
    for(i=0 ; i<7 ; i++) //Boucle de parcours horizontal
    {
        for(j=0 ; j<5 ; j++) //Boucle de parcours vertical
        {
            if(plateau[i][j] != NULL) //Une carte existe
            {
                plateauVide = 0; //Indication que le plateau n'est pas vide
            }
        }
    }

    if(plateauVide == 1) //Le plateau est vide
    {
        if(caseX == 0 && caseY == 2) //La case s�lectionn�e est bien celle de d�part
        {
            if(carteAPoser->gauche == 1 && reverseCarte == 0) //La carte a une jonction avec la gauche
            {
                plateau[0][2] = carteAPoser; //Ajout de la carte sur le plateau
                carteMalPlacee = 0; //Indication que la carte est bien plac�e
            }
            else if(reverseCarte == 1) //Le reverse est activ�
            {
                if(strcmp(carteAPoser->typeJonction, "Td") == 0) //Il s'agit de la carte T Droite
                {
                    plateau[0][2] = carteAPoser; //Ajout de la carte sur le plateau
                    plateau[0][2]->visuelAvant = tabBIT[70]; //Prise de la BITMAP reverse du T Droite
                    plateau[0][2]->droite = 0; //Actualisation de la jonction droite
                    plateau[0][2]->gauche = 1; //Actualisation de la jonction gauche
                    carteMalPlacee = 0; //Indication que la carte est bien plac�e
                }
                else if(strcmp(carteAPoser->typeJonction, "Th") == 0) //Il s'agit de la carte T Haut
                {
                    plateau[0][2] = carteAPoser; //Ajout de la carte sur le plateau
                    plateau[0][2]->visuelAvant = tabBIT[69]; //Prise de la BITMAP reverse du T Haut
                    plateau[0][2]->haut = 0; //Actualisation de la jonction haute
                    plateau[0][2]->bas = 1; //Actualisation de la jonction basse
                    carteMalPlacee = 0; //Indication que la carte est bien plac�e
                }
            }
        }
    }

    //M�me logique pour la suite

    else if(plateauVide == 0)
    {
        if(plateau[caseX][caseY] == NULL)
        {
            if(reverseCarte == 1)
            {
                if(strcmp(carteAPoser->typeJonction, "Td") == 0)
                {
                    if(caseX > 0)
                    {
                        if(plateau[caseX-1][caseY] != NULL && plateau[caseX-1][caseY]->droite == 0)
                        {
                            erreur = 1;
                        }
                        else if(plateau[caseX-1][caseY] != NULL && plateau[caseX-1][caseY]->droite == 1)
                        {
                            lien = 1;
                        }
                    }

                    if(caseY > 0)
                    {
                        if(plateau[caseX][caseY-1] != NULL && plateau[caseX][caseY-1]->bas == 0)
                        {
                            erreur = 1;
                        }
                        else if(plateau[caseX][caseY-1] != NULL && plateau[caseX][caseY-1]->bas == 1)
                        {
                            lien = 1;
                        }
                    }

                    if(caseY < 4)
                    {
                        if(plateau[caseX][caseY+1] != NULL && plateau[caseX][caseY+1]->haut == 0)
                        {
                            erreur = 1;
                        }
                        else if(plateau[caseX][caseY+1] != NULL && plateau[caseX][caseY+1]->haut == 1)
                        {
                            lien = 1;
                        }
                    }

                    if(plateau[caseX][caseY-1] == NULL && plateau[caseX][caseY+1] == NULL && plateau[caseX-1][caseY] == NULL)
                    {
                        carteSeule = 1;
                    }

                    if(erreur == 0 && carteSeule == 0 && lien == 1)
                    {
                        plateau[caseX][caseY] = carteAPoser;
                        plateau[caseX][caseY]->visuelAvant = tabBIT[70];
                        plateau[caseX][caseY]->droite = 0;
                        plateau[caseX][caseY]->gauche = 1;
                        carteMalPlacee = 0;
                    }
                }

                else if(strcmp(carteAPoser->typeJonction, "Th") == 0)
                {
                    if(caseX > 0)
                    {
                        if(plateau[caseX-1][caseY] != NULL && plateau[caseX-1][caseY]->droite == 0)
                        {
                            erreur = 1;
                        }
                        else if(plateau[caseX-1][caseY] != NULL && plateau[caseX-1][caseY]->droite == 1)
                        {
                            lien = 1;
                        }
                    }

                    if(caseX < 6)
                    {
                        if(plateau[caseX+1][caseY] != NULL && plateau[caseX+1][caseY]->gauche == 0)
                        {
                            erreur = 1;
                        }
                        else if(plateau[caseX+1][caseY] != NULL && plateau[caseX+1][caseY]->gauche == 1)
                        {
                            lien = 1;
                        }
                    }

                    if(caseY < 4)
                    {
                        if(plateau[caseX][caseY+1] != NULL && plateau[caseX][caseY+1]->haut == 0)
                        {
                            erreur = 1;
                        }
                        else if(plateau[caseX][caseY+1] != NULL && plateau[caseX][caseY+1]->haut == 1)
                        {
                            lien = 1;
                        }
                    }

                    if(plateau[caseX][caseY+1] == NULL && plateau[caseX-1][caseY] == NULL && plateau[caseX+1][caseY] == NULL)
                    {
                        carteSeule = 1;
                    }

                    if(erreur == 0 && carteSeule == 0 && lien == 1)
                    {
                        plateau[caseX][caseY] = carteAPoser;
                        plateau[caseX][caseY]->visuelAvant = tabBIT[69];
                        plateau[caseX][caseY]->haut = 0;
                        plateau[caseX][caseY]->bas = 1;
                        carteMalPlacee = 0;
                    }
                }
            }

            else if(reverseCarte == 0)
            {
                if(carteAPoser->haut == 1)
                {
                    if(caseY > 0)
                    {
                        if(plateau[caseX][caseY-1] != NULL && plateau[caseX][caseY-1]->bas == 0)
                        {
                            erreur = 1;
                        }
                        else if(plateau[caseX][caseY-1] != NULL && plateau[caseX][caseY-1]->bas == 1)
                        {
                            lien = 1;
                        }
                    }
                }
                else if(carteAPoser->haut == 0)
                {
                    if(caseY > 0)
                    {
                        if(plateau[caseX][caseY-1] != NULL && plateau[caseX][caseY-1]->bas == 1)
                        {
                            erreur = 1;
                        }
                    }
                }

                if(carteAPoser->bas == 1)
                {
                    if(caseY < 4)
                    {
                        if(plateau[caseX][caseY+1] != NULL && plateau[caseX][caseY+1]->haut == 0)
                        {
                            erreur = 1;
                        }
                        else if(plateau[caseX][caseY+1] != NULL && plateau[caseX][caseY+1]->haut == 1)
                        {
                            lien = 1;
                        }
                    }
                }
                else if(carteAPoser->bas == 0)
                {
                    if(caseY < 4)
                    {
                        if(plateau[caseX][caseY+1] != NULL && plateau[caseX][caseY+1]->haut == 1)
                        {
                            erreur = 1;
                        }
                    }
                }

                if(carteAPoser->gauche == 1)
                {
                    if(caseX > 0)
                    {
                        if(plateau[caseX-1][caseY] != NULL && plateau[caseX-1][caseY]->droite == 0)
                        {
                            erreur = 1;
                        }
                        else if(plateau[caseX-1][caseY] != NULL && plateau[caseX-1][caseY]->droite == 1)
                        {
                            lien = 1;
                        }
                    }
                }
                else if(carteAPoser->gauche == 0)
                {
                    if(caseX > 0)
                    {
                        if(plateau[caseX-1][caseY] != NULL && plateau[caseX-1][caseY]->droite == 1)
                        {
                            erreur = 1;
                        }
                    }
                }

                if(carteAPoser->droite == 1)
                {
                    if(caseX < 6)
                    {
                        if(plateau[caseX+1][caseY] != NULL && plateau[caseX+1][caseY]->gauche == 0)
                        {
                            erreur = 1;
                        }
                        else if(plateau[caseX+1][caseY] != NULL && plateau[caseX+1][caseY]->gauche == 1)
                        {
                            lien = 1;
                        }
                    }
                }
                else if(carteAPoser->droite == 0)
                {
                    if(caseX < 6)
                    {
                        if(plateau[caseX+1][caseY] != NULL && plateau[caseX+1][caseY]->gauche == 1)
                        {
                            erreur = 1;
                        }
                    }
                }

                if(plateau[caseX][caseY-1] == NULL && plateau[caseX][caseY+1] == NULL && plateau[caseX-1][caseY] == NULL && plateau[caseX+1][caseY] == NULL)
                {
                    carteSeule = 1;
                }

                if(erreur == 0 && carteSeule == 0 && lien == 1)
                {
                    plateau[caseX][caseY] = carteAPoser;
                    carteMalPlacee = 0;
                }
            }
        }
    }

    return carteMalPlacee;
}


/*
Sous-programme testant l'atteinte des poissons (lancement de la fin de partie)
*/
int testFinPartie(t_carteTerrain *cartePosee, int caseX, int caseY, int reverseCarte)
{
    if((caseX == 6 && caseY == 0) || (caseX == 6 && caseY == 2) || (caseX == 6 && caseY == 4)) //Si la case consid�r�e est bien l'une de celles � gauche des cases Arriv�e
    {
        if(reverseCarte == 1) //Le reverse est activ�
        {
            if(strcmp(cartePosee->typeJonction, "Td") == 0) //La carte pos�e est un T Droite reverse, donc pas possible
            {
                return 0; //Indication que ce n'est pas la fin de la partie
            }
        }
        else //Le reverse n'est pas activ�
        {
            if(strcmp(cartePosee->typeJonction, "Td") == 0 || strcmp(cartePosee->typeJonction, "Th") == 0 || strcmp(cartePosee->typeJonction, "TD") == 0 || strcmp(cartePosee->typeJonction, "Ca") == 0) //La carte pos�e poss�de une jonction droite
            {
                return 1; //Indication que c'est la fin de la partie
            }
        }
    }

    return 0;
}


/*
Sous-programme de v�rification si le placement du r�chauffement climatique est valide
*/
int testRechauffementClimatique(int caseX, int caseY, int dirRechauffement, t_carteTerrain *plateau[7][5])
{
    int carteMalPlacee = 0; //Permet d'indiquer si la carte est bien plac�e ou non
    int erreur = 0; //Permet d'indiquer si le placement n'est pas possible

    if(dirRechauffement == 0) //Le r�chauffement climatique vise la gauche
    {
        if(caseX > 0 && plateau[caseX-1][caseY] == NULL) //Le r�chauffement climatique n'est pas en bordure gauche mais la case � sa gauche n'existe pas
        {
            erreur = 1; //Indication que la situation n'est pas ad�quate
        }
        else if(caseX == 0) //Le r�chauffement climatique est en bordure gauche
        {
            erreur = 1; //Indication que la situation n'est pas ad�quate
        }
    }

    //M�me raisonnement pour les autres directions du r�chauffement climatique

    else if(dirRechauffement == 1)
    {
        if(caseY > 0 && plateau[caseX][caseY-1] == NULL)
        {
            erreur = 1;
        }
        else if(caseY == 0)
        {
            erreur = 1;
        }
    }

    else if(dirRechauffement == 2)
    {
        if(caseX < 6 && plateau[caseX+1][caseY] == NULL)
        {
            erreur = 1;
        }
        else if(caseX == 6)
        {
            erreur = 1;
        }
    }

    else if(dirRechauffement == 3)
    {
        if(caseY < 6 && plateau[caseX][caseY+1] == NULL)
        {
            erreur = 1;
        }
        else if(caseY == 4)
        {
            erreur = 1;
        }
    }

    if(erreur == 1) //Une erreur a �t� d�tect�e
    {
        carteMalPlacee = 1; //Indication que la carte est mal plac�e
    }

    return carteMalPlacee;
}


/*
Sous-programme d'application du r�chauffement climatique
*/
void applicationRechauffementClimatique(int caseX, int caseY, int dirRechauffement, t_carteTerrain *plateau[7][5], t_defausse *defausse)
{
    if(dirRechauffement == 0) //Le r�chauffement climatique vise � gauche
    {
        defausse->imageDefausse = plateau[caseX-1][caseY]->visuelAvant; //La carte vis� passe dans la d�fausse
        plateau[caseX-1][caseY] = NULL; //Suppression de la carte vis�e du plateau
    }

    //M�me m�thode pour les autres directions possibles du r�chauffement climatique

    else if(dirRechauffement == 1)
    {
        defausse->imageDefausse = plateau[caseX][caseY-1]->visuelAvant;
        defausse->etat = 1;
        plateau[caseX][caseY-1] = NULL;
    }

    else if(dirRechauffement == 2)
    {
        defausse->imageDefausse = plateau[caseX+1][caseY]->visuelAvant;
        defausse->etat = 1;
        plateau[caseX+1][caseY] = NULL;
    }

    else if(dirRechauffement == 3)
    {
        defausse->imageDefausse = plateau[caseX][caseY+1]->visuelAvant;
        defausse->etat = 1;
        plateau[caseX][caseY+1] = NULL;
    }
}


/*
Sous-programme de v�rification du placement de la carte map
*/
int testMap(int mouse_x, int mouse_y, BITMAP *tabCasesArrive[2][3])
{
    int carteARetourner = -1; //Permet de retourner la carte � r�v�ler, si il y en a une

    if(mouse_y>=174 && mouse_y<=289) //La case s�lectionn�e est la premi�re
    {
        carteARetourner = 0; //Indication que la carte s�lectionn�e est la premi�re
    }

    //M�me logique pour les deux autres cases possibles

    else if(mouse_y>=404 && mouse_y<=519)
    {
        carteARetourner = 1;
    }
    else if(mouse_y >=634 && mouse_y<=749)
    {
        carteARetourner = 2;
    }

    return carteARetourner;
}
