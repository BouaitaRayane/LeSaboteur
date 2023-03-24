#include "header.h"

/*
Sous-programme de gestion d'une partie normale
*/
void partieNormale(t_joueur **tabJoueurs, int nbreJoueurs, t_pioche *pioche, BITMAP *tabBIT[BIT], FONT *tabPolices[POLICES])
{
    int i; //Permet le bon fonctionnement de la boucle de parcours du tableau de joueurs
    int manche = 0; //Permet le comptage des manches
    int phase; //Permet de d�terminer la phase d'un tour (choix de l'action, action, fin du tour)
    int partieNormale = 1; //Permet de d�terminer si la partie est dans son �tat normal
    int finTour = 0; //Permet de d�terminer la fin du tour d'un joueur afin de passer au suivant
    int devoilementCaseArrivee = -1; //Permet de mettre en place la fonctionnalit� de r�v�lation d'une des cases Arriv�e par la carte MAP
    int finPartie = 0; //Permet de d�terminer la fin de la partie
    int ENDgame = 0; //Permet de d�terminer lorsqu'il n'y a plus de carte dans la pioche et qu'on se rapproche du game over
    int passerSonTour = 0; //Permet de compter le nombre de passage de tour et d'ainsi mettre fin � la partie
    int numSave = 0; //Permet de d�terminer le num�ro de la save dans laquelle sauvegarder les informations
    int erreur = 0; //Permet de d�terminer s'il y a une erreur

    t_defausse defausse; //Contient la d�fausse
    BITMAP *tabCasesArrivee[2][3]; //Tableau contenant les cases Arriv�e ainsi que le dos de ces "cartes"
    t_carteTerrain *plateau[7][5]; //Tableau repr�sentant le plateau de jeu

    initCasesArrivee(tabCasesArrivee, tabBIT); //Initialisation du tableau contenant les cases Arriv�e
    initPlateau(plateau); //Initialisation du plateau de jeu
    defausse.etat = 0; //Initialisation de l'�tat de la d�fausse pour indiquer qu'elle est vide

    //pioche->indiceSommet = 5; //Permet l'atteinte plus rapide du END game / Debug fonction

    while(partieNormale) //Boucle de d�roulement tant que la partie est en cours
    {
        manche++; //Augmentation de l'indice de manche une fois que tous les joueurs ont jou� leur tour

        for(i=0 ; i<nbreJoueurs ; i++) //Boucle de parcours du tableau de joueurs afin qu'ils jouent tous
        {
            if(partieNormale) //Si la partie a encore lieu
            {
                ecranIntermediaire(tabBIT[0], tabBIT[64], tabJoueurs[i]->pingouinIdentite, tabJoueurs[i]->pseudo, tabPolices[3], tabPolices[5], tabPolices[6]); //Ecran intermediaire tant que ENTER n'est pas press�

                //Initialisation des variables � risque
                finTour = 0; //Tour en cours
                phase = 1; //Passage par leur choix de l'action obligatoire
                devoilementCaseArrivee = -1; //Permet de ne pas d�voiler les cases Arriv�e

                initPosCartes(tabJoueurs[i]->mainJoueur); //Initialisation de la position des cartes au d�but du tour

                if(pioche->indiceSommet == 0) //Il n'y a plus de cartes dans la pioche
                {
                    ENDgame = 1; //Indication que nous sommes dans le END game
                }

                while(!finTour) //Boucle de d�roulement du tour du joueur
                {
                    switch(phase) //Gestion de la phase du tour dans laquelle le joueur se trouve
                    {
                        case 1 : //Phase de choix de l'action � effectuer
                            phase = choixAction(tabJoueurs, nbreJoueurs, i, manche, pioche, tabBIT[0], tabBIT[65], tabBIT[66], &defausse, tabBIT[27], tabPolices, plateau, tabCasesArrivee, ENDgame, &passerSonTour, &finPartie); //Choix de l'action � effectuer
                            break;

                        case 2 : //Phase de "jouage" d'une carte
                            phase = jouerCarte(tabJoueurs, nbreJoueurs, i, pioche, tabBIT[0], tabBIT[65], tabBIT[66], &defausse, tabBIT[27], tabBIT, tabPolices, plateau, tabCasesArrivee, &devoilementCaseArrivee, &finPartie, &ENDgame); //Processus de "jouage" d'une carte
                            if(ENDgame == 1 && phase != 1) //Nous sommes en END game et le joueur a fait une action ("jouer une carte")
                            {
                                passerSonTour = 0; //R�initialisation du compteur de passage de tour
                            }
                            break;

                        case 3 : //Phase de d�fausse des cartes
                            phase = defausseTroisCartes(tabJoueurs, nbreJoueurs, i, pioche, tabBIT[0], tabBIT[65], tabBIT[66], &defausse, tabBIT[27], tabPolices, plateau, tabCasesArrivee, &ENDgame); //Processus de d�fausse des cartes
                            break;

                        case 4 : //Phase de sauvegarde de la partie
                            numSave = recupNumFichierSave(); //R�cup�ration du num�ro de sauvegarde disponible
                            erreur = processusSave(numSave, tabJoueurs, nbreJoueurs, i, manche, ENDgame, plateau, tabCasesArrivee, defausse, pioche, tabBIT); //Sauvegarde des informations

                            if(erreur == 0) //Il n'y a pas eu d'erreur lors de la sauvegarde
                            {
                                ecranSave(tabJoueurs, nbreJoueurs, i, pioche, tabBIT[0], tabBIT[65], tabBIT[66], &defausse, tabBIT[27], tabPolices, plateau, tabCasesArrivee, numSave); //Ecran interm�diaire montrant le fichier dans lequel a �t� sauvegard� la partie
                            }
                            else //La sauvegarde a �chou�
                            {
                                allegro_message("ERREUR\nLa sauvegarde n'a pas pu etre effectuee.");
                            }

                            phase = 1; //Retour � la phase de choix de l'action
                            break;

                        case 5 : //Phase de "quittage" de la partie en cours et de retour au menu principal
                            finTour = 1; //Indication que le tour est termin�
                            partieNormale = 0; //Indication que la partie est termin�e
                            break;

                        case 6 : //Phase de demande de passage au joueur suivant
                            initPosCartes(tabJoueurs[i]->mainJoueur); //Initialisation de la position des cartes au d�but du tour
                            demandeSuivant(tabJoueurs, nbreJoueurs, i, pioche, tabBIT[0], tabBIT[65], tabBIT[66], &defausse, tabBIT[27], tabPolices, plateau, tabCasesArrivee, devoilementCaseArrivee, finPartie, passerSonTour); //Demande de passage au joueur suivant

                            if(passerSonTour == nbreJoueurs) //Tous les joueurs ont pass� leur tour
                            {
                                phase = 9; //Passage � l'�cran de game over
                            }
                            else if(finPartie == 1) //La partie s'est termin�e de mani�re normale, lancement d'un des deux �crans de fin principaux
                            {
                                if(tabJoueurs[i]->role == 'm') //Le gagnant est le macareux
                                {
                                    phase = 8;
                                }
                                else //Le gagnant est un pingouin
                                {
                                    phase = 7;
                                }
                            }
                            else //La partie n'est pas encore termin�e, indication que le tour est simplement termin�
                            {
                                finTour = 1;
                            }
                            break;

                        case 7 : //Ecran de fin de victoire des pingouins
                            ecranFinPingouins(tabBIT[0], tabBIT[51], tabBIT[52], tabBIT[53], tabBIT[54], tabBIT[55], tabBIT[56], tabBIT[57], tabPolices[4]); //Ecran de fin
                            finTour = 1; //Indication que le tour est termin�
                            partieNormale = 0; //Indication que la partie est termin�e
                            break;

                        case 8 : //Ecran de fin de victoire du macareux
                            ecranFinMacareux(tabBIT[0], tabBIT[58], tabBIT[59], tabBIT[60], tabBIT[61], tabBIT[62], tabBIT[63], tabPolices[4]); //Ecran de fin
                            finTour = 1; //Indication que le tour est termin�
                            partieNormale = 0; //Indication que la partie est termin�e
                            break;

                        case 9 : //Ecran de game over
                            ecranGameOver(tabBIT[0], tabBIT[48], tabBIT[49], tabBIT[50], tabPolices[3]); //Ecran de game over
                            finTour = 1; //Indication que le tour est termin�
                            partieNormale = 0; //Indication que la partie est termin�e
                            break;
                    }
                }
            }
        }
    }
}


