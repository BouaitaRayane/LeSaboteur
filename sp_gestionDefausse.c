#include "header.h"


/*
Sous-programme permettant de savoir si la pioche (pile) est vide
*/
int estVide(t_pioche * PileCarte)
{
    if (PileCarte->indiceSommet==0) //Cas o� la pioche est vide
    {
        return 1;
    }
    else //Cas o� la pioche n'est pas vide
    {
        return 0;
    }
}


/*
Sous-programme de d�fausse de trois cartes
*/
int defausseTroisCartes(t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, t_pioche *pioche, BITMAP *doubleBuffer, BITMAP *plateauJeu, BITMAP *vignetteMacareux, t_defausse *defausse, BITMAP *carteDos, FONT *tabPolices[POLICES], t_carteTerrain *plateau[7][5], BITMAP *tabCartesArrivee[2][3], int *ENDgame)
{
    int i; //Permet le bon fonctionnement des diff�rentes boucles
    int done = 0; //Permet le bon fonctionnement de la boucle de d�roulement
    int decalageAutreJoueur = 0; //Permet le d�calage progressif en Y des autres joueurs
    int nbreCartesChoisies = 0; //Permet de compter le nombre de cartes choisies pour la d�fausse
    int clic = 0; //Permet d'�viter la r�p�tition trop rapide des conditions de clic
    int phaseSortie = 5; //Permet de conna�tre l'�tat du tour en sortie du sous-programme
    int choixDefausse = 1; //Permet de r�cup�rer le choix de conservation ou de d�fausse des cartes s�lectionn�es

    t_carte *tabCartesChoisies[3]; //Permet de contenir les informations des cartes saisies

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
        blit(carteDos, doubleBuffer, 0, 0, 1231, 46, carteDos->w, carteDos->h); //Pioche
        textprintf_centre_ex(doubleBuffer, tabPolices[5], 1273, 90, makecol(255, 255, 255), -1, "%d", pioche->indiceSommet); //Affichage du nombre de cartes restantes dans la pioche
        if(defausse->etat == 1) //La d�fausse existe
        {
            blit(defausse->imageDefausse, doubleBuffer, 0, 0, 1381, 46, defausse->imageDefausse->w, defausse->imageDefausse->h); //D�fausse
        }

        //Demande de s�lection des trois cartes � d�fausser
        textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 200, makecol(255, 255, 0), -1, "Choississez trois cartes");

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

        if(nbreCartesChoisies < 3) //Tant que le joueur n'a pas s�lectionn� trois cartes
        {
            choixCarte(tabJoueurs[joueurActuel]->mainJoueur, tabCartesChoisies, &nbreCartesChoisies, &clic, doubleBuffer); //Processus de s�lection d'une nouvelle carte � d�fausser

            if(nbreCartesChoisies == 0) //Le joueur n'a s�lectionn� aucune carte
            {
                textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 350, makecol(255, 255, 255), -1, "Aucune carte choisie");
            }
            else if(nbreCartesChoisies == 1) //Le joueur a s�lectionn� une carte
            {
                textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 300, makecol(255, 255, 255), -1, "%d carte choisie", nbreCartesChoisies);
                textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 350, makecol(255, 255, 255), -1, "Voulez-vous la defausser ?");
            }
            else if(nbreCartesChoisies == 2) //Le joueur a s�lectionn� deux cartes
            {
                textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 300, makecol(255, 255, 255), -1, "%d cartes choisies", nbreCartesChoisies);
                textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 350, makecol(255, 255, 255), -1, "Voulez-vous les defausser ?");
            }

            if(nbreCartesChoisies > 0) //Le joueur a s�lectionn� au moins une carte, demande de validation de la d�fausse
            {
                if(mouse_x>=1295 && mouse_x<=1332 && mouse_y>=388 && mouse_y<=410) //La souris passe sur le texte "OUI"
                {
                    textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 390, makecol(255, 0, 0), -1, "OUI"); //Affichage du texte en rouge
                    if(mouse_b & 1 && clic == 0) //Clic sur "OUI"
                    {
                        clic = 1; //Evite la r�p�tition successive de la condition de clic
                        choixDefausse = 2; //Indication que le processus de d�fausse peut s'effectuer
                    }
                    else if(!(mouse_b & 1) && clic == 1) //Permet la r�initialisation de la capacit� de cliquer
                    {
                        clic = 0;
                    }
                }
                else //La souris est ailleurs sur l'�cran
                {
                    textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 390, makecol(255, 255, 255), -1, "OUI"); //Le texte s'affiche en blanc
                }
            }
        }
        else if(nbreCartesChoisies == 3) //Le joueur a s�lectionn� les trois cartes � d�fausser
        {
            choixDefausse = demandeDefausse(&clic, nbreCartesChoisies, doubleBuffer, tabPolices[6]); //Demande de validation du processus de d�fausse
        }

        //Affichage des cartes du joueur actuel
        affichageCartesJoueurActuel(tabJoueurs[joueurActuel], doubleBuffer);

        if(choixDefausse == 1) //Le joueur n'a pas encore choisi de d�fausser ses cartes
        {
            if(mouse_x>=1282 && mouse_x<=1340 && mouse_y>=597 && mouse_y<=623) //La souris passe sur "RETOUR"
            {
                textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 600, makecol(255, 0, 0), -1, "Retour"); //Affichage du texte en rouge
                if(mouse_b & 1 && clic == 0) //Clic sur "RETOUR"
                {
                    clic = 1; //Evite la r�p�tition de la condition en cas de maintien du clic gauche
                    phaseSortie = 1; //Retour au menu de s�lection de l'action
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
        }
        else if(choixDefausse == 2) //Le joueur a choisi les cartes � d�fausser et souhaite les d�fausser
        {
            //Processus de sauvegarde de la derni�re carte d�fauss�e dans la d�fausse
            if(tabCartesChoisies[nbreCartesChoisies-1]->typeAT == 'a') //Il s'agit d'une carte Action
            {
                defausse->imageDefausse = tabCartesChoisies[nbreCartesChoisies-1]->typeAction->visuelAvant;
                defausse->etat = 1;
                //blit(tabCartesChoisies[nbreCartesChoisies-1]->typeAction->visuelAvant, defausse, 0, 0, 0, 0, tabCartesChoisies[nbreCartesChoisies-1]->typeAction->visuelAvant->w, tabCartesChoisies[nbreCartesChoisies-1]->typeAction->visuelAvant->h); //Sauvegarde de la BITMAP
            }
            else //Il s'agit d'une carte Terrain
            {
                defausse->imageDefausse = tabCartesChoisies[nbreCartesChoisies-1]->typeTerrain->visuelAvant;
                defausse->etat = 1;
                //blit(tabCartesChoisies[nbreCartesChoisies-1]->typeTerrain->visuelAvant, defausse, 0, 0, 0, 0, tabCartesChoisies[nbreCartesChoisies-1]->typeTerrain->visuelAvant->w, tabCartesChoisies[nbreCartesChoisies-1]->typeTerrain->visuelAvant->h); //Sauvegarde de la BITMAP
            }

            for(i=0 ; i<6 ; i++) //Parcours de la main du joueur actuel en qu�te des cartes s�lectionn�es
            {
                if(tabJoueurs[joueurActuel]->mainJoueur[i] != NULL && tabJoueurs[joueurActuel]->mainJoueur[i]->etat == 't') //La carte est une carte ayant �t� s�lectionn�e pour la d�fausse
                {
                    tabJoueurs[joueurActuel]->mainJoueur[i] = NULL; //Suppression de la carte de la main du joueur
                }
            }

            for(i=0 ; i<6 ; i++) //Parcours de la main du joueur pour remplacer les cartes supprim�es
            {
                if(*ENDgame == 0) //Il reste des cartes dans la pioche
                {
                    if(tabJoueurs[joueurActuel]->mainJoueur[i] == NULL) //La carte dans la main n'existe plus
                    {
                        if(pioche->indiceSommet > 0)
                        {
                            (pioche->indiceSommet)--; //D�cr�mentation de l'indice sommet de la pioche
                            (pioche->indiceNbPlaces)++; //Incr�mentation du nombre de place de la pioche

                            //R�cup�ration de la nouvelle carte depuis la pioche
                            tabJoueurs[joueurActuel]->mainJoueur[i] = pioche->deck[pioche->indiceSommet];
                            free(pioche->deck[pioche->indiceSommet]);
                            pioche->deck[pioche->indiceSommet] = NULL;
                        }
                        else if(pioche->indiceSommet == 0) //Il ne reste plus de carte dans la pioche
                        {
                            *ENDgame = 1; //Indication qu'on se trouve dans le END game, la carte n'est pas remplac�e
                        }
                    }
                }
            }

            done = 1; //Indication que le processus de d�fausse est termin�
            phaseSortie = 6; //Indication de passage � la phase de demande de passage au joueur suivant
        }
        else if(choixDefausse == 3) //Le joueur a choisi les trois cartes � d�fausser et ne souhaite finalement pas les d�fausser, r�initialisation du processus de d�fausse
        {
            for(i=0 ; i<6 ; i++) //Boucle de r�initialisation de l'�tat de chaque carte
            {
                if(tabJoueurs[joueurActuel]->mainJoueur[i] != NULL) //La carte existe
                {
                    tabJoueurs[joueurActuel]->mainJoueur[i]->etat = 'm'; //R�initialisation de l'�tat de la carte
                }
            }
            nbreCartesChoisies = 0; //R�initialisation du nombre de cartes choisies pour la d�fausse
            choixDefausse = 1; //Remise � 0 du processus de s�lection de cartes � d�fausser
        }

        blit(doubleBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //Affichage du double buffer
    }

    return phaseSortie;
}


