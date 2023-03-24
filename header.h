#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>

#define POLICES 10
#define BIT 73


    ///////////////////////////////
    // DEFINITION DES STRUCTURES //
    ///////////////////////////////

/*
D�finition de la structure d'une carte de type Terrain
*/
typedef struct carteTerrain {
    int haut;
    int bas;
    int gauche;
    int droite;
    char typeJonction[3];
    BITMAP *visuelAvant;
    BITMAP *visuelArriere;
} t_carteTerrain;

/*
D�finition de la structure d'une carte de type Action
*/
typedef struct carteAction {
    char effet;
    char action;
    BITMAP *visuelAvant;
    BITMAP *visuelArriere;
} t_carteAction;

/*
D�finition de la structure d'une carte g�n�rale
*/
typedef struct carte {
    t_carteTerrain *typeTerrain;
    t_carteAction *typeAction;
    char typeAT;
    int posX;
    int posY;
    int xInit;
    int yInit;
    char etat;
} t_carte;

/*
D�finition de la structure de la pioche
*/
typedef struct pioche {
    t_carte *deck[69];
    int indiceSommet;
    int indiceNbPlaces;
    int posX;
    int poxY;
} t_pioche;

/*
D�finition de la structure d'un joueur
*/
typedef struct joueur {
    char *pseudo;
    char role;
    int score;
    int nbreCarte;
    t_carte *mainJoueur[6];
    BITMAP *penalite[3];
    BITMAP *pingouinIdentite;
    BITMAP *pingouinVignette;
} t_joueur;

/*
D�finition de la structure d'un maillon permettant la cr�ation d'un pseudo
*/
typedef struct maillonPseudo {
    char lettre;
    struct maillonPseudo *suivant;
} t_maillonPseudo;

/*
D�finition de la structure des images de profil
*/
typedef struct logo {
    BITMAP * image;
    char etat;
} t_logo;

/*
D�finition de la structure des pingouins dans l'�cran de victoire du macareux
*/
typedef struct pingouin {
    int posX;
    int posY;
    int dirX;
    int dirY;
    int etat;
    BITMAP *pingouinFace;
    BITMAP *pingouinDos;
    BITMAP *pingouinGauche;
    BITMAP *pingouinDroite;
} t_pingouin;

/*
D�finition de la structure eau dans le programme d'�cran de fin du macareux
*/
typedef struct eau {
    int posX;
    int posY;
    int dirX;
    int dirY;
} t_eau;

/*
D�finition de la structure de la d�fausse
*/
typedef struct defausse {
    int etat;
    BITMAP *imageDefausse;
} t_defausse;

/*
D�finition de la structure d'un maillon permettant la mise en place de la sauvegarde des scores
*/
typedef struct maillonScore {
    char *pseudoJoueur;
    int scoreJoueur;
    struct maillonScore *suivant;
} t_maillonScore;

    ////////////////////////////////////
    // PROTOTYPES DES SOUS-PROGRAMMES //
    ////////////////////////////////////

/*
Sous-programmes d'initialisation
*/
void initialisationAllegro(); //Initialisation Allegro
void initPolicesBITmusique(FONT *tabPolices[POLICES], BITMAP *tabBIT[BIT], SAMPLE *tabMusique[1], int *erreur); //Initialisation des polices et des BITMAPs
int initialisationPolices(FONT *tabPolices[POLICES]); //Initialisation des polices
int initialisationBITMAPS(BITMAP *tabBIT[BIT]); //Initialisation des BITMAPs

/*
Sous-programmes de destruction / lib�ration
*/
void liberationPolicesBIT(FONT *tabPolices[POLICES], BITMAP *tabBIT[BIT]); //Lib�ration des polices et des BITMAPs
void liberationPolices(FONT *tabPolices[POLICES], int etatActuel); //Lib�ration des polices
void liberationBITMAPS(BITMAP *tabBIT[BIT], int etatActuel); //Lib�ration des BITMAPs

