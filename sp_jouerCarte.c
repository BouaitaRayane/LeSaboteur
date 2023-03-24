#include "header.h"

/*
Sous-programme permettant au joueur actuel de jouer une de ses cartes
*/
int jouerCarte(t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, t_pioche *pioche, BITMAP *doubleBuffer, BITMAP *plateauJeu, BITMAP *vignetteMacareux, t_defausse *defausse, BITMAP *carteDos, BITMAP *tabBIT[BIT], FONT *tabPolices[POLICES], t_carteTerrain *plateau[7][5], BITMAP *tabCartesArrivee[2][3], int *devoilementCaseArrivee, int *finPartie, int *ENDgame)
{
    int i; //Permet l'affichage des joueurs autres
    int done = 0; //Permet le bon fonctionnement de la boucle de d�roulement
    int decalageAutreJoueur = 0; //Permet le d�calage vertical de l'affichage des profils des autres joueurs
    int phaseSortie = 5; //Permet de conna�tre l'�tat du tour en sortie du sous-programme
    int clic = 0; //Permet d'�viter la r�p�tition successive des conditions de clic
    int diffX, diffY; //Permet la sauvegarde du d�calage en X et en Y entre la souris et la carte s�lectionn�e afin de pouvoir la r�afficher avec un d�calage constant
    int carteSaisie = -1; //Permet d'indiquer si une carte est prise ou non et, si oui, laquelle
    int carteMalPlacee = 0; //Permet de d�terminer si une carte est mal positionn�e ou non
    int caseX, caseY; //Permet de r�cup�rer les coordonn�es de la case vis� lors du placement d'une carte Terrain / R�chauffement climatique
    int reverseCarte = 0; //Permet de g�rer la fonctionnalit� de reverse des cartes Terrain Td et Th
    int indicReverse = 0; //Permet d'appliquer le reverse des cartse Terrain Td et Th
    int message = 0, alphaMessage = -255; //Permettent de g�rer l'apparition des messages d'erreur et leur opacit�
    int dirRechauffementClimatique = 0; //Permet de stocker l'information de la direction point�e par une carte r�chauffement climatique

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
        textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 200, makecol(255, 255, 0), -1, "Saisissez une carte");
        textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 225, makecol(255, 255, 0), -1, "et jouez la");

        //Affichage du profil du joueur actuel
        affichageProfilJoueurActuel(tabJoueurs[joueurActuel], doubleBuffer, vignetteMacareux, tabPolices[6], tabPolices[5]);

        if(carteSaisie == -1) //Aucune carte n'a encore �t� s�lectionn�e, attente d'une s�lection
        {
            carteSaisie = carteSousSouris(tabJoueurs[joueurActuel]->mainJoueur, &diffX, &diffY, &clic); //Test de s�lection d'une carte
            reverseCarte = 0; //R�initialisation du reverse
        }


        /////////////////////////
        // LA CARTE EST SAISIE //
        /////////////////////////

        if(carteSaisie != -1 && carteMalPlacee == 0) //Une carte a �t� s�lectionn�e
        {
            deplacementCarte(diffX, diffY, tabJoueurs[joueurActuel]->mainJoueur[carteSaisie], &reverseCarte, &indicReverse); //La carte se d�place selon la souris du joueur

            //Processus d'illumination de la case du terrain vis�e
            if(mouse_x>=444 && mouse_x<=1039 && mouse_y>=174 && mouse_y<=750)
            {
                recupCasePlateau(mouse_x, mouse_y, &caseX, &caseY); //R�cup�ration des coordonn�es de la case du plateau vis�e

                if(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAT == 'a') //Il s'agit d'une carte Action
                {
                    if(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAction->action == 'l') //Il s'agit du r�chauffement climatique (seule carte action ayant un impact sur le plateau)
                    {
                        illuminationRechauffementClimatique(&dirRechauffementClimatique, caseX, caseY, doubleBuffer, tabPolices[6]); //Illumination adapt�e aux caract�ristiques du r�chauffement climatique
                    }
                }

                if(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAT == 't') //Il s'agit d'une carte Terrain
                {
                    rectfill(doubleBuffer, 444 + caseX*85, 174 + caseY*115, 444 + (caseX+1)*85, 174 + (caseY+1)*115, makecol(255, 255, 0)); //Rectangle jaune sur la case vis�e

                    if(strcmp(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeTerrain->typeJonction, "Td") == 0 || strcmp(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeTerrain->typeJonction, "Th") == 0) //Si la carte s�lectionn�e est un T Droite ou un T Haut
                    {
                        //Indication de la possibilit� de reverse le sens de la carte
                        textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 400, makecol(255, 255, 255), -1, "Appuyez sur R");
                        textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 425, makecol(255, 255, 255), -1, "pour retourner la carte");
                    }
                }
            }

            illuminationJoueurCible(nbreJoueurs, doubleBuffer); //Possible illumination du profil d'un joueur adverse

            if(mouse_b & 1) //Clic gauche, annulation du placement de la carte
            {
                carteMalPlacee = 1; //Indication que la carte est "mal plac�e" afin qu'elle retourn � sa place d'origine
                clic = 0; //R�initialisation de la capacit� de clic du joueur
                message = 9; //Indication que l'annulation a eu lieu
            }
            else if(!(mouse_b & 2) && clic == 1) //Le joueur rel�che / pose la carte jusque l� s�lectionn�e
            {

                //////////////////////////////////////////////
                // DIFFERENTS PLACEMENTS DE CARTE POSSIBLES //
                //////////////////////////////////////////////

                clic = 0; //R�initialisation de la capacit� de cliquer
                if(mouse_x>=0 && mouse_x<=360 && mouse_y>=348 && mouse_y<=348+(nbreJoueurs-1)*130) //Il s'agit d'une p�nalit� / r�paration � appliquer � un joueur
                {
                    carteMalPlacee = distribPenaliteReparation(tabJoueurs, nbreJoueurs, joueurActuel, tabJoueurs[joueurActuel]->mainJoueur[carteSaisie], tabBIT, &message); //Distribution de la p�nalit� / r�paration
                    if(carteMalPlacee == 0) //Bon placement de la carte, suppression de la carte de la main et remplacement par une carte de la pioche
                    {
                        defausse->imageDefausse = tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAction->visuelAvant; //Actualisation de la d�fausse
                        defausse->etat = 1; //Indication que la d�fausse n'est plus vide

                        if(pioche->indiceSommet > 0) //La pioche n'est pas encore vide
                        {
                            (pioche->indiceSommet)--; //D�cr�mentation de l'indice sommet de la pioche
                            (pioche->indiceNbPlaces)++; //Incr�mentation du nombre de place de la pioche

                            //R�cup�ration de la nouvelle carte depuis la pioche
                            tabJoueurs[joueurActuel]->mainJoueur[carteSaisie] = pioche->deck[pioche->indiceSommet];
                            free(pioche->deck[pioche->indiceSommet]);
                            pioche->deck[pioche->indiceSommet] = NULL;
                        }
                        else //La pioche est vide, processus de END game
                        {
                            *ENDgame = 1; //Indication que le END game est en cours
                            tabJoueurs[joueurActuel]->mainJoueur[carteSaisie] = NULL; //Suppression de la carte de la main du joueur
                        }

                        done = 1; //Indication que le joueur a jou� et qu'on peut quitter cette boucle de d�roulement
                        phaseSortie = 6; //Indication qu'on passe au menu pr� joueur suivant en sortie de ce sous-programme
                    }
                    else //La carte s�lectionn�e n'est pas une carte appropri�e
                    {
                        if(message == 0)
                        {
                            message = 3;
                        }
                    }
                }
                else if(mouse_x>=0 && mouse_x<=360 && mouse_y<348 && mouse_y>=0) //Il s'agit d'une r�paration de p�nalit� personnelle
                {
                    if(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAT == 'a' && tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAction->effet == 'r') //Carte action est de type r�paration
                    {
                        carteMalPlacee = verifReparation(tabJoueurs[joueurActuel], tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAction, tabBIT, &message); //Distribution de la r�paration
                        if(carteMalPlacee == 0) //Bon placement de la carte, suppression de la carte de la main et remplacement par une carte de la pioche
                        {
                            defausse->imageDefausse = tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAction->visuelAvant; //Actualisation de la d�fausse
                            defausse->etat = 1; //Indication que la d�fausse n'est plus vide

                            if(pioche->indiceSommet > 0) //La pioche n'est pas encore vide
                            {
                                (pioche->indiceSommet)--; //D�cr�mentation de l'indice sommet de la pioche
                                (pioche->indiceNbPlaces)++; //Incr�mentation du nombre de place de la pioche

                                //Processus de pioche d'une nouvelle carte
                                tabJoueurs[joueurActuel]->mainJoueur[carteSaisie] = pioche->deck[pioche->indiceSommet];
                                free(pioche->deck[pioche->indiceSommet]);
                                pioche->deck[pioche->indiceSommet] = NULL;
                            }
                            else //La pioche est vide, processus de END game
                            {
                                *ENDgame = 1; //Indication que le END game est en cours
                                tabJoueurs[joueurActuel]->mainJoueur[carteSaisie] = NULL; //Suppression de la carte de la main du joueur
                            }

                            done = 1; //Indication que le joueur a jou� et qu'on peut quitter cette boucle de d�roulement
                            phaseSortie = 6; //Indication qu'on passe au menu pr� joueur suivant en sortie de ce sous-programme
                        }
                    }
                    else //La carte s�lectionn�e n'est pas une carte r�paration, �chec du processus
                    {
                        message = 1;
                        carteMalPlacee = 1;
                    }
                }
                else if(mouse_x>=444 && mouse_x<=1039 && mouse_y>=174 && mouse_y<=750) //Il s'agit du placement d'une carte Terrain
                {
                    if(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAT == 't') //La carte s�lectionn�e est de type Terrain
                    {
                        if(possessionPenalite(tabJoueurs[joueurActuel]->penalite) == 0) //Le joueur ne poss�de pas de p�nalit�
                        {
                            recupCasePlateau(mouse_x, mouse_y, &caseX, &caseY); //R�cup�ration des coordonn�es de la case vis�e

                            carteMalPlacee = placementCarte(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeTerrain, plateau, caseX, caseY, reverseCarte, tabBIT); //Placement ou non de la carte sur le plateau de jeu

                            if(carteMalPlacee == 0) //Bon placement de la carte, suppression de la carte de la main et remplacement par une carte de la pioche
                            {
                                *finPartie = testFinPartie(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeTerrain, caseX, caseY, reverseCarte); //V�rification du placement de la carte et d�duction si fin de partie
                                if(*finPartie == 1) //Il s'agit de la fin de la partie
                                {
                                    calculScoresFinPartie(tabJoueurs, nbreJoueurs, joueurActuel, caseX, caseY, tabCartesArrivee, tabBIT); //Calcul du score en fin de partie
                                    sauvegardeScores(tabJoueurs, nbreJoueurs); //Sauvegarde des scores dans le fichier d�di�

                                    done = 1; //Indication que le joueur a jou� et qu'on peut quitter cette boucle de d�roulement
                                    phaseSortie = 6; //Indication qu'on passe au menu pr� joueur suivant en sortie de ce sous-programme
                                }
                                else if(pioche->indiceSommet > 0) //Si la pioche n'est pas vide
                                {
                                    (pioche->indiceSommet)--; //D�cr�mentation de l'indice sommet de la pioche
                                    (pioche->indiceNbPlaces)++; //Incr�mentation du nombre de place de la pioche

                                    //R�cup�ration de la carte au sommet de la pioche
                                    tabJoueurs[joueurActuel]->mainJoueur[carteSaisie] = pioche->deck[pioche->indiceSommet];
                                    free(pioche->deck[pioche->indiceSommet]);
                                    pioche->deck[pioche->indiceSommet] = NULL;

                                    done = 1; //Indication que le joueur a jou� et qu'on peut quitter cette boucle de d�roulement
                                    phaseSortie = 6; //Indication qu'on passe au menu pr� joueur suivant en sortie de ce sous-programme
                                }
                                else if(pioche->indiceSommet == 0) //La pioche est vide, processus de END game
                                {
                                    *ENDgame = 1; //Indication que le END game est en cours
                                    tabJoueurs[joueurActuel]->mainJoueur[carteSaisie] = NULL; //Suppression de la carte de la main du joueur
                                    done = 1; //Indication que le joueur a jou� et qu'on peut quitter cette boucle de d�roulement
                                    phaseSortie = 6; //Indication qu'on passe au menu pr� joueur suivant en sortie de ce sous-programme
                                }
                            }
                            else
                            {
                                message = 5;
                            }
                        }
                        else //Le joueur poss�de une p�nalit�, il ne peut pas jouer de carte terrain
                        {
                            carteMalPlacee = 1; //Indication que la carte est � remettre dans la main du joueur
                            message = 2;
                        }
                    }
                    else if(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAT == 'a' && tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAction->action == 'l') //La carte s�lectionn�e est le r�chauffement climatique, seule carte action pouvant �tre pos�e sur le terrain
                    {
                        if(possessionPenalite(tabJoueurs[joueurActuel]->penalite) == 0) //Le joueur ne poss�de pas de p�nalit�
                        {
                            recupCasePlateau(mouse_x, mouse_y, &caseX, &caseY); //R�cup�ration des coordonn�es de la case vis�e

                            carteMalPlacee = testRechauffementClimatique(caseX, caseY, dirRechauffementClimatique, plateau); //V�rification de la validit� du placement du r�chauffement climatique

                            if(carteMalPlacee == 0) //La carte est bien plac�e
                            {
                                applicationRechauffementClimatique(caseX, caseY, dirRechauffementClimatique, plateau, defausse); //Application du r�chauffement climatique

                                if(pioche->indiceSommet > 0) //La pioche n'est pas encore vide
                                {
                                    (pioche->indiceSommet)--; //D�cr�mentation de l'indice sommet de la pioche
                                    (pioche->indiceNbPlaces)++; //Incr�mentation du nombre de place de la pioche

                                    //R�cup�ration de la nouvelle carte depuis la pioche
                                    tabJoueurs[joueurActuel]->mainJoueur[carteSaisie] = pioche->deck[pioche->indiceSommet];
                                    free(pioche->deck[pioche->indiceSommet]);
                                    pioche->deck[pioche->indiceSommet] = NULL;
                                }
                                else //La pioche est vide, processus de END game
                                {
                                    *ENDgame = 1; //Indication que le END game est en cours
                                    tabJoueurs[joueurActuel]->mainJoueur[carteSaisie] = NULL; //Suppression de la carte de la main du joueur
                                }

                                done = 1; //Indication que le joueur a jou� et qu'on peut quitter cette boucle de d�roulement
                                phaseSortie = 6; //Indication qu'on passe au menu pr� joueur suivant en sortie de ce sous-programme
                            }
                            else
                            {
                                message = 5;
                            }
                        }
                        else //Le joueur poss�de au moins une p�nalit�, il ne peut pas jouer cette carte
                        {
                            carteMalPlacee = 1; //Indication que la carte est mal plac�e
                            message = 2;
                        }
                    }
                    else //La carte s�lectionn�e n'est pas adapt�e au placement sur le plateau de jeu
                    {
                        carteMalPlacee = 1; //Indication que la carte est mal plac�e
                        message = 1;
                    }
                }

                else if(mouse_x>1039 && mouse_x<=1124) //Le joueur souhaite r�v�l� l'une des cases Arriv�e
                {
                    if(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAT == 'a' && tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAction->action == 'g') //La carte s�lectionn�e est la MAP
                    {
                        *devoilementCaseArrivee = testMap(mouse_x, mouse_y, tabCartesArrivee); //Test de la validit� de MAP

                        if(*devoilementCaseArrivee == -1) //Aucune case n'a �t� s�lectionn�e, le positionnement de la carte n'�tait pas bon
                        {
                            message = 5;
                            carteMalPlacee = 1; //Indication que la carte est mal plac�e
                        }
                        else
                        {
                            defausse->imageDefausse = tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAction->visuelAvant; //Actualisation de la d�fausse
                            defausse->etat = 1; //Indication que la d�fausse n'est plus vide

                            if(pioche->indiceSommet > 0) //La pioche n'est pas encore vide
                            {
                                (pioche->indiceSommet)--; //D�cr�mentation de l'indice sommet de la pioche
                                (pioche->indiceNbPlaces)++; //Incr�mentation du nombre de place de la pioche

                                //R�cup�ration de la nouvelle carte depuis la pioche
                                tabJoueurs[joueurActuel]->mainJoueur[carteSaisie] = pioche->deck[pioche->indiceSommet];
                                free(pioche->deck[pioche->indiceSommet]);
                                pioche->deck[pioche->indiceSommet] = NULL;
                            }
                            else //La pioche est vide, processus de END game
                            {
                                *ENDgame = 1; //Indication que le END game est en cours
                                tabJoueurs[joueurActuel]->mainJoueur[carteSaisie] = NULL; //Suppression de la carte de la main du joueur
                            }

                            done = 1; //Indication que le joueur a jou� et qu'on peut quitter cette boucle de d�roulement
                            phaseSortie = 6; //Indication qu'on passe au menu pr� joueur suivant en sortie de ce sous-programme
                        }
                    }
                    else
                    {
                        message = 6;
                        carteMalPlacee = 1; //Indication que la carte est mal positionn�e
                    }
                }
                else //La position choisie n'est pas valide
                {
                    carteMalPlacee = 1; //Indication que la carte a �t� mal positionn�e
                }
            }
        }

        //La carte a �t� mal positionn�e
        if(carteMalPlacee == 1)
        {
            retourCarte(tabJoueurs[joueurActuel]->mainJoueur); //Processus de retour de la carte vers son emplacement initial
            if(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->posX == tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->xInit && tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->posY == tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->yInit) //La carte est de retour � son emplacement initial
            {
                carteMalPlacee = 0; //Indication que la carte est de nouveau � sa place
                carteSaisie = -1; //Indication qu'une nouvelle carte peut �tre s�lectionn�e
            }
        }

        if(message != 0) //Un message doit �tre affich�
        {
            messageErreur(message, &alphaMessage, doubleBuffer, tabPolices[6], tabPolices[6]); //Affichage du message qui se dissipe progressivement
            if(alphaMessage >= 255) //Le message a totalement disparu
            {
                message = 0; //R�initialisation de la fonctionnalit� de message
                alphaMessage = -255; //R�initilisation de la fonctionnalit� d'effacement progressif du message
            }
        }

        //Processus d'affichage des informations des autres joueurs dans l'ordre de passage
        i = (joueurActuel+1)%nbreJoueurs;
        while(i != joueurActuel) //Boucle permettant l'affichage des joueurs dans l'ordre de passage
        {
            affichageProfilAutresJoueurs(tabJoueurs[i], doubleBuffer, tabPolices[6], tabPolices[7], decalageAutreJoueur); //Affichage des profils
            decalageAutreJoueur++; //D�calage du profil suivant vers le bas

            i = (i+1)%nbreJoueurs; //Passage au joueur suivant
        }

        //Affichage des cartes du joueur actuel avec possibilit� de reverse
        affichageCartesJoueurActuelReverse(tabJoueurs[joueurActuel], carteSaisie, reverseCarte, doubleBuffer, tabBIT[69], tabBIT[70]);

        if(mouse_x>=1282 && mouse_x<=1340 && mouse_y>=597 && mouse_y<=623 && carteMalPlacee == 0) //La souris passe sur "RETOUR" et aucune carte est en situation de retour � sa position initiale
        {
            textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 600, makecol(255, 0, 0), -1, "Retour"); //Affichage du texte en rouge
            if(mouse_b & 1 && clic == 0) //Clic sur "RETOUR"
            {
                clic = 1; //Evite la r�p�tition de la condition en cas de maintien du clic gauche

                phaseSortie = 1; //Retour au menu de s�lection de l'action
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

    return phaseSortie;
}