/*
Sous-programme de s�lection d'une carte � d�fausser
*/
void choixCarte(t_carte *mainJoueur[6], t_carte *tabCartesChoisies[3], int *nbreCartesChoisies, int *clic, BITMAP *doubleBuffer)
{
    int i; //Permet le bon fonctionnement de la boucle de parcours des informations de position de chaque carte

    for(i=0 ; i<6 ; i++) //Boucle de parcours de la main du joueur actuel
    {
        if(mainJoueur[i] != NULL) //La carte existe
        {
            if(mainJoueur[i]->etat == 'm') //La carte n'est pas d�j� s�lectionn� pour la d�fausse
            {
                if(mainJoueur[i]->typeAT == 'a') //La carte est de type Action
                {
                    if(mouse_x>=mainJoueur[i]->posX && mouse_x<=mainJoueur[i]->posX + mainJoueur[i]->typeAction->visuelAvant->w && mouse_y>=mainJoueur[i]->posY && mouse_y<=mainJoueur[i]->posY + mainJoueur[i]->typeAction->visuelAvant->h) //La souris passe sur la carte
                    {
                        rectfill(doubleBuffer, mainJoueur[i]->posX - 7, mainJoueur[i]->posY - 7, mainJoueur[i]->posX + mainJoueur[i]->typeAction->visuelAvant->w + 7, mainJoueur[i]->posY + mainJoueur[i]->typeAction->visuelAvant->h + 7, makecol(255, 255, 0)); //Affichage d'un rectangle jaune derri�re la carte
                        if(mouse_b & 1 && *clic == 0) //Clic sur la carte
                        {
                            *clic = 1; //Evite la r�p�tition successive de la condition de clic
                            mainJoueur[i]->etat = 't'; //Actualisation de l'�tat de carte afin d'�viter la res�lection
                            tabCartesChoisies[*nbreCartesChoisies] = mainJoueur[i]; //Ajout de la carte dans le tableau des cartes choisies pour la d�fausse
                            (*nbreCartesChoisies)++; //Augmentation du nombre de cartes choisies pour la d�fausse
                        }
                        else if(!(mouse_b & 1) && *clic == 1) //Permet la r�initialisation de la capacit� de cliquer
                        {
                            *clic = 0;
                        }
                    }
                }
                else //La carte est de type Terrain
                {
                    if(mouse_x>=mainJoueur[i]->posX && mouse_x<=mainJoueur[i]->posX + mainJoueur[i]->typeTerrain->visuelAvant->w && mouse_y>=mainJoueur[i]->posY && mouse_y<=mainJoueur[i]->posY + mainJoueur[i]->typeTerrain->visuelAvant->h) //La souris passe sur la carte
                    {
                        rectfill(doubleBuffer, mainJoueur[i]->posX - 7, mainJoueur[i]->posY - 7, mainJoueur[i]->posX + mainJoueur[i]->typeTerrain->visuelAvant->w + 7, mainJoueur[i]->posY + mainJoueur[i]->typeTerrain->visuelAvant->h + 7, makecol(255, 255, 0)); //Affichage d'un rectangle jaune derri�re la carte
                        if(mouse_b & 1 && *clic == 0) //Clic sur la carte
                        {
                            *clic = 1; //Evite la r�p�tition successive de la condition de clic
                            mainJoueur[i]->etat = 't'; //Actualisation de l'�tat de la carte afin d'�viter la res�lection
                            tabCartesChoisies[*nbreCartesChoisies] = mainJoueur[i]; //Ajout de la carte dans le tableau des cartes choisies pour la d�fausse
                            (*nbreCartesChoisies)++; //Augmentation du nombre de cartes choisies pour la d�fausse
                        }
                        else if(!(mouse_b & 1) && *clic == 1) //Permet la r�initialisation de la capacit� de cliquer
                        {
                            *clic = 0;
                        }
                    }
                }
            }
            else if(mainJoueur[i]->etat == 't') //La carte a d�j� �t� s�lectionn�, possibilit� de la d�selctionner
            {
                if(mainJoueur[i]->typeAT == 'a') //La carte est de type Action
                {
                    if(mouse_x>=mainJoueur[i]->posX && mouse_x<=mainJoueur[i]->posX + mainJoueur[i]->typeAction->visuelAvant->w && mouse_y>=mainJoueur[i]->posY && mouse_y<=mainJoueur[i]->posY + mainJoueur[i]->typeAction->visuelAvant->h) //La souris passe sur la carte
                    {
                        if(mouse_b & 1 && *clic == 0) //Clic sur la carte
                        {
                            *clic = 1; //Evite la r�p�tition successive de la condition de clic
                            mainJoueur[i]->etat = 'm'; //R�initialisation de l'�tat de la carte
                            (*nbreCartesChoisies)--; //Diminution du nombre de cartes choisies pour la d�fausse
                        }
                        else if(!(mouse_b & 1) && *clic == 1) //Permet la r�initialisation de la capacit� de cliquer
                        {
                            *clic = 0;
                        }
                    }
                }
                else //La carte est du type Terrain
                {
                    if(mouse_x>=mainJoueur[i]->posX && mouse_x<=mainJoueur[i]->posX + mainJoueur[i]->typeTerrain->visuelAvant->w && mouse_y>=mainJoueur[i]->posY && mouse_y<=mainJoueur[i]->posY + mainJoueur[i]->typeTerrain->visuelAvant->h) //La souris passe sur la carte
                    {
                        if(mouse_b & 1 && *clic == 0) //Clic sur la carte
                        {
                            *clic = 1; //Evite la r�p�tition successive de la condition de clic
                            mainJoueur[i]->etat = 'm'; //R�initialisation de l'�tat de la carte
                            (*nbreCartesChoisies)--; //Diminution du nombre de cartes choisies pour la d�fausse
                        }
                        else if(!(mouse_b & 1) && *clic == 1) //Permet la r�initialisation de la capacit� de cliquer
                        {
                            *clic = 0;
                        }
                    }
                }
            }
        }
    }
}


