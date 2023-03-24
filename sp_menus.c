#include "header.h"

/*
Sous-programme du MENU PRINCIPAL
*/
int menuPrincipal(int *choixMenu, BITMAP *tabBIT[BIT], FONT *tabPolices[POLICES], int *nbreJoueurs, int *choixSave, int *etatMusique, SAMPLE *tabMusique[1])
{
    int done = 0; //Indicateur de suite du processus
    int clic = 0; //Permet d'évtier la validation successive des conditions de clics
    int transitionMusique = 0;

    while(!done) //Boucle de déroulement tant que le processus stagne
    {
        clear_bitmap(tabBIT[0]); //Réinitialisation du double buffer

        //Affichage du menu principale
        blit(tabBIT[1], tabBIT[0], 0, 0, 0, 0, tabBIT[1]->w,tabBIT[1]->h); //Image de fond
        textprintf_centre_ex(tabBIT[0], tabPolices[0], SCREEN_W/2,75, makecol(255,255,0), -1, "LE MACAREUX");
        textprintf_centre_ex(tabBIT[0], tabPolices[1], SCREEN_W/2, 230, makecol(255,255,255), -1, "NOUVELLE PARTIE");
        textprintf_centre_ex(tabBIT[0], tabPolices[1], SCREEN_W/2, 280, makecol(255,255,255), -1, "REPRENDRE PARTIE");
        textprintf_centre_ex(tabBIT[0], tabPolices[1], SCREEN_W/2, 330, makecol(255,255,255), -1, "REGLES");
        textprintf_centre_ex(tabBIT[0], tabPolices[1], SCREEN_W/2, 380, makecol(255,255,255), -1, "SCORES JOUEURS");
        textprintf_centre_ex(tabBIT[0], tabPolices[1], SCREEN_W/2, 430, makecol(255,255,255), -1, "QUITTER");


        //Appel des différents sous-programmes en fonction de la position de la souris
        if ((mouse_x<=890 && mouse_x>=610) && (mouse_y<=260 && mouse_y>=230)) //La souris passe sur "NOUVELLE PARTIE"
        {
            textprintf_centre_ex(tabBIT[0], tabPolices[1], SCREEN_W/2, 230, makecol(255,255,0), -1, "NOUVELLE PARTIE"); //Le texte s'affiche en jaune

            if(mouse_b & 1) //Clic sur "NOUVELLE PARTIE"
            {
                menuNewPartie(tabBIT[0], tabBIT[1], tabPolices[1], tabPolices[0], nbreJoueurs, &done, choixMenu);
            }
        }

        else if ((mouse_x<=900 && mouse_x>=600) && (mouse_y<=310 && mouse_y>=280)) //La souris passe sur "REPRENDRE PARTIE"
        {
            textprintf_centre_ex(tabBIT[0], tabPolices[1], SCREEN_W/2, 280, makecol(255,255,0), -1, "REPRENDRE PARTIE"); //Le texte s'affiche en jaune

            if (mouse_b & 1) //Clic de sur "REPRENDRE PARTIE"
            {
                *choixSave = menuReprendre(tabBIT[0],tabBIT[1], tabPolices[1], tabPolices[0], choixMenu); //Passage au menu "REPRENDRE PARTIE"
                if(*choixSave != 0) //Le joueur a choisi une sauvegarde à lancer
                {
                    ecranNonFonctionnel(tabBIT[0], tabBIT[1], tabPolices[1], tabPolices[0], *choixSave); //Ecran expliquant que la fonctionnalité ne fonctionne malheureusement pas...
                }
                done = 1;
            }
        }

        else if ((mouse_x<=800 && mouse_x>=700) && (mouse_y<=360 && mouse_y>=330)) //La souris passe sur "REGLES"
        {
            textprintf_centre_ex(tabBIT[0], tabPolices[1], SCREEN_W/2, 330, makecol(255,255,0), -1, "REGLES"); //Le texte s'affiche en jaune

            if(mouse_b & 1) //Clic sur "REGLES"
            {
                menuRegles(tabBIT[0], tabBIT[2], tabBIT[3], tabBIT[4], tabBIT[5], tabBIT[6], tabBIT[7], tabBIT[8]); //Passage au menu "REGLES"
            }
        }

        else if ((mouse_x<=890 && mouse_x>=620) && (mouse_y<=410 && mouse_y>=380)) //La souris passe sur "SCORES JOUEURS"
        {
            textprintf_centre_ex(tabBIT[0], tabPolices[1], SCREEN_W/2,380, makecol(255,255,0), -1, "SCORES JOUEURS"); //Le texte s'affiche en jaune

            if(mouse_b & 1) //Clic sur "SCORES JOUEURS"
            {
                affichageScoresEnregistres(tabBIT[0], tabBIT[1], tabBIT[3], tabBIT[4], tabBIT[5], tabBIT[6], tabBIT[7], tabBIT[8], tabPolices[1], tabPolices[0]);
            }
        }

        else if ((mouse_x<=810 && mouse_x>=690) && (mouse_y<=460 && mouse_y>=430)) //La souris passe sur "QUITTER"
        {
            textprintf_centre_ex(tabBIT[0], tabPolices[1], SCREEN_W/2, 430, makecol(255,0,0), -1, "QUITTER"); //Le texte s'affiche en rouge

            if (mouse_b & 1) //Clic sur "QUITTER"
            {
                *choixMenu = 3; //Indicateur de direction pour le main
                done=1; //Indicateur de sortie
            }
        }

        //Détermination de l'état de la musique
        if(mouse_x>=1000 && mouse_x<=1050 && mouse_y>=550 && mouse_y<=600) //La souris passe sur le logo de musique
        {
            if(mouse_b & 1 && clic == 0) //Clic sur le logo de son
            {
                clic = 1; //Permet d'éviter la répétition successive de la condition de clic

                //Inversion de l'état de etatMusique
                if(*etatMusique == 1)
                {
                    *etatMusique = 0;
                }
                else
                {
                    *etatMusique = 1;
                }
            }
            else if(!(mouse_b & 1) && clic == 1) //Réinitialisation de la capacité de clic
            {
                clic = 0;
            }
        }
        if(*etatMusique == 1) //La musique peut être jouée
        {
            if(transitionMusique == 1) //Elle a précédemment été désactivée, il faut la relancer (cette merveille)
            {
                transitionMusique = 0; //Indication que la transition Eteint - Allumé est faite
                play_sample(tabMusique[0], 100, 0, 1500, 0); //Lancement de la musique
            }
            masked_stretch_blit(tabBIT[71], tabBIT[0], 0, 0, 300, 300, 1000, 550, 50, 50); //Affichage de l'audio activé
        }
        else //La musique est stoppée
        {
            transitionMusique = 1; //Indication qu'une transition devra être faite en cas de relancement de la musique
            stop_sample(tabMusique[0]); //Arrêt de la musique
            masked_stretch_blit(tabBIT[72], tabBIT[0], 0, 0, 300, 300, 1000, 550, 50, 50); //Affichage de l'audio désactivé
        }

        //Gestion des écrans de fin accessibles via le menu principal
        if(key[KEY_Q]) //Pression sur A, game over
        {
            ecranGameOver(tabBIT[0], tabBIT[48], tabBIT[49], tabBIT[50], tabPolices[3]);
        }
        else if(key[KEY_B]) //Pression sur B, écran de fin des pingouins
        {
            ecranFinPingouins(tabBIT[0], tabBIT[51], tabBIT[52], tabBIT[53], tabBIT[54], tabBIT[55], tabBIT[56], tabBIT[57], tabPolices[4]);
        }
        else if(key[KEY_C]) //Pression sur C, écran de fin du macareux
        {
            ecranFinMacareux(tabBIT[0], tabBIT[58], tabBIT[59], tabBIT[60], tabBIT[61], tabBIT[62], tabBIT[63], tabPolices[4]);
        }

        blit(tabBIT[0], screen, 0,0,0,0, tabBIT[0]->w, tabBIT[0]->h); //Affichage du double buffer
    }

    return 0;
}