/*
Sous-programme d�terminant si la carte est, oui ou non, sous la souris
*/
int carteSousSouris(t_carte* mainJoueur[6], int *diffX, int *diffY, int *clic)
{
    int i; //Permet le bon fonctionnement du parcours de la main du joueur actuel

    for(i=0 ; i<6 ; i++) //Boucle de parcours de la main du joueur actuel
    {
        if(mainJoueur[i] != NULL) //La carte existe
        {
            if(mainJoueur[i]->typeAT == 'a') //La carte est de type Action
            {
                if(mouse_x>=mainJoueur[i]->posX && mouse_x<=mainJoueur[i]->posX+mainJoueur[i]->typeAction->visuelAvant->w && mouse_y>=mainJoueur[i]->posY && mouse_y<=mainJoueur[i]->posY+mainJoueur[i]->typeAction->visuelAvant->h) //La souris est sur la carte
                {
                    if(mouse_b & 2 && *clic == 0) //Clic sur la carte
                    {
                        *diffX = mouse_x - mainJoueur[i]->posX; //Position en X de la carte par rapport � la souris
                        *diffY = mouse_y - mainJoueur[i]->posY; //Position en Y de la carte par rapport � la souris

                        *clic = 1; //Evite la r�p�tition successive de la condition

                        return i; //Retour de l'indice de la carte s�lectionn�e
                    }
                }
            }
            else if(mainJoueur[i]->typeAT == 't') //La carte est de type Terrain
            {
                if(mouse_x>=mainJoueur[i]->posX && mouse_x<=mainJoueur[i]->posX+mainJoueur[i]->typeTerrain->visuelAvant->w && mouse_y>=mainJoueur[i]->posY && mouse_y<=mainJoueur[i]->posY+mainJoueur[i]->typeTerrain->visuelAvant->h) //La souris est sur la carte
                {
                    if(mouse_b & 2 && *clic == 0) //Clic sur la carte
                    {
                        *diffX = mouse_x - mainJoueur[i]->posX; //Position en X de la carte par rapport � la souris
                        *diffY = mouse_y - mainJoueur[i]->posY; //Position en Y de la carte par rapport � la souris

                        *clic = 1; //Evite la r�p�tition successive de la condition

                        return i; //Retour de l'indice de la carte s�lectionn�e
                    }
                }
            }
        }
    }

    return -1; //Permet d'indiquer qu'aucune carte n'a �t� s�lectionn�e
}


