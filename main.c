#include "header.h"

int main()
{
    srand(time(NULL)); //Permet de mettre en place l'al�atoire dans le programme

    int erreur; //Permet d'indiquer une erreur
    int choixMenu; //Permet de r�cup�rer la section du jeu suivant le menu principal
    int nbreJoueurs; //Permet de recevoir le nombre de joueurs dans une partie
    int quitter = 0; //Permet de d�terminer lorsque le joueur souhaite quitter la partie
    int choixSave = 0; //Permet de d�terminer le num�ro des fichiers de sauvegarde
    int indicMusique = 1; //Permet d'arr�ter et de relance la musique

    t_pioche pioche; //Contient la pioche du jeu
    t_joueur **tabJoueurs; //Permet de stocker les informations de l'ensemble des joueurs

    BITMAP *tabBIT[BIT]; //Contient l'ensemble des BITMAPs du jeu
    FONT *tabPolices[POLICES]; //Contient l'ensemble des polices du jeu
    SAMPLE *tabMusique[1]; //Contient la musique du menu principal

    //Initialisation des fonctionnalit�s d'Allegro
    initialisationAllegro();

    //Processus d'initialisation des polices, des BITMAPs et de la musique
    initPolicesBITmusique(tabPolices, tabBIT, tabMusique, &erreur);
    if(erreur == 1) //Erreur lors de l'initialisation des polices et des BITMAPs
    {
        allegro_message("ERREUR :\nUn probleme a eu lieu lors du chargement des polices et des BITMAPS.\nLe programme va s'arreter.");
    }
    else //Les polices et les BITMAPs ont �t� charg�es
    {
        do
        {
            play_sample(tabMusique[0], 75, 0, 1300, 0); //Lancement de la musique

            menuPrincipal(&choixMenu, tabBIT, tabPolices, &nbreJoueurs, &choixSave, &indicMusique, tabMusique); //Passage par les divers menus du jeu

            switch(choixMenu)
            {
                case 1 : //Cas de la cr�ation d'une nouvelle partie
                    creationDeck(&pioche, tabBIT, &erreur); //Cr�ation du deck de la partie

                    if(erreur == 1) //Erreur lors de la cr�ation du deck
                    {
                        allegro_message("ERREUR :\nUn probleme a eu lieu lors de la creation du deck.\nLe programme va s'arreter.");
                    }
                    else //Cr�ation du deck r�ussie, le processus de cr�ation de partie peut continuer
                    {
                        tabJoueurs = creationJoueurs(nbreJoueurs, &pioche, &erreur, tabBIT, tabPolices); //Processus de cr�ation des joueurs
                        if(erreur == 1) //Erreur lors de la cr�ation des joueurs
                        {
                            allegro_message("ERREUR :\nUn probleme a eu lieu lors de l'initialisation des joueurs\nLe programme va s'arr�ter.");
                        }
                        else //Cr�ation des joueurs effectu�e avec succ�s
                        {
                            if(indicMusique == 1) //La musique n'a pas �t� d�sactiv�e
                            {
                                stop_sample(tabMusique[0]); //La musique s'arre^te � l'entr�e dans la partie
                            }

                            partieNormale(tabJoueurs, nbreJoueurs, &pioche, tabBIT, tabPolices); //Lancement de la partie

                            liberationTabJoueurs(tabJoueurs, nbreJoueurs-1); //Lib�ration du tableau de joueur en fin de partie
                        }

                        liberationPile(&pioche, 68, 1); //Lib�ration de la pioche en fin de partie
                    }
                    break;

                case 2 : //Cas d'une reprise de partie existante

                    // NON FONCTIONNEL //
                    //tabJoueurs = chargementPartie(&nbreJoueurs, &erreur, tabBIT, choixSave);

                    //affichageCartesJoueurActuel(tabJoueurs[0], tabBIT[0]);

                    break;

                case 3 : //Le joueur d�cide de quitter le jeu
                    quitter = 1;
                    break;
            }
        }
        while(quitter == 0); //Boucle de d�roulement g�n�ral tant que le joueur n'a pas d�cid� de quitter le jeu depuis le menu principal
    }

    liberationPolicesBIT(tabPolices, tabBIT); //Lib�ration de l'ensemble des polices et des BITMAPs utilis�es dans le programmes

    printf("Toutes les liberations ont ete effectuees, merci d'avoir joue.\n");
    return 0;

}
END_OF_MAIN();

