#include "header.h"

/*
Sous-programme de récupération du numéro du fichier dans lequel enregistrer
*/
int recupNumFichierSave()
{
    FILE * fic; //Permet de gérer le fichier contenant le numéro de la sauvegarde disponible
    int recip = 0; //Permet de stocker le numéro de la sauvegarde disponible
    int recipActu; //Permet de réécrire le fichier disponible actualisé

    //Ouverture afin de récupérer le nombre de sauvegarde
    fic = fopen("sauvegardes/nombreSAVE.txt","r");
    if (fic == NULL) //Ouverture échouée
    {
        allegro_message("ERREUR\nProbleme lors de la mise en place de la save");
    }
    else //Ouverture reussie
    {
        fscanf(fic,"%d",&recip); //Récupération du numéro de la save dispo
        fclose(fic); //Fermeture du fichier de récupération du numéro de save dispo
    }

    //Réouverture du même fichier pour augmenter le numéro de la sauvegarde sipo
    fic = fopen("sauvegardes/nombreSAVE.txt","w");
    if (fic==NULL) //Ouverture échouée
    {
        allegro_message("ERREUR\nProbleme lors de la mise en place de la save");
        recip = 0; //Indication qu'une erreur a eu lieu
    }
    else //Ouverture réussie
    {
        recipActu = (recip+1)%5; //Augmentation de l'indice de save dispo
        if(recipActu == 0)
        {
            recipActu = 1;
        }

        fprintf(fic,"%d",recipActu); //Ecriture de l'indice dans le fichier
        fclose(fic); //Fermeture du fichier contenant le numéro de la save dispo
    }

    return recip;
}


/*
Sous-programme de sauvegarde des informations de la partie dans le fichier disponible précédemment trouvé
*/
int processusSave(int numFichier, t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, int manche, int ENDgame, t_carteTerrain *plateau[7][5], BITMAP *tabCasesArrivee[2][3], t_defausse defausse, t_pioche *pioche, BITMAP *tabBIT[BIT])
{
    int erreur = 0;

    FILE *fic = NULL; //Permet d'ouvrir le fichier dans lequel vont être enregistrées l'ensemble des informations de la partie

    erreur = saveInfosPartie(manche, joueurActuel, ENDgame, numFichier, fic); //Sauvegarde des infos de la partie
    if(erreur == 0) //La sauvegarde a réussi
    {
        erreur = saveInfosJoueurs(tabJoueurs, nbreJoueurs, fic, numFichier, tabBIT); //Sauvegarde des infos des joueurs
        if(erreur == 0) //Sauvegarde réussie
        {
            erreur = savePiocheDefausse(pioche, defausse, numFichier, fic, tabBIT); //Sauvegarde des données de la pioche et de la défausse
            if(erreur == 0) //Sauvegarde réussie
            {
                erreur = savePlateauEntier(plateau, tabCasesArrivee, numFichier, fic, tabBIT); //Sauvegarde des données du plateau entier
            }
        }
    }

    return erreur;
}