/*
Sous-programme d'actualisation de la position de la carte en fonction de la souris
*/
void deplacementCarte(int diffX, int diffY, t_carte *carte, int *reverseCarte, int *indicReverse)
{
    carte->posX = mouse_x - diffX; //Actualisation de l'emplacement en X
    carte->posY = mouse_y - diffY; //Actualisation de l'emplacement en Y

    //Processus d'inversion du sens de la carte pour les croisements HAUT et DROITE
    if(carte->typeAT == 't' && (strcmp(carte->typeTerrain->typeJonction, "Td") == 0 || strcmp(carte->typeTerrain->typeJonction, "Th") == 0)) //La carte s�lectionn�e est un croisement HAUT ou DROITE
    {
        if(key[KEY_R] && *indicReverse == 0) //Le joueur appuie sur R
        {
            *indicReverse = 1; //Evite la r�p�tition successive de la condition

            //Inversion de la valeur de reverseCarte
            if(*reverseCarte == 0) //Passe de 0 � 1
            {
                *reverseCarte = 1;
            }
            else //Passe de 1 � 0
            {
                *reverseCarte = 0;
            }
        }
        else if(!(key[KEY_R]) && *indicReverse == 1) //Le joueur n'appuie pas sur R, r�initialisation de la capacit� de reverse la carte
        {
            *indicReverse = 0;
        }
    }
}