/*
Sous-programme du menu de "NOUVELLE PARTIE"
*/
void menuNewPartie (BITMAP * buffer, BITMAP * fde, FONT * fnt, FONT * Titre, int *nbreJoueurs, int *done, int *choixMenu)
{
    int indicateur = 0; //Permet de rester dans le menu tant que pas de retour

    while (indicateur!=1) //Tant que le retour n'est pas demandé
    {
        clear_bitmap(buffer); //Réinitialisation du double buffer

        //Affichage des différents éléments du menu
        blit(fde, buffer, 0, 0, 0, 0, fde->w,fde->h); //Fond d'écran
        textprintf_centre_ex(buffer, Titre, SCREEN_W/2,75, makecol(255,255,0), -1, "LE MACAREUX");
        textprintf_centre_ex(buffer, fnt, SCREEN_W/2, 250, makecol(255,255,255), -1, "NOMBRE DE JOUEURS");
        textprintf_centre_ex(buffer, fnt, SCREEN_W/2, 310, makecol(255,255,255), -1, "    2   3   4    ");
        textprintf_centre_ex(buffer, fnt, SCREEN_W/2, 570, makecol(255,255,255), -1, "RETOUR");

        if ((mouse_x<=685 && mouse_x>=670) && (mouse_y<=335 && mouse_y>=310)) //Souris sur le "2"
        {
            textprintf_centre_ex(buffer, fnt, 675, 310, makecol(255,255,0), -1, "2"); //Le texte s'affiche en jaune

            if (mouse_b & 1) //Clic de la souris sur "2"
            {
                *choixMenu = 1; //Détermine la continuité du processus comme celui de nouvelle partie
                *nbreJoueurs = 2; //Détermination du nombre de joueurs de la partie à 2
                *done = 1; //Permet de sortir des menus pour passer au processus suivant
                indicateur = 1; //Permet de sortir de ce menu
            }
        }

        else if ((mouse_x<=755 && mouse_x>=745) && (mouse_y<=335 && mouse_y>=310)) //Souris sur le "3"
        {
            textprintf_centre_ex(buffer, fnt, 750, 310, makecol(255,255,0), -1, "3"); //Le texte s'affiche en jaune

            if (mouse_b & 1) //Clic de la souris sur "3"
            {
                *choixMenu = 1; //Détermine la continuité du processus comme celui de nouvelle partie
                *nbreJoueurs = 3; //Détermination du nombre de joueurs de la partie à 3
                *done = 1; //Permet de sortir des menus pour passer au processus suivant
                indicateur = 1; //Permet de sortir de ce menu
            }
        }

        else if ((mouse_x<=830 && mouse_x>=820) && (mouse_y<=335 && mouse_y>=310)) //Souris sur le "4"
        {
            textprintf_centre_ex(buffer, fnt, 825, 310, makecol(255,255,0), -1, "4"); //Le texte s'affiche en jaune

            if (mouse_b & 1) //Clic de la souris sur "4"
            {
                *choixMenu = 1; //Détermine la continuité du processus comme celui de nouvelle partie
                *nbreJoueurs = 4; //Détermination du nombre de joueurs de la partie à 3
                *done = 1; //Permet de sortir des menus pour passer au processus suivant
                indicateur = 1; //Permet de sortir de ce menu
            }
        }

        else if ((mouse_x<=SCREEN_W/2+50 && mouse_x>=SCREEN_W/2-50) && (mouse_y<=590 && mouse_y>=570)) //Souris sur "RETOUR"
        {
            textprintf_centre_ex(buffer, fnt, SCREEN_W/2, 570, makecol(255,0,0), -1, "RETOUR"); //Le texte s'affiche en rouge

            if (mouse_b & 1) //Clic sur "RETOUR"
            {
                indicateur=1; //Permet de sortir de ce menu
            }
        }


        //Afichage du doubleBuffer
        blit(buffer,screen, 0,0,0,0, buffer->w, buffer->h);
    }
}