/*
Sous-programme d'initialisation des cases arriv�e
*/
void initCasesArrivee(BITMAP *tab[2][3], BITMAP *tabBIT[BIT])
{
    int i; //Permet le bon fonctionnement du parcours du tableau
    int j; //Permet l'�change de deux lignes du tableau
    BITMAP *stockage; //Permet de stocker l'�l�ment � �changer

    for(i=0 ; i<3 ; i++) //Boucle d'intialisation du tableau
    {
        tab[0][i] = tabBIT[45+i]; //Case Poisson 1 || Poisson 2 || Poisson 3
        tab[1][i] = tabBIT[27]; //Carte de dos
    }

    for(i=2 ; i>=1 ; i--) //Boucle de m�lange du tableau
    {
        j = rand()%(i+1); //Ligne � �changer

        //Echange de la BITMAP Poisson
        stockage = tab[0][j];
        tab[0][j] = tab[0][i];
        tab[0][i] = stockage;

        //Echange de la BITMAP Carte de dos
        stockage = tab[1][j];
        tab[1][j] = tab[1][j];
        tab[1][i] = stockage;
    }
}


/*
Sous-programme d'initialisation du plateau de jeu
*/
void initPlateau(t_carteTerrain *plateau[7][5])
{
    int i, j; //Permettent le parcours horizontal et vertical du plateau de jeu

    for(i=0 ; i<7 ; i++) //Boucle de parcours horizontal
    {
        for(j=0 ; j<5 ; j++) //Boucle de parcours vertical
        {
            plateau[i][j] = NULL; //Initialisation de la case en question
        }
    }
}