/*
Sous-programmes de menu
*/
int menuPrincipal(int *choixMenu, BITMAP *tabBIT[BIT], FONT *tabPolices[POLICES], int *nbreJoueurs, int *choixSave, int *etatMusique, SAMPLE *tabMusique[1]); //Menu principal
void menuNewPartie (BITMAP * buffer, BITMAP * fde, FONT * fnt, FONT * Titre, int *nbreJoueurs, int *done, int *choixMenu); //menu de cr�ation de partie (demande du nombre de joueurs)
int menuReprendre (BITMAP * buffer, BITMAP * fde, FONT * fnt, FONT * Titre, int *choixMenu); //Menu de reprise d'une partie depuis une sauvegarde
void ecranNonFonctionnel(BITMAP *doubleBuffer, BITMAP *fondEcran, FONT * fnt, FONT * Titre, int numFichier); //Ecran du chargement de partie non fonctionnel
void menuRegles(BITMAP *doubleBuffer, BITMAP *regles, BITMAP *flecheHautBlanche, BITMAP *flecheHautJaune, BITMAP *flecheBasBlanche, BITMAP *flecheBasJaune, BITMAP* flecheRetourBlanche, BITMAP *flecheRetourRouge); //Menu des r�gles
void affichageScoresEnregistres(BITMAP *doubleBuffer, BITMAP *fondEcran, BITMAP *flecheHautBlanche, BITMAP *flecheHautJaune, BITMAP *flecheBasBlanche, BITMAP *flecheBasJaune, BITMAP* flecheRetourBlanche, BITMAP *flecheRetourRouge, FONT *police, FONT *titre); //Menu de l'affichage des scores
void defilementScores(int *f, BITMAP *doubleBuffer, BITMAP *flecheHautBlanche, BITMAP *flecheHautJaune, BITMAP *flecheBasBlanche, BITMAP *flecheBasJaune, int nbreJoueursEnregistres); //D�filement des scores vers le bas / haut
void scoreGraph(t_maillonScore *liste, BITMAP *doubleBuffer, FONT *police,int *y, int *f); //Affichage des scores enregistr�s

/*
Sous-programme de cr�ation du deck
*/
void creationDeck(t_pioche *pioche, BITMAP *tabBIT[BIT], int *erreur); //Processus de cr�ation du deck
void initTabCartes(t_carte *deck[69], BITMAP *tabBIT[BIT], int *erreur);  //Initialisation du tableau initial contenant les cartes
void liberationDeckInitial(t_carte *deck[69], int position); //Lib�ration du tableau � l'origine des cartes
void melangeDeckInitial(t_carte *deck[69]); //M�lange du tableau contenant les cartes
void initPioche(t_pioche *pioche, int *erreur); //Initialisation de la pioche
void liberationPile(t_pioche *pioche, int etatActuel, int cas); //Lib�ration de la pioche
void empilerPioche(t_pioche *pileCarte, t_carte *nouvo, int *erreur); //Processus d'empilement de la pioche avec les don�nes du tableau
int estPleine(t_pioche *PileCarte); //Permet de d�terminer si la pioche / pile est pleine
void affichageDeckInitial(t_carte *deck[69]); //Affichage du deck initial
void affichagePioche(t_pioche *pioche, int *erreur); //Affichage de la pioche
int estVide(t_pioche * PileCarte); //V�rification si la pioche est vide

/*
Sous-programmes de cr�ation des joueurs
*/
t_joueur** creationJoueurs(int nbreJoueurs, t_pioche *pioche, int *erreur, BITMAP *tabBIT[BIT], FONT *tabPolices[POLICES]); //Cr�ation des joueurs
void liberationTabJoueurs(t_joueur **tabJoueurs, int etatActuel); //Lib�ration du tableau dynamique contenant les joueurs en cas d'erreur
char* creationPseudo(int *erreur, int joueurActuel, t_joueur **tabJoueurs, BITMAP *doubleBuffer, BITMAP *fondEcran, FONT *policeTitre, FONT *policeJoueur, FONT *policePseudo, int numJoueur);
t_maillonPseudo* ajoutLettrePseudo(t_maillonPseudo *liste, char carac, int *erreur); //Ajout d'une lettre dans le pseudo d'un joueur
void liberationListePseudo(t_maillonPseudo *liste); //Lib�ration de la liste contenant le pseudo
t_maillonPseudo* retraitLettrePseudo(t_maillonPseudo *liste); //Retrait d'une lettre dans le pseudo d'un joueur
void affichageListePseudo(t_maillonPseudo *liste, int indiceLettre, BITMAP *doubleBuffer, FONT *policePseudo); //Affichage de la liste contenant le pseudo d'un joueur
char* ListeVersChaineCar(t_maillonPseudo *liste, int indiceLettre, int *erreur); //Conversion de la liste cha�n�e vers une cha�ne dynamique de caract�res
void creationTabPP(t_logo tab[8], BITMAP *tabBIT[BIT]); //Cr�ation du tableau contenant les pingouins identit�
int choixLogo(t_logo tab[8], FONT *policeTitre, FONT *policeJoueur, BITMAP * doubleBuffer, BITMAP *fondEcran, BITMAP * cnt); //Processus de choix du logo du joueur
t_carte* creationMain(t_pioche *pioche, int *erreur); //Cr�ation de la main du joueur
char* creationTabRoles(int nbreJoueurs, int *erreur); //Cr�ation du tableau d'affectation des r�les
void melangeTabRoles(char *tabRoles, int nbreJoueurs); //M�lange du tableau d'affectation des r�les