/*
Sous-programme permettant l'illumination du profil du joueur vis� par une p�nalit�
*/
void illuminationJoueurCible(int nbreJoueurs, BITMAP *doubleBuffer)
{
    if(nbreJoueurs == 2) //2 joueurs dans la partie
    {
        if(mouse_x>=0 && mouse_x<=360 && mouse_y>=348 && mouse_y<=482) //La souris passe au dessus du profil de l'autre joueur
        {
            rectfill(doubleBuffer, 0, 348, 358, 482, makecol(75, 150, 150)); //Illumination du profil
        }
    }

    //M�me logique pour les autres quantit�s de joueurs

    else if(nbreJoueurs == 3)
    {
        if(mouse_x>=0 && mouse_x<=360 && mouse_y>=348 && mouse_y<=482)
        {
            rectfill(doubleBuffer, 0, 348, 358, 482, makecol(75, 150, 150));
        }
        else if(mouse_x>=0 && mouse_x<=360 && mouse_y>482 && mouse_y<=616)
        {
            rectfill(doubleBuffer, 0, 482, 358, 616, makecol(75, 150, 150));
        }
    }
    else if(nbreJoueurs == 4)
    {
        if(mouse_x>=0 && mouse_x<=360 && mouse_y>=348 && mouse_y<=482)
        {
            rectfill(doubleBuffer, 0, 348, 358, 482, makecol(75, 150, 150));
        }
        else if(mouse_x>=0 && mouse_x<=360 && mouse_y>482 && mouse_y<=616)
        {
            rectfill(doubleBuffer, 0, 482, 358, 616, makecol(75, 150, 150));
        }
        else if(mouse_x>=0 && mouse_x<=360 && mouse_y>616 && mouse_y<=750)
        {
            rectfill(doubleBuffer, 0, 616, 358, 750, makecol(75, 150, 150));
        }
    }
}


