#include "header.h"


/*
Sous-programme permettant de savoir si la pioche (pile) est vide
*/
int estVide(t_pioche * PileCarte)
{
    if (PileCarte->indiceSommet==0) //Cas où la pioche est vide
    {
        return 1;
    }
    else //Cas où la pioche n'est pas vide
    {
        return 0;
    }
}


/*
Sous-programme de défausse de trois cartes
*/
int defausseTroisCartes(t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, t_pioche *pioche, BITMAP *doubleBuffer, BITMAP *plateauJeu, BITMAP *vignetteMacareux, t_defausse *defausse, BITMAP *carteDos, FONT *tabPolices[POLICES], t_carteTerrain *plateau[7][5], BITMAP *tabCartesArrivee[2][3], int *ENDgame)
{
    int i; //Permet le bon fonctionnement des différentes boucles
    int done = 0; //Permet le bon fonctionnement de la boucle de déroulement
    int decalageAutreJoueur = 0; //Permet le décalage progressif en Y des autres joueurs
    int nbreCartesChoisies = 0; //Permet de compter le nombre de cartes choisies pour la défausse
    int clic = 0; //Permet d'éviter la répétition trop rapide des conditions de clic
    int phaseSortie = 5; //Permet de connaître l'état du tour en sortie du sous-programme
    int choixDefausse = 1; //Permet de récupérer le choix de conservation ou de défausse des cartes sélectionnées

    t_carte *tabCartesChoisies[3]; //Permet de contenir les informations des cartes saisies

    while(!done) //Boucle de déroulement tant qu'aucune action n'a été choisie
    {
        decalageAutreJoueur = 0; //Réinitialisation du décalage vertical des infos des autres joueurs

        clear_bitmap(doubleBuffer); //Réinitialisation du double buffer
        blit(plateauJeu, doubleBuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //Affichage du plateau de jeu

        //Affichage du plateau contenant les chemins formés par les cartes Terrain
        affichagePlateau(plateau, doubleBuffer);

        //Affichage des cases Arrivée
        affichageCartesArrivee(tabCartesArrivee, doubleBuffer);

        //Affichage de la pioche et de la défausse
        blit(carteDos, doubleBuffer, 0, 0, 1231, 46, carteDos->w, carteDos->h); //Pioche
        textprintf_centre_ex(doubleBuffer, tabPolices[5], 1273, 90, makecol(255, 255, 255), -1, "%d", pioche->indiceSommet); //Affichage du nombre de cartes restantes dans la pioche
        if(defausse->etat == 1) //La défausse existe
        {
            blit(defausse->imageDefausse, doubleBuffer, 0, 0, 1381, 46, defausse->imageDefausse->w, defausse->imageDefausse->h); //Défausse
        }

        //Demande de sélection des trois cartes à défausser
        textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 200, makecol(255, 255, 0), -1, "Choississez trois cartes");

        //Affichage du profil du joueur actuel
        affichageProfilJoueurActuel(tabJoueurs[joueurActuel], doubleBuffer, vignetteMacareux, tabPolices[6], tabPolices[5]);

        //Processus d'affichage des informations des autres joueurs dans l'ordre de passage
        i = (joueurActuel+1)%nbreJoueurs;
        while(i != joueurActuel) //Boucle permettant l'affichage des joueurs dans l'ordre de passage
        {
            affichageProfilAutresJoueurs(tabJoueurs[i], doubleBuffer, tabPolices[6], tabPolices[7], decalageAutreJoueur); //Affichage des profils
            decalageAutreJoueur++; //Décalage du profil suivant vers le bas

            i = (i+1)%nbreJoueurs; //Passage au joueur suivant
        }

        if(nbreCartesChoisies < 3) //Tant que le joueur n'a pas sélectionné trois cartes
        {
            choixCarte(tabJoueurs[joueurActuel]->mainJoueur, tabCartesChoisies, &nbreCartesChoisies, &clic, doubleBuffer); //Processus de sélection d'une nouvelle carte à défausser

            if(nbreCartesChoisies == 0) //Le joueur n'a sélectionné aucune carte
            {
                textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 350, makecol(255, 255, 255), -1, "Aucune carte choisie");
            }
            else if(nbreCartesChoisies == 1) //Le joueur a sélectionné une carte
            {
                textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 300, makecol(255, 255, 255), -1, "%d carte choisie", nbreCartesChoisies);
                textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 350, makecol(255, 255, 255), -1, "Voulez-vous la defausser ?");
            }
            else if(nbreCartesChoisies == 2) //Le joueur a sélectionné deux cartes
            {
                textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 300, makecol(255, 255, 255), -1, "%d cartes choisies", nbreCartesChoisies);
                textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 350, makecol(255, 255, 255), -1, "Voulez-vous les defausser ?");
            }

            if(nbreCartesChoisies > 0) //Le joueur a sélectionné au moins une carte, demande de validation de la défausse
            {
                if(mouse_x>=1295 && mouse_x<=1332 && mouse_y>=388 && mouse_y<=410) //La souris passe sur le texte "OUI"
                {
                    textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 390, makecol(255, 0, 0), -1, "OUI"); //Affichage du texte en rouge
                    if(mouse_b & 1 && clic == 0) //Clic sur "OUI"
                    {
                        clic = 1; //Evite la répétition successive de la condition de clic
                        choixDefausse = 2; //Indication que le processus de défausse peut s'effectuer
                    }
                    else if(!(mouse_b & 1) && clic == 1) //Permet la réinitialisation de la capacité de cliquer
                    {
                        clic = 0;
                    }
                }
                else //La souris est ailleurs sur l'écran
                {
                    textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 390, makecol(255, 255, 255), -1, "OUI"); //Le texte s'affiche en blanc
                }
            }
        }
        else if(nbreCartesChoisies == 3) //Le joueur a sélectionné les trois cartes à défausser
        {
            choixDefausse = demandeDefausse(&clic, nbreCartesChoisies, doubleBuffer, tabPolices[6]); //Demande de validation du processus de défausse
        }

        //Affichage des cartes du joueur actuel
        affichageCartesJoueurActuel(tabJoueurs[joueurActuel], doubleBuffer);

        if(choixDefausse == 1) //Le joueur n'a pas encore choisi de défausser ses cartes
        {
            if(mouse_x>=1282 && mouse_x<=1340 && mouse_y>=597 && mouse_y<=623) //La souris passe sur "RETOUR"
            {
                textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 600, makecol(255, 0, 0), -1, "Retour"); //Affichage du texte en rouge
                if(mouse_b & 1 && clic == 0) //Clic sur "RETOUR"
                {
                    clic = 1; //Evite la répétition de la condition en cas de maintien du clic gauche
                    phaseSortie = 1; //Retour au menu de sélection de l'action
                    for(i=0 ; i<6 ; i++) //Boucle de réinitialisation de l'état de chaque carte
                    {
                        if(tabJoueurs[joueurActuel]->mainJoueur[i] != NULL) //La carte existe
                        {
                            tabJoueurs[joueurActuel]->mainJoueur[i]->etat = 'm'; //Réinitialisation de l'état de la carte
                        }
                    }
                    done = 1; //Permet de sortir de la boucle de déroulement de ce menu de défausse
                }
                else if(!(mouse_b & 1) && clic == 1) //Remise à 0 de la possibilité de clic
                {
                    clic = 0;
                }
            }
            else //La souris est ailleurs sur l'écran
            {
                textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 600, makecol(255, 255, 255), -1, "Retour"); //Affichage du texte en blanc
            }
        }
        else if(choixDefausse == 2) //Le joueur a choisi les cartes à défausser et souhaite les défausser
        {
            //Processus de sauvegarde de la dernière carte défaussée dans la défausse
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

            for(i=0 ; i<6 ; i++) //Parcours de la main du joueur actuel en quête des cartes sélectionnées
            {
                if(tabJoueurs[joueurActuel]->mainJoueur[i] != NULL && tabJoueurs[joueurActuel]->mainJoueur[i]->etat == 't') //La carte est une carte ayant été sélectionnée pour la défausse
                {
                    tabJoueurs[joueurActuel]->mainJoueur[i] = NULL; //Suppression de la carte de la main du joueur
                }
            }

            for(i=0 ; i<6 ; i++) //Parcours de la main du joueur pour remplacer les cartes supprimées
            {
                if(*ENDgame == 0) //Il reste des cartes dans la pioche
                {
                    if(tabJoueurs[joueurActuel]->mainJoueur[i] == NULL) //La carte dans la main n'existe plus
                    {
                        if(pioche->indiceSommet > 0)
                        {
                            (pioche->indiceSommet)--; //Décrémentation de l'indice sommet de la pioche
                            (pioche->indiceNbPlaces)++; //Incrémentation du nombre de place de la pioche

                            //Récupération de la nouvelle carte depuis la pioche
                            tabJoueurs[joueurActuel]->mainJoueur[i] = pioche->deck[pioche->indiceSommet];
                            free(pioche->deck[pioche->indiceSommet]);
                            pioche->deck[pioche->indiceSommet] = NULL;
                        }
                        else if(pioche->indiceSommet == 0) //Il ne reste plus de carte dans la pioche
                        {
                            *ENDgame = 1; //Indication qu'on se trouve dans le END game, la carte n'est pas remplacée
                        }
                    }
                }
            }

            done = 1; //Indication que le processus de défausse est terminé
            phaseSortie = 6; //Indication de passage à la phase de demande de passage au joueur suivant
        }
        else if(choixDefausse == 3) //Le joueur a choisi les trois cartes à défausser et ne souhaite finalement pas les défausser, réinitialisation du processus de défausse
        {
            for(i=0 ; i<6 ; i++) //Boucle de réinitialisation de l'état de chaque carte
            {
                if(tabJoueurs[joueurActuel]->mainJoueur[i] != NULL) //La carte existe
                {
                    tabJoueurs[joueurActuel]->mainJoueur[i]->etat = 'm'; //Réinitialisation de l'état de la carte
                }
            }
            nbreCartesChoisies = 0; //Réinitialisation du nombre de cartes choisies pour la défausse
            choixDefausse = 1; //Remise à 0 du processus de sélection de cartes à défausser
        }

        blit(doubleBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //Affichage du double buffer
    }

    return phaseSortie;
}