/*
Sous-programme ed sauvegarde des infos des joueurs
*/
int saveInfosJoueurs(t_joueur **tabJoueurs, int nbreJoueurs, FILE *fic, int numFichier, BITMAP *tabBIT[BIT])
{
    int i; //Permet le bon fonctionnement de la boucle de parcours du tableau de joueurs
    int j; //Permet le bon fonctionnement des boucles de parcours des mains et pénalités de chaque joueur
    int erreur = 0; //Permet d'indiquer qu'une erreur a eu lieu ou non
    int indiceBITMAP = -1; //Permet de récupérer l'indice d'une BITMAP dans le tableau de BITMAPs
    char nomFichier[30]; //Permet de récupérer le nom du fichier dans lequel sauvegarder les informations

    sprintf(nomFichier, "sauvegardes/infosJoueurs%d.txt", numFichier); //Récupération du nom du fichier dédié aux infos des joueurs

    fic = fopen(nomFichier, "w"); //Ouverture du fichier
    if(fic == NULL) //Erreur lors de l'ouverture
    {
        erreur = 1; //Indication qu'une erreur a eu lieu
    }
    else //Ouverture réussie
    {
        fprintf(fic, "%d\n", nbreJoueurs); //Inscription du nombre de joueurs

        for(i=0 ; i<nbreJoueurs ; i++) //Boucle de parcours du tableau de joueurs
        {
            fprintf(fic, "%s\n", tabJoueurs[i]->pseudo); //Pseudo
            fprintf(fic, "%d\n", tabJoueurs[i]->score); //Score
            fprintf(fic, "%c\n", tabJoueurs[i]->role); //Role

            indiceBITMAP = rechercheIndiceBITMAP(tabJoueurs[i]->pingouinIdentite, tabBIT);
            fprintf(fic, "%d\n", indiceBITMAP); //Pingouin identité

            indiceBITMAP = rechercheIndiceBITMAP(tabJoueurs[i]->pingouinVignette, tabBIT);
            fprintf(fic, "%d\n", indiceBITMAP); //Pingouin vignette

            for(j=0 ; j<6 ; j++) //Boucle de parcours de la main du joueur
            {
                indiceBITMAP = -1; //Réinitialisation de l'indice de la BITMAP recherchée

                if(tabJoueurs[i]->mainJoueur[j] == NULL) //La carte n'existe pas
                {
                    fprintf(fic, "-1\n"); //Indication que la BITMAP à prochainement récupérer est NULL
                }
                else //La carte existe
                {
                    fprintf(fic, "%c\n", tabJoueurs[i]->mainJoueur[j]->typeAT);
                    fprintf(fic, "%c\n", tabJoueurs[i]->mainJoueur[j]->etat);

                    if(tabJoueurs[i]->mainJoueur[j]->typeAT == 'a') //Il s'agit d'une carte action
                    {
                        fprintf(fic, "%c\n", tabJoueurs[i]->mainJoueur[j]->typeAction->action);
                        fprintf(fic, "%c\n", tabJoueurs[i]->mainJoueur[j]->typeAction->effet);

                        indiceBITMAP = rechercheIndiceBITMAP(tabJoueurs[i]->mainJoueur[j]->typeAction->visuelAvant, tabBIT);
                        fprintf(fic, "%d\n", indiceBITMAP);
                    }
                    else //Il s'agit d'une carte Terrain
                    {
                        fprintf(fic, "%s\n", tabJoueurs[i]->mainJoueur[j]->typeTerrain->typeJonction);
                        fprintf(fic, "%d\n", tabJoueurs[i]->mainJoueur[j]->typeTerrain->haut);
                        fprintf(fic, "%d\n", tabJoueurs[i]->mainJoueur[j]->typeTerrain->bas);
                        fprintf(fic, "%d\n", tabJoueurs[i]->mainJoueur[j]->typeTerrain->gauche);
                        fprintf(fic, "%d\n", tabJoueurs[i]->mainJoueur[j]->typeTerrain->droite);

                        indiceBITMAP = rechercheIndiceBITMAP(tabJoueurs[i]->mainJoueur[j]->typeTerrain->visuelAvant, tabBIT);
                        fprintf(fic, "%d\n", indiceBITMAP);
                    }
                }
            }

            for(j=0 ; j<3 ; j++)
            {
                indiceBITMAP = -1; //Réinitialisation de l'indice de la BITMAP recherchée

                if(tabJoueurs[i]->penalite[j] == NULL) //La carte n'existe pas
                {
                    fprintf(fic, "-1\n"); //Indication que la BITMAP à prochainement récupérer est NULL
                }
                else //La carte existe
                {
                    indiceBITMAP = rechercheIndiceBITMAP(tabJoueurs[i]->penalite[j], tabBIT);
                    fprintf(fic, "%d\n", indiceBITMAP);
                }
            }
        }

        fclose(fic); //Fermeture du fichier contenant les infos des joueurs
    }

    return erreur;
}