/*
Sous-programmes des �crans de fin
*/
void ecranGameOver(BITMAP *doubleBuffer, BITMAP *fondEcranGameOver, BITMAP *poisson1, BITMAP *poisson2, FONT *policeGameOver); //Ecran de Game Over
void textGameOver(FONT *fnt,BITMAP *doubleBuffer); //Texte de l'�cran de fin
void ecranFinPingouins(BITMAP *doubleBuffer, BITMAP *fondEcranPingouins, BITMAP *feu1, BITMAP *feu2, BITMAP *feu3, BITMAP *nuage1, BITMAP *nuage2, BITMAP *nuage3, FONT *police); //Ecran de fin des pingouins
void feuAnim(BITMAP *feu[3],BITMAP *doubleBuffer, int indicateur); //Animation du feu
void nuagesAnim(BITMAP *nuage1, BITMAP *nuage2,BITMAP *nuage3, BITMAP *doubleBuffer,int i,int j, int k); //Animation des nuages
void ecranFinMacareux(BITMAP *doubleBuffer, BITMAP *fondEcran, BITMAP *macareux, BITMAP *pingouinDos, BITMAP *pingouinDroite, BITMAP *pingouinFace, BITMAP *pingouinGauche, FONT *police); //Ecran de fin du macareux
void initPingouins(t_pingouin tabPingouins[3], BITMAP *pingouinDeDos, BITMAP *pingouinDeDroite, BITMAP *pingouinDeFace, BITMAP *pingouinDeGauche); //Initialisation des pingouins de l'�cran de fin du macareux
void eauAnim(BITMAP *screen, int i); //Animation de l'eau lorsque SPACE est press�
void actualisationTabPingouin(t_pingouin tab[3]); //Acutalisation des mouvements des pingouins
int collisionActeurs(t_pingouin pingouin1, t_pingouin pingouin2); //Gestion des collisions entre les pingouins
void actualisationPingouin(t_pingouin tabPingouins[3]); //Actualisation de chaque pingouin
void dessinTabPingouin(BITMAP *doubleBuffer, t_pingouin tab[3]); //Affichage de chaque pingouin

/*
Sous-programmes de gestion de partie
*/
void partieNormale(t_joueur **tabJoueurs, int nbreJoueurs, t_pioche *pioche, BITMAP *tabBIT[BIT], FONT *tabPolices[POLICES]); //Gestion de la partie
void initCasesArrivee(BITMAP *tab[2][3], BITMAP *tabBIT[BIT]); //Initialisation du tableau contenant les cases Arriv�e
void initPlateau(t_carteTerrain *plateau[7][5]); //Initialisation du plateau de jeu
void ecranIntermediaire(BITMAP *doubleBuffer, BITMAP *fondEcran, BITMAP *pingouinJoueur, char* pseudo, FONT *policePseudo, FONT *policeDemande, FONT *policeEntrer); //Ecran interm�diaire appelant le joueur actuel
void initPosCartes(t_carte *mainJoueur[6]); //Initialisation de la position des cartes du joueur actuel en d�but de tour
int choixAction(t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, int manche, t_pioche *pioche, BITMAP *doubleBuffer, BITMAP *plateauJeu, BITMAP *vignetteMacareux, t_defausse *defausse, BITMAP *carteDos, FONT *tabPolices[POLICES], t_carteTerrain *plateau[7][5], BITMAP *tabCartesArrivee[2][3], int ENDgame, int *passerTour, int *finPartie); //Choix de l'action � effectuer
void affichagePlateau(t_carteTerrain *plateau[7][5], BITMAP *doubleBuffer); //Affichage du plateau de jeu
void affichageCartesArrivee(BITMAP *tab[2][3], BITMAP *doubleBuffer); //Affichage des cases Arriv�e
void affichageProfilJoueurActuel(t_joueur *joueurActuel, BITMAP *doubleBuffer, BITMAP *vignetteMacareux, FONT *policeScore, FONT *policePseudo); //Affichage du profil du joueur actuel
void affichageProfilAutresJoueurs(t_joueur *autreJoueur, BITMAP *doubleBuffer, FONT *policePseudo, FONT *policeScore, int decalageBas); //Affichage du profil des autres joueurs
void affichageCartesJoueurActuel(t_joueur *joueurActuel, BITMAP *doubleBuffer); //Affichage des cartes du joueur actuel
int affichageDemandeAction(BITMAP *doubleBuffer, FONT *policeDemande, FONT *policeActions, int ENDgame, int *passerTour, int nbreJoueurs, int *finPartie); //L'utilisateur choisit l'action � effectuer
void ecranSave(t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, t_pioche *pioche, BITMAP *doubleBuffer, BITMAP *plateauJeu, BITMAP *vignetteMacareux, t_defausse *defausse, BITMAP *carteDos, FONT *tabPolices[POLICES], t_carteTerrain *plateau[7][5], BITMAP *tabCasesArrivee[2][3], int numFichier); //Ecran apr�s une sauvegarde
void demandeSuivant(t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, t_pioche *pioche, BITMAP *doubleBuffer, BITMAP *plateauJeu, BITMAP *vignetteMacareux, t_defausse *defausse, BITMAP *carteDos, FONT *tabPolices[POLICES], t_carteTerrain *plateau[7][5], BITMAP *tabCasesArrivee[2][3], int devoilementCaseArrivee, int finPartie, int passerTour); //Demande de passage au joueur suivant
void calculScoresFinPartie(t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, int caseX, int caseY, BITMAP *tabCasesArrivee[2][3], BITMAP *tabBIT[BIT]); //Calcul des scores en fin de partie
void messageErreur(int cas, int *impossible, BITMAP *doubleBuffer, FONT *policeImpossible, FONT *policeRaison);