/*
Sous-programme du menu "REPRENDRE PARTIE"
*/
int menuReprendre (BITMAP * buffer, BITMAP * fde, FONT * fnt, FONT * Titre, int *choixMenu)
{
    int indicateur = 0; //Permet de réaliser la fonction de retour à la page précédente

    while (indicateur!=1) //Tant que le retour n'est pas demandé
    {
        clear_bitmap(buffer); //Réinitialisation du double buffer

        //Affichage du fond et des elements du menu dans le double buffer
        blit(fde, buffer, 0, 0, 0, 0, fde->w,fde->h); //Image de fond
        textprintf_centre_ex(buffer, Titre, 750,30, makecol(198,255,255), -1, "LE MACAREUX"); //Titre
        textprintf_centre_ex(buffer, fnt, 750, 120, makecol(255,255,255), -1, "CHARGER UNE SAUVEGARDE"); //Titre section

        if (mouse_x>=690 && mouse_x<= 810 && mouse_y>= 265 && mouse_y<= 275) //La souris passe sur "SAUVEGARDE 1"
        {
            textprintf_centre_ex(buffer, fnt, 750,250, makecol(255,255,0), -1, "Sauvegarde 1"); //Le texte s'affiche en jaune
            if(mouse_b & 1) //Clic sur "SAUVEGARDE 1"
            {
                *choixMenu = 2; //Indication qu'on passe au processus de création d'une partie depuis des données existantes
                return 1; //Retour indiquant le fichier à lancer
            }
        }
        else //La souris est ailleurs sur l'écran
        {
            textprintf_centre_ex(buffer, fnt, 750,250, makecol(255,255,255), -1, "Sauvegarde 1"); //Le texte s'affiche en blanc
        }

        //Même logique pour les autres sauvegardes

        if (mouse_x>=690 && mouse_x<= 810 && mouse_y>= 335 && mouse_y<= 345)
        {
            textprintf_centre_ex(buffer, fnt, 750,320, makecol(255,255,0), -1, "Sauvegarde 2");
            if(mouse_b & 1)
            {
                *choixMenu = 2;
                return 2;
            }
        }
        else
        {
            textprintf_centre_ex(buffer, fnt, 750,320, makecol(255,255,255), -1, "Sauvegarde 2");
        }

        if (mouse_x>=690 && mouse_x<= 810 && mouse_y>=395  && mouse_y<=415)
        {
            textprintf_centre_ex(buffer, fnt, 750,390, makecol(255,255,0), -1, "Sauvegarde 3");
            if(mouse_b & 1)
            {
                *choixMenu = 2;
                return 3;
            }
        }
        else
        {
            textprintf_centre_ex(buffer, fnt, 750,390, makecol(255,255,255), -1, "Sauvegarde 3");
        }

        if (mouse_x>=690 && mouse_x<= 810 && mouse_y>=465  && mouse_y<=485)
        {
            textprintf_centre_ex(buffer, fnt, 750,460, makecol(255,255,0), -1, "Sauvegarde 4");
            if(mouse_b & 1)
            {
                *choixMenu = 2;
                return 4;
            }
        }
        else
        {
            textprintf_centre_ex(buffer, fnt, 750,460, makecol(255,255,255), -1, "Sauvegarde 4");
        }

        //Demande de retour au menu principale
        if ((mouse_x<=SCREEN_W/2+50 && mouse_x>=SCREEN_W/2-50) && (mouse_y<=590 && mouse_y>=570)) //la souris passe sur "RETOUR"
        {
            textprintf_centre_ex(buffer, fnt, SCREEN_W/2, 570, makecol(255,0,0), -1, "RETOUR"); //Affichage du texte en rouge
            if (mouse_b & 1) //Clic sur "RETOUR"
            {
                indicateur=1; //Indication qu'on peut sortir de la boucle de déroulement pour revenir au menu principal
            }
        }
        else //La souris est ailleurs sur l'écran
        {
            textprintf_centre_ex(buffer, fnt, SCREEN_W/2, 570, makecol(255,255,255), -1, "RETOUR"); //Affichage du texte en blanc
        }

        //Affichage du double Buffer
        blit(buffer,screen, 0,0,0,0, buffer->w, buffer->h);
    }

    return 0;
}