/*
Sous-programme de sauvegarde des infos de la partie
*/
int saveInfosPartie(int manche, int joueurActuel, int ENDgame, int numFichier, FILE *fic)
{
    int erreur = 0; //Permet d'indiquer qu'une erreur a eu lieu ou non
    char nomFichier[30]; //Permet de récupérer le nom du fichier dans lequel enregistrer les données

    sprintf(nomFichier, "sauvegardes/infosPartie%d.txt", numFichier); //Récupération du nom du fichier

    fic = fopen(nomFichier, "w"); //Ouverture du fichier
    if(fic == NULL) //Ouverture échouée
    {
        erreur = 1;
    }
    else //Ouverture réussie
    {
        fprintf(fic, "%d\n", manche); //Sauvegarde de la manche
        fprintf(fic, "%d\n", joueurActuel); //Sauvegarde du joueur actuel
        fprintf(fic, "%d\n", ENDgame); //Sauvegarde de l'état de END game ou non

        fclose(fic); //Fermeture du fichier
    }

    return erreur;
}


/*
Sous-programme de sauvegarde des infos sur la pioche et la défausse
*/
int savePiocheDefausse(t_pioche *pioche, t_defausse defausse, int numFichier, FILE *fic, BITMAP *tabBIT[BIT])
{
    int i;
    int erreur = 0; //Permet d'indiquer qu'une erreur a eu lieu ou non
    int indiceBITMAP = -1; //Permet de récupérer l'indice d'une BITMAP dans le tableau de BITMAPs
    char nomFichier[40]; //Permet de récupérer le nom du fichier dans lequel enregistrer les données

    sprintf(nomFichier, "sauvegardes/infosPiocheDefausse%d.txt", numFichier); //Récupération du nom du fichier

    fic = fopen(nomFichier, "w"); //Ouverture du fichier
    if(fic == NULL) //Ouverture échouée
    {
        erreur = 1;
    }
    else //Ouverture réussie
    {
        indiceBITMAP = rechercheIndiceBITMAP(defausse.imageDefausse, tabBIT); //Ecriture de la BITMAP de la défausse
        fprintf(fic, "%d\n", indiceBITMAP);

        //Infos de la pioche
        fprintf(fic, "%d\n", pioche->indiceSommet); //Indice Sommet

        //Ecriture des infos des cartes dans la pioche
        for(i=0 ; i<pioche->indiceSommet ; i++)
        {
            //Infos générales
            fprintf(fic, "%c\n", pioche->deck[i]->typeAT);
            fprintf(fic, "%c\n", pioche->deck[i]->etat);

            if(pioche->deck[i]->typeAT == 'a') //Infos en cas d'une carte de type Action
            {
                fprintf(fic, "%c\n", pioche->deck[i]->typeAction->action);
                fprintf(fic, "%c\n", pioche->deck[i]->typeAction->effet);

                indiceBITMAP = rechercheIndiceBITMAP(pioche->deck[i]->typeAction->visuelAvant, tabBIT);
                fprintf(fic, "%d\n", indiceBITMAP);

                fprintf(fic, "27\n");
            }
            else if(pioche->deck[i]->typeAT == 't') //Infos en cas d'une carte de type Terrain
            {
                fprintf(fic, "%s\n", pioche->deck[i]->typeTerrain->typeJonction);
                fprintf(fic, "%d\n", pioche->deck[i]->typeTerrain->haut);
                fprintf(fic, "%d\n", pioche->deck[i]->typeTerrain->bas);
                fprintf(fic, "%d\n", pioche->deck[i]->typeTerrain->gauche);
                fprintf(fic, "%d\n", pioche->deck[i]->typeTerrain->droite);

                indiceBITMAP = rechercheIndiceBITMAP(pioche->deck[i]->typeTerrain->visuelAvant, tabBIT);
                fprintf(fic, "%d\n", indiceBITMAP);

                fprintf(fic, "27\n");
            }

        }

        fclose(fic); //Fermeture du fichier
    }

    return erreur;
}


