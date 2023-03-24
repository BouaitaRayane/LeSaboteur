#include "header.h"

/*
Sous-programme de sauvegarde des scores dans un fichier à part afin de pouvoir les afficher depuis le menu principal
*/
void sauvegardeScores(t_joueur **tabJoueurs, int nbreJoueurs)
{
    int erreur = 0; //Permet de détecter une erreur
    t_maillonScore *nouveau = NULL; //Permet la création de nouveau maillon
    t_maillonScore *liste = NULL; //Permet la mise en place de la liste chaînée contenant les pseudos et scores des joueurs

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

        if(erreur == 0) //Les données ont bien été récupérées, le processus peut continuer
        {
            erreur = ajoutScores(tabJoueurs, nbreJoueurs, liste, nouveau); //Procédure d'ajout du score / joueur dans la liste des juoeurs ayant joué

            if(erreur == 0) //Le processus s'est passé sans problème
            {
                fic = fopen("scores.txt","w"); //Ouverture du même fichier mais en écriture afin d'y écrire la liste actualisée

                if(fic==NULL) //Problème lors de l'ouverture du fichier, le processus va s'arrêter
                {
                    allegro_message("ERREUR\nLa sauvegarde des scores n'a pas pu avoir lieu\nRaison : probleme d'ouverture de fichier en ecriture");
                }
                else //Ouverture du fichier réussie
                {
                    remiseDansFichier(fic,liste); //Ecriture des informations dans le fichier
                    fclose(fic); //Fermeture du fichier
                }
            }
            else //Une erreur a eu lieu
            {
                allegro_message("ERREUR\nLa sauvegarde des scores n'a pas pu avoir lieu\nRaison : probleme lors de l'ajout des donnees");
            }
        }
        else //Les données n'ont pas pu être récupérées, le processus va s'arrêter
        {
            allegro_message("ERREUR\nLa sauvegarde des scores n'a pas pu avoir lieu\nRaison : probleme de recuperation des donnees");
        }

        destructionListe(liste);
    }
}


/*
Sous-programme de création d'un nouveau maillon
*/
t_maillonScore* creationMaillon(FILE *fic)
{
    int i; //Permet le bon fonctionnement de la boucle de parcours de recherche du caractère de retour à la ligne
    char recup[30];
    t_maillonScore *nouveau; //Permet la création d'un nouveau maillon

    nouveau = (t_maillonScore*)malloc(sizeof(t_maillonScore)); //Allocation du nouveau maillon
    if(nouveau != NULL) //Allocation réussie
    {
        fscanf(fic, "%d ", &(nouveau->scoreJoueur)); //Récupération du score du joueur pré existant
        fgets(recup, 30, fic); //Récupération du pseudo du joueur pré existant

        for(i=0; i<strlen(recup); i++) //Suppression du caractère de retour à la ligne en fin du pseudo (problème lié à la gestion des fichiers en C)
        {
            if(recup[i] == '\n') //Caractère de retour à la ligne trouvé
            {
                recup[i] = '\0'; //Remplacement par le caractère de fin de chaîne de caractère
            }
        }

        nouveau->pseudoJoueur = (char*)malloc((strlen(recup) + 1) * sizeof(char)); //Allocation de la chaîne de caractère du pseudo du joueur
        if(nouveau->pseudoJoueur == NULL) //Allocation échouée
        {
            free(nouveau); //Libération du maillon
            nouveau = NULL; //Réinitialisation du maillon
        }
        else //Allocation réussie
        {
            strcpy(nouveau->pseudoJoueur, recup); //Copie du pseudo récupéré dans le maillon

            nouveau->suivant = NULL; //Initialisation du pointeur sur le maillon suivant
        }
    }

    return nouveau;
}


/*
Sous-programme d'ajout du maillon en fin de liste
*/
t_maillonScore* ajoutFin(t_maillonScore* liste, t_maillonScore* nouveau)
{
    t_maillonScore* ptcourant; //Mise en place du parcoureur de liste

    if(liste == NULL) //La liste est vide
    {
        return nouveau;
    }
    else //La liste n'est pas vide
    {
        //Processus de parcours de la liste en quête du dernier maillon
        ptcourant = liste;

        while(ptcourant->suivant != NULL) //Parcours de la liste
        {
            ptcourant = ptcourant->suivant;
        }

        ptcourant->suivant = nouveau; //Ajout du nouveau maillon en fin de liste

        return liste;
    }
}