/*
Sous-programme de l'écran du chargement non fonctionnel
*/
void ecranNonFonctionnel(BITMAP *doubleBuffer, BITMAP *fondEcran, FONT * fnt, FONT * Titre, int numFichier)
{
    int done = 0; //Permet le bon fonctionnement de la boucle de déroulement

    while(!done) //Boucle de déroulement tant que le joueur n'a pas demandé le retour au menu principal
    {
        clear_bitmap(doubleBuffer); //Réinitialisation du double buffer

        //Affichage du fond et des elements du menu dans le double buffer
        blit(fondEcran, doubleBuffer, 0, 0, 0, 0, fondEcran->w,fondEcran->h); //Image de fond
        textprintf_centre_ex(doubleBuffer, Titre, SCREEN_W/2,30, makecol(198,255,255), -1, "LE MACAREUX"); //Titre

        textprintf_centre_ex(doubleBuffer, fnt, SCREEN_W/2,320, makecol(255,255,255), -1, "La fonctionnalite n'est pas terminee...");
        textprintf_centre_ex(doubleBuffer, fnt, SCREEN_W/2,360, makecol(255,255,255), -1, "Neanmoins la sauvegarde fonctionne et nous vous");
        textprintf_centre_ex(doubleBuffer, fnt, SCREEN_W/2,400, makecol(255,255,255), -1, "conseillons d'aller regarder les fichiers suivis d'un %d", numFichier);
        textprintf_centre_ex(doubleBuffer, fnt, SCREEN_W/2,440, makecol(255,255,255), -1, "dans le dossier du projet.");

        //Demande de retour au menu principale
        if ((mouse_x<=SCREEN_W/2+50 && mouse_x>=SCREEN_W/2-50) && (mouse_y<=590 && mouse_y>=570)) //la souris passe sur "RETOUR"
        {
            textprintf_centre_ex(doubleBuffer, fnt, SCREEN_W/2, 570, makecol(255,0,0), -1, "RETOUR AU MENU PRINCIPAL"); //Affichage du texte en rouge
            if (mouse_b & 1) //Clic sur "RETOUR"
            {
                done=1; //Indication qu'on peut sortir de la boucle de déroulement pour revenir au menu principal
            }
        }
        else //La souris est ailleurs sur l'écran
        {
            textprintf_centre_ex(doubleBuffer, fnt, SCREEN_W/2, 570, makecol(255,255,255), -1, "RETOUR AU MENU PRINCIPAL"); //Affichage du texte en blanc
        }

        //Affichage du double Buffer
        blit(doubleBuffer,screen, 0,0,0,0, doubleBuffer->w, doubleBuffer->h);
    }
}


