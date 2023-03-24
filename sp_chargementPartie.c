#include "header.h"

// NON FONCTIONNEL //

t_joueur** chargementPartie(int *nbreJoueurs, int *erreur, BITMAP *tabBIT[BIT], int numFichier)
{
    int i; //Permet le bon fonctionnement de la boucle de remplissage
    int j; //Permet le bon fonctionnement dela boucle d'initialisation des pénalités
    int m; //Permet le bon fonctionnement des boucles de suppression en cas d'erreur d'allocation
    int indiceBITMAP; //Permet de récupérer l'indice d'une BITMAP dans le tableau de BITMAPs
    int intermediaire;
    char nomFichier[30]; //Permet la récupération du nom du fichier de sauvegarde
    char recup[30];
    t_joueur **tabJoueurs; //Contient le tableau de données des joueurs
    FILE *fic = NULL; //Permet l'ouverture du fichier de sauvegarde

    sprintf(nomFichier, "sauvegardes/infosJoueurs%d.txt", numFichier); //Récupération du nom du fichier de sauvegarde

    fic = fopen(nomFichier, "r");
    if(fic == NULL)
    {
        *erreur = 1;
    }
    else
    {
        fscanf(fic, "%d", nbreJoueurs);

        tabJoueurs = (t_joueur**)malloc(*nbreJoueurs * sizeof(t_joueur*)); //Allocation de l'espace nécessaire au tableau de joueurs
        if(tabJoueurs == NULL) //Erreur d'allocation
        {
            *erreur = 1;
        }
        else //Allocation réussie
        {
            for(i=0 ; i<*nbreJoueurs ; i++) //Boucle d'allocation mémoire de chaque structure joueur du tableau
            {
                if(*erreur == 0) //Processus se réalisant tant qu'aucune erreur
                {
                    tabJoueurs[i] = (t_joueur*)malloc(sizeof(t_joueur)); //Allocation de la structure
                    if(tabJoueurs[i] == NULL) //Allocation échouée
                    {
                        *erreur = 1;
                        liberationTabJoueurs(tabJoueurs, i-1); //Processus de libération de ce qui a été précédemment créé
                    }
                }
            }

            if(*erreur == 0) //Cas où l'ensemble des allocations s'est déroulé sans erreur
            {
                for(i=0 ; i<*nbreJoueurs ; i++)
                {
                    fscanf(fic, "%s", recup);

                    tabJoueurs[i]->pseudo = (char*)malloc((strlen(recup)+1) * sizeof(char)); //Allocation de la place nécessaire au pseudo
                    if(tabJoueurs[i]->pseudo == NULL) //Allocation échouée
                    {
                        *erreur = 1;
                        liberationTabJoueurs(tabJoueurs, i-1); //Processus de libération de ce qui a été précédemment créé
                    }
                    else //Allocation réussie
                    {
                        strcpy(tabJoueurs[i]->pseudo, recup);

                        fscanf(fic, "%d %c", &(tabJoueurs[i]->score), &(tabJoueurs[i]->role)); //Score

                        fscanf(fic, "%d", &indiceBITMAP);
                        tabJoueurs[i]->pingouinIdentite = tabBIT[indiceBITMAP]; //Pingouin identité

                        fscanf(fic, "%d", &indiceBITMAP);
                        tabJoueurs[i]->pingouinVignette = tabBIT[indiceBITMAP]; //Pingouin vignette

                        for(j=0 ; j<6 ; j++) //Boucle de remplissage de la main du joueur
                        {
                            if(*erreur == 0) //Aucune erreur pour le moment
                            {
                                tabJoueurs[i]->mainJoueur[j] = (t_carte*)malloc(sizeof(t_carte)); //Allocation de la place nécessaire à une carte
                                if(tabJoueurs[i]->mainJoueur[j] == NULL) //Allocation échouée
                                {
                                    *erreur = 1; //Indication qu'une erreur a eu lieu
                                    for(m=j-1 ; m>=0 ; m--) //Libération des cartes précédemment allouées
                                    {
                                        free(tabJoueurs[i]->mainJoueur[j]); //Libération
                                    }
                                }
                                else //Allocation réussie
                                {
                                    fscanf(fic, "%d", &intermediaire);
                                    printf("%d", intermediaire);
                                    if(intermediaire != -1)
                                    {
                                        fscanf(fic, "%c", &(tabJoueurs[i]->mainJoueur[j]->typeAT));
                                        fscanf(fic, "%c", &(tabJoueurs[i]->mainJoueur[j]->etat));

                                        if(tabJoueurs[i]->mainJoueur[j]->typeAT == 'a')
                                        {

                                            fscanf(fic, "%c", &(tabJoueurs[i]->mainJoueur[j]->typeAction->action));
                                            fscanf(fic, "%c", &(tabJoueurs[i]->mainJoueur[j]->typeAction->effet));

                                            fscanf(fic, "%d", &indiceBITMAP);
                                            tabJoueurs[i]->mainJoueur[j]->typeAction->visuelAvant = tabBIT[indiceBITMAP];

                                            tabJoueurs[i]->mainJoueur[j]->typeAction->visuelArriere = tabBIT[27];
                                        }
                                        else
                                        {
                                            fscanf(fic, "%s", tabJoueurs[i]->mainJoueur[j]->typeTerrain->typeJonction);

                                            //strcpy(tabJoueurs[i]->mainJoueur[j]->typeTerrain->typeJonction, recup);

                                            fscanf(fic, "%d", &(tabJoueurs[i]->mainJoueur[j]->typeTerrain->haut));
                                            fscanf(fic, "%d", &(tabJoueurs[i]->mainJoueur[j]->typeTerrain->bas));
                                            fscanf(fic, "%d", &(tabJoueurs[i]->mainJoueur[j]->typeTerrain->gauche));
                                            fscanf(fic, "%d", &(tabJoueurs[i]->mainJoueur[j]->typeTerrain->droite));

                                            fscanf(fic, "%d", &indiceBITMAP);
                                            tabJoueurs[i]->mainJoueur[j]->typeTerrain->visuelAvant = tabBIT[indiceBITMAP];

                                            tabJoueurs[i]->mainJoueur[j]->typeTerrain->visuelArriere = tabBIT[27];
                                        }
                                    }
                                    else
                                    {
                                        tabJoueurs[i]->mainJoueur[j] = NULL;
                                    }
                                }
                            }
                        }

                        if(*erreur == 0)
                        {
                            for(j=0 ; j<3 ; j++)
                            {
                                fscanf(fic, "%d\n", &indiceBITMAP);
                                if(indiceBITMAP != -1)
                                {
                                    tabJoueurs[i]->penalite[j] = tabBIT[indiceBITMAP];
                                }
                            }
                        }
                    }
                }
            }
        }

        fclose(fic);
    }

    return tabJoueurs;
}