/*
Sous-programmes de la d�fausse des cartes
*/
int defausseTroisCartes(t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, t_pioche *pioche, BITMAP *doubleBuffer, BITMAP *plateauJeu, BITMAP *vignetteMacareux, t_defausse *defausse, BITMAP *carteDos, FONT *tabPolices[POLICES], t_carteTerrain *plateau[7][5], BITMAP *tabCartesArrivee[2][3], int *ENDgame); //Processus de d�fausse de cartes
void choixCarte(t_carte *mainJoueur[6], t_carte *tabCartesChoisies[3], int *nbreCartesChoisies, int *clic, BITMAP *doubleBuffer); //Processus de s�lection / d�s�lection d'une carte � d�fausser
int demandeDefausse(int *clic, int nbreCartesChoisies, BITMAP *doubleBuffer, FONT *policeTexte); //Demande de d�fausse dans le cas o� le joueur a s�lectionn� 3 cartes

/*
Sous-programmes de "jouage" d'une carte
*/
int jouerCarte(t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, t_pioche *pioche, BITMAP *doubleBuffer, BITMAP *plateauJeu, BITMAP *vignetteMacareux, t_defausse *defausse, BITMAP *carteDos, BITMAP *tabBIT[BIT], FONT *tabPolices[POLICES], t_carteTerrain *plateau[7][5], BITMAP *tabCartesArrivee[2][3], int *devoilementCaseArrivee, int *finPartie, int *ENDgame); //Processus de "jouage" d'une carte
int carteSousSouris(t_carte* mainJoueur[6], int *diffX, int *diffY, int *clic); //Permet de savoir si une carte est s�lectionn�e ou non
void deplacementCarte(int diffX, int diffY, t_carte *carte, int *reverseCarte, int *indicReverse); //Actualisation de la position de la carte en fonction de celle de la souris et du d�calage initialement enregistr� au clic de cette derni�re
void illuminationJoueurCible(int nbreJoueurs, BITMAP *doubleBuffer); //Possible illumination d'un joueur adverse vis� par une carte Action
int distribPenaliteReparation(t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, t_carte *carteSaisie, BITMAP *tabBIT[BIT], int *message); //Application de la p�nalit� / r�paration � un autre joueur
int verifPenalite(t_joueur *joueurVise, t_carteAction *cartePenalite, BITMAP *tabBIT[BIT], int *message); //V�rification de la validit� d'une p�nalit�
int verifReparation(t_joueur *joueurVise, t_carteAction *cartePenalite, BITMAP *tabBIT[BIT], int *message); //V�rification de la validit� de la r�paration
int possessionPenalite(BITMAP *penalite[3]); //D�termination de la possession ou non d'une carte p�nalit� par le joueur actuel
void retourCarte(t_carte* mainJoueur[6]); //Retour de la carte s�lectionn�e en cas de mauvais placement
void affichageCartesJoueurActuelReverse(t_joueur *joueurActuel, int carteSaisie, int carteReverse, BITMAP *doubleBuffer, BITMAP *banquiseThReverse, BITMAP *banquiseTdReverse); //Affichage des cartes du joueur actuel avec possibilit� de reverse les cartes consid�r�es par cette fonctionnalit�

