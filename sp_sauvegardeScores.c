#include "header.h"

/*
Sous-programme de sauvegarde des scores dans un fichier � part afin de pouvoir les afficher depuis le menu principal
*/
void sauvegardeScores(t_joueur **tabJoueurs, int nbreJoueurs)
{
    int erreur = 0; //Permet de d�tecter une erreur
    t_maillonScore *nouveau = NULL; //Permet la cr�ation de nouveau maillon
    t_maillonScore *liste = NULL; //Permet la mise en place de la liste cha�n�e contenant les pseudos et scores des joueurs

    FILE* fic = NULL; //Permet la gestion du fichier des scores

    fic = fopen("scores.txt","r"); //Ouverture du fichier afin de r�cup�rer les scores d�j� pr�sents
    if(fic==NULL) //Erreur lors de l'ouverture du fichier, simple indication que la sauvegarde n'aura pas lieu
    {
        allegro_message("ERREUR\nLa sauvegarde des scores n'a pas pu avoir lieu\nRaison : probleme d'ouverture de fichier de recuperation");
    }
    else //Ouverture du fichier r�ussie
    {
        while(!feof(fic))//On r�cup�re les donn�es du fichier dans une liste
        {
            if(erreur == 0) //Aucune erreur n'a, pour l'instant, �t� d�tect�e
            {
                nouveau = creationMaillon(fic); //R�cup�ration des donn�es int�ressantes dans un nouveau maillon
                if(nouveau == NULL) //Erreur lors de la cr�ation du nouveau maillon
                {
                    erreur = 1; //Indication qu'une erreur a eu lieu
                }
                else //Cr�ation du nouveau maillon r�ussie
                {
                    liste = ajoutFin(liste,nouveau); //Ajout du nouveau maillon dans la liste cha�n�e
                }
            }
        }
        fclose(fic); //Fermeture du fichier

        if(erreur == 0) //Les donn�es ont bien �t� r�cup�r�es, le processus peut continuer
        {
            erreur = ajoutScores(tabJoueurs, nbreJoueurs, liste, nouveau); //Proc�dure d'ajout du score / joueur dans la liste des juoeurs ayant jou�

            if(erreur == 0) //Le processus s'est pass� sans probl�me
            {
                fic = fopen("scores.txt","w"); //Ouverture du m�me fichier mais en �criture afin d'y �crire la liste actualis�e

                if(fic==NULL) //Probl�me lors de l'ouverture du fichier, le processus va s'arr�ter
                {
                    allegro_message("ERREUR\nLa sauvegarde des scores n'a pas pu avoir lieu\nRaison : probleme d'ouverture de fichier en ecriture");
                }
                else //Ouverture du fichier r�ussie
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
        else //Les donn�es n'ont pas pu �tre r�cup�r�es, le processus va s'arr�ter
        {
            allegro_message("ERREUR\nLa sauvegarde des scores n'a pas pu avoir lieu\nRaison : probleme de recuperation des donnees");
        }

        destructionListe(liste);
    }
}


/*
Sous-programme de cr�ation d'un nouveau maillon
*/
t_maillonScore* creationMaillon(FILE *fic)
{
    int i; //Permet le bon fonctionnement de la boucle de parcours de recherche du caract�re de retour � la ligne
    char recup[30];
    t_maillonScore *nouveau; //Permet la cr�ation d'un nouveau maillon

    nouveau = (t_maillonScore*)malloc(sizeof(t_maillonScore)); //Allocation du nouveau maillon
    if(nouveau != NULL) //Allocation r�ussie
    {
        fscanf(fic, "%d ", &(nouveau->scoreJoueur)); //R�cup�ration du score du joueur pr� existant
        fgets(recup, 30, fic); //R�cup�ration du pseudo du joueur pr� existant

        for(i=0; i<strlen(recup); i++) //Suppression du caract�re de retour � la ligne en fin du pseudo (probl�me li� � la gestion des fichiers en C)
        {
            if(recup[i] == '\n') //Caract�re de retour � la ligne trouv�
            {
                recup[i] = '\0'; //Remplacement par le caract�re de fin de cha�ne de caract�re
            }
        }

        nouveau->pseudoJoueur = (char*)malloc((strlen(recup) + 1) * sizeof(char)); //Allocation de la cha�ne de caract�re du pseudo du joueur
        if(nouveau->pseudoJoueur == NULL) //Allocation �chou�e
        {
            free(nouveau); //Lib�ration du maillon
            nouveau = NULL; //R�initialisation du maillon
        }
        else //Allocation r�ussie
        {
            strcpy(nouveau->pseudoJoueur, recup); //Copie du pseudo r�cup�r� dans le maillon

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
        //Processus de parcours de la liste en qu�te du dernier maillon
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
Sous-programme de destruction de la liste cha�n�e
*/
void destructionListe(t_maillonScore* liste)
{
    if(liste == NULL) //Cas d'arr�t de la r�cursivit�
    {

    }
    else //La r�cursivit� peut continuer
    {
        destructionListe(liste->suivant); //R�cursivit�

        //Suppression des �l�ments � supprimer
        free(liste->pseudoJoueur);
        free(liste);
    }
}


/*
Sous-programme d'ajout des scores dans la liste de scores
*/
int ajoutScores(t_joueur **tabJoueurs, int nbreJoueurs, t_maillonScore *liste, t_maillonScore *nouveau)
{
    int i; //Permet le bon fonctionnement de la boucle de parcours du tableau des joueurs pr�sents dans la partie
    int indicateur = 0; //Permet l'indication de la pr� existence d'un joueur dans le fichier des scores
    int erreur = 0; //Permet de d�tecter une erreur
    t_maillonScore *ptc = NULL; //Mise en place du parcoureur de liste

    for(i=0; i<nbreJoueurs; i++) //Parcours du tableau des joueurs de la partie
    {
        indicateur = 0; //R�initialisation de l'indicateur de pr� existence
        ptc = liste; //R�initialisation du parcoureur de liste

        while(ptc != NULL)//Parcours de la liste
        {
            if(indicateur == 0) //Le joueur n'a, pour l'instant, pas encore �t� trouv�
            {
                if(strcmp(ptc->pseudoJoueur,tabJoueurs[i]->pseudo) == 0) //Le joueur existait d�j� dans le fichier de sauvegarde des scores
                {
                    (ptc->scoreJoueur)+= tabJoueurs[i]->score; //On lui ajoute son score
                    indicateur = 1; //Indication que le joueur a �t� trouv�
                }
            }
            ptc = ptc->suivant; //Passage au maillon suivant
        }

        if(indicateur == 0) //Le joueur n'a pas �t� trouv� dans la liste, il va falloir l'ajouter
        {
            nouveau = creationNouveauMaillon(tabJoueurs[i]->pseudo,tabJoueurs[i]->score); //Cr�ation d'un nouveau maillon avec les donn�es du nouveau joueur
            if(nouveau == NULL) //Erreur lors de la cr�ation du nouveau maillon
            {
                erreur = 1; //Indication qu'une erreur a eu lieu
            }
            else //Cr�ation du nouveau maillon r�ussie
            {
                liste = ajoutFin(liste,nouveau); //Ajout de ce nouveau maillon en fin de liste
            }
        }
    }

    return erreur;
}


/*
Sous-programme de cr�ation d'un nouveau maillon dans le cas o� le joueur n'est pas r�f�renc� dans le fichier de sauvegarde des scores
*/
t_maillonScore* creationNouveauMaillon(char *pseudoRecu,int scoreRecu)
{
    t_maillonScore *nouveau; //Permet la cr�ation du nouveau maillon

    nouveau = (t_maillonScore*)malloc(sizeof(t_maillonScore)); //Allocation du nouveau maillon
    if(nouveau != NULL) //Allocation r�ussie
    {
        //nouveau->pseudoJoueur = pseudoRecu;
        nouveau->scoreJoueur = scoreRecu; //Affectation du score li� a ce m�me pseudo

        nouveau->pseudoJoueur = (char*)malloc((strlen(pseudoRecu) + 1)*sizeof(char)); //Allocation de la cha�ne contenant le pseudo du joueur � ajouter
        {
            if(nouveau->pseudoJoueur == NULL) //Allocation �cho�ue
            {
                free(nouveau); //Lib�ration du maillon
                nouveau = NULL; //R�initialisation du maillon
            }
            else //Allocation r�ussie
            {
                strcpy(nouveau->pseudoJoueur,pseudoRecu); //Copie du pseudo dans le nouveau maillon
            }
        }

        nouveau->suivant = NULL; //Initialisation du pointeur sur le maillon suivant
    }

    return nouveau;
}


/*
Sous-programme d'�criture de la liste actualis�e dans le fichier de scores
*/
void remiseDansFichier(FILE *fic, t_maillonScore *liste)
{
    if(liste->suivant == NULL) //Cas d'arr�t de la r�cursivit�
    {
        fprintf(fic,"%d %s",liste->scoreJoueur,liste->pseudoJoueur); //Remise du dernier nom sans retour � la ligne cette fois-ci pour �viter les probl�mes lors de la lecture
    }
    else //La r�cursivit� peut continuer
    {
        fprintf(fic,"%d %s\n",liste->scoreJoueur,liste->pseudoJoueur); //Remise des noms dans la liste
        remiseDansFichier(fic,liste->suivant); //R�cursivit�
    }
}

