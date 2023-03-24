#include "header.h"

/*
Sous-programme de cr�ation des joueurs
*/
t_joueur** creationJoueurs(int nbreJoueurs, t_pioche *pioche, int *erreur, BITMAP *tabBIT[BIT], FONT *tabPolices[POLICES])
{
    int i; //Permet le bon fonctionnement de la boucle de remplissage
    int j; //Permet le bon fonctionnement dela boucle d'initialisation des p�nalit�s
    int couleur; //Permet de r�cup�rer la couleur du pingouin que souhaite le joueur
    t_joueur **tabJoueurs; //Contient le tableau de donn�es des joueurs
    t_logo tabPP[8]; //Contient les diff�rentes images de profil disponiables
    char *tabRoles;

    tabJoueurs = (t_joueur**)malloc(nbreJoueurs * sizeof(t_joueur*)); //Allocation de l'espace n�cessaire au tableau de joueurs
    if(tabJoueurs == NULL) //Erreur d'allocation
    {
        *erreur = 1;
    }
    else //Allocation r�ussie
    {
        for(i=0 ; i<nbreJoueurs ; i++) //Boucle d'allocation m�moire de chaque structure joueur du tableau
        {
            if(*erreur == 0) //Processus se r�alisant tant qu'aucune erreur
            {
                tabJoueurs[i] = (t_joueur*)malloc(sizeof(t_joueur)); //Allocation de la structure
                if(tabJoueurs[i] == NULL) //Allocation �chou�e
                {
                    *erreur = 1;
                    liberationTabJoueurs(tabJoueurs, i-1); //Processus de lib�ration de ce qui a �t� pr�c�demment cr��
                }
            }
        }
        if(*erreur == 0) //Cas o� l'ensemble des allocations s'est d�roul� sans erreur
        {
            tabRoles = creationTabRoles(nbreJoueurs, erreur); //Cr�ation du tableau contenant les r�les disponibles
            if(*erreur == 1) //Cr�ation du tableau �chou�
            {
                liberationTabJoueurs(tabJoueurs, nbreJoueurs-1); //Lib�ration du tableau contenant les informations des joueurs
            }
            else //Cr�ation du tableau de r�les r�ussie
            {
                creationTabPP(tabPP, tabBIT); //Cr�ation du tableau des images de profil pingouin

                for(i=0 ; i<nbreJoueurs ; i++) //Boucle de remplissage
                {
                    if(*erreur == 0) //Processus se r�alisant tant qu'aucune erreur
                    {
                        fflush(stdin);
                        tabJoueurs[i]->pseudo = creationPseudo(erreur, i, tabJoueurs, tabBIT[0], tabBIT[1], tabPolices[0], tabPolices[1], tabPolices[2], i+1); //R�cup�ration du pseudo du joueur
                        if(*erreur == 1) //R�cup�ration �chou�e
                        {
                            liberationTabJoueurs(tabJoueurs, nbreJoueurs-1); //Lib�ration de ce qui a �t� pr�c�demment cr��
                        }
                        else //R�cup�ration r�ussie, suite du processus de cr�ation du joueur
                        {
                            //Cr�ation du profil visuel du joueur
                            couleur = choixLogo(tabPP, tabPolices[0], tabPolices[1], tabBIT[0], tabBIT[9], tabBIT[10]); //R�cup�ration de la couleur du pingouin souhait�e
                            tabJoueurs[i]->pingouinIdentite = tabBIT[couleur]; //Enregistrement du pingouin total
                            tabJoueurs[i]->pingouinVignette = tabBIT[couleur + 8]; //Enregistrement de la vignette

                            tabJoueurs[i]->role = tabRoles[i];

                            //R�cup�ration de la main initiale depuis la pioche
                            for(j=0 ; j<6 ; j++)
                            {
                                tabJoueurs[i]->mainJoueur[j] = creationMain(pioche, erreur);
                            }

                            //Initialisation des p�nalit�s
                            for(j=0 ; j<3 ; j++)
                            {
                                tabJoueurs[i]->penalite[j] = NULL;
                            }

                            tabJoueurs[i]->score = 0; //Initialisation du score du joueur
                            tabJoueurs[i]->nbreCarte = 7; //Initialisation du nombre de cartes du joueur
                        }
                    }
                }
            }
        }
    }

    return tabJoueurs;
}