/*
Sous-programme de validation / annulation de la d�fausse
*/
int demandeDefausse(int *clic, int nbreCartesChoisies, BITMAP *doubleBuffer, FONT *policeTexte)
{
    //Affichage de la demande de validation
    textprintf_centre_ex(doubleBuffer, policeTexte, 1312, 300, makecol(255, 255, 255), -1, "Vous avez choisi 3 cartes");
    textprintf_centre_ex(doubleBuffer, policeTexte, 1312, 325, makecol(255, 255, 255), -1, "Voulez-vous les defausser ?");

    if(mouse_x>=1295 && mouse_x<=1332 && mouse_y>=368 && mouse_y<=390) //La souris passe sur le texte "OUI"
    {
        textprintf_centre_ex(doubleBuffer, policeTexte, 1312, 370, makecol(255, 0, 0), -1, "OUI"); //Affichage du texte en rouge
        if(mouse_b & 1 && *clic == 0) //Clic sur "OUI"
        {
            *clic = 1; //Evite la r�p�tition successive de la condition de clic
            return 2; //Indication que la d�fausse peut se faire
        }
        else if(!(mouse_b & 1) && *clic == 1) //Permet la r�initialisation de la capacit� de cliquer
        {
            *clic = 0;
        }
    }
    else //La souris est ailleurs sur l'�cran
    {
        textprintf_centre_ex(doubleBuffer, policeTexte, 1312, 370, makecol(255, 255, 255), -1, "OUI"); //Le texte s'affiche en blanc
    }

    if(mouse_x>=1290 && mouse_x<=1335 && mouse_y>=408 && mouse_y<=428) //La souris passe sur le texte "NON"
    {
        textprintf_centre_ex(doubleBuffer, policeTexte, 1312, 410, makecol(255, 255, 0), -1, "NON"); //Affichage du texte en jaune
        if(mouse_b & 1 && *clic == 0) //clic sur "NON"
        {
            *clic = 1; //Evite la r�p�tition successive de la condition de clic
            return 3; //Indication de la r�initialisation du processus de d�fausse
        }
        else if(!(mouse_b & 1) && *clic == 1) //Permet la r�initialisation de la capacit� de cliquer
        {
            *clic = 0;
        }
    }
    else //La souris est ailleurs sur l'�cran
    {
        textprintf_centre_ex(doubleBuffer, policeTexte, 1312, 410, makecol(255, 255, 255), -1, "NON"); //Le texte s'affiche en blanc
    }

    return 1;
}