void menuRegles(BITMAP *doubleBuffer, BITMAP *regles, BITMAP *flecheHautBlanche, BITMAP *flecheHautJaune, BITMAP *flecheBasBlanche, BITMAP *flecheBasJaune, BITMAP* flecheRetourBlanche, BITMAP *flecheRetourRouge)
{
    int screenY = 0; //Permet de faire défiler la page de manière verticale
    int done = 0; //Permet de réaliser la fonction de retour à la page précédente

    while(!done) //Tant que le retour n'est pas demandé
    {
        clear_bitmap(doubleBuffer); //Réinitialisation du double buffer

        blit(regles,doubleBuffer,0,screenY,0,0,SCREEN_W,SCREEN_H); //Image de fond

        // Flèches
        if(screenY > 0)
        {
            masked_stretch_blit(flecheHautBlanche,doubleBuffer,0,0,flecheHautBlanche->w,flecheHautBlanche->h,1400,30,100,100);

        }
        if(screenY < regles->h - SCREEN_H)
        {
            masked_stretch_blit(flecheBasBlanche,doubleBuffer,0,0,flecheBasBlanche->w,flecheBasBlanche->h,1400,620,100,100);
        }

        if(mouse_x>=1400 && mouse_x<=1500 && mouse_y>=620 && mouse_y<=720 && mouse_b & 1)
        {
            screenY+=5;
            masked_stretch_blit(flecheBasJaune,doubleBuffer,0,0,flecheBasJaune->w,flecheBasJaune->h,1400,620,100,100);
        }

        if(mouse_x>=1400 && mouse_x<=1500 && mouse_y>=30 && mouse_y<=130 && mouse_b & 1)
        {
            screenY-=5;
            masked_stretch_blit(flecheHautJaune,doubleBuffer,0,0,flecheHautJaune->w,flecheHautJaune->h,1400,30,100,100);
        }

        if ( screenY < 0 )
        {
            screenY = 0;
            rectfill(doubleBuffer,1400,30,1500,300,makecol(143,218,223));
        }
        if ( screenY > regles->h - SCREEN_H )
        {
            screenY = regles->h - SCREEN_H;
            rectfill(doubleBuffer,1400,620,1500,720,makecol(143,218,223));
        }

        //Flèche de retour
        masked_stretch_blit(flecheRetourBlanche,doubleBuffer,0,0,flecheRetourBlanche->w,flecheRetourBlanche->h,1410,SCREEN_H/2 - 50,70,70);

        if(mouse_x >1410 && mouse_x < SCREEN_W && mouse_y < SCREEN_H/2 && mouse_y > flecheRetourBlanche->h + 20 ) //La souris passe sur la flèche de retour
        {
            masked_stretch_blit(flecheRetourRouge,doubleBuffer,0,0,flecheRetourRouge->w,flecheRetourRouge->h,1410,SCREEN_H/2 - 50,70,70); //La flèche s'affiche en rouge

            if(mouse_b&1) //Clic sur flèche de retour, retour à la page précédente
            {
                done = 1;
            }
        }

        blit(doubleBuffer,screen,0,0,0,0,SCREEN_W,SCREEN_H); // Affichage du double buffer sur l'écran
    }
}