/*
Sous-programme de l'�cran interm�diaire de demande du joueur
*/
void ecranIntermediaire(BITMAP *doubleBuffer, BITMAP *fondEcran, BITMAP *pingouinJoueur, char* pseudo, FONT *policePseudo, FONT *policeDemande, FONT *policeEntrer)
{
    while(!key[KEY_ENTER]) //Boucle de d�roulement tant que la touche ENTER n'est pas press�e
    {
        clear_bitmap(doubleBuffer); //R�initialisation du double buffer

        blit(fondEcran, doubleBuffer, 0, 0, 0, 0, fondEcran->w, fondEcran->h); //Affichage du plateau de jeu

        masked_stretch_blit(pingouinJoueur, doubleBuffer, 0, 0, pingouinJoueur->w, pingouinJoueur->h, 50, 200, pingouinJoueur->w*3, pingouinJoueur->h*3); //Affichage du pingouin du joueur actuel

        //Affichage de la demande de pr�sence du joueur
        textprintf_centre_ex(doubleBuffer, policePseudo, SCREEN_W/2, 400, makecol(0, 0, 0), -1, "%s", pseudo);
        textprintf_centre_ex(doubleBuffer, policeDemande, SCREEN_W/2, 500, makecol(0, 0, 0), -1, "Est-ce bien toi ?");
        textprintf_ex(doubleBuffer, policeEntrer, 1260, 710, makecol(0, 0, 0), -1, "Appuie sur ENTER");

        blit(doubleBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //Affichage du double buffer
    }
}


/*
Sous-programme d'initialisation de la position des cartes
*/
void initPosCartes(t_carte *mainJoueur[6])
{
    int i; //Permet le bon fonctionnement de la boucle d'initialisation
    int positionInitiale = 388; //Permet de d�finir la valeur d'abscisse la plus � gauche

    for(i=0 ; i<6 ; i++) //Boucle d'initialisation
    {
        if(mainJoueur[i] != NULL) //Cas o� il y a une carte
        {
            mainJoueur[i]->posX = positionInitiale+i*140; //Initialisation de la position en X avec un d�calage progressif vers la droite
            mainJoueur[i]->xInit = positionInitiale+i*140; //Initialisation de la position initiale en X de la carte
            mainJoueur[i]->posY = 28; //Initialisation de la position en Y
            mainJoueur[i]->yInit = 28; //Initialisation de la position initiale en Y de la carte
        }
    }
}


/*
Sous-programme de choix de l'action � effectuer
*/
int choixAction(t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, int manche, t_pioche *pioche, BITMAP *doubleBuffer, BITMAP *plateauJeu, BITMAP *vignetteMacareux, t_defausse *defausse, BITMAP *carteDos, FONT *tabPolices[POLICES], t_carteTerrain *plateau[7][5], BITMAP *tabCartesArrivee[2][3], int ENDgame, int *passerTour, int *finPartie)
{
    int i; //Permet l'affichage du profil des autres joueurs
    int phase; //Permet de d�terminer la phase du tour en cours en sortie du sous-programme
    int done = 0; //Permet de mettre en place la boucle de d�roulement du choix de l'action
    int decalageAutreJoueur = 0; //Permet l'affichage du profil des autres joueurs

    while(!done) //Boucle de d�roulement tant qu'aucune action n'a �t� choisie
    {
        decalageAutreJoueur = 0; //R�initialisation du d�calage vertical des infos des autres joueurs

        clear_bitmap(doubleBuffer); //R�initialisation du double buffer
        blit(plateauJeu, doubleBuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //Affichage du plateau de jeu

        //Affichage du plateau contenant les chemins form�s par les cartes Terrain
        affichagePlateau(plateau, doubleBuffer);

        //Affichage des cases Arriv�e
        affichageCartesArrivee(tabCartesArrivee, doubleBuffer);

        //Affichage de la pioche et de la d�fausse
        blit(carteDos, doubleBuffer, 0, 0, 1231, 46, carteDos->w, carteDos->h); //Carte de dos repr�sentant la pioche
        textprintf_centre_ex(doubleBuffer, tabPolices[5], 1273, 90, makecol(255, 255, 255), -1, "%d", pioche->indiceSommet); //Nombre de cartes restantes dans la pioche
        if(defausse->etat == 1) //La d�fausse existe
        {
            blit(defausse->imageDefausse, doubleBuffer, 0, 0, 1381, 46, defausse->imageDefausse->w, defausse->imageDefausse->h); //D�fausse
        }

        //Affichage de la manche actuel
        textprintf_centre_ex(doubleBuffer, tabPolices[8], 1312, 200, makecol(255, 255, 0), -1, "Manche");
        textprintf_centre_ex(doubleBuffer, tabPolices[9], 1312, 250, makecol(255, 255, 0), -1, "%d", manche);

        //Affichage du profil du joueur actuel
        affichageProfilJoueurActuel(tabJoueurs[joueurActuel], doubleBuffer, vignetteMacareux, tabPolices[6], tabPolices[5]);

        //Processus d'affichage des informations des autres joueurs dans l'ordre de passage
        i = (joueurActuel+1)%nbreJoueurs;
        while(i != joueurActuel) //Boucle permettant l'affichage des joueurs dans l'ordre de passage
        {
            affichageProfilAutresJoueurs(tabJoueurs[i], doubleBuffer, tabPolices[6], tabPolices[7], decalageAutreJoueur); //Affichage des profils
            decalageAutreJoueur++; //D�calage du profil suivant vers le bas

            i = (i+1)%nbreJoueurs; //Passage au joueur suivant
        }

        //Affichage des cartes du joueur actuel
        affichageCartesJoueurActuel(tabJoueurs[joueurActuel], doubleBuffer);

        //Demande de l'action � effectuer, r�cup�ration de la phase suivante du tour
        phase = affichageDemandeAction(doubleBuffer, tabPolices[1], tabPolices[6], ENDgame, passerTour, nbreJoueurs, finPartie);

        blit(doubleBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //Affichage du double buffer

        if(phase != 1) //Cas o� le joueur demande � effectuer une action, sortie de ce menu
        {
            done = 1; //Indication que le processus de choix de l'action est termin�
        }
    }

    return phase;
}


/*
Sous-programme d'affichage du plateau
*/
void affichagePlateau(t_carteTerrain *plateau[7][5], BITMAP *doubleBuffer)
{
    int i, j; //Permettent le bon fonctionnement des boucles de parcours des dimensions du tableau

    for(i=0 ; i<7 ; i++) //Parcours des colonnes
    {
        for(j=0 ; j<5 ; j++) //Parcours des lignes
        {
            if(plateau[i][j] != NULL) //La case contient une carte
            {
                blit(plateau[i][j]->visuelAvant, doubleBuffer, 0, 0, 444 + i*85, 174 + j*115, plateau[i][j]->visuelAvant->w, plateau[i][j]->visuelAvant->h); //Affichage de la carte
            }
        }
    }
}


/*
Sous-programme d'affichage des cartes arriv�e
*/
void affichageCartesArrivee(BITMAP *tab[2][3], BITMAP *doubleBuffer)
{
    int i; //Permet le bon fonctionnement de la boucle d'affichage

    for(i=0 ; i<3 ; i++) //Boucle d'affichage des dos de cartes des cases arriv�e
    {
        blit(tab[1][i], doubleBuffer, 0, 0, 1039, 174 + i*230, tab[1][i]->w, tab[1][i]->h); //Affichage de la carte de dos
    }
}


/*
Sous-programme d'affichage du profil du joueur actuel
*/
void affichageProfilJoueurActuel(t_joueur *joueurActuel, BITMAP *doubleBuffer, BITMAP *vignetteMacareux, FONT *policeScore, FONT *policePseudo)
{
    int i; //Permet le bon fonctionnement de la boucle d'affichage des p�nalit�s du joueur actuel

    //Affichage de la vignette de macareux ou pingouin en fonction du r�le du joueur
    if(joueurActuel->role == 'm') //Joueur macareux
    {
        stretch_blit(vignetteMacareux, doubleBuffer, 0, 0, vignetteMacareux->w, vignetteMacareux->h, 25, 25, 110, 110); //Affichage de la vignette macareux
    }
    else //Joueur pingouin
    {
        stretch_blit(joueurActuel->pingouinVignette, doubleBuffer, 0, 0, joueurActuel->pingouinVignette->w, joueurActuel->pingouinVignette->h, 25, 25, 110, 110); //Affichage de la vignette pingouin
    }

    textprintf_ex(doubleBuffer, policePseudo, 155, 38, makecol(255, 255, 255), -1, "%s", joueurActuel->pseudo); //Affichage du pseudo
    textprintf_ex(doubleBuffer, policeScore, 153, 90, makecol(255, 255, 255), -1, "Score : %d", joueurActuel->score); //Affichage du score

    for(i=0 ; i<3 ; i++) //Boucle d'affichage des p�nalit�s du joueur actuel
    {
        if(joueurActuel->penalite[i] != NULL) //La p�nalit� existe
        {
            blit(joueurActuel->penalite[i], doubleBuffer, 0, 0, 27+i*110, 207, joueurActuel->penalite[i]->w, joueurActuel->penalite[i]->h); //Affichage de la p�nalit� dans le compartiment d�di�
        }
    }
}


/*
Sous-programme d'affichage du profil des autres joueurs
*/
void affichageProfilAutresJoueurs(t_joueur *autreJoueur, BITMAP *doubleBuffer, FONT *policePseudo, FONT *policeScore, int decalageBas)
{
    int i; //Permet le bon fonctionnement de la boucle d'affichage des p�nalit�s des autres joueurs
    int positionInitiale = 378; //Permet de d�terminer la position initiale en Y du premier joueur autre

    stretch_blit(autreJoueur->pingouinVignette, doubleBuffer, 0, 0, autreJoueur->pingouinVignette->w, autreJoueur->pingouinVignette->h, 40, positionInitiale + decalageBas*134, 75, 75); //Affichage de la vignette pingouin
    textprintf_ex(doubleBuffer, policePseudo, 125, positionInitiale + decalageBas*134, makecol(255, 255, 255), -1, "%s", autreJoueur->pseudo); //Affichage du pseudo
    textprintf_ex(doubleBuffer, policeScore, 125, positionInitiale + 35 + decalageBas * 134, makecol(255, 255, 255), -1, "Score : %d", autreJoueur->score); //Affichage du score

    for(i=0 ; i<3 ; i++) //Boucle d'affichage des p�nalit�s des p�nalit�s de l'autre joueur
    {
        if(autreJoueur->penalite[i] != NULL) //La p�nalit� existe
        {
            stretch_blit(autreJoueur->penalite[i], doubleBuffer, 0, 0, autreJoueur->penalite[i]->w, autreJoueur->penalite[i]->h, 233 + i*40, 378 + 33 + decalageBas * 134, 30, 41); //Affichage de la p�nalit� en tout petit dans les compartiments d�di�s
        }
        else //La p�nalit� n'existe pas
        {
            rectfill(doubleBuffer, 233+i*40, 378+33+decalageBas*134, 233+i*40+30, 378+33+decalageBas*134+41, makecol(255, 255, 153)); //Affichage d'un rectangle jaune
        }
    }
}


/*
Sous-programme d'affichage des cartes du joueur actuel
*/
void affichageCartesJoueurActuel(t_joueur *joueurActuel, BITMAP *doubleBuffer)
{
    int j; //Permet le bon fonctionnement de la boucle de parcours de la main du joueur

    for(j=0 ; j<6 ; j++) //boucle de parcours de la min du joueur
    {
        if(joueurActuel->mainJoueur[j] != NULL) //La carte existe
        {
            if(joueurActuel->mainJoueur[j]->etat == 'm') //La carte est dans la main
            {
                if(joueurActuel->mainJoueur[j]->typeAT == 'a') //Cas d'une carte action
                {
                    blit(joueurActuel->mainJoueur[j]->typeAction->visuelAvant, doubleBuffer, 0, 0, joueurActuel->mainJoueur[j]->posX, joueurActuel->mainJoueur[j]->posY, joueurActuel->mainJoueur[j]->typeAction->visuelAvant->w,joueurActuel->mainJoueur[j]->typeAction->visuelAvant->h); //Affichage de la carte Action
                }
                else //Cas d'une carte terrain
                {
                    blit(joueurActuel->mainJoueur[j]->typeTerrain->visuelAvant, doubleBuffer, 0, 0, joueurActuel->mainJoueur[j]->posX, joueurActuel->mainJoueur[j]->posY, joueurActuel->mainJoueur[j]->typeTerrain->visuelAvant->w,joueurActuel->mainJoueur[j]->typeTerrain->visuelAvant->h); //Affichage de la carte Terrain
                }
            }
            else if(joueurActuel->mainJoueur[j]->etat == 't') //La carte est en transit pour la d�fausse
            {
                if(joueurActuel->mainJoueur[j]->typeAT == 'a') //Il s'agit d'une carte Action
                {
                    rectfill(doubleBuffer, joueurActuel->mainJoueur[j]->posX - 7, joueurActuel->mainJoueur[j]->posY - 7, joueurActuel->mainJoueur[j]->posX + joueurActuel->mainJoueur[j]->typeAction->visuelAvant->w + 7, joueurActuel->mainJoueur[j]->posY + joueurActuel->mainJoueur[j]->typeAction->visuelAvant->h + 7, makecol(0, 0, 0)); //Rectangle noir autour de la carte en cas de s�lection pour la d�fausse
                    blit(joueurActuel->mainJoueur[j]->typeAction->visuelAvant, doubleBuffer, 0, 0, joueurActuel->mainJoueur[j]->posX, joueurActuel->mainJoueur[j]->posY, joueurActuel->mainJoueur[j]->typeAction->visuelAvant->w,joueurActuel->mainJoueur[j]->typeAction->visuelAvant->h); //Affichage de la carte Action
                }
                else //Il s'agit d'une carte Terrain
                {
                    rectfill(doubleBuffer, joueurActuel->mainJoueur[j]->posX - 7, joueurActuel->mainJoueur[j]->posY - 7, joueurActuel->mainJoueur[j]->posX + joueurActuel->mainJoueur[j]->typeTerrain->visuelAvant->w + 7, joueurActuel->mainJoueur[j]->posY + joueurActuel->mainJoueur[j]->typeTerrain->visuelAvant->h + 7, makecol(0, 0, 0)); //Rectangle noir autour de la carte en cas de s�lection pour la d�fausse
                    blit(joueurActuel->mainJoueur[j]->typeTerrain->visuelAvant, doubleBuffer, 0, 0, joueurActuel->mainJoueur[j]->posX, joueurActuel->mainJoueur[j]->posY, joueurActuel->mainJoueur[j]->typeTerrain->visuelAvant->w,joueurActuel->mainJoueur[j]->typeTerrain->visuelAvant->h); //Affichage de la carte Terrain
                }
            }
        }
    }
}


/*
Sous-programme de demande d'action dans la phase de choix de l'action � effectuer
*/
int affichageDemandeAction(BITMAP *doubleBuffer, FONT *policeDemande, FONT *policeActions, int ENDgame, int *passerTour, int nbreJoueurs, int *finPartie)
{
    //Affichage de la demande
    textprintf_centre_ex(doubleBuffer, policeDemande, 1312, 350, makecol(255, 255, 255), -1, "Que voulez-vous faire ?");

    //Affichage des diff�rentes actions possibles en blanc / rouge

    //"JOUER UNE CARTE"
    if(mouse_x>=1220 && mouse_x<=1410 && mouse_y>=423 && mouse_y<=445) //Souris sur "Jouer une carte"
    {
        textprintf_centre_ex(doubleBuffer, policeActions, 1312, 425, makecol(255, 255, 0), -1, "Jouer une carte"); //Le texte s'affiche en jaune
        if(mouse_b & 1) //Clic sur "Jouer une carte"
        {
            return 2; //Passage au processus de "jouage" d'une carte
        }
    }
    else //La souris est ailleurs sur l'�cran
    {
        textprintf_centre_ex(doubleBuffer, policeActions, 1312, 425, makecol(255, 255, 255), -1, "Jouer une carte"); //Le texte s'affiche en blanc
    }

    if(ENDgame == 0) //La pioche n'est pas vide
    {
        //"DEFAUSSER 3 CARTES"
        if(mouse_x>=1200 && mouse_x<=1420 && mouse_y>=468 && mouse_y<=495) //Souris sur "Defausser 3 cartes"
        {
            textprintf_centre_ex(doubleBuffer, policeActions, 1312, 475, makecol(255, 255, 0), -1, "Defausser des cartes"); //Le texte s'affiche en jaune
            if(mouse_b & 1) //Clic sur "Defausser 3 cartes"
            {
                return 3; //Passage au processus de d�fausse des cartes
            }
        }
        else //La souris est ailleurs sur l'�cran
        {
            textprintf_centre_ex(doubleBuffer, policeActions, 1312, 475, makecol(255, 255, 255), -1, "Defausser des cartes"); //Le texte s'affiche en blanc
        }
    }
    else //La pioche est vide, on ne peut plus d�fausser de carte
    {
        //"PASSER SON TOUR"
        if(mouse_x>=1200 && mouse_x<=1420 && mouse_y>=468 && mouse_y<=495) //Souris sur "Passer son tour"
        {
            textprintf_centre_ex(doubleBuffer, policeActions, 1312, 475, makecol(255, 0, 0), -1, "Passer son tour"); //Le texte s'affiche en jaune
            if(mouse_b & 1) //Clic sur "Defausser 3 cartes"
            {
                (*passerTour)++; //Augmentation de l'indice repr�sentant le nombre de passage de tour d'affil�e
                if(*passerTour == nbreJoueurs) //Tous les joueurs ont pass� leur tour d'affil�e
                {
                    *finPartie = 1; //Indication que la partie est termin�e
                }

                return 6; //Passage � la demande de passage au joueur suivant

            }
        }
        else //La souris est ailleurs sur l'�cran
        {
            textprintf_centre_ex(doubleBuffer, policeActions, 1312, 475, makecol(255, 255, 255), -1, "Passer son tour"); //Le texte s'affiche en blanc
        }
    }


    //"SAUVEGARDER LA PARTIE"
    if(mouse_x>=1185 && mouse_x<=1440 && mouse_y>=518 && mouse_y<545) //Souris sur "Sauvegarder la partie"
    {
        textprintf_centre_ex(doubleBuffer, policeActions, 1312, 525, makecol(255, 255, 0), -1, "Sauvegarder la partie"); //le texte s'affiche en jaune
        if(mouse_b & 1) //Clic sur "Sauvegarder la partie"
        {
            return 4; //Processus de sauvegarde de la partie
        }
    }
    else //La souris est ailleurs sur l'�cran
    {
        textprintf_centre_ex(doubleBuffer, policeActions, 1312, 525, makecol(255, 255, 255), -1, "Sauvegarder la partie"); //Affichage du texte en blanc
    }

    if(key[KEY_P])
    {
        return 4;
    }

    //"QUITTER"
    if(mouse_x>=1280 && mouse_x<=1355 && mouse_y>=568 && mouse_y<=595) //Souris sur "Quitter"
    {
        textprintf_centre_ex(doubleBuffer, policeActions, 1312, 575, makecol(255, 0, 0), -1, "Quitter"); //Le texte s'affiche en rouge
        if(mouse_b & 1) //Clic sur "Quitter"
        {
            return 5; //Processus de "quittage" de la partie en cours et de retour au menu principal
        }
    }
    else //La souris est ailleurs sur l'�cran
    {
        textprintf_centre_ex(doubleBuffer, policeActions, 1312, 575, makecol(255, 255, 255), -1, "Quitter"); //Le texte s'affiche en blanc
    }

    return 1; //De base, on reste dans cette phase de choix de l'action � effectuer
}


/*
Sous-programme de l'�cran affich� lors d'une sauvegarde
*/
void ecranSave(t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, t_pioche *pioche, BITMAP *doubleBuffer, BITMAP *plateauJeu, BITMAP *vignetteMacareux, t_defausse *defausse, BITMAP *carteDos, FONT *tabPolices[POLICES], t_carteTerrain *plateau[7][5], BITMAP *tabCasesArrivee[2][3], int numFichier)
{
    int i; //Permet le bon fonctionnement des diff�rentes boucles
    int done = 0; //Permet le bon fonctionnement de la boucle de d�roulement
    int decalageAutreJoueur = 0; //Permet le d�calage progressif en Y des autres joueurs
    int clic = 0; //Permet d'�viter la r�p�tition trop rapide des conditions de clic

    while(!done) //Boucle de d�roulement tant que le joueur actuel n'a pas d�cid� de passer au joueur suivant
    {
        decalageAutreJoueur = 0; //R�initialisation du d�calage vertical des infos des autres joueurs

        clear_bitmap(doubleBuffer); //R�initialisation du double buffer
        blit(plateauJeu, doubleBuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //Affichage du plateau de jeu

        //Affichage du plateau contenant les chemins form�s par les cartes Terrain
        affichagePlateau(plateau, doubleBuffer);

        //Affichage des cases Arriv�e de dos
        affichageCartesArrivee(tabCasesArrivee, doubleBuffer);

        //Affichage de la pioche et de la d�fausse
        blit(carteDos, doubleBuffer, 0, 0, 1231, 46, carteDos->w, carteDos->h); //Pioche
        textprintf_centre_ex(doubleBuffer, tabPolices[5], 1273, 90, makecol(255, 255, 255), -1, "%d", pioche->indiceSommet); //Nombre de cartes restantes dans la pioche
        if(defausse->etat == 1) //La d�fausse existe
        {
            blit(defausse->imageDefausse, doubleBuffer, 0, 0, 1381, 46, defausse->imageDefausse->w, defausse->imageDefausse->h); //D�fausse
        }

        //Indication de l'emplacement de la sauvegarde
        textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 200, makecol(255, 255, 0), -1, "La partie est");
        textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 235, makecol(255, 255, 0), -1, "sauvegardee dans :");
        textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 400, makecol(255, 255, 255), -1, "Sauvegarde %d", numFichier);

        //Affichage du profil du joueur actuel
        affichageProfilJoueurActuel(tabJoueurs[joueurActuel], doubleBuffer, vignetteMacareux, tabPolices[6], tabPolices[5]);

        //Processus d'affichage des informations des autres joueurs dans l'ordre de passage
        i = (joueurActuel+1)%nbreJoueurs;
        while(i != joueurActuel) //Boucle permettant l'affichage des joueurs dans l'ordre de passage
        {
            affichageProfilAutresJoueurs(tabJoueurs[i], doubleBuffer, tabPolices[6], tabPolices[7], decalageAutreJoueur); //Affichage des profils
            decalageAutreJoueur++; //D�calage du profil suivant vers le bas

            i = (i+1)%nbreJoueurs; //Passage au joueur suivant
        }

        //Affichage des cartes du joueur actuel
        affichageCartesJoueurActuel(tabJoueurs[joueurActuel], doubleBuffer);

        if(mouse_x>=1282 && mouse_x<=1340 && mouse_y>=597 && mouse_y<=623) //La souris passe sur "RETOUR"
        {
            textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 600, makecol(255, 0, 0), -1, "Retour"); //Affichage du texte en rouge
            if(mouse_b & 1 && clic == 0) //Clic sur "RETOUR"
            {
                clic = 1; //Evite la r�p�tition de la condition en cas de maintien du clic gauche
                for(i=0 ; i<6 ; i++) //Boucle de r�initialisation de l'�tat de chaque carte
                {
                    if(tabJoueurs[joueurActuel]->mainJoueur[i] != NULL) //La carte existe
                    {
                        tabJoueurs[joueurActuel]->mainJoueur[i]->etat = 'm'; //R�initialisation de l'�tat de la carte
                    }
                }
                done = 1; //Permet de sortir de la boucle de d�roulement de ce menu de d�fausse
            }
            else if(!(mouse_b & 1) && clic == 1) //Remise � 0 de la possibilit� de clic
            {
                clic = 0;
            }
        }
        else //La souris est ailleurs sur l'�cran
        {
            textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 600, makecol(255, 255, 255), -1, "Retour"); //Affichage du texte en blanc
        }

        blit(doubleBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //Affichage du double buffer
    }
}