/*
Sous-programme d'application de la p�nalit� / r�paration � un autre joueur
*/
int distribPenaliteReparation(t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, t_carte *carteSaisie, BITMAP *tabBIT[BIT], int *message)
{
    int joueurVise; //Permet de d�terminer le joueur vis� par la r�paration / p�nalit�
    int valide = 1; //Permet de d�terminer si le placement est valide (0) ou non (1)

    if(nbreJoueurs == 2) //2 joueurs dans la partie
    {
        if(mouse_x>=0 && mouse_x<=360 && mouse_y>=348 && mouse_y<=482) //La souris passe au dessus du profil de l'autre joueur
        {
            if(carteSaisie->typeAT == 'a') //Il s'agit d'une carte action
            {
                joueurVise = (joueurActuel + 1)%nbreJoueurs; //D�termination du joueur vis�

                if(carteSaisie->typeAction->effet == 'p') //Il s'agit d'une carte p�nalit�
                {
                    valide = verifPenalite(tabJoueurs[joueurVise], carteSaisie->typeAction, tabBIT, message); //Processus de test et d'application de la p�nalit� au joueur vis�
                }
                else if(carteSaisie->typeAction->effet == 'r') //Il s'agit d'une carte r�paration
                {
                    valide = verifReparation(tabJoueurs[joueurVise], carteSaisie->typeAction, tabBIT, message); //Processus de test et d'application de la r�paration au joueur vis�
                }
            }
            else //La carte n'est pas une carte Action
            {
                *message = 1;
            }
        }
    }

    //M�me logique pour les autres quantit�s de joueurs


    else if(nbreJoueurs == 3)
    {
        if(mouse_x>=0 && mouse_x<=360 && mouse_y>=348 && mouse_y<=482)
        {
            if(carteSaisie->typeAT == 'a')
            {
                joueurVise = (joueurActuel + 1)%nbreJoueurs;

                if(carteSaisie->typeAction->effet == 'p')
                {
                    valide = verifPenalite(tabJoueurs[joueurVise], carteSaisie->typeAction, tabBIT, message);
                }
                else if(carteSaisie->typeAction->effet == 'r')
                {
                    valide = verifReparation(tabJoueurs[joueurVise], carteSaisie->typeAction, tabBIT, message);
                }
            }
            else //La carte n'est pas une carte Action
            {
                *message = 1;
            }
        }
        else if(mouse_x>=0 && mouse_x<=360 && mouse_y>482 && mouse_y<=616)
        {
            if(carteSaisie->typeAT == 'a')
            {
                joueurVise = (joueurActuel + 2)%nbreJoueurs;

                if(carteSaisie->typeAction->effet == 'p')
                {
                    valide = verifPenalite(tabJoueurs[joueurVise], carteSaisie->typeAction, tabBIT, message);
                }
                else if(carteSaisie->typeAction->effet == 'r')
                {
                    valide = verifReparation(tabJoueurs[joueurVise], carteSaisie->typeAction, tabBIT, message);
                }
            }
            else //La carte n'est pas une carte Action
            {
                *message = 1;
            }
        }
    }
    else if(nbreJoueurs == 4)
    {
        if(mouse_x>=0 && mouse_x<=360 && mouse_y>=348 && mouse_y<=482)
        {
            if(carteSaisie->typeAT == 'a')
            {
                joueurVise = (joueurActuel + 1)%nbreJoueurs;

                if(carteSaisie->typeAction->effet == 'p')
                {
                    valide = verifPenalite(tabJoueurs[joueurVise], carteSaisie->typeAction, tabBIT, message);
                }
                else if(carteSaisie->typeAction->effet == 'r')
                {
                    valide = verifReparation(tabJoueurs[joueurVise], carteSaisie->typeAction, tabBIT, message);
                }
            }
            else //La carte n'est pas une carte Action
            {
                *message = 1;
            }
        }
        else if(mouse_x>=0 && mouse_x<=360 && mouse_y>482 && mouse_y<=616)
        {
            if(carteSaisie->typeAT == 'a')
            {
                joueurVise = (joueurActuel + 2)%nbreJoueurs;

                if(carteSaisie->typeAction->effet == 'p')
                {
                    valide = verifPenalite(tabJoueurs[joueurVise], carteSaisie->typeAction, tabBIT, message);
                }
                else if(carteSaisie->typeAction->effet == 'r')
                {
                    valide = verifReparation(tabJoueurs[joueurVise], carteSaisie->typeAction, tabBIT, message);
                }
            }
            else //La carte n'est pas une carte Action
            {
                *message = 1;
            }
        }
        else if(mouse_x>=0 && mouse_x<=360 && mouse_y>616 && mouse_y<=750)
        {
            if(carteSaisie->typeAT == 'a')
            {
                joueurVise = (joueurActuel + 3)%nbreJoueurs;

                if(carteSaisie->typeAction->effet == 'p')
                {
                    valide = verifPenalite(tabJoueurs[joueurVise], carteSaisie->typeAction, tabBIT, message);
                }
                else if(carteSaisie->typeAction->effet == 'r')
                {
                    valide = verifReparation(tabJoueurs[joueurVise], carteSaisie->typeAction, tabBIT, message);
                }
            }
            else //La carte n'est pas une carte Action
            {
                *message = 1;
            }
        }
    }


    return valide;
}