/*
Sous-programme de lib�ration du tableau contenant les informations de tous les joueurs
*/
void liberationTabJoueurs(t_joueur **tabJoueurs, int etatActuel)
{
    int i; //Permet le bon fonctionnement de la boucle de suppression du tableau en cas d'erreur
    //int j; //Permet le bon fonctionnement des boucles de suppression de la main du joueur et de ses p�nalit�s

    for(i=etatActuel ; i>=0 ; i--)
    {

        free(tabJoueurs[i]->pseudo); //Suppression du pseudo du joueur

        free(tabJoueurs[i]); //Lib�ration finale des informations d'un joueur
    }

    free(tabJoueurs); //Lib�ration du tableau de joueurs
}


/*
Sous-programme de cr�ation du pseudo
*/
char* creationPseudo(int *erreur, int joueurActuel, t_joueur **tabJoueurs, BITMAP *doubleBuffer, BITMAP *fondEcran, FONT *policeTitre, FONT *policeJoueur, FONT *policePseudo, int numJoueur)
{
    int i; //Permet le bon fonctionnement de la boucle de parcours du tableau de joueurs pour la v�rification de la non existence du pseudo
    int touche; //Permet de r�cup�rer le scancode d'une touche de clavier
    int indiceLettre = 0; //Permet de conna�tre le nombre de lettres dans le mot
    int fin = 0; //Permet de confirmer la cr�ation d'un mot par la pression de la touche ENTER
    int alphaMessageErreur = -400; //Permet l'effet de disparition progressive d'un message d'erreur
    int message = 0; //Permet de savoir quel message d'erreur afficher
    int pseudoDejaExistant = 0; //Permet l'indication de la pr� existence du pseudo
    char carac; //Permet la r�cup�ration du caract�re correspondant au scancode d'une touche
    char *pseudo; //Permet la conversion de la liste cha�n�e en cha�ne dynamique de caract�res
    t_maillonPseudo *listePseudo = NULL; //Liste permettant la mise en place de la touche BACKSPACE

    //Mise en page d'origine afin d'actualiser l'�cran et le passer sur ce menu
    blit(fondEcran, doubleBuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    textprintf_centre_ex(doubleBuffer, policeTitre, SCREEN_W/2,75, makecol(255,255,0), -1, "LE MACAREUX");
    textprintf_centre_ex(doubleBuffer, policeJoueur, SCREEN_W/2,200, makecol(255,255,255), -1, "Joueur %d", numJoueur);
    textprintf_centre_ex(doubleBuffer, policeJoueur, SCREEN_W/2,230, makecol(255,255,255), -1, "Saisissez votre pseudo");
    blit(doubleBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

    do //Boucle de blindage du pseudo en cas de nom d�j� existant
    {
        pseudoDejaExistant = 0;

        //Lib�ration de la m�moire clavier
        while(keypressed())
        {
            touche = readkey(); //Suppression de l'ensemble des touches ayant �t� press�es avant cette �tape
        }

        //Boucle de d�roulement tant que la touche ENTER n'a pas �t� press�e
        while(fin==0 && *erreur==0)
        {
            if (keypressed()) //Cas o� une touche est press�e
            {
                clear_bitmap(doubleBuffer); //R�initialisation du double buffer

                //Mise en page d'origine
                blit(fondEcran, doubleBuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
                textprintf_centre_ex(doubleBuffer, policeTitre, SCREEN_W/2,75, makecol(255,255,0), -1, "LE MACAREUX");
                textprintf_centre_ex(doubleBuffer, policeJoueur, SCREEN_W/2,200, makecol(255,255,255), -1, "Joueur %d", numJoueur);
                textprintf_centre_ex(doubleBuffer, policeJoueur, SCREEN_W/2,230, makecol(255,255,255), -1, "Saisissez votre pseudo");

                touche=readkey(); //R�cup�ration du scancode de la touche

                carac=(char)touche; //R�cup�ration du caract�re associ� au scancode de la touche

                if(carac == '\r') //Cas de la touche ENTER
                {
                    if(indiceLettre == 0) //Le joueur n'a pas saisi de lettre
                    {
                        message = 7; //Affichage du message correspondant
                    }
                    else //Le joueur a saisi un pseudo
                    {
                        fin = 1; //Fin de la saisie
                    }
                }
                else //Cas d'une touche diff�rente de ENTER
                {
                    if(carac != '\b' && indiceLettre <= 25) //Cas d'une touche diff�rente de BACKSPACE
                    {
                        listePseudo = ajoutLettrePseudo(listePseudo, carac, erreur); //Ajout du nouveau caract�re dans le pseudo, supprime la liste en cas d'erreur
                        if(*erreur == 0) //Erreur lors de l'ajout d'une lettre dans la liste cha�n�e
                        {
                            indiceLettre++; //Incr�mentation de l'indice comptant le nombre de lettres dans le pseudo
                        }
                    }

                    else if(carac == '\b' && indiceLettre > 0) //Cas de la touche BACKSPACE
                    {
                        listePseudo = retraitLettrePseudo(listePseudo); //Suppression de la derni�re lettre du pseudo
                        indiceLettre--; //D�cr�mentation de l'indice comptant le nombre de leyyres dans le pseudo
                    }

                    if(*erreur == 0) //Si aucune erreur n'a eu lieu
                    {
                        affichageListePseudo(listePseudo, indiceLettre, doubleBuffer, policePseudo); //Affichage de la liste cha�n�e contenant le pseudo

                    }
                }
            }

            if(message != 0) //Un message doit �tre affich�
            {
                messageErreur(message, &alphaMessageErreur, doubleBuffer, policeJoueur, policeJoueur); //Affichage du message qui se dissipe progressivement
                if(alphaMessageErreur >= 255) //Le message a totalement disparu
                {
                    message = 0; //R�initialisation de la fonctionnalit� de message
                    alphaMessageErreur = -400; //R�initilisation de la fonctionnalit� d'effacement progressif du message
                }
            }

            blit(doubleBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //Affichage du double buffer
        }

        if(*erreur == 0) //Aucune erreur lors de la cr�ation du pseudo
        {
            //Conversion de la liste cha�n�e en une cha�ne dynamique de caract�res
            pseudo = ListeVersChaineCar(listePseudo, indiceLettre, erreur);
            if(*erreur == 1) //Erreur lors de la cr�ation de la cha�ne dynamique de caract�res
            {
                pseudo = NULL;
            }
            else //Conversion r�ussie
            {
                liberationListePseudo(listePseudo); //Lib�ration de la liste cha�n�e
                listePseudo = NULL; //R�initialisation de la t�te de liste

                for(i=0 ; i<joueurActuel ; i++) //Parcours du tableau de joueurs afin de v�rifier que le pseudo n'existe pas
                {
                    if(strcmp(pseudo, tabJoueurs[i]->pseudo) == 0) //Le pseudo existe
                    {
                        pseudoDejaExistant = 1; //Indication que le pseudo existe d�j�
                        fin = 0; //Indication que le processus n'est pas termin�
                        indiceLettre = 0; //R�initialisation du nombre de lettres dans le pseudo
                        message = 8; //Affichage du message correspondant
                    }
                }
            }
        }
        else //Erreur lors de la cr�ation du pseudo
        {
            pseudo = NULL;
        }
    }
    while(pseudoDejaExistant == 1 && *erreur == 0); //Blindage tant que le pseudo saisi existe d�j� et qu'il n'y a pas eu d'erreur

    return pseudo;
}


/*
Sous-programme d'ajout d'une lettre dans la liste cha�n�e contenant le pseudo
*/
t_maillonPseudo* ajoutLettrePseudo(t_maillonPseudo *liste, char carac, int *erreur)
{
    t_maillonPseudo *nouveau; //Permet d'ajouter la nouvelle lettre dans la liste contenant le pseudo
    t_maillonPseudo *ptc; //Permet de parcourir la liste pour ajouter le nouveau maillon � la fin de cette derni�re

    nouveau = (t_maillonPseudo*)malloc(sizeof(t_maillonPseudo)); //Allocation du nouveau maillon
    if(nouveau == NULL) //Erreur d'allocation
    {
        *erreur = 1; //Incr�mentation de l'indicateur d'erreur

        liberationListePseudo(liste); //Lib�ration de la lite cha�n�e

        return NULL;
    }
    else //Allocation r�ussie
    {
        //Initialisation du nouveau maillon
        nouveau->lettre = carac;
        nouveau->suivant = NULL;

        if(liste == NULL) //Cas de la lsite vide
        {
            return nouveau;
        }
        else //Cas de la liste non vide
        {
            //Processus de parcours de la liste jusqu'au dernier maillon
            ptc = liste;

            while(ptc->suivant != NULL)
            {
                ptc = ptc->suivant;
            }

            ptc->suivant = nouveau; //Ajout du nuoveau maillon en fin de liste

            return liste;
        }
    }
}


/*
Sous-programme de lib�ration de la liste cha�n�e contenant le pseudo du joueur
*/
void liberationListePseudo(t_maillonPseudo *liste)
{
    if(liste == NULL) //Cas d'arr�t
    {

    }
    else
    {
        liberationListePseudo(liste->suivant); //R�cursivit� de la fonction permettant la lib�ration de la liste par le dernier maillon
        free(liste); //Lib�ration du maillon
    }
}


/*
Sous-programme de suppression / retrait de la derni�re lettre de la liste cha�n�e (cas du BACKSPACE)
*/
t_maillonPseudo* retraitLettrePseudo(t_maillonPseudo *liste)
{
    t_maillonPseudo *ptc; //Permet le parcours de la liste cha�n�e
    t_maillonPseudo *ptp; //Permet le parcours de la liste cha�n�e

    if(liste->suivant == NULL) //Cas de la liste contenant un maillon
    {
        free(liste); //Lib�ration de la liste

        return NULL;
    }
    else
    {
        //Processus de parcours de la liste jusqu'au dernier maillon
        ptc = liste;
        ptp = liste;

        while(ptc->suivant != NULL)
        {
            ptp = ptc;
            ptc = ptc->suivant;
        }

        //Processus de suppression du dernier maillon
        ptp->suivant = NULL;
        free(ptc);

        return liste;
    }
}


/*
Sous-programme d'affichage de la liste cha�n�e contenant le pseudo
*/
void affichageListePseudo(t_maillonPseudo *liste, int indiceLettre, BITMAP *doubleBuffer, FONT *policePseudo)
{
    t_maillonPseudo *ptc; //Permet le parcours de la liste cha�n�e contenant le pseudo
    int tailleLettre = 25; //Permet de d�terminer la taille d'une lettre / de la police
    int positionDepart = SCREEN_W/2 - (indiceLettre * (tailleLettre/2)); //Permet de centrer le mot compos� des lettres contenues dans chaque maillon de la liste

    //Processus de parcours et d'affichage de chaque lettre contenue dans la liste
    ptc = liste;

    while(ptc != NULL)
    {
        textprintf_ex(doubleBuffer, policePseudo, positionDepart, 300, makecol(255,255,255), -1, "%c", ptc->lettre); //Affichage de la lettre
        positionDepart += tailleLettre; //D�calage de la prochaine lettre pour centrer le mot
        ptc = ptc->suivant; //Passage au maillon suivant;
    }
}


/*
Sous-programme de conversion de la liste cha�n�e en cha�ne dynamique de caract�res
*/
char* ListeVersChaineCar(t_maillonPseudo *liste, int indiceLettre, int *erreur)
{
    int i; //Permet le bon fonctionnement de la boucle de remplissage de la cha�ne dynamique de caract�res
    char *chaineCar; //Permet de cr�er la cha�ne dynamique de caract�res qui va accueillir le pseudo
    t_maillonPseudo *ptc; //Permet le parcours de la liste cha�n�e contenant le pseudo

    chaineCar = (char*)malloc((indiceLettre + 1) * sizeof(char)); //Allocation dynamique pour la cha�ne dynamique de caract�res en fonction du nombre de lettres dans le mot (indiceLettre)
    if(chaineCar == NULL) //Allocation �chou�e
    {
        *erreur = 1; //Incr�mentation de l'indicateur d'erreur

        liberationListePseudo(liste); //Lib�ration de la lite cha�n�e

        return NULL;
    }
    else //Allocation r�ussie
    {
        ptc = liste; //Permet le parcours de la liste

        for(i=0 ; i<indiceLettre ; i++) //Boucle de remplissage de la cha�ne de caract�res
        {
            chaineCar[i] = ptc->lettre;
            ptc = ptc->suivant;
        }
    }

    chaineCar[indiceLettre] = '\0'; //Ajout du caract�re prouvant la nature de cha�ne de caract�re

    return chaineCar;
}


/*
Sous-programme de cr�ation du tableau de r�les
*/
char* creationTabRoles(int nbreJoueurs, int *erreur)
{
    int i;
    char *tabRoles;

    tabRoles = (char*)malloc(nbreJoueurs * sizeof(char)); //Allocation du tableau de r�les en fonction du nombre de joueurs
    if(tabRoles == NULL) //Allocation �chou�e
    {
        *erreur = 1; //Indication qu'une erreur a eu lieu
    }
    else //Allocation r�ussie
    {
        for(i=0 ; i<nbreJoueurs ; i++) //Boucle de remplissage
        {
            if(i == 0) //Injection du macareux (l'imposteur)
            {
                tabRoles[i] = 'm';
            }
            else //Injection des pingouins
            {
                tabRoles[i] = 'p';
            }
        }

        //Processus de m�lange du tableau de r�les
        melangeTabRoles(tabRoles, nbreJoueurs);
    }

    return tabRoles;
}


/*
Sous-programme de m�lange du tableau de r�les (m�lange de Fisher-Yates)
*/
void melangeTabRoles(char *tabRoles, int nbreJoueurs)
{
    int i; //Permet le bon fonctionnement de la boucle de m�lange
    int j; //permet l'�change entre deux cases du tableau
    char echange; //Permet l'�change de deux cases du tableau contenant un caract�re

    for(i=nbreJoueurs-1 ; i>=1 ; i--) //Boucle de m�lange
    {
        j = rand()%(i+1); //R�cup�ration d'une case al�atoire

        //Echange des deux cases � m�langer
        echange = tabRoles[j];
        tabRoles[j] = tabRoles[i];
        tabRoles[i] = echange;
    }
}


/*
Sous-programme de creation des BITMAPS necessaire pour le choix des logos
*/
void creationTabPP(t_logo tab[8], BITMAP *tabBIT[BIT])
{
    int i;

    //Chargement de chaque BITMAP
    tab[0].image = tabBIT[12];
    tab[1].image = tabBIT[11];
    tab[2].image = tabBIT[13];
    tab[3].image = tabBIT[14];
    tab[4].image = tabBIT[15];
    tab[5].image = tabBIT[16];
    tab[6].image = tabBIT[17];
    tab[7].image = tabBIT[18];

    //Initialisation de la variable etat
    for (i=0; i<8; i++)
    {
        tab[i].etat = 'a';
    }
}


/*
Sous-programme permettant le choix d'un logo
*/
int choixLogo(t_logo tab[8], FONT *policeTitre, FONT *policeJoueur, BITMAP * doubleBuffer, BITMAP *fondEcran, BITMAP * cnt)
{
    int done = 0; //Permet de mettre en place la boucle de d�roulement tant que le pingouin identit� n'a pas �t� choisi
    int couleur; //Permet de retourner la position du pingouin choisi dans le tableau principal de BITMAPs

    while(!done) //Boucle de d�roulement tant que le pingouin identit� n'a pas �t� choisi
    {
        clear_bitmap(doubleBuffer); //R�initialisation du double buffer

        //Mise en page habituelle
        blit(fondEcran, doubleBuffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        textprintf_centre_ex(doubleBuffer, policeTitre, 750, 30, makecol(255,255,0), -1, "LE MACAREUX");
        textprintf_centre_ex(doubleBuffer, policeJoueur, 750, 590, makecol(255,255,255), -1, "CHOISSISSEZ VOTRE LOGO");

        // CHOIX DU PINGOUIN //

        if (tab[0].etat == 'a') //Le pingouin cyan n'a pas encore �t� pris
        {
            draw_sprite(doubleBuffer, tab[0].image, 350, 170); //Affichage du pingouin cyan
            if (mouse_x >=350 && mouse_x<=460 && mouse_y >= 170 && mouse_y <= 300) //La souris passe sur le pingouin cyan
            {
                //Affichage du contour selon la condition
                draw_sprite(doubleBuffer, cnt, 350, 170);
                draw_sprite(doubleBuffer, tab[0].image, 350, 170);

                if (mouse_b & 1) //Clic sur le pingouin cyan
                {
                    tab[0].etat = 'z'; //Indication que le pingouin est pris

                    couleur = 12; //Le pingouin cyan est en tabBIT[12] dans le tableau principal des BITMAPs
                    done = 1; //Indication que le pingouin identit� a �t� choisi
                }
            }
        }

        //M�me principe pour le reste des pingouins disponibles

        if (tab[1].etat == 'a')
        {
            draw_sprite(doubleBuffer, tab[1].image, 570, 170);
            if (mouse_x >=570 && mouse_x<=680 && mouse_y >= 170 && mouse_y <= 300)
            {
                draw_sprite(doubleBuffer, cnt, 570, 170);
                draw_sprite(doubleBuffer, tab[1].image, 570, 170);
                if (mouse_b & 1)
                {
                    tab[1].etat = 'z';

                    couleur = 11;
                    done = 1;
                }

            }
        }
        if (tab[2].etat == 'a')
        {
            draw_sprite(doubleBuffer, tab[2].image, 790, 170);
            if (mouse_x >=790 && mouse_x<=900 && mouse_y >= 170 && mouse_y <= 300)
            {
                draw_sprite(doubleBuffer, cnt, 790, 170);
                draw_sprite(doubleBuffer, tab[2].image, 790, 170);
                if (mouse_b & 1)
                {
                    tab[2].etat = 'z';

                    couleur = 13;
                    done = 1;
                }

            }
        }
        if (tab[3].etat == 'a')
        {
            draw_sprite(doubleBuffer, tab[3].image, 1010, 170);
            if (mouse_x >=1010 && mouse_x<=1120 && mouse_y >= 170 && mouse_y <= 300)
            {
                draw_sprite(doubleBuffer, cnt, 1010, 170);
                draw_sprite(doubleBuffer, tab[3].image, 1010, 170);
                if (mouse_b & 1)
                {
                    tab[3].etat = 'z';

                    couleur = 14;
                    done = 1;
                }

            }
        }
        if (tab[4].etat == 'a')
        {
            draw_sprite(doubleBuffer, tab[4].image, 350, 370);
            if (mouse_x >=350 && mouse_x<=460 && mouse_y >= 370 && mouse_y <= 500)
            {
                draw_sprite(doubleBuffer, cnt, 350, 370);
                draw_sprite(doubleBuffer, tab[4].image, 350, 370);
                if (mouse_b & 1)
                {
                    tab[4].etat = 'z';

                    couleur = 15;
                    done = 1;
                }

            }
        }
        if (tab[5].etat == 'a')
        {
            draw_sprite(doubleBuffer, tab[5].image, 570, 370);
            if (mouse_x >=570 && mouse_x<=680 && mouse_y >= 370 && mouse_y <= 500)
            {
                draw_sprite(doubleBuffer, cnt, 570, 370);
                draw_sprite(doubleBuffer, tab[5].image, 570, 370);
                if (mouse_b & 1)
                {
                    tab[5].etat = 'z';

                    couleur = 16;
                    done = 1;
                }

            }
        }
        if (tab[6].etat == 'a')
        {
            draw_sprite(doubleBuffer, tab[6].image, 790, 370);
            if (mouse_x >=790 && mouse_x<=900 && mouse_y >= 370 && mouse_y <= 500)
            {
                draw_sprite(doubleBuffer, cnt, 790, 370);
                draw_sprite(doubleBuffer, tab[6].image, 790, 370);
                if (mouse_b & 1)
                {
                    tab[6].etat = 'z';

                    couleur = 17;
                    done = 1;
                }

            }
        }
        if (tab[7].etat == 'a')
        {
            draw_sprite(doubleBuffer, tab[7].image, 1010, 370);
            if (mouse_x >=1010 && mouse_x<=1120 && mouse_y >= 370 && mouse_y <= 500)
            {
                draw_sprite(doubleBuffer, cnt, 1010, 370);
                draw_sprite(doubleBuffer, tab[7].image, 1010, 370);
                if (mouse_b & 1)
                {
                    tab[7].etat = 'z';

                    couleur = 18;
                    done = 1;
                }

            }
        }

        blit(doubleBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H); //Affichage du double buffer
    }

    return couleur;
}


/*
Sous-programme permettant la cr�ation de la main du joueur (programme de d�pilement de la pioche
*/
t_carte* creationMain(t_pioche *pioche, int *erreur)
{
    (pioche->indiceSommet)--; //D�cr�mentation de l'indice sommet de la pioche
    (pioche->indiceNbPlaces)++; //Incr�mentation du nombre de place de la pioche

    return pioche->deck[pioche->indiceSommet];
}