/*
Sous-programme de demande de passage au joueur suivant
*/
void demandeSuivant(t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, t_pioche *pioche, BITMAP *doubleBuffer, BITMAP *plateauJeu, BITMAP *vignetteMacareux, t_defausse *defausse, BITMAP *carteDos, FONT *tabPolices[POLICES], t_carteTerrain *plateau[7][5], BITMAP *tabCasesArrivee[2][3], int devoilementCaseArrivee, int finPartie, int passerTour)
{
    int i; //Permet le bon fonctionnement des diff�rentes boucles
    int done = 0; //Permet le bon fonctionnement de la boucle de d�roulement
    int decalageAutreJoueur = 0; //Permet le d�calage progressif en Y des autres joueurs
    int clic = 0; //Permet d'�viter la r�p�tition trop rapide des conditions de clic
    int indicSave = 0; //Permet de d�terminer, en cas de game over, la save des scores

    while(!done) //Boucle de d�roulement tant que le joueur actuel n'a pas d�cid� de passer au joueur suivant
    {
        decalageAutreJoueur = 0; //R�initialisation du d�calage vertical des infos des autres joueurs

        clear_bitmap(doubleBuffer); //R�initialisation du double buffer
        blit(plateauJeu, doubleBuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //Affichage du plateau de jeu

        //Affichage du plateau contenant les chemins form�s par les cartes Terrain
        affichagePlateau(plateau, doubleBuffer);

        if(finPartie == 0) //La partie n'est pas encore termin�e, les cases Arriv�e s'affichent de dos
        {
            //Affichage des cases Arriv�e de dos
            affichageCartesArrivee(tabCasesArrivee, doubleBuffer);
        }
        else //La partie est termin�e, les cases Arriv�e s'affichent de face
        {
            for(i=0 ; i<3 ; i++) //Boucle d'affichage des faces de cartes des cases arriv�e
            {
                blit(tabCasesArrivee[0][i], doubleBuffer, 0, 0, 1039, 174 + i*230, tabCasesArrivee[0][i]->w, tabCasesArrivee[0][i]->h); //Affichage de la carte de face
            }
        }

        //Le joueur vient d'utiliser une carte MAP sur une des cases Arriv�e
        if(devoilementCaseArrivee != -1)
        {
            blit(tabCasesArrivee[0][devoilementCaseArrivee], doubleBuffer, 0, 0, 1039, 174 + devoilementCaseArrivee*230, tabCasesArrivee[0][devoilementCaseArrivee]->w, tabCasesArrivee[0][devoilementCaseArrivee]->h); //Affichage de la face avant de la case vis�e
        }

        //Affichage de la pioche et de la d�fausse
        blit(carteDos, doubleBuffer, 0, 0, 1231, 46, carteDos->w, carteDos->h); //Pioche
        textprintf_centre_ex(doubleBuffer, tabPolices[5], 1273, 90, makecol(255, 255, 255), -1, "%d", pioche->indiceSommet); //Nombre de cartes restantes dans la pioche
        if(defausse->etat == 1) //La d�fausse existe
        {
            blit(defausse->imageDefausse, doubleBuffer, 0, 0, 1381, 46, defausse->imageDefausse->w, defausse->imageDefausse->h); //D�fausse
        }

        if(finPartie == 0) //La partie n'est pas encore termin�e, demande de passage au joueur suivant
        {
            //Demande de s�lection de passage au joueur suivant
            textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 200, makecol(255, 255, 0), -1, "Vous avez termine ?");
        }
        else //La partie est termin�e, demande de passage � l'�cran de fin
        {
            //Demande de fin de partie
            textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 200, makecol(255, 255, 0), -1, "La partie peut se terminer ?");
            if(passerTour == nbreJoueurs) //Tous les joueurs ont pass� leur tour
            {
                textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 350, makecol(255, 255, 255), -1, "Plus personne ne peut jouer");
                if(indicSave == 0) //La sauvegarde des scores n'a pas encore eu lieu
                {
                    for(i=0 ; i<nbreJoueurs ; i++) //Parcours du tableau de joueurs afin de donner 6 points au macareux
                    {
                        if(tabJoueurs[i]->role == 'm') //Macareux trouv�
                        {
                            tabJoueurs[i]->score = 6;
                        }
                    }
                    sauvegardeScores(tabJoueurs, nbreJoueurs); //Sauvegarde des scores dans le fichier d�di�

                    indicSave = 1; //Indication que la sauvegarde des scores a eu lieu
                }
            }
        }

        //Affichage du profil du joueur actuel
        affichageProfilJoueurActuel(tabJoueurs[joueurActuel], doubleBuffer, vignetteMacareux, tabPolices[6], tabPolices[5]);

        //Processus d'affichage des informations des autres joueurs dans l'ordre de passage
        i = (joueurActuel+1)%nbreJoueurs;
        while(i != joueurActuel) //Boucle permettant l'affichage des joueurs dans l'ordre de passage
        {
            affichageProfilAutresJoueurs(tabJoueurs[i], doubleBuffer, tabPolices[6], tabPolices[7], decalageAutreJoueur); //Affichage des profils
            if(finPartie == 1) //C'est la fin de partie, le joueur macareux est r�v�l�
            {
                if(tabJoueurs[i]->role == 'm') //Le joueur dont on affiche actuellement le profil est un macareux
                {
                    stretch_blit(vignetteMacareux, doubleBuffer, 0, 0, vignetteMacareux->w, vignetteMacareux->h, 40, 378 + decalageAutreJoueur*134, 75, 75); //Affichage de la vignette macareux
                }
            }
            decalageAutreJoueur++; //D�calage du profil suivant vers le bas

            i = (i+1)%nbreJoueurs; //Passage au joueur suivant
        }

        //Affichage des cartes du joueur actuel
        affichageCartesJoueurActuel(tabJoueurs[joueurActuel], doubleBuffer);

        if(mouse_x>=1295 && mouse_x<=1332 && mouse_y>=448 && mouse_y<=473) //La souris passe sur "OUI"
        {
            textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 450, makecol(255, 0, 0), -1, "OUI"); //Affichage du texte en rouge
            if(mouse_b & 1 && clic == 0) //Clic sur "OUI"
            {
                clic = 1; //Evite la r�p�tition successive de la condition de clic
                done = 1;
            }
            else if(!(mouse_b & 1) && clic == 1) //Permet la r�initialisation de la capacit� de cliquer
            {
                clic = 0;
            }
        }
        else //La souris est ailleurs sur l'�cran
        {
            textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 450, makecol(255, 255, 255), -1, "OUI"); //Affichage du texte en blanc
        }

        blit(doubleBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //Affichage du double buffer
    }
}