/*
Sous-programme de v�rification si la carte p�nalit� peut �tre donn� au joueur vis�
*/
int verifPenalite(t_joueur *joueurVise, t_carteAction *cartePenalite, BITMAP *tabBIT[BIT], int *message)
{
    int i; //Permet le bon fonctionnement de la boucle de parcours des p�nalit�s du joueur vis�
    int existence = 0; //Permet de d�terminer si le joueur vis� poss�de d�j� la p�nalit� distribu�e
    int ajout = 0; //Permet de d�terminer l'ajout de la p�nalit� lorsque celle-ci n'est pas d�j� pr�sente chez le joueur vis�
    int valide = 1; //Permet de d�terminer si, au final, la p�nalit� donn�e peut �tre administr�e ou non

    if(cartePenalite->action == 'h') //Tra�neau cass�
    {
        for(i=0 ; i<3 ; i++) //Boucle de recherche de cette p�nalit� dans les p�nalit�s du joueur vis�
        {
            if(joueurVise->penalite[i] != NULL) //Une p�nalit� existe
            {
                if(existence == 0) //Pour l'instant, la m�me p�nalit� n'a pas �t� trouv�e
                {
                    if(joueurVise->penalite[i] == tabBIT[32]) //La p�nalit� est d�j� pr�sente dans les p�nalit�s du joueur vis�
                    {
                        existence = 1; //Indication que la p�nalit� est d�j� pr�sente, le programme va finalement retourner valide = 1, car ce n'est pas possible
                        *message = 3;
                    }
                }
            }
        }
        if(existence == 0) //Apr�s parcours, la p�nalit� n'a pas �t� trouv�e, il faut donc la rajouter
        {
            for(i=0 ; i<3 ; i++) //Boucle de parcours des p�nalit�s en qu�te de case vide
            {
                if(ajout == 0) //Processus s'effectuant tant que la nouvelle p�nalit� n'a pas �t� ajout�e
                {
                    if(joueurVise->penalite[i] == NULL) //Emplacement libre pour la nouvelle p�nalit�
                    {
                        joueurVise->penalite[i] = tabBIT[32]; //R�cup�ration de la nouvelle p�nalit�
                        ajout = 1; //Indication que la nouvelle p�nalit� a �t� ajout�e
                        valide = 0; //Indication que la situation est correcte
                    }
                }
            }
        }
    }

    //M�me processus pour les autres p�nalit�s

    else if(cartePenalite->action == 'i') //Canne � p�che cass�e
    {
        for(i=0 ; i<3 ; i++)
        {
            if(joueurVise->penalite[i] != NULL)
            {
                if(existence == 0)
                {
                    if(joueurVise->penalite[i] == tabBIT[28])
                    {
                        existence = 1;
                        *message = 3;
                    }
                }
            }
        }
        if(existence == 0)
        {
            for(i=0 ; i<3 ; i++)
            {
                if(ajout == 0)
                {
                    if(joueurVise->penalite[i] == NULL)
                    {
                        joueurVise->penalite[i] = tabBIT[28];
                        ajout = 1;
                        valide = 0;
                    }
                }
            }
        }
    }

    else if(cartePenalite->action == 'j') //Scie � glace cass�e
    {
        for(i=0 ; i<3 ; i++)
        {
            if(joueurVise->penalite[i] != NULL)
            {
                if(existence == 0)
                {
                    if(joueurVise->penalite[i] == tabBIT[30])
                    {
                        existence = 1;
                        *message = 3;
                    }
                }
            }
        }
        if(existence == 0)
        {
            for(i=0 ; i<3 ; i++)
            {
                if(ajout == 0)
                {
                    if(joueurVise->penalite[i] == NULL)
                    {
                        joueurVise->penalite[i] = tabBIT[30];
                        ajout = 1;
                        valide = 0;
                    }
                }
            }
        }
    }

    else if(cartePenalite->action == 'k') //Tous les outils sont cass�s
    {
        for(i=0 ; i<3 ; i++) //Boucle de parcours des p�nalit�s du joueur vis�
        {
            if(joueurVise->penalite[i] != NULL) //Le joueur poss�de d�j� une des p�nalit�s existantes
            {
                existence = 1; //Indication qu'il poss�de d�j� une p�nalit�
                *message = 3;
            }
        }
        if(existence == 0) //Le joueur vis� ne poss�de pas de p�nalit�
        {
            //Ajout des trois p�nalit�s possibles dans le tableau des p�nalit�s du joueur vis�
            joueurVise->penalite[0] = tabBIT[28];
            joueurVise->penalite[1] = tabBIT[30];
            joueurVise->penalite[2] = tabBIT[32];
            valide = 0; //Indication que la situation est possible
        }
    }

    return valide;
}


