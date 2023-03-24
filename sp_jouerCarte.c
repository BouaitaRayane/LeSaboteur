#include "header.h"

/*
Sous-programme permettant au joueur actuel de jouer une de ses cartes
*/
int jouerCarte(t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, t_pioche *pioche, BITMAP *doubleBuffer, BITMAP *plateauJeu, BITMAP *vignetteMacareux, t_defausse *defausse, BITMAP *carteDos, BITMAP *tabBIT[BIT], FONT *tabPolices[POLICES], t_carteTerrain *plateau[7][5], BITMAP *tabCartesArrivee[2][3], int *devoilementCaseArrivee, int *finPartie, int *ENDgame)
{
    int i; //Permet l'affichage des joueurs autres
    int done = 0; //Permet le bon fonctionnement de la boucle de déroulement
    int decalageAutreJoueur = 0; //Permet le décalage vertical de l'affichage des profils des autres joueurs
    int phaseSortie = 5; //Permet de connaître l'état du tour en sortie du sous-programme
    int clic = 0; //Permet d'éviter la répétition successive des conditions de clic
    int diffX, diffY; //Permet la sauvegarde du décalage en X et en Y entre la souris et la carte sélectionnée afin de pouvoir la réafficher avec un décalage constant
    int carteSaisie = -1; //Permet d'indiquer si une carte est prise ou non et, si oui, laquelle
    int carteMalPlacee = 0; //Permet de déterminer si une carte est mal positionnée ou non
    int caseX, caseY; //Permet de récupérer les coordonnées de la case visé lors du placement d'une carte Terrain / Réchauffement climatique
    int reverseCarte = 0; //Permet de gérer la fonctionnalité de reverse des cartes Terrain Td et Th
    int indicReverse = 0; //Permet d'appliquer le reverse des cartse Terrain Td et Th
    int message = 0, alphaMessage = -255; //Permettent de gérer l'apparition des messages d'erreur et leur opacité
    int dirRechauffementClimatique = 0; //Permet de stocker l'information de la direction pointée par une carte réchauffement climatique

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
        textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 200, makecol(255, 255, 0), -1, "Saisissez une carte");
        textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 225, makecol(255, 255, 0), -1, "et jouez la");

        //Affichage du profil du joueur actuel
        affichageProfilJoueurActuel(tabJoueurs[joueurActuel], doubleBuffer, vignetteMacareux, tabPolices[6], tabPolices[5]);

        if(carteSaisie == -1) //Aucune carte n'a encore été sélectionnée, attente d'une sélection
        {
            carteSaisie = carteSousSouris(tabJoueurs[joueurActuel]->mainJoueur, &diffX, &diffY, &clic); //Test de sélection d'une carte
            reverseCarte = 0; //Réinitialisation du reverse
        }


        /////////////////////////
        // LA CARTE EST SAISIE //
        /////////////////////////

        if(carteSaisie != -1 && carteMalPlacee == 0) //Une carte a été sélectionnée
        {
            deplacementCarte(diffX, diffY, tabJoueurs[joueurActuel]->mainJoueur[carteSaisie], &reverseCarte, &indicReverse); //La carte se déplace selon la souris du joueur

            //Processus d'illumination de la case du terrain visée
            if(mouse_x>=444 && mouse_x<=1039 && mouse_y>=174 && mouse_y<=750)
            {
                recupCasePlateau(mouse_x, mouse_y, &caseX, &caseY); //Récupération des coordonnées de la case du plateau visée

                if(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAT == 'a') //Il s'agit d'une carte Action
                {
                    if(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAction->action == 'l') //Il s'agit du réchauffement climatique (seule carte action ayant un impact sur le plateau)
                    {
                        illuminationRechauffementClimatique(&dirRechauffementClimatique, caseX, caseY, doubleBuffer, tabPolices[6]); //Illumination adaptée aux caractéristiques du réchauffement climatique
                    }
                }

                if(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAT == 't') //Il s'agit d'une carte Terrain
                {
                    rectfill(doubleBuffer, 444 + caseX*85, 174 + caseY*115, 444 + (caseX+1)*85, 174 + (caseY+1)*115, makecol(255, 255, 0)); //Rectangle jaune sur la case visée

                    if(strcmp(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeTerrain->typeJonction, "Td") == 0 || strcmp(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeTerrain->typeJonction, "Th") == 0) //Si la carte sélectionnée est un T Droite ou un T Haut
                    {
                        //Indication de la possibilité de reverse le sens de la carte
                        textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 400, makecol(255, 255, 255), -1, "Appuyez sur R");
                        textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 425, makecol(255, 255, 255), -1, "pour retourner la carte");
                    }
                }
            }

            illuminationJoueurCible(nbreJoueurs, doubleBuffer); //Possible illumination du profil d'un joueur adverse

            if(mouse_b & 1) //Clic gauche, annulation du placement de la carte
            {
                carteMalPlacee = 1; //Indication que la carte est "mal placée" afin qu'elle retourn à sa place d'origine
                clic = 0; //Réinitialisation de la capacité de clic du joueur
                message = 9; //Indication que l'annulation a eu lieu
            }
            else if(!(mouse_b & 2) && clic == 1) //Le joueur relâche / pose la carte jusque là sélectionnée
            {

                //////////////////////////////////////////////
                // DIFFERENTS PLACEMENTS DE CARTE POSSIBLES //
                //////////////////////////////////////////////

                clic = 0; //Réinitialisation de la capacité de cliquer
                if(mouse_x>=0 && mouse_x<=360 && mouse_y>=348 && mouse_y<=348+(nbreJoueurs-1)*130) //Il s'agit d'une pénalité / réparation à appliquer à un joueur
                {
                    carteMalPlacee = distribPenaliteReparation(tabJoueurs, nbreJoueurs, joueurActuel, tabJoueurs[joueurActuel]->mainJoueur[carteSaisie], tabBIT, &message); //Distribution de la pénalité / réparation
                    if(carteMalPlacee == 0) //Bon placement de la carte, suppression de la carte de la main et remplacement par une carte de la pioche
                    {
                        defausse->imageDefausse = tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAction->visuelAvant; //Actualisation de la défausse
                        defausse->etat = 1; //Indication que la défausse n'est plus vide

                        if(pioche->indiceSommet > 0) //La pioche n'est pas encore vide
                        {
                            (pioche->indiceSommet)--; //Décrémentation de l'indice sommet de la pioche
                            (pioche->indiceNbPlaces)++; //Incrémentation du nombre de place de la pioche

                            //Récupération de la nouvelle carte depuis la pioche
                            tabJoueurs[joueurActuel]->mainJoueur[carteSaisie] = pioche->deck[pioche->indiceSommet];
                            free(pioche->deck[pioche->indiceSommet]);
                            pioche->deck[pioche->indiceSommet] = NULL;
                        }
                        else //La pioche est vide, processus de END game
                        {
                            *ENDgame = 1; //Indication que le END game est en cours
                            tabJoueurs[joueurActuel]->mainJoueur[carteSaisie] = NULL; //Suppression de la carte de la main du joueur
                        }

                        done = 1; //Indication que le joueur a joué et qu'on peut quitter cette boucle de déroulement
                        phaseSortie = 6; //Indication qu'on passe au menu pré joueur suivant en sortie de ce sous-programme
                    }
                    else //La carte sélectionnée n'est pas une carte appropriée
                    {
                        if(message == 0)
                        {
                            message = 3;
                        }
                    }
                }
                else if(mouse_x>=0 && mouse_x<=360 && mouse_y<348 && mouse_y>=0) //Il s'agit d'une réparation de pénalité personnelle
                {
                    if(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAT == 'a' && tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAction->effet == 'r') //Carte action est de type réparation
                    {
                        carteMalPlacee = verifReparation(tabJoueurs[joueurActuel], tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAction, tabBIT, &message); //Distribution de la réparation
                        if(carteMalPlacee == 0) //Bon placement de la carte, suppression de la carte de la main et remplacement par une carte de la pioche
                        {
                            defausse->imageDefausse = tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAction->visuelAvant; //Actualisation de la défausse
                            defausse->etat = 1; //Indication que la défausse n'est plus vide

                            if(pioche->indiceSommet > 0) //La pioche n'est pas encore vide
                            {
                                (pioche->indiceSommet)--; //Décrémentation de l'indice sommet de la pioche
                                (pioche->indiceNbPlaces)++; //Incrémentation du nombre de place de la pioche

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

                            done = 1; //Indication que le joueur a joué et qu'on peut quitter cette boucle de déroulement
                            phaseSortie = 6; //Indication qu'on passe au menu pré joueur suivant en sortie de ce sous-programme
                        }
                    }
                    else //La carte sélectionnée n'est pas une carte réparation, échec du processus
                    {
                        message = 1;
                        carteMalPlacee = 1;
                    }
                }
                else if(mouse_x>=444 && mouse_x<=1039 && mouse_y>=174 && mouse_y<=750) //Il s'agit du placement d'une carte Terrain
                {
                    if(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAT == 't') //La carte sélectionnée est de type Terrain
                    {
                        if(possessionPenalite(tabJoueurs[joueurActuel]->penalite) == 0) //Le joueur ne possède pas de pénalité
                        {
                            recupCasePlateau(mouse_x, mouse_y, &caseX, &caseY); //Récupération des coordonnées de la case visée

                            carteMalPlacee = placementCarte(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeTerrain, plateau, caseX, caseY, reverseCarte, tabBIT); //Placement ou non de la carte sur le plateau de jeu

                            if(carteMalPlacee == 0) //Bon placement de la carte, suppression de la carte de la main et remplacement par une carte de la pioche
                            {
                                *finPartie = testFinPartie(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeTerrain, caseX, caseY, reverseCarte); //Vérification du placement de la carte et déduction si fin de partie
                                if(*finPartie == 1) //Il s'agit de la fin de la partie
                                {
                                    calculScoresFinPartie(tabJoueurs, nbreJoueurs, joueurActuel, caseX, caseY, tabCartesArrivee, tabBIT); //Calcul du score en fin de partie
                                    sauvegardeScores(tabJoueurs, nbreJoueurs); //Sauvegarde des scores dans le fichier dédié

                                    done = 1; //Indication que le joueur a joué et qu'on peut quitter cette boucle de déroulement
                                    phaseSortie = 6; //Indication qu'on passe au menu pré joueur suivant en sortie de ce sous-programme
                                }
                                else if(pioche->indiceSommet > 0) //Si la pioche n'est pas vide
                                {
                                    (pioche->indiceSommet)--; //Décrémentation de l'indice sommet de la pioche
                                    (pioche->indiceNbPlaces)++; //Incrémentation du nombre de place de la pioche

                                    //Récupération de la carte au sommet de la pioche
                                    tabJoueurs[joueurActuel]->mainJoueur[carteSaisie] = pioche->deck[pioche->indiceSommet];
                                    free(pioche->deck[pioche->indiceSommet]);
                                    pioche->deck[pioche->indiceSommet] = NULL;

                                    done = 1; //Indication que le joueur a joué et qu'on peut quitter cette boucle de déroulement
                                    phaseSortie = 6; //Indication qu'on passe au menu pré joueur suivant en sortie de ce sous-programme
                                }
                                else if(pioche->indiceSommet == 0) //La pioche est vide, processus de END game
                                {
                                    *ENDgame = 1; //Indication que le END game est en cours
                                    tabJoueurs[joueurActuel]->mainJoueur[carteSaisie] = NULL; //Suppression de la carte de la main du joueur
                                    done = 1; //Indication que le joueur a joué et qu'on peut quitter cette boucle de déroulement
                                    phaseSortie = 6; //Indication qu'on passe au menu pré joueur suivant en sortie de ce sous-programme
                                }
                            }
                            else
                            {
                                message = 5;
                            }
                        }
                        else //Le joueur possède une pénalité, il ne peut pas jouer de carte terrain
                        {
                            carteMalPlacee = 1; //Indication que la carte est à remettre dans la main du joueur
                            message = 2;
                        }
                    }
                    else if(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAT == 'a' && tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAction->action == 'l') //La carte sélectionnée est le réchauffement climatique, seule carte action pouvant être posée sur le terrain
                    {
                        if(possessionPenalite(tabJoueurs[joueurActuel]->penalite) == 0) //Le joueur ne possède pas de pénalité
                        {
                            recupCasePlateau(mouse_x, mouse_y, &caseX, &caseY); //Récupération des coordonnées de la case visée

                            carteMalPlacee = testRechauffementClimatique(caseX, caseY, dirRechauffementClimatique, plateau); //Vérification de la validité du placement du réchauffement climatique

                            if(carteMalPlacee == 0) //La carte est bien placée
                            {
                                applicationRechauffementClimatique(caseX, caseY, dirRechauffementClimatique, plateau, defausse); //Application du réchauffement climatique

                                if(pioche->indiceSommet > 0) //La pioche n'est pas encore vide
                                {
                                    (pioche->indiceSommet)--; //Décrémentation de l'indice sommet de la pioche
                                    (pioche->indiceNbPlaces)++; //Incrémentation du nombre de place de la pioche

                                    //Récupération de la nouvelle carte depuis la pioche
                                    tabJoueurs[joueurActuel]->mainJoueur[carteSaisie] = pioche->deck[pioche->indiceSommet];
                                    free(pioche->deck[pioche->indiceSommet]);
                                    pioche->deck[pioche->indiceSommet] = NULL;
                                }
                                else //La pioche est vide, processus de END game
                                {
                                    *ENDgame = 1; //Indication que le END game est en cours
                                    tabJoueurs[joueurActuel]->mainJoueur[carteSaisie] = NULL; //Suppression de la carte de la main du joueur
                                }

                                done = 1; //Indication que le joueur a joué et qu'on peut quitter cette boucle de déroulement
                                phaseSortie = 6; //Indication qu'on passe au menu pré joueur suivant en sortie de ce sous-programme
                            }
                            else
                            {
                                message = 5;
                            }
                        }
                        else //Le joueur possède au moins une pénalité, il ne peut pas jouer cette carte
                        {
                            carteMalPlacee = 1; //Indication que la carte est mal placée
                            message = 2;
                        }
                    }
                    else //La carte sélectionnée n'est pas adaptée au placement sur le plateau de jeu
                    {
                        carteMalPlacee = 1; //Indication que la carte est mal placée
                        message = 1;
                    }
                }

                else if(mouse_x>1039 && mouse_x<=1124) //Le joueur souhaite révélé l'une des cases Arrivée
                {
                    if(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAT == 'a' && tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAction->action == 'g') //La carte sélectionnée est la MAP
                    {
                        *devoilementCaseArrivee = testMap(mouse_x, mouse_y, tabCartesArrivee); //Test de la validité de MAP

                        if(*devoilementCaseArrivee == -1) //Aucune case n'a été sélectionnée, le positionnement de la carte n'était pas bon
                        {
                            message = 5;
                            carteMalPlacee = 1; //Indication que la carte est mal placée
                        }
                        else
                        {
                            defausse->imageDefausse = tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->typeAction->visuelAvant; //Actualisation de la défausse
                            defausse->etat = 1; //Indication que la défausse n'est plus vide

                            if(pioche->indiceSommet > 0) //La pioche n'est pas encore vide
                            {
                                (pioche->indiceSommet)--; //Décrémentation de l'indice sommet de la pioche
                                (pioche->indiceNbPlaces)++; //Incrémentation du nombre de place de la pioche

                                //Récupération de la nouvelle carte depuis la pioche
                                tabJoueurs[joueurActuel]->mainJoueur[carteSaisie] = pioche->deck[pioche->indiceSommet];
                                free(pioche->deck[pioche->indiceSommet]);
                                pioche->deck[pioche->indiceSommet] = NULL;
                            }
                            else //La pioche est vide, processus de END game
                            {
                                *ENDgame = 1; //Indication que le END game est en cours
                                tabJoueurs[joueurActuel]->mainJoueur[carteSaisie] = NULL; //Suppression de la carte de la main du joueur
                            }

                            done = 1; //Indication que le joueur a joué et qu'on peut quitter cette boucle de déroulement
                            phaseSortie = 6; //Indication qu'on passe au menu pré joueur suivant en sortie de ce sous-programme
                        }
                    }
                    else
                    {
                        message = 6;
                        carteMalPlacee = 1; //Indication que la carte est mal positionnée
                    }
                }
                else //La position choisie n'est pas valide
                {
                    carteMalPlacee = 1; //Indication que la carte a été mal positionnée
                }
            }
        }

        //La carte a été mal positionnée
        if(carteMalPlacee == 1)
        {
            retourCarte(tabJoueurs[joueurActuel]->mainJoueur); //Processus de retour de la carte vers son emplacement initial
            if(tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->posX == tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->xInit && tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->posY == tabJoueurs[joueurActuel]->mainJoueur[carteSaisie]->yInit) //La carte est de retour à son emplacement initial
            {
                carteMalPlacee = 0; //Indication que la carte est de nouveau à sa place
                carteSaisie = -1; //Indication qu'une nouvelle carte peut être sélectionnée
            }
        }

        if(message != 0) //Un message doit être affiché
        {
            messageErreur(message, &alphaMessage, doubleBuffer, tabPolices[6], tabPolices[6]); //Affichage du message qui se dissipe progressivement
            if(alphaMessage >= 255) //Le message a totalement disparu
            {
                message = 0; //Réinitialisation de la fonctionnalité de message
                alphaMessage = -255; //Réinitilisation de la fonctionnalité d'effacement progressif du message
            }
        }

        //Processus d'affichage des informations des autres joueurs dans l'ordre de passage
        i = (joueurActuel+1)%nbreJoueurs;
        while(i != joueurActuel) //Boucle permettant l'affichage des joueurs dans l'ordre de passage
        {
            affichageProfilAutresJoueurs(tabJoueurs[i], doubleBuffer, tabPolices[6], tabPolices[7], decalageAutreJoueur); //Affichage des profils
            decalageAutreJoueur++; //Décalage du profil suivant vers le bas

            i = (i+1)%nbreJoueurs; //Passage au joueur suivant
        }

        //Affichage des cartes du joueur actuel avec possibilité de reverse
        affichageCartesJoueurActuelReverse(tabJoueurs[joueurActuel], carteSaisie, reverseCarte, doubleBuffer, tabBIT[69], tabBIT[70]);

        if(mouse_x>=1282 && mouse_x<=1340 && mouse_y>=597 && mouse_y<=623 && carteMalPlacee == 0) //La souris passe sur "RETOUR" et aucune carte est en situation de retour à sa position initiale
        {
            textprintf_centre_ex(doubleBuffer, tabPolices[6], 1312, 600, makecol(255, 0, 0), -1, "Retour"); //Affichage du texte en rouge
            if(mouse_b & 1 && clic == 0) //Clic sur "RETOUR"
            {
                clic = 1; //Evite la répétition de la condition en cas de maintien du clic gauche

                phaseSortie = 1; //Retour au menu de sélection de l'action
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

        blit(doubleBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //Affichage du double buffer
    }

    return phaseSortie;
}


/*
Sous-programme déterminant si la carte est, oui ou non, sous la souris
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
                        *diffX = mouse_x - mainJoueur[i]->posX; //Position en X de la carte par rapport à la souris
                        *diffY = mouse_y - mainJoueur[i]->posY; //Position en Y de la carte par rapport à la souris

                        *clic = 1; //Evite la répétition successive de la condition

                        return i; //Retour de l'indice de la carte sélectionnée
                    }
                }
            }
            else if(mainJoueur[i]->typeAT == 't') //La carte est de type Terrain
            {
                if(mouse_x>=mainJoueur[i]->posX && mouse_x<=mainJoueur[i]->posX+mainJoueur[i]->typeTerrain->visuelAvant->w && mouse_y>=mainJoueur[i]->posY && mouse_y<=mainJoueur[i]->posY+mainJoueur[i]->typeTerrain->visuelAvant->h) //La souris est sur la carte
                {
                    if(mouse_b & 2 && *clic == 0) //Clic sur la carte
                    {
                        *diffX = mouse_x - mainJoueur[i]->posX; //Position en X de la carte par rapport à la souris
                        *diffY = mouse_y - mainJoueur[i]->posY; //Position en Y de la carte par rapport à la souris

                        *clic = 1; //Evite la répétition successive de la condition

                        return i; //Retour de l'indice de la carte sélectionnée
                    }
                }
            }
        }
    }

    return -1; //Permet d'indiquer qu'aucune carte n'a été sélectionnée
}


/*
Sous-programme d'actualisation de la position de la carte en fonction de la souris
*/
void deplacementCarte(int diffX, int diffY, t_carte *carte, int *reverseCarte, int *indicReverse)
{
    carte->posX = mouse_x - diffX; //Actualisation de l'emplacement en X
    carte->posY = mouse_y - diffY; //Actualisation de l'emplacement en Y

    //Processus d'inversion du sens de la carte pour les croisements HAUT et DROITE
    if(carte->typeAT == 't' && (strcmp(carte->typeTerrain->typeJonction, "Td") == 0 || strcmp(carte->typeTerrain->typeJonction, "Th") == 0)) //La carte sélectionnée est un croisement HAUT ou DROITE
    {
        if(key[KEY_R] && *indicReverse == 0) //Le joueur appuie sur R
        {
            *indicReverse = 1; //Evite la répétition successive de la condition

            //Inversion de la valeur de reverseCarte
            if(*reverseCarte == 0) //Passe de 0 à 1
            {
                *reverseCarte = 1;
            }
            else //Passe de 1 à 0
            {
                *reverseCarte = 0;
            }
        }
        else if(!(key[KEY_R]) && *indicReverse == 1) //Le joueur n'appuie pas sur R, réinitialisation de la capacité de reverse la carte
        {
            *indicReverse = 0;
        }
    }
}


/*
Sous-programme permettant l'illumination du profil du joueur visé par une pénalité
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

    //Même logique pour les autres quantités de joueurs

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
Sous-programme d'application de la pénalité / réparation à un autre joueur
*/
int distribPenaliteReparation(t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, t_carte *carteSaisie, BITMAP *tabBIT[BIT], int *message)
{
    int joueurVise; //Permet de déterminer le joueur visé par la réparation / pénalité
    int valide = 1; //Permet de déterminer si le placement est valide (0) ou non (1)

    if(nbreJoueurs == 2) //2 joueurs dans la partie
    {
        if(mouse_x>=0 && mouse_x<=360 && mouse_y>=348 && mouse_y<=482) //La souris passe au dessus du profil de l'autre joueur
        {
            if(carteSaisie->typeAT == 'a') //Il s'agit d'une carte action
            {
                joueurVise = (joueurActuel + 1)%nbreJoueurs; //Détermination du joueur visé

                if(carteSaisie->typeAction->effet == 'p') //Il s'agit d'une carte pénalité
                {
                    valide = verifPenalite(tabJoueurs[joueurVise], carteSaisie->typeAction, tabBIT, message); //Processus de test et d'application de la pénalité au joueur visé
                }
                else if(carteSaisie->typeAction->effet == 'r') //Il s'agit d'une carte réparation
                {
                    valide = verifReparation(tabJoueurs[joueurVise], carteSaisie->typeAction, tabBIT, message); //Processus de test et d'application de la réparation au joueur visé
                }
            }
            else //La carte n'est pas une carte Action
            {
                *message = 1;
            }
        }
    }

    //Même logique pour les autres quantités de joueurs


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
Sous-programme de vérification si la carte pénalité peut être donné au joueur visé
*/
int verifPenalite(t_joueur *joueurVise, t_carteAction *cartePenalite, BITMAP *tabBIT[BIT], int *message)
{
    int i; //Permet le bon fonctionnement de la boucle de parcours des pénalités du joueur visé
    int existence = 0; //Permet de déterminer si le joueur visé possède déjà la pénalité distribuée
    int ajout = 0; //Permet de déterminer l'ajout de la pénalité lorsque celle-ci n'est pas déjà présente chez le joueur visé
    int valide = 1; //Permet de déterminer si, au final, la pénalité donnée peut être administrée ou non

    if(cartePenalite->action == 'h') //Traîneau cassé
    {
        for(i=0 ; i<3 ; i++) //Boucle de recherche de cette pénalité dans les pénalités du joueur visé
        {
            if(joueurVise->penalite[i] != NULL) //Une pénalité existe
            {
                if(existence == 0) //Pour l'instant, la même pénalité n'a pas été trouvée
                {
                    if(joueurVise->penalite[i] == tabBIT[32]) //La pénalité est déjà présente dans les pénalités du joueur visé
                    {
                        existence = 1; //Indication que la pénalité est déjà présente, le programme va finalement retourner valide = 1, car ce n'est pas possible
                        *message = 3;
                    }
                }
            }
        }
        if(existence == 0) //Après parcours, la pénalité n'a pas été trouvée, il faut donc la rajouter
        {
            for(i=0 ; i<3 ; i++) //Boucle de parcours des pénalités en quête de case vide
            {
                if(ajout == 0) //Processus s'effectuant tant que la nouvelle pénalité n'a pas été ajoutée
                {
                    if(joueurVise->penalite[i] == NULL) //Emplacement libre pour la nouvelle pénalité
                    {
                        joueurVise->penalite[i] = tabBIT[32]; //Récupération de la nouvelle pénalité
                        ajout = 1; //Indication que la nouvelle pénalité a été ajoutée
                        valide = 0; //Indication que la situation est correcte
                    }
                }
            }
        }
    }

    //Même processus pour les autres pénalités

    else if(cartePenalite->action == 'i') //Canne à pêche cassée
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

    else if(cartePenalite->action == 'j') //Scie à glace cassée
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

    else if(cartePenalite->action == 'k') //Tous les outils sont cassés
    {
        for(i=0 ; i<3 ; i++) //Boucle de parcours des pénalités du joueur visé
        {
            if(joueurVise->penalite[i] != NULL) //Le joueur possède déjà une des pénalités existantes
            {
                existence = 1; //Indication qu'il possède déjà une pénalité
                *message = 3;
            }
        }
        if(existence == 0) //Le joueur visé ne possède pas de pénalité
        {
            //Ajout des trois pénalités possibles dans le tableau des pénalités du joueur visé
            joueurVise->penalite[0] = tabBIT[28];
            joueurVise->penalite[1] = tabBIT[30];
            joueurVise->penalite[2] = tabBIT[32];
            valide = 0; //Indication que la situation est possible
        }
    }

    return valide;
}


/*
Sous-programme de vérification si la carte réparation peut être donnée au joueur visé
*/
int verifReparation(t_joueur *joueurVise, t_carteAction *cartePenalite, BITMAP *tabBIT[BIT], int *message)
{
    int i; //Permet le bon fonctionnement de la boucle de parcours des pénalités du joueur visé
    int existence1 = 0; //Permet de déterminer si le joueur visé possède bien la pénalité à réparer
    int existence2 = 0;
    int valide = 1; //Permet de déterminer si, au final, la pénalité donnée peut être administrée ou non

    if(cartePenalite->action == 'a') //Traîneau réparé
    {
        for(i=0 ; i<3 ; i++) //Boucle de recherche de la pénalité en question
        {
            if(joueurVise->penalite[i] != NULL) //Une pénalité existe
            {
                if(existence1 == 0) //Pour l'instant, la pénbalité n'a pas été trouvée
                {
                    if(joueurVise->penalite[i] == tabBIT[32]) //La pénalité est présente dans les pénalités du joueur visé
                    {
                        existence1 = 1; //Indication que la pénalité est présente
                        joueurVise->penalite[i] = NULL; //Suppression de la pénalité
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

    //Même logique pour les cartes simples

    else if(cartePenalite->action == 'b') //Canne à pêche réparée
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

    else if(cartePenalite->action == 'c') //Scie à glace réparée
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

    else if(cartePenalite->action == 'd') //Traîneau en bois et canne à pêche réparés
    {
        for(i=0 ; i<3 ; i++) //Parcours des pénalités pour voir si elles y sont
        {
            if(joueurVise->penalite[i] != NULL) //Si la pénalité existe
            {
                if(joueurVise->penalite[i] == tabBIT[32]) //Il s'agit d'une des pénalités recherchées
                {
                    joueurVise->penalite[i] = NULL; //Suppression de la pénalité
                    existence1 = 1; //Indication que la première pénalité recherchée a été trouvée
                }
                if(joueurVise->penalite[i] == tabBIT[28]) //Il s'agit de l'autre pénalité recherchée
                {
                    joueurVise->penalite[i] = NULL; //Suppression de la pénalité
                    existence2 = 1; //Indication que la deuxième pénalité recherchée a été trouvée
                }
            }
        }
        if(existence1 == 1 || existence2 == 1) //Les deux pénalités ont été trouvées
        {
            valide = 0; //Indication que la situation est valide
        }
        else if(existence1 == 0 && existence2 == 0)
        {
            *message = 4;
        }
    }

    //Même raisonnement pour les autres cartes doubles

    else if(cartePenalite->action == 'e') //Canne à pêche et scie à glace réparées
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

    else if(cartePenalite->action == 'f') //Scie à glace et traîneau en bois réparés
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
Sous-programme vérifiant si le joueur actuel possède au moins une pénalité
*/
int possessionPenalite(BITMAP *penalite[3])
{
    int i; //Permet le bon fonctionnement de la boucle de parcours du tableau de pénalité du joueur actuel
    int presence = 0; //Permet de déterminer la présence ou non d'une carte pénalité dans le tableau de pénalité dédié

    for(i=0 ; i<3 ; i++) //Boucle de parcours du tableau de pénalités
    {
        if(presence == 0) //Pour l'instant aucune pénalité détectée
        {
            if(penalite[i] != NULL) //Pénalité détectée
            {
                presence = 1; //Indication que le joueur possède une carte pénalité
            }
        }
    }

    return presence;
}


/*
Sous-programme mettant en place le retour de la carte en cas de mauvais positionnement ou bien d'annulation du drag and drop (permet aussi la remise à leur place de l'ensemble des cartes en cas de problème plus général)
*/
void retourCarte(t_carte* mainJoueur[6])
{
    int i; //Permet le bon fonctionnement de la boucle de parcours de la main du joueur actuel

    for(i=0 ; i<6 ; i++) //Boucle de parcours de la main du joueur actuel
    {
        if(mainJoueur[i] != NULL) //La carte existe
        {
            if((mainJoueur[i]->posX != mainJoueur[i]->xInit || mainJoueur[i]->posY != mainJoueur[i]->yInit) && !(mouse_b & 2)) //La carte n'est pas à sa position initiale et le joueur n'est pas en train de la bouger
            {
                if(mainJoueur[i]->posX > mainJoueur[i]->xInit) //La carte est trop à droite
                {
                    (mainJoueur[i]->posX)--; //Décalage vers la gauche
                }
                else if(mainJoueur[i]->posX < mainJoueur[i]->xInit) //La carte est trop à gauche
                {
                    (mainJoueur[i]->posX)++; //Décalage vers la droite
                }

                if(mainJoueur[i]->posY > mainJoueur[i]->yInit) //La carte est trop en bas
                {
                    (mainJoueur[i]->posY)--; //Décalage vers le haut
                }
                else if(mainJoueur[i]->posY < mainJoueur[i]->yInit) //La carte est trop en haut
                {
                    (mainJoueur[i]->posY)++; //Décalage vers le bas
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
                if(j == carteSaisie && carteReverse == 1) //Si il s'agit de la carte sélectionnée et que le reverse est enclenché
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
                else //La carte peut être affichée normalement
                {
                    blit(joueurActuel->mainJoueur[j]->typeTerrain->visuelAvant, doubleBuffer, 0, 0, joueurActuel->mainJoueur[j]->posX, joueurActuel->mainJoueur[j]->posY, joueurActuel->mainJoueur[j]->typeTerrain->visuelAvant->w,joueurActuel->mainJoueur[j]->typeTerrain->visuelAvant->h); //Affichage de la carte Terrain
                }
            }
        }
    }
}