/*
Sous-programme de calcul des scores en fin de partie
*/
void calculScoresFinPartie(t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, int caseX, int caseY, BITMAP *tabCasesArrivee[2][3], BITMAP *tabBIT[BIT])
{
    int i; //Permet le parcours du tableau de joueurs en partant du joueur ayant gagn�
    int caseArrivee; //Permet de savoir quelle case du tableau de cases a �t� atteinte

    if(tabJoueurs[joueurActuel]->role == 'p') //Le joueur gagnant est un pingouin
    {
        if(caseX == 6 && caseY == 0) //Case 0 du tableau de cases atteinte
        {
            caseArrivee = 0;

            //Attribution de la bonne quantit� de points au pingouin gagnant
            if(tabCasesArrivee[0][caseArrivee] == tabBIT[45]) //Poisson 1
            {
                tabJoueurs[joueurActuel]->score = 1;
            }
            else if(tabCasesArrivee[0][caseArrivee] == tabBIT[46]) //Poisson 2
            {
                tabJoueurs[joueurActuel]->score = 2;
            }
            else if(tabCasesArrivee[0][caseArrivee] == tabBIT[47]) //Poison 3
            {
                tabJoueurs[joueurActuel]->score = 3;
            }
        }
        else if(caseX == 6 && caseY == 2) //Case 1 du tableau de cases atteinte
        {
            caseArrivee = 1;

            //Attribution de la bonne quantit� de points au pingouin gagnant
            if(tabCasesArrivee[0][caseArrivee] == tabBIT[45]) //Poisson 1
            {
                tabJoueurs[joueurActuel]->score = 1;
            }
            else if(tabCasesArrivee[0][caseArrivee] == tabBIT[46]) //Poisson 2
            {
                tabJoueurs[joueurActuel]->score = 2;
            }
            else if(tabCasesArrivee[0][caseArrivee] == tabBIT[47]) //Poisson 3
            {
                tabJoueurs[joueurActuel]->score = 3;
            }
        }
        else if(caseX == 6 && caseY == 4) //Case 2 du tableau de cases atteintes
        {
            caseArrivee = 2;

            //Attribution de la bonne quantit� de points au pingouin gagnant
            if(tabCasesArrivee[0][caseArrivee] == tabBIT[45]) //Poisson 1
            {
                tabJoueurs[joueurActuel]->score = 1;
            }
            else if(tabCasesArrivee[0][caseArrivee] == tabBIT[46]) //Poisson 2
            {
                tabJoueurs[joueurActuel]->score = 2;
            }
            else if(tabCasesArrivee[0][caseArrivee] == tabBIT[47]) //Poisson 3
            {
                tabJoueurs[joueurActuel]->score = 3;
            }
        }

        //Parcours du reste du tableau de joueurs
        i = (joueurActuel+1)%nbreJoueurs;
        while(i != joueurActuel) //Boucle permettant l'affichage des joueurs dans l'ordre de passage
        {
            if(tabJoueurs[i]->role == 'p') //Le joueur est un pingouin
            {
                caseArrivee = (caseArrivee + 1)%3; //La case Arriv�e consid�r�e est la suivante dans le tableau de cases

                if(tabCasesArrivee[0][caseArrivee] == tabBIT[45])
                {
                    tabJoueurs[i]->score = 1;
                }
                else if(tabCasesArrivee[0][caseArrivee] == tabBIT[46])
                {
                    tabJoueurs[i]->score = 2;
                }
                else if(tabCasesArrivee[0][caseArrivee] == tabBIT[47])
                {
                    tabJoueurs[i]->score = 3;
                }
            }

            i = (i+1)%nbreJoueurs; //Passage au joueur suivant
        }
    }

    //Le joueur gagnant est le macareux
    else if(tabJoueurs[joueurActuel]->role == 'm')
    {
        tabJoueurs[joueurActuel]->score = 6; //Attribution de 6 points au macareux
    }
}