/*
Sous-programme de v�rification si la carte r�paration peut �tre donn�e au joueur vis�
*/
int verifReparation(t_joueur *joueurVise, t_carteAction *cartePenalite, BITMAP *tabBIT[BIT], int *message)
{
    int i; //Permet le bon fonctionnement de la boucle de parcours des p�nalit�s du joueur vis�
    int existence1 = 0; //Permet de d�terminer si le joueur vis� poss�de bien la p�nalit� � r�parer
    int existence2 = 0;
    int valide = 1; //Permet de d�terminer si, au final, la p�nalit� donn�e peut �tre administr�e ou non

    if(cartePenalite->action == 'a') //Tra�neau r�par�
    {
        for(i=0 ; i<3 ; i++) //Boucle de recherche de la p�nalit� en question
        {
            if(joueurVise->penalite[i] != NULL) //Une p�nalit� existe
            {
                if(existence1 == 0) //Pour l'instant, la p�nbalit� n'a pas �t� trouv�e
                {
                    if(joueurVise->penalite[i] == tabBIT[32]) //La p�nalit� est pr�sente dans les p�nalit�s du joueur vis�
                    {
                        existence1 = 1; //Indication que la p�nalit� est pr�sente
                        joueurVise->penalite[i] = NULL; //Suppression de la p�nalit�
                        valide = 0; //Indication que la situation est possible
                    }
                }
            }
        }
        if(existence1 == 0)
        {
            *message = 4;
        }
    }

    //M�me logique pour les cartes simples

    else if(cartePenalite->action == 'b') //Canne � p�che r�par�e
    {
        for(i=0 ; i<3 ; i++)
        {
            if(joueurVise->penalite[i] != NULL)
            {
                if(existence1 == 0)
                {
                    if(joueurVise->penalite[i] == tabBIT[28])
                    {
                        existence1 = 1;
                        joueurVise->penalite[i] = NULL;
                        valide = 0;
                    }
                }
            }
        }
        if(existence1 == 0)
        {
            *message = 4;
        }
    }

    else if(cartePenalite->action == 'c') //Scie � glace r�par�e
    {
        for(i=0 ; i<3 ; i++)
        {
            if(joueurVise->penalite[i] != NULL)
            {
                if(existence1 == 0)
                {
                    if(joueurVise->penalite[i] == tabBIT[30])
                    {
                        existence1 = 1;
                        joueurVise->penalite[i] = NULL;
                        valide = 0;
                    }
                }
            }
        }
        if(existence1 == 0)
        {
            *message = 4;
        }
    }

    else if(cartePenalite->action == 'd') //Tra�neau en bois et canne � p�che r�par�s
    {
        for(i=0 ; i<3 ; i++) //Parcours des p�nalit�s pour voir si elles y sont
        {
            if(joueurVise->penalite[i] != NULL) //Si la p�nalit� existe
            {
                if(joueurVise->penalite[i] == tabBIT[32]) //Il s'agit d'une des p�nalit�s recherch�es
                {
                    joueurVise->penalite[i] = NULL; //Suppression de la p�nalit�
                    existence1 = 1; //Indication que la premi�re p�nalit� recherch�e a �t� trouv�e
                }
                if(joueurVise->penalite[i] == tabBIT[28]) //Il s'agit de l'autre p�nalit� recherch�e
                {
                    joueurVise->penalite[i] = NULL; //Suppression de la p�nalit�
                    existence2 = 1; //Indication que la deuxi�me p�nalit� recherch�e a �t� trouv�e
                }
            }
        }
        if(existence1 == 1 || existence2 == 1) //Les deux p�nalit�s ont �t� trouv�es
        {
            valide = 0; //Indication que la situation est valide
        }
        else if(existence1 == 0 && existence2 == 0)
        {
            *message = 4;
        }
    }

    //M�me raisonnement pour les autres cartes doubles

    else if(cartePenalite->action == 'e') //Canne � p�che et scie � glace r�par�es
    {
        for(i=0 ; i<3 ; i++)
        {
            if(joueurVise->penalite[i] != NULL)
            {
                if(joueurVise->penalite[i] == tabBIT[28])
                {
                    joueurVise->penalite[i] = NULL;
                    existence1 = 1;
                }
                if(joueurVise->penalite[i] == tabBIT[30])
                {
                    joueurVise->penalite[i] = NULL;
                    existence2 = 1;
                }
            }
        }
        if(existence1 == 1 || existence2 == 1)
        {
            valide = 0;
        }
        else if(existence1 == 0 && existence2 == 0)
        {
            *message = 4;
        }
    }

    else if(cartePenalite->action == 'f') //Scie � glace et tra�neau en bois r�par�s
    {
        for(i=0 ; i<3 ; i++)
        {
            if(joueurVise->penalite[i] != NULL)
            {
                if(joueurVise->penalite[i] == tabBIT[30])
                {
                    joueurVise->penalite[i] = NULL;
                    existence1 = 1;
                }
                if(joueurVise->penalite[i] == tabBIT[32])
                {
                    joueurVise->penalite[i] = NULL;
                    existence2 = 1;
                }
            }
        }
        if(existence1 == 1 || existence2 == 1)
        {
            valide = 0;
        }
        else if(existence1 == 0 && existence2 == 0)
        {
            *message = 4;
        }
    }

    return valide;
}