/*
Sous-programme de destruction de la liste chaînée
*/
void destructionListe(t_maillonScore* liste)
{
    if(liste == NULL) //Cas d'arrêt de la récursivité
    {

    }
    else //La récursivité peut continuer
    {
        destructionListe(liste->suivant); //Récursivité

        //Suppression des éléments à supprimer
        free(liste->pseudoJoueur);
        free(liste);
    }
}


/*
Sous-programme d'ajout des scores dans la liste de scores
*/
int ajoutScores(t_joueur **tabJoueurs, int nbreJoueurs, t_maillonScore *liste, t_maillonScore *nouveau)
{
    int i; //Permet le bon fonctionnement de la boucle de parcours du tableau des joueurs présents dans la partie
    int indicateur = 0; //Permet l'indication de la pré existence d'un joueur dans le fichier des scores
    int erreur = 0; //Permet de détecter une erreur
    t_maillonScore *ptc = NULL; //Mise en place du parcoureur de liste

    for(i=0; i<nbreJoueurs; i++) //Parcours du tableau des joueurs de la partie
    {
        indicateur = 0; //Réinitialisation de l'indicateur de pré existence
        ptc = liste; //Réinitialisation du parcoureur de liste

        while(ptc != NULL)//Parcours de la liste
        {
            if(indicateur == 0) //Le joueur n'a, pour l'instant, pas encore été trouvé
            {
                if(strcmp(ptc->pseudoJoueur,tabJoueurs[i]->pseudo) == 0) //Le joueur existait déjà dans le fichier de sauvegarde des scores
                {
                    (ptc->scoreJoueur)+= tabJoueurs[i]->score; //On lui ajoute son score
                    indicateur = 1; //Indication que le joueur a été trouvé
                }
            }
            ptc = ptc->suivant; //Passage au maillon suivant
        }

        if(indicateur == 0) //Le joueur n'a pas été trouvé dans la liste, il va falloir l'ajouter
        {
            nouveau = creationNouveauMaillon(tabJoueurs[i]->pseudo,tabJoueurs[i]->score); //Création d'un nouveau maillon avec les données du nouveau joueur
            if(nouveau == NULL) //Erreur lors de la création du nouveau maillon
            {
                erreur = 1; //Indication qu'une erreur a eu lieu
            }
            else //Création du nouveau maillon réussie
            {
                liste = ajoutFin(liste,nouveau); //Ajout de ce nouveau maillon en fin de liste
            }
        }
    }

    return erreur;
}


/*
Sous-programme de création d'un nouveau maillon dans le cas où le joueur n'est pas référencé dans le fichier de sauvegarde des scores
*/
t_maillonScore* creationNouveauMaillon(char *pseudoRecu,int scoreRecu)
{
    t_maillonScore *nouveau; //Permet la création du nouveau maillon

    nouveau = (t_maillonScore*)malloc(sizeof(t_maillonScore)); //Allocation du nouveau maillon
    if(nouveau != NULL) //Allocation réussie
    {
        //nouveau->pseudoJoueur = pseudoRecu;
        nouveau->scoreJoueur = scoreRecu; //Affectation du score lié a ce même pseudo

        nouveau->pseudoJoueur = (char*)malloc((strlen(pseudoRecu) + 1)*sizeof(char)); //Allocation de la chaîne contenant le pseudo du joueur à ajouter
        {
            if(nouveau->pseudoJoueur == NULL) //Allocation échoéue
            {
                free(nouveau); //Libération du maillon
                nouveau = NULL; //Réinitialisation du maillon
            }
            else //Allocation réussie
            {
                strcpy(nouveau->pseudoJoueur,pseudoRecu); //Copie du pseudo dans le nouveau maillon
            }
        }

        nouveau->suivant = NULL; //Initialisation du pointeur sur le maillon suivant
    }

    return nouveau;
}


/*
Sous-programme d'écriture de la liste actualisée dans le fichier de scores
*/
void remiseDansFichier(FILE *fic, t_maillonScore *liste)
{
    if(liste->suivant == NULL) //Cas d'arrêt de la récursivité
    {
        fprintf(fic,"%d %s",liste->scoreJoueur,liste->pseudoJoueur); //Remise du dernier nom sans retour à la ligne cette fois-ci pour éviter les problèmes lors de la lecture
    }
    else //La récursivité peut continuer
    {
        fprintf(fic,"%d %s\n",liste->scoreJoueur,liste->pseudoJoueur); //Remise des noms dans la liste
        remiseDansFichier(fic,liste->suivant); //Récursivité
    }
}