/*
Sous-programme d'affichage des scores enregistrés dans le fichier dédié
*/
void affichageScoresEnregistres(BITMAP *doubleBuffer, BITMAP *fondEcran, BITMAP *flecheHautBlanche, BITMAP *flecheHautJaune, BITMAP *flecheBasBlanche, BITMAP *flecheBasJaune, BITMAP* flecheRetourBlanche, BITMAP *flecheRetourRouge, FONT *police, FONT *titre)
{
    int done = 0; //Permet le bon fonctionnement de la boucle de déroulement
    int y = 100; //Permet l'affichage décalé des joueurs
    int f = 0; //Permet le défilement de l'écran
    int nbreJoueursEnregistres = 0;

    int erreur = 0; //Permet de détecter une erreur
    t_maillonScore *nouveau = NULL; //Permet la création de nouveau maillon
    t_maillonScore *liste = NULL; //Permet la mise en place de la liste chaînée contenant les pseudos et scores des joueurs
    t_maillonScore *ptc = NULL;

    FILE* fic = NULL; //Permet la gestion du fichier des scores

    fic = fopen("scores.txt","r"); //Ouverture du fichier afin de récupérer les scores déjà présents
    if(fic==NULL) //Erreur lors de l'ouverture du fichier, simple indication que la sauvegarde n'aura pas lieu
    {
        allegro_message("ERREUR\nLa sauvegarde des scores n'a pas pu avoir lieu\nRaison : probleme d'ouverture de fichier de recuperation");
    }
    else //Ouverture du fichier réussie
    {
        while(!feof(fic))//On récupère les données du fichier dans une liste
        {
            if(erreur == 0) //Aucune erreur n'a, pour l'instant, été détectée
            {
                nouveau = creationMaillon(fic); //Récupération des données intéressantes dans un nouveau maillon
                if(nouveau == NULL) //Erreur lors de la création du nouveau maillon
                {
                    erreur = 1; //Indication qu'une erreur a eu lieu
                }
                else //Création du nouveau maillon réussie
                {
                    liste = ajoutFin(liste,nouveau); //Ajout du nouveau maillon dans la liste chaînée
                }
            }
        }
        fclose(fic); //Fermeture du fichier

        if(erreur == 0)
        {
            //Compatge du nombre ed joueur enregistrés
            ptc = liste;
            while(ptc != NULL) //Parcours de comptage
            {
                nbreJoueursEnregistres++;
                ptc = ptc->suivant;
            }

            while (!done) //Boucle de déroulement tant que la touche ESC n'est pas pressée
            {
                clear_bitmap(doubleBuffer); //Réinitialisation du double buffer

                blit(fondEcran,doubleBuffer,0,0,0,0,SCREEN_W,SCREEN_H); // Affichage du fond dans le double buffer

                defilementScores(&f, doubleBuffer, flecheHautBlanche, flecheHautJaune, flecheBasBlanche, flecheBasJaune, nbreJoueursEnregistres); //Défilement de l'écran

                scoreGraph(liste,doubleBuffer,police, &y,&f); //Affichage des pseudos et des scores de chaque joueur enregistré

                //Mise en place du titre de la section
                rectfill(doubleBuffer,0,0,SCREEN_W,200,makecol(158,218,222));
                textprintf_centre_ex(doubleBuffer, titre, 750,30, makecol(255,255,0), -1, "LE MACAREUX");
                textprintf_centre_ex(doubleBuffer, police, 750,130, makecol(198,255,255), -1, "SCORES");

                if(mouse_x >1075 && mouse_x < 1075 + 70 && mouse_y >SCREEN_H/2-50 && mouse_y < SCREEN_H/2-50+70) //La souris passe sur la flèche de retour
                {
                    masked_stretch_blit(flecheRetourRouge,doubleBuffer,0,0,flecheRetourRouge->w,flecheRetourRouge->h,1075,SCREEN_H/2 - 50,70,70); //La flèche s'affiche en rouge

                    if(mouse_b&1) //Clic sur flèche de retour, retour à la page précédente
                    {
                        done = 1;
                    }
                }
                else //La souris est ailleurs sur l'écran
                {
                    masked_stretch_blit(flecheRetourBlanche,doubleBuffer,0,0,flecheRetourBlanche->w,flecheRetourBlanche->h,1075,SCREEN_H/2 - 50,70,70); //La flèche apparaît en blanc
                }

                blit(doubleBuffer,screen,0,0,0,0,SCREEN_W,SCREEN_H); // Affichage du double buffer sur l'écran

                rest(5); //Permet de ralentir l'animation
            }
        }

        destructionListe(liste);// Destruction de la liste
    }
}