/*
Sous-programmes de placement d'une carte
*/
void recupCasePlateau(int posActuX, int posActuY, int *caseX, int *caseY); //R�cup�ration des coordonn�es de la case vis�e
void illuminationRechauffementClimatique(int *dirRechauffement, int caseX, int caseY, BITMAP *doubleBuffer, FONT *policeTexte); //Illumination des cases concern�es par le r�chauffement climatique
int placementCarte(t_carteTerrain *carteAPoser, t_carteTerrain *plateau[7][5], int caseX, int caseY, int reverseCarte, BITMAP *tabBIT[BIT]); //Placement ou non de la carte s�lectionn�e sur le plateau
int testFinPartie(t_carteTerrain *cartePosee, int caseX, int caseY, int reverseCarte); //V�rification de l'�tat de fin de partie ou non
int testRechauffementClimatique(int caseX, int caseY, int dirRechauffement, t_carteTerrain *plateau[7][5]); //V�rification de la validit� du r�chauffement climatique
void applicationRechauffementClimatique(int caseX, int caseY, int dirRechauffement, t_carteTerrain *plateau[7][5], t_defausse *defausse); //Application du r�chauffement climatique
int testMap(int mouse_x, int mouse_y, BITMAP *tabCasesArrive[2][3]); //V�rification de la validit� du placement de MAP

/*
Sous-programmes de sauvegarde des scores des joueurs
*/
void sauvegardeScores(t_joueur **tabJoueurs, int nbreJoueurs); //Processus de sauvegarde des scores dans un fichier
t_maillonScore* creationMaillon(FILE *fic); //Cr�atino d'un maillon lors de la r�cup�ration des donn�es existant d�j� dans le fichier
t_maillonScore* ajoutFin(t_maillonScore* liste, t_maillonScore* nouveau); //Ajout du nouveau maillon en fin de la liste
void destructionListe(t_maillonScore* liste); //Destruction de la liste
int ajoutScores(t_joueur **tabJoueurs, int nbreJoueurs, t_maillonScore *liste, t_maillonScore *nouveau); //Ajout du score / du joueur en fonction de sa pr� pr�sence dans le fichier
t_maillonScore* creationNouveauMaillon(char *pseudoRecu,int scoreRecu); //Cr�ation d'un maillon lors de l'ajout des donn�es de la partie
void remiseDansFichier(FILE *fic, t_maillonScore *liste); //Ecriture des informations actuali�es dans le fichier des scores

/*
Sous-programmes de sauvegarde de la partie
*/
int recupNumFichierSave(); //R�cup�ration du num�ro du fichier � ouvrir pour la sauvegarde
int processusSave(int numFichier, t_joueur **tabJoueurs, int nbreJoueurs, int joueurActuel, int manche, int ENDgame, t_carteTerrain *plateau[7][5], BITMAP *tabCasesArrivee[2][3], t_defausse defausse, t_pioche *pioche, BITMAP *tabBIT[BIT]); //Sauvegarde g�n�rale
int saveInfosJoueurs(t_joueur **tabJoueurs, int nbreJoueurs, FILE *fic, int numFichier, BITMAP *tabBIT[BIT]); //Sauvegarde des infos des joueurs
int saveInfosPartie(int manche, int joueurActuel, int ENDgame, int numFichier, FILE *fic); //Sauvegarde des informations de la partie
int savePiocheDefausse(t_pioche *pioche, t_defausse defausse, int numFichier, FILE *fic, BITMAP *tabBIT[BIT]); //Sauvegarde des infos sur la pioche et la d�fausse
int savePlateauEntier(t_carteTerrain *plateau[7][5], BITMAP *tabCasesArrivee[2][3], int numFichier, FILE *fic, BITMAP *tabBIT[BIT]); //Sauvegarde des informations du plateau entier
int rechercheIndiceBITMAP(BITMAP *bitmapRecherchee, BITMAP *tabBIT[BIT]); //Recherche de l'indice d'une BITMAP dans le tableau de BITMAPs

/*
Sous-programmes de chargement d'une partie
*/
t_joueur** chargementPartie(int *nbreJoueurs, int *erreur, BITMAP *tabBIT[BIT], int numFichier); //Chargement d'une partie depuis une sauvegrade



#endif // HEADER_H_INCLUDED