/*
Sous-programme de gestion des messages d'erreur
*/
void messageErreur(int cas, int *impossible, BITMAP *doubleBuffer, FONT *policeImpossible, FONT *policeRaison)
{
    int alpha = 0; //Intialise l'opacit� � 0

    (*impossible)++; //Augmentation progressive de l'indicateur d'effacement du message

    if(*impossible>=0) //Si l'indicateur rentre dans l'intervalle >= 0 && <= 255 alpha prend sa valeur
    {
        alpha = *impossible;
    }

    //R�glage pour l'opacit�
    drawing_mode(DRAW_MODE_TRANS,NULL,0,0);
    set_trans_blender(255,0,0,alpha);

    //Diff�rents messages possibles
    switch(cas)
    {
        case 1 : //Carte de mauvais type par rapport � l'action souhait�e
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 400, makecol(255, 0, 0), -1, "IMPOSSIBLE");
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 430, makecol(255, 255, 255), -1, "Le type de carte");
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 460, makecol(255, 255, 255), -1, "n'est pas valide");
            break;

        case 2 : //Le joueur poss�de une p�nalit� et ne peut donc pas jouer de carte Terrain
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 400, makecol(255, 0, 0), -1, "IMPOSSIBLE");
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 430, makecol(255, 255, 255), -1, "Vous possedez au moins");
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 460, makecol(255, 255, 255), -1, "une penalite");
            break;

        case 3 : //Le joueur vis� poss�de d�j� la p�nalit� qu'on veut lui donner
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 400, makecol(255, 0, 0), -1, "IMPOSSIBLE");
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 430, makecol(255, 255, 255), -1, "Le joueur vise possede");
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 460, makecol(255, 255, 255), -1, "deja cette penalite");
            break;

        case 4 : //La p�nalit� que l'on souhaite r�par�e n'est pas poss�d�e par le joueur vis�
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 400, makecol(255, 0, 0), -1, "IMPOSSIBLE");
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 430, makecol(255, 255, 255), -1, "La penalite correspondante");
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 460, makecol(255, 255, 255), -1, "n'est pas active");
            break;

        case 5 : //La position de la carte Terrain / r�chauffement climatique n'est pas valide
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 400, makecol(255, 0, 0), -1, "IMPOSSIBLE");
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 430, makecol(255, 255, 255), -1, "Le positionnement choisi");
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 460, makecol(255, 255, 255), -1, "n'est pas valide");
            break;

        case 6 : //La carte s�lectionn�e pour r�v�ler les cases d'arriv�e n'est pas la carte MAP
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 400, makecol(255, 0, 0), -1, "IMPOSSIBLE");
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 430, makecol(255, 255, 255), -1, "Il vous faut la carte");
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 460, makecol(255, 255, 255), -1, "MAP pour faire ceci");
            break;

        case 7 :
            textprintf_centre_ex(doubleBuffer, policeImpossible, SCREEN_W/2, 500, makecol(255, 0, 0), -1, "IMPOSSIBLE");
            textprintf_centre_ex(doubleBuffer, policeRaison, SCREEN_W/2, 550, makecol(255, 255, 255), -1, "Votre pseudo est vide");
            break;

        case 8 :
            textprintf_centre_ex(doubleBuffer, policeImpossible, SCREEN_W/2, 500, makecol(255, 0, 0), -1, "IMPOSSIBLE");
            textprintf_centre_ex(doubleBuffer, policeRaison, SCREEN_W/2, 550, makecol(255, 255, 255), -1, "Le pseudo est deja pris");
            break;

        case 9 :
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 400, makecol(255, 0, 0), -1, "ANNUATION");
            textprintf_centre_ex(doubleBuffer, policeRaison, 1312, 430, makecol(255, 255, 255), -1, "du placement de la carte");
            break;
    }

    if(cas == 7 || cas == 8)
    {
        rectfill(doubleBuffer,SCREEN_W/2 - 200,450,SCREEN_W/2 + 200,600,makecol(158,218,222)); //Apparition progressive d'un rectangle bleu par dessus le texte
    }
    else
    {
        rectfill(doubleBuffer,1150,350,SCREEN_W,500,makecol(158,218,222)); //Apparition progressive d'un rectangle bleu par dessus le texte
    }

    solid_mode(); //Retour au mode d'affichage normal
}