/*
Sous-programme de défilement des joueurs ayant joué et étant sauvegardés dans le fichier des scores
*/
void defilementScores(int *f, BITMAP *doubleBuffer, BITMAP *flecheHautBlanche, BITMAP *flecheHautJaune, BITMAP *flecheBasBlanche, BITMAP *flecheBasJaune, int nbreJoueursEnregistres)
{
    if(*f <= 0 && *f >=-60*nbreJoueursEnregistres)
    {
        if(mouse_x >1075 && mouse_x < 1075 + 70 && mouse_y > 210 && mouse_y < 210+70 && mouse_b & 1) //La souris passe sur la flèche de retour
        {
            masked_stretch_blit(flecheHautJaune,doubleBuffer,0,0,flecheHautJaune->w,flecheHautJaune->h,1075, 210,70,70); //La flèche s'affiche en rouge

            if(mouse_b&1) //Clic sur flèche de retour, retour à la page précédente
            {
                *f-=5; //Montée
            }
        }
        else
        {
            masked_stretch_blit(flecheHautBlanche,doubleBuffer,0,0,flecheHautBlanche->w,flecheHautBlanche->h,1075,210,70,70);
        }
    }

    if(*f < 0)
    {
        if(mouse_x >1075 && mouse_x < 1075 + 70 && mouse_y > 440 && mouse_y < 440+70 && mouse_b & 1) //La souris passe sur la flèche de retour
        {
            masked_stretch_blit(flecheBasJaune,doubleBuffer,0,0,flecheBasJaune->w,flecheBasJaune->h,1075, 440,70,70); //La flèche s'affiche en rouge

            if(mouse_b&1) //Clic sur flèche de retour, retour à la page précédente
            {
                *f+=5;//On monte
            }
        }
        else
        {
            masked_stretch_blit(flecheBasBlanche,doubleBuffer,0,0,flecheBasBlanche->w,flecheBasBlanche->h,1075,440,70,70);
        }
    }
}


/*
Sous-programme d'affichage des scores enregistrés
*/
void scoreGraph(t_maillonScore *liste, BITMAP *doubleBuffer, FONT *police,int *y, int *f)
{
    while(liste != NULL)//Parcours de la liste
    {
        textprintf_centre_ex(doubleBuffer,police, SCREEN_W / 2,*y, makecol(255, 255, 0), -1,"%s | %d",liste->pseudoJoueur,liste->scoreJoueur); //Affichage du pseudo et du score du joueur

        *y = *y +60;//Espaces entre chaque scores
        liste = liste->suivant;
    }

    *y = 200+(*f); // Variation de y en fonction des flèches directionnelles
}