/*
Sous-programme de sélection d'une carte à défausser
*/
void choixCarte(t_carte *mainJoueur[6], t_carte *tabCartesChoisies[3], int *nbreCartesChoisies, int *clic, BITMAP *doubleBuffer)
{
    int i; //Permet le bon fonctionnement de la boucle de parcours des informations de position de chaque carte

    for(i=0 ; i<6 ; i++) //Boucle de parcours de la main du joueur actuel
    {
        if(mainJoueur[i] != NULL) //La carte existe
        {
            if(mainJoueur[i]->etat == 'm') //La carte n'est pas déjà sélectionné pour la défausse
            {
                if(mainJoueur[i]->typeAT == 'a') //La carte est de type Action
                {
                    if(mouse_x>=mainJoueur[i]->posX && mouse_x<=mainJoueur[i]->posX + mainJoueur[i]->typeAction->visuelAvant->w && mouse_y>=mainJoueur[i]->posY && mouse_y<=mainJoueur[i]->posY + mainJoueur[i]->typeAction->visuelAvant->h) //La souris passe sur la carte
                    {
                        rectfill(doubleBuffer, mainJoueur[i]->posX - 7, mainJoueur[i]->posY - 7, mainJoueur[i]->posX + mainJoueur[i]->typeAction->visuelAvant->w + 7, mainJoueur[i]->posY + mainJoueur[i]->typeAction->visuelAvant->h + 7, makecol(255, 255, 0)); //Affichage d'un rectangle jaune derrière la carte
                        if(mouse_b & 1 && *clic == 0) //Clic sur la carte
                        {
                            *clic = 1; //Evite la répétition successive de la condition de clic
                            mainJoueur[i]->etat = 't'; //Actualisation de l'état de carte afin d'éviter la resélection
                            tabCartesChoisies[*nbreCartesChoisies] = mainJoueur[i]; //Ajout de la carte dans le tableau des cartes choisies pour la défausse
                            (*nbreCartesChoisies)++; //Augmentation du nombre de cartes choisies pour la défausse
                        }
                        else if(!(mouse_b & 1) && *clic == 1) //Permet la réinitialisation de la capacité de cliquer
                        {
                            *clic = 0;
                        }
                    }
                }
                else //La carte est de type Terrain
                {
                    if(mouse_x>=mainJoueur[i]->posX && mouse_x<=mainJoueur[i]->posX + mainJoueur[i]->typeTerrain->visuelAvant->w && mouse_y>=mainJoueur[i]->posY && mouse_y<=mainJoueur[i]->posY + mainJoueur[i]->typeTerrain->visuelAvant->h) //La souris passe sur la carte
                    {
                        rectfill(doubleBuffer, mainJoueur[i]->posX - 7, mainJoueur[i]->posY - 7, mainJoueur[i]->posX + mainJoueur[i]->typeTerrain->visuelAvant->w + 7, mainJoueur[i]->posY + mainJoueur[i]->typeTerrain->visuelAvant->h + 7, makecol(255, 255, 0)); //Affichage d'un rectangle jaune derrière la carte
                        if(mouse_b & 1 && *clic == 0) //Clic sur la carte
                        {
                            *clic = 1; //Evite la répétition successive de la condition de clic
                            mainJoueur[i]->etat = 't'; //Actualisation de l'état de la carte afin d'éviter la resélection
                            tabCartesChoisies[*nbreCartesChoisies] = mainJoueur[i]; //Ajout de la carte dans le tableau des cartes choisies pour la défausse
                            (*nbreCartesChoisies)++; //Augmentation du nombre de cartes choisies pour la défausse
                        }
                        else if(!(mouse_b & 1) && *clic == 1) //Permet la réinitialisation de la capacité de cliquer
                        {
                            *clic = 0;
                        }
                    }
                }
            }
            else if(mainJoueur[i]->etat == 't') //La carte a déjà été sélectionné, possibilité de la déselctionner
            {
                if(mainJoueur[i]->typeAT == 'a') //La carte est de type Action
                {
                    if(mouse_x>=mainJoueur[i]->posX && mouse_x<=mainJoueur[i]->posX + mainJoueur[i]->typeAction->visuelAvant->w && mouse_y>=mainJoueur[i]->posY && mouse_y<=mainJoueur[i]->posY + mainJoueur[i]->typeAction->visuelAvant->h) //La souris passe sur la carte
                    {
                        if(mouse_b & 1 && *clic == 0) //Clic sur la carte
                        {
                            *clic = 1; //Evite la répétition successive de la condition de clic
                            mainJoueur[i]->etat = 'm'; //Réinitialisation de l'état de la carte
                            (*nbreCartesChoisies)--; //Diminution du nombre de cartes choisies pour la défausse
                        }
                        else if(!(mouse_b & 1) && *clic == 1) //Permet la réinitialisation de la capacité de cliquer
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
                            *clic = 1; //Evite la répétition successive de la condition de clic
                            mainJoueur[i]->etat = 'm'; //Réinitialisation de l'état de la carte
                            (*nbreCartesChoisies)--; //Diminution du nombre de cartes choisies pour la défausse
                        }
                        else if(!(mouse_b & 1) && *clic == 1) //Permet la réinitialisation de la capacité de cliquer
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
Sous-programme de validation / annulation de la défausse
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
            *clic = 1; //Evite la répétition successive de la condition de clic
            return 2; //Indication que la défausse peut se faire
        }
        else if(!(mouse_b & 1) && *clic == 1) //Permet la réinitialisation de la capacité de cliquer
        {
            *clic = 0;
        }
    }
    else //La souris est ailleurs sur l'écran
    {
        textprintf_centre_ex(doubleBuffer, policeTexte, 1312, 370, makecol(255, 255, 255), -1, "OUI"); //Le texte s'affiche en blanc
    }

    if(mouse_x>=1290 && mouse_x<=1335 && mouse_y>=408 && mouse_y<=428) //La souris passe sur le texte "NON"
    {
        textprintf_centre_ex(doubleBuffer, policeTexte, 1312, 410, makecol(255, 255, 0), -1, "NON"); //Affichage du texte en jaune
        if(mouse_b & 1 && *clic == 0) //clic sur "NON"
        {
            *clic = 1; //Evite la répétition successive de la condition de clic
            return 3; //Indication de la réinitialisation du processus de défausse
        }
        else if(!(mouse_b & 1) && *clic == 1) //Permet la réinitialisation de la capacité de cliquer
        {
            *clic = 0;
        }
    }
    else //La souris est ailleurs sur l'écran
    {
        textprintf_centre_ex(doubleBuffer, policeTexte, 1312, 410, makecol(255, 255, 255), -1, "NON"); //Le texte s'affiche en blanc
    }

    return 1;
}
