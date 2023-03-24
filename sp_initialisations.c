#include "header.h"

/*
Sous-programme d'initialisation d'ALLEGRO
*/
void initialisationAllegro()
{
    //Initialisation d'Allegro
    allegro_init();

    //D�finition du mode graphique
    set_color_depth(desktop_color_depth());
    if((set_gfx_mode(GFX_AUTODETECT_WINDOWED,1500,750,0,0))!=0) //Erreur dans la mise en place du mode graphique
    {
        allegro_message("Probleme de mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    //Installation de la souris
    install_mouse();

    //Installation du clavier
    install_keyboard();

    show_mouse(screen); //Permet de voir la souris sur l'interface graphique

    install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL); //Installation du son
}


/*
Sous-programme d'initialisation des polices et des BITMAPs
*/
void initPolicesBITmusique(FONT *tabPolices[POLICES], BITMAP *tabBIT[BIT], SAMPLE *tabMusique[1], int *erreur)
{
    *erreur = initialisationPolices(tabPolices); //Initialisation des polices
    if(*erreur == 0) //Initialisation des polices r�ussie
    {
        *erreur = initialisationBITMAPS(tabBIT); //Initialisation des BITMAPs
        if(*erreur == 1) //Erreur lors de l'initialisation des BITMAPs
        {
            liberationPolices(tabPolices, POLICES-1); //Lib�ration des polices pr�c�demment charg�es
        }
        else //L'initialisation des BITMAPs s'est faite sans probl�me
        {
            tabMusique[0] = load_sample("audioLancement.wav"); //Chargement de la musique
            if(tabMusique[0] == NULL) //Chargement de la musique �chou�e
            {
                *erreur = 1; //Indication qu'une erreur a eu lieu
                liberationPolicesBIT(tabPolices, tabBIT); //Lib�ration des tableaux de polices et de BITMAPs
            }
        }
    }
}


/*
Sous-programme d'initialisation des polices
*/
int initialisationPolices(FONT *tabPolices[POLICES])
{
    int i; //Permet de parcourir le tableau de polices
    int erreur = 0; //Permet d'indiquer qu'une erreur s'est produite

    for(i=0 ; i<POLICES ; i++) //Parcours du tableau de polices
    {
        if(erreur == 0) //Processus r�alis� tant qu'aucune erreur d�tect�e
        {
            switch(i) //Chargement de la police en fonction de la case du tableau
            {
                case 0 :
                    tabPolices[0] = load_font("polices/04_b55.pcx", NULL, NULL);
                    break;

                case 1 :
                    tabPolices[1] = load_font("polices/pixellari24.pcx", NULL, NULL);
                    break;

                case 2 :
                    tabPolices[2] = load_font("polices/fixedsys28.pcx", NULL, NULL);
                    break;

                case 3 :
                    tabPolices[3] = load_font("polices/04_b48.pcx", NULL, NULL);
                    break;

                case 4 :
                    tabPolices[4] = load_font("polices/04_b36.pcx", NULL, NULL);
                    break;

                case 5 :
                    tabPolices[5] = load_font("polices/pixellari28.pcx", NULL, NULL);
                    break;

                case 6 :
                    tabPolices[6] = load_font("polices/pixellari20.pcx", NULL, NULL);
                    break;

                case 7 :
                    tabPolices[7] = load_font("polices/pixellari14.pcx", NULL, NULL);
                    break;

                case 8 :
                    tabPolices[8] = load_font("polices/04_b30.pcx", NULL, NULL);
                    break;

                case 9 :
                    tabPolices[9] = load_font("polices/04_b24.pcx", NULL, NULL);
                    break;
            }

            if(tabPolices[i] == NULL) //Cas o� le chargement a �chou�
            {
                erreur = 1; //Indication qu'une erreur s'est produite
                liberationPolices(tabPolices, i-1); //Lib�ration des polices pr�c�demment charg�es
            }
        }
    }

    return erreur;
}

        ////////////////////////////
        // REFERENCES DES POLICES //
        ////////////////////////////


/*
tabPolices[0] : 04_b 55 - Police du titre "LE MACAREUX5
tabPolices[1] : pixellari 24 - Police du texte du menu
tabPolices[2] : fixedsys 28 - Police du pseudo
tabPolices[3] : 04_b 48 -
tabPolices[4] : 04_b 36 - Police du Game Over
tabPolices[5] : pixellari 28 - Police de la demande dans l'�cran interm�diaire
tabPolices[6] : pixellari 20 - Police du bouton ENTER dans l'�cran interm�diaire
tabPolices[7] : pixellari 14 - Police du joueur actuel lors d'une partie
tabPolices[8] : 04_b 30 - Police du texte "MANCHE" dans la partie
tabPolices[9] : 04_b 24 - Police de la valeur de la manche dans la partie
*/


/*
Sous-programme d'initialisation des BITMAPs
*/
int initialisationBITMAPS(BITMAP *tabBIT[BIT])
{
    int i; //Permet de parcourir le tableau de BITMAPs
    int erreur = 0; //Permet d'indiquer qu'une erreur s'est produite

    for(i=0 ; i<BIT ; i++) //Parcours du tableau de BITMAPs
    {
        if(erreur == 0) //Processus r�alis� tant qu'aucune erreur d�tect�e
        {
            switch(i) //Chargement de la BITMAP en fonction de la case du tableau
            {
                case 0 :
                    tabBIT[0] = create_bitmap(SCREEN_W, SCREEN_H); //Double buffer
                    break;

                case 1 :
                    tabBIT[1] = load_bitmap("images/fondEcranMenuPrincipal.bmp", NULL); //Image de fond du menu principal
                    break;

                case 2 :
                    tabBIT[2] = load_bitmap("images/regles/reglesImage.bmp", NULL); //Image des r�gles
                    break;

                case 3 :
                    tabBIT[3] = load_bitmap("images/regles/flecheHautB.bmp", NULL); //Fl�che haute blanche des r�gles
                    break;

                case 4 :
                    tabBIT[4] = load_bitmap("images/regles/flecheHautJ.bmp", NULL); //Fl�che haute jaune des r�gles
                    break;

                case 5 :
                    tabBIT[5] = load_bitmap("images/regles/flecheBasB.bmp", NULL); //Fl�che basse blanche des r�gles
                    break;

                case 6 :
                    tabBIT[6] = load_bitmap("images/regles/flecheBasJ.bmp", NULL); //Fl�che basse jaune des r�gles
                    break;

                case 7 :
                    tabBIT[7] = load_bitmap("images/regles/flecheRetourB.bmp", NULL); //Fl�che de retur blanche des r�gles
                    break;

                case 8 :
                    tabBIT[8] = load_bitmap("images/regles/flecheRetourR.bmp", NULL); //Fl�che de retour rouge des r�gles
                    break;

                case 9 :
                    tabBIT[9] = load_bitmap("images/fondEcranMenuVide.bmp", NULL); //Fond d'�cran vide
                    break;

                case 10 :
                    tabBIT[10] = load_bitmap("images/pingouinsIdentite/pingouinPPcontour.bmp", NULL); //Contour des photos des (photos de profil) PP de pingouins
                    break;

                case 11 :
                    tabBIT[11] = load_bitmap("images/pingouinsIdentite/pingouinPPbleu.bmp", NULL); //Pingouin identit� bleu
                    break;

                case 12 :
                    tabBIT[12] = load_bitmap("images/pingouinsIdentite/pingouinPPcyan.bmp", NULL); //Pingouin identit� cyan
                    break;

                case 13 :
                    tabBIT[13] = load_bitmap("images/pingouinsIdentite/pingouinPPgris.bmp", NULL); //Pingouin identit� gris
                    break;

                case 14 :
                    tabBIT[14] = load_bitmap("images/pingouinsIdentite/pingouinPProse.bmp", NULL); //Pingouin identit� rose
                    break;

                case 15 :
                    tabBIT[15] = load_bitmap("images/pingouinsIdentite/pingouinPProuge.bmp", NULL); //Pingouin identit� rouge
                    break;

                case 16 :
                    tabBIT[16] = load_bitmap("images/pingouinsIdentite/pingouinPPvertClair.bmp", NULL); //Pingouin identit� vert clair
                    break;

                case 17 :
                    tabBIT[17] = load_bitmap("images/pingouinsIdentite/pingouinPPvertFonce.bmp", NULL); //Pingouin identit� vert fonc�
                    break;

                case 18 :
                    tabBIT[18] = load_bitmap("images/pingouinsIdentite/pingouinPPviolet.bmp", NULL); //Pingouin identit� violet
                    break;

                case 19 :
                    tabBIT[19] = load_bitmap("images/pingouinsVignettes/pingouinVignetteBleu.bmp", NULL); //Vignette pingouin bleu
                    break;

                case 20 :
                    tabBIT[20] = load_bitmap("images/pingouinsVignettes/pingouinVignetteCyan.bmp", NULL); //Vignette pingouin cyan
                    break;

                case 21 :
                    tabBIT[21] = load_bitmap("images/pingouinsVignettes/pingouinVignetteGris.bmp", NULL); //Vignette pingouin gris
                    break;

                case 22 :
                    tabBIT[22] = load_bitmap("images/pingouinsVignettes/pingouinVignetteRose.bmp", NULL); //Vignette pingouin rose
                    break;

                case 23 :
                    tabBIT[23] = load_bitmap("images/pingouinsVignettes/pingouinVignetteRouge.bmp", NULL); //Vignette pingouin rouge
                    break;

                case 24 :
                    tabBIT[24] = load_bitmap("images/pingouinsVignettes/pingouinVignetteVertClair.bmp", NULL); //Vignette pingouin vert clair
                    break;

                case 25 :
                    tabBIT[25] = load_bitmap("images/pingouinsVignettes/pingouinVignetteVertFonce.bmp", NULL); //Vignette pingouin vert fonc�
                    break;

                case 26 :
                    tabBIT[26] = load_bitmap("images/pingouinsVignettes/pingouinVignetteViolet.bmp", NULL); //Vignette pingouin violet
                    break;

                case 27 :
                    tabBIT[27] = load_bitmap("images/cartes/carteDos.bmp", NULL); //Dos de carte
                    break;

                case 28 :
                    tabBIT[28] = load_bitmap("images/cartes/cartesAction/cartesCassees/canneAPecheCassee.bmp", NULL); //Canne � p�che cass�e
                    break;

                case 29 :
                    tabBIT[29] = load_bitmap("images/cartes/cartesAction/cartesCassees/rechauffementClimatique.bmp", NULL); //R�chauffement climatique
                    break;

                case 30 :
                    tabBIT[30] = load_bitmap("images/cartes/cartesAction/cartesCassees/scieCassee.bmp", NULL); //Scie cass�e
                    break;

                case 31 :
                    tabBIT[31] = load_bitmap("images/cartes/cartesAction/cartesCassees/tousLesOutilsCasses.bmp", NULL); //Tous les outils cass�s
                    break;

                case 32 :
                    tabBIT[32] = load_bitmap("images/cartes/cartesAction/cartesCassees/traineauCasse.bmp", NULL); //Tra�neau cass�
                    break;

                case 33 :
                    tabBIT[33] = load_bitmap("images/cartes/cartesAction/cartesReparees/canneAPeche.bmp", NULL); //Canne � p�che r�par�e
                    break;

                case 34 :
                    tabBIT[34] = load_bitmap("images/cartes/cartesAction/cartesReparees/map.bmp", NULL); //Map
                    break;

                case 35 :
                    tabBIT[35] = load_bitmap("images/cartes/cartesAction/cartesReparees/scie.bmp", NULL); //Scie
                    break;

                case 36 :
                    tabBIT[36] = load_bitmap("images/cartes/cartesAction/cartesReparees/scieCanneAPeche.bmp", NULL); //Scie et canne � p�che r�par�es
                    break;

                case 37 :
                    tabBIT[37] = load_bitmap("images/cartes/cartesAction/cartesReparees/traineau.bmp", NULL); //Tra�neau r�par�
                    break;

                case 38 :
                    tabBIT[38] = load_bitmap("images/cartes/cartesAction/cartesReparees/traineauCanneAPeche.bmp", NULL); //Tra�neau et canne � p�che r�par�s
                    break;

                case 39 :
                    tabBIT[39] = load_bitmap("images/cartes/cartesAction/cartesReparees/traineauScie.bmp", NULL); //Tra�neau et scie r�par�s
                    break;

                case 40 :
                    tabBIT[40] = load_bitmap("images/cartes/cartesTerrain/carteDeplacementCroix.bmp", NULL); //Banquise en croix
                    break;

                case 41 :
                    tabBIT[41] = load_bitmap("images/cartes/cartesTerrain/carteDeplacementCulDeSac.bmp", NULL); //Banquise en cul de sac
                    break;

                case 42 :
                    tabBIT[42] = load_bitmap("images/cartes/cartesTerrain/carteDeplacementLigne.bmp", NULL); //Banquise en ligne
                    break;

                case 43 :
                    tabBIT[43] = load_bitmap("images/cartes/cartesTerrain/carteDeplacementT1.bmp", NULL); //Banquise en T1
                    break;

                case 44 :
                    tabBIT[44] = load_bitmap("images/cartes/cartesTerrain/carteDeplacementT2.bmp", NULL); //Banquise en T2
                    break;

                case 45 :
                    tabBIT[45] = load_bitmap("images/cartes/cartesPoisson/poisson1.bmp", NULL); //1 poisson
                    break;

                case 46 :
                    tabBIT[46] = load_bitmap("images/cartes/cartesPoisson/poisson2.bmp", NULL); //2 poissons
                    break;

                case 47 :
                    tabBIT[47] = load_bitmap("images/cartes/cartesPoisson/poisson3.bmp", NULL); //3 poissons
                    break;

                case 48 :
                    tabBIT[48] = load_bitmap("images/ecransFin/gameOver/fondGameOver.bmp", NULL); //Fond d'�crandu game over
                    break;

                case 49 :
                    tabBIT[49] = load_bitmap("images/ecransFin/gameOver/poissonGameOver1.bmp", NULL); //Poisson 1 du game over
                    break;

                case 50 :
                    tabBIT[50] = load_bitmap("images/ecransFin/gameOver/poissonGameOver2.bmp", NULL); //Poisson 2 du game over
                    break;

                case 51 :
                    tabBIT[51] = load_bitmap("images/ecransFin/victoirePingouins/banquiseEcranFinPingouin.bmp", NULL); //Banquise de l'�cran de fin des pingouins
                    break;

                case 52 :
                    tabBIT[52] = load_bitmap("images/ecransFin/victoirePingouins/feu1.bmp", NULL); //Feu 1
                    break;

                case 53 :
                    tabBIT[53] = load_bitmap("images/ecransFin/victoirePingouins/feu2.bmp", NULL); //Feu 2
                    break;

                case 54 :
                    tabBIT[54] = load_bitmap("images/ecransFin/victoirePingouins/feu3.bmp", NULL); //Feu 3
                    break;

                case 55 :
                    tabBIT[55] = load_bitmap("images/ecransFin/victoirePingouins/nuage1.bmp", NULL); //Nuage 1
                    break;

                case 56 :
                    tabBIT[56] = load_bitmap("images/ecransFin/victoirePingouins/nuage2.bmp", NULL); //Nuage 2
                    break;

                case 57 :
                    tabBIT[57] = load_bitmap("images/ecransFin/victoirePingouins/nuage3.bmp", NULL); //Nuage 3
                    break;

                case 58 :
                    tabBIT[58] = load_bitmap("images/ecransFin/victoireMaquareux/banquiseMacareux.bmp", NULL); //Banquise de l'�cran de fin du macareux
                    break;

                case 59 :
                    tabBIT[59] = load_bitmap("images/ecransFin/victoireMaquareux/macareux.bmp", NULL); //Macareux
                    break;

                case 60 :
                    tabBIT[60] = load_bitmap("images/ecransFin/victoireMaquareux/pingouinDos.bmp", NULL); //Pingouin de dos
                    break;

                case 61 :
                    tabBIT[61] = load_bitmap("images/ecransFin/victoireMaquareux/pingouinDroite.bmp", NULL); //Pingouin vers la droite
                    break;

                case 62 :
                    tabBIT[62] = load_bitmap("images/ecransFin/victoireMaquareux/pingouinFace.bmp", NULL); //Pingouin de face
                    break;

                case 63 :
                    tabBIT[63] = load_bitmap("images/ecransFin/victoireMaquareux/pingouinGauche.bmp", NULL); //Pingouin vers la gauche
                    break;

                case 64 :
                    tabBIT[64] = load_bitmap("images/fondEcranIntermediaire.bmp", NULL);
                    break;

                case 65 :
                    tabBIT[65] = load_bitmap("images/plateauJeu.bmp", NULL);
                    break;

                case 66 :
                    tabBIT[66] = load_bitmap("images/pingouinsVignettes/macareuxVignette.bmp", NULL);
                    break;

                case 67 :
                    tabBIT[67] = load_bitmap("images/macareux.bmp", NULL);
                    break;

                case 68 :
                    tabBIT[68] = create_bitmap(85, 115);
                    clear_to_color(tabBIT[68], makecol(255, 255, 0));
                    break;

                case 69 :
                    tabBIT[69] = load_bitmap("images/cartes/cartesTerrain/carteDeplacementT1reverse.bmp", NULL); //Banquise en T1 reverse
                    break;

                case 70 :
                    tabBIT[70] = load_bitmap("images/cartes/cartesTerrain/carteDeplacementT2reverse.bmp", NULL); //Banquise en T2 reverse
                    break;

                case 71 :
                    tabBIT[71] = load_bitmap("images/son.bmp", NULL); //Logo du son
                    break;

                case 72 :
                    tabBIT[72] = load_bitmap("images/sonCroix.bmp", NULL); //Logo du son barr�
                    break;
            }

            if(tabBIT[i] == NULL) //Cas o� le chargement a �chou�
            {
                erreur = 1; //Indication qu'une erreur a eu lieu
                liberationBITMAPS(tabBIT, i-1); //Lib�ration des BITMAPs pr�c�demment charg�es
            }
        }
    }

    return erreur;
}

        ////////////////////////////
        // REFERENCES DES BITMAPS //
        ////////////////////////////

/*
tabBIT[0] : Double buffer
tabBIT[1] : Fond d'�cran du menu
tabBIT[2] : R�gles - Image du texte
tabBIT[3] : R�gles - Fl�che haute blanche
tabBIT[4] : R�gles - Fl�che haute jaune
tabBIT[5] : R�gles - Fl�che basse blanche
tabBIT[6] : R�gles - Fl�che basse jaune
tabBIT[7] : R�gles - Fl�che de retour blanche
tabBIT[8] : R�gles - Fl�che de retour rouge
tabBIT[9] : Fond d'�cran du menu VIDE
tabBIT[10] : Pingouin identit� - Contour
tabBIT[11] : Pingouin identit� - Bleu
tabBIT[12] : Pingouin identit� - Cyan
tabBIT[13] : Pingouin identit� - Gris
tabBIT[14] : Pingouin identit� - Rose
tabBIT[15] : Pingouin identit� - Rouge
tabBIT[16] : Pingouin identit� - Vert clair
tabBIT[17] : Pingouin identit� - Vert fonc�
tabBIT[18] : Pingouin identit� - Violet
tabBIT[19] : Vignette - Bleu
tabBIT[20] : Vignette - Cyan
tabBIT[21] : Vignette - Gris
tabBIT[22] : Vignette - Rose
tabBIT[23] : Vignette - Rouge
tabBIT[24] : Vignette - Vert clair
tabBIT[25] : Vignette - Vert fonc�
tabBIT[26] : Vignette - Violet
tabBIT[27] : Carte - Dos
tabBIT[28] : Carte - Action - Cass�e - Canne � p�che
tabBIT[29] : Carte - Action - Cass�e - R�chauffement climatique
tabBIT[30] : Carte - Action - Cass�e - Scie cass�e
tabBIT[31] : Carte - Action - Cass�e - Tous les outils cass�s
tabBIT[32] : Carte - Action - Cass�e - Tra�neau
tabBIT[33] : Carte - Action - R�par�e - Canne � p�che
tabBIT[34] : Carte - Action - R�par�e - Map
tabBIT[35] : Carte - Action - R�par�e - Scie
tabBIT[36] : Carte - Action - R�par�e - Scie & canne � p�che
tabBIT[37] : Carte - Action - R�par�e - Tra�neau
tabBIT[38] : Carte - Action - R�par�e - Tra�neau & canne � p�che
tabBIT[39] : Carte - Action - R�par�e - Tra�neau & scie
tabBIT[40] : Carte - Terrain - Croix
tabBIT[41] : Carte - Terrain - Cul de sac
tabBIT[42] : Carte - Terrain - Ligne
tabBIT[43] : Carte - Terrain - T1
tabBIT[44] : Carte - Terrain - T2
tabBIT[45] : Carte - Poisson - 1
tabBIT[46] : Carte - Poisson - 2
tabBIT[47] : Carte - Poisson - 3
tabBIT[48] : Ecran de fin - Game Over - Fond d'�cran
tabBIT[49] : Ecran de fin - Game Over - Poisson 1
tabBIT[50] : Ecran de fin - Game Over - Poisson 2
tabBIT[51] : Ecran de fin - Victoire des pingouins - Banquise de l'�cran de fin
tabBIT[52] : Ecran de fin - Victoire des pingouins - Feu 1
tabBIT[53] : Ecran de fin - Victoire des pingouins - Feu 2
tabBIT[54] : Ecran de fin - Victoire des pingouins - Feu 3
tabBIT[55] : Ecran de fin - Victoire des pingouins - Nuage 1
tabBIT[56] : Ecran de fin - Victoire des pingouins - Nuage 2
tabBIT[57] : Ecran de fin - Victoire des pingouins - Nuage 3
tabBIT[58] : Ecran de fin - Victoire du maquareux - Banquise de l'�cran de fin
tabBIT[59] : Ecran de fin - Victoire du maquareux - Macareux
tabBIT[60] : Ecran de fin - Victoire du maquareux - Pingouin de dos
tabBIT[61] : Ecran de fin - Victoire du maquareux - Pingouin vers la droite
tabBIT[62] : Ecran de fin - Victoire du maquareux - Pingouin de face
tabBIT[63] : Ecran de fin - Victoire du maquareux - Pingouin vers la gauche
tabBIT[64] : Ecran interm�diaire - Fond d'�cran
tabBIT[65] : Plateau de jeu
tabBIT[66] : Vignette de macareux
tabBIT[67] : Macareux
tabBIT[68] : D�fausse
tabBIT[69] : Banquise T1 Reverse
tabBIT[70] : Banquise T2 Reverse
tabBIT[71] : Logo du son
tabBIT[72] : Logo du son barr�
*/