/*
Sous-programme de sauvegarde des informations du plateau entier
*/
int savePlateauEntier(t_carteTerrain *plateau[7][5], BITMAP *tabCasesArrivee[2][3], int numFichier, FILE *fic, BITMAP *tabBIT[BIT])
{
    int i; //Permet le bon fonctionnement de la boucle parcours horizontal des tableaux
    int j; //Permet le bon fonctionnement des boucles de parcours vertical des tableaux
    int erreur = 0; //Permet d'indiquer qu'une erreur a eu lieu ou non
    int indiceBITMAP = -1; //Permet de récupérer l'indice d'une BITMAP dans le tableau de BITMAPs
    char nomFichier[30]; //Permet de récupérer le nom du fichier dans lequel sauvegarder les informations

    sprintf(nomFichier, "sauvegardes/infosPlateau%d.txt", numFichier); //Récupération du nom du fichier dédié aux infos des joueurs

    fic = fopen(nomFichier, "w"); //Ouverture du fichier
    if(fic == NULL) //Erreur lors de l'ouverture
    {
        erreur = 1; //Indication qu'une erreur a eu lieu
    }
    else //Ouverture réussie
    {
        for(i=0 ; i<2 ; i++) //Parcours horizontal du tableau de cases d'arrivée
        {
            for(j=0 ; j<3 ; j++) //Parcours vertical du tableau de cases d'arrivée
            {
                indiceBITMAP = rechercheIndiceBITMAP(tabCasesArrivee[i][j], tabBIT); //Recherche de la BITMAP
                fprintf(fic, "%d\n", indiceBITMAP); //Affichage de l'indice de la BITMAP
            }
        }

        for(i=0 ; i<7 ; i++) //Parcours horizontal du plateau de jeu
        {
            for(j=0 ; j<5 ; j++) //Parcours vertical du plateau de jeu
            {
                if(plateau[i][j] != NULL) //La case n'est pas vide
                {
                    //Ecriture des informations de BITMAPs
                    indiceBITMAP = rechercheIndiceBITMAP(plateau[i][j]->visuelAvant, tabBIT);
                    fprintf(fic, "%d\n", indiceBITMAP);
                    fprintf(fic, "27\n");

                    fprintf(fic, "%s\n", plateau[i][j]->typeJonction); //Ecriture de la jonction

                    if(indiceBITMAP != -1) //Cas où la BITMAP existe bien
                    {
                        //Indications sur les jonctions
                        fprintf(fic, "%d\n", plateau[i][j]->haut);
                        fprintf(fic, "%d\n", plateau[i][j]->bas);
                        fprintf(fic, "%d\n", plateau[i][j]->gauche);
                        fprintf(fic, "%d\n", plateau[i][j]->droite);
                    }
                }
                else //La case est vide
                {
                    fprintf(fic, "-1\n");
                }
            }
        }

        fclose(fic);
    }

    return erreur;
}


/*
Sous-programme de recherche de l'indice d'une BITMAP au sein du tableau de BITMAPs
*/
int rechercheIndiceBITMAP(BITMAP *bitmapRecherchee, BITMAP *tabBIT[BIT])
{
    int i; //Permet le bon fonctionnement de la boucle de parcours du tableau de BITMAPs
    int indiceBITMAP = -1; //Permet de récupérer l'indice de la BITMAP recherchée dans le tableau de BITMAPs

    for(i=0 ; i<BIT ; i++) //Boucle de parcours du tableau de BITMAPs
    {
        if(indiceBITMAP == -1) //La BITMAP n'a pas encore été trouvée
        {
            if(tabBIT[i] == bitmapRecherchee) //La BITMAP recherchée correspond à celle dans la case visée du tableau de BITMAPs
            {
                indiceBITMAP = i; //Récupération de l'indice de la BITMAP dans le tableau de BITMAPs
            }
        }
    }

    return indiceBITMAP;
}