/*
Sous-programme v�rifiant si le joueur actuel poss�de au moins une p�nalit�
*/
int possessionPenalite(BITMAP *penalite[3])
{
    int i; //Permet le bon fonctionnement de la boucle de parcours du tableau de p�nalit� du joueur actuel
    int presence = 0; //Permet de d�terminer la pr�sence ou non d'une carte p�nalit� dans le tableau de p�nalit� d�di�

    for(i=0 ; i<3 ; i++) //Boucle de parcours du tableau de p�nalit�s
    {
        if(presence == 0) //Pour l'instant aucune p�nalit� d�tect�e
        {
            if(penalite[i] != NULL) //P�nalit� d�tect�e
            {
                presence = 1; //Indication que le joueur poss�de une carte p�nalit�
            }
        }
    }

    return presence;
}


/*
Sous-programme mettant en place le retour de la carte en cas de mauvais positionnement ou bien d'annulation du drag and drop (permet aussi la remise � leur place de l'ensemble des cartes en cas de probl�me plus g�n�ral)
*/
void retourCarte(t_carte* mainJoueur[6])
{
    int i; //Permet le bon fonctionnement de la boucle de parcours de la main du joueur actuel

    for(i=0 ; i<6 ; i++) //Boucle de parcours de la main du joueur actuel
    {
        if(mainJoueur[i] != NULL) //La carte existe
        {
            if((mainJoueur[i]->posX != mainJoueur[i]->xInit || mainJoueur[i]->posY != mainJoueur[i]->yInit) && !(mouse_b & 2)) //La carte n'est pas � sa position initiale et le joueur n'est pas en train de la bouger
            {
                if(mainJoueur[i]->posX > mainJoueur[i]->xInit) //La carte est trop � droite
                {
                    (mainJoueur[i]->posX)--; //D�calage vers la gauche
                }
                else if(mainJoueur[i]->posX < mainJoueur[i]->xInit) //La carte est trop � gauche
                {
                    (mainJoueur[i]->posX)++; //D�calage vers la droite
                }

                if(mainJoueur[i]->posY > mainJoueur[i]->yInit) //La carte est trop en bas
                {
                    (mainJoueur[i]->posY)--; //D�calage vers le haut
                }
                else if(mainJoueur[i]->posY < mainJoueur[i]->yInit) //La carte est trop en haut
                {
                    (mainJoueur[i]->posY)++; //D�calage vers le bas
                }
            }
        }
    }
}


/*
Sous-programme d'affichage des cartes du joueur actuel lorsque celui-ci saisit une carte
*/
void affichageCartesJoueurActuelReverse(t_joueur *joueurActuel, int carteSaisie, int carteReverse, BITMAP *doubleBuffer, BITMAP *banquiseThReverse, BITMAP *banquiseTdReverse)
{
    int j; //Permet le bon fonctionnement de la boucle de parcours de la main du joueur

    for(j=0 ; j<6 ; j++) //boucle de parcours de la min du joueur
    {
        if(joueurActuel->mainJoueur[j] != NULL) //La carte existe
        {
            if(joueurActuel->mainJoueur[j]->typeAT == 'a') //Cas d'une carte Action
            {
                blit(joueurActuel->mainJoueur[j]->typeAction->visuelAvant, doubleBuffer, 0, 0, joueurActuel->mainJoueur[j]->posX, joueurActuel->mainJoueur[j]->posY, joueurActuel->mainJoueur[j]->typeAction->visuelAvant->w,joueurActuel->mainJoueur[j]->typeAction->visuelAvant->h); //Affichage de la carte Action
            }
            else //Cas d'une carte Terrain
            {
                if(j == carteSaisie && carteReverse == 1) //Si il s'agit de la carte s�lectionn�e et que le reverse est enclench�
                {
                    if(strcmp(joueurActuel->mainJoueur[j]->typeTerrain->typeJonction, "Td") == 0) //La carte est un T Droite
                    {
                        draw_sprite(doubleBuffer, banquiseTdReverse, joueurActuel->mainJoueur[j]->posX, joueurActuel->mainJoueur[j]->posY); //Affichage de la version reverse du T Droite
                    }
                    else if(strcmp(joueurActuel->mainJoueur[j]->typeTerrain->typeJonction, "Th") == 0) //La carte est un T Haut
                    {
                        draw_sprite(doubleBuffer, banquiseThReverse, joueurActuel->mainJoueur[j]->posX, joueurActuel->mainJoueur[j]->posY); //Affichage de la version reverse du T Haut
                    }
                }
                else //La carte peut �tre affich�e normalement
                {
                    blit(joueurActuel->mainJoueur[j]->typeTerrain->visuelAvant, doubleBuffer, 0, 0, joueurActuel->mainJoueur[j]->posX, joueurActuel->mainJoueur[j]->posY, joueurActuel->mainJoueur[j]->typeTerrain->visuelAvant->w,joueurActuel->mainJoueur[j]->typeTerrain->visuelAvant->h); //Affichage de la carte Terrain
                }
            }
        }
    }
}
