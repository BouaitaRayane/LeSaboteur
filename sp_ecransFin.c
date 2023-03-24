#include "header.h"

/*
Sous-programme du Game Over
*/
void ecranGameOver(BITMAP *doubleBuffer, BITMAP *fondEcranGameOver, BITMAP *poisson1, BITMAP *poisson2, FONT *policeGameOver)
{
    int indic = 0; //Permet d'alterner entre les deux états du poisson
    int i = - poisson1->w + 400; //Permet de faire défiler le poisson de gauche à droite

    while (!key[KEY_ESC]) //Boucle de déroulement tant que la touche ESCAPE n'est pas pressée
    {
        clear_bitmap(doubleBuffer); //Réinitialisation du double buffer

        blit(fondEcranGameOver,doubleBuffer,0,0,0,0,SCREEN_W,SCREEN_H); //Affichage du fond d'écran

        textGameOver(policeGameOver,doubleBuffer); //Affichage du texte "GAME OVER"

        //Alternance de l'état du poisson
        if(indic == 1)
        {
            masked_blit(poisson1, doubleBuffer, 0,0,i,200,poisson1->w,poisson1->w);
            indic = 0; //Passage à l'état opposé
        }
        else if(indic == 0)
        {
            masked_blit(poisson2, doubleBuffer, 0,0,i,200,poisson1->w,poisson1->w);
            indic = 1; //Passage à l'état opposé
        }

        i+=10; //Augmentation de la position (en X) du poisson

        if(i == SCREEN_W-150) //Le poisson arrive au bord de l'écran, sa position doit être réinitialisée
        {
            i = - poisson1->w + 360; //Réinitialisation de la position du poisson
        }

        blit(doubleBuffer,screen,0,0,0,0,SCREEN_W,SCREEN_H); //Affichage du double buffer sur l'écran

        rest(20); //Permet de ralentir l'animation
    }
}


/*
Sous-programme d'affichage du texte "GAME OVER" sur l'écran de Game Over
*/
void textGameOver(FONT *fnt,BITMAP *doubleBuffer)
{
    //Affichage de chacune des lettres de "GAME OVER" avec une couleur différente
    textout_centre_ex(doubleBuffer,fnt,"G", 2*SCREEN_W/14,60,makecol(255,0,0),-1);
    textout_centre_ex(doubleBuffer,fnt,"A", 3*SCREEN_W/14,60,makecol(0,255,0),-1);
    textout_centre_ex(doubleBuffer,fnt,"M", 4*SCREEN_W/14,60,makecol(0,0,255),-1);
    textout_centre_ex(doubleBuffer,fnt,"E", 5*SCREEN_W/14,60,makecol(255,255,0),-1);
    textout_centre_ex(doubleBuffer,fnt,"O", 8*SCREEN_W/14,60,makecol(0,255,0),-1);
    textout_centre_ex(doubleBuffer,fnt,"V", 9*SCREEN_W/14,60,makecol(255,0,0),-1);
    textout_centre_ex(doubleBuffer,fnt,"E", 10*SCREEN_W/14,60,makecol(0,255,255),-1);
    textout_centre_ex(doubleBuffer,fnt,"R", 11*SCREEN_W/14,60,makecol(255,0,255),-1);
}


/*
Sous-programme de l'écran de fin des pingouins
*/
void ecranFinPingouins(BITMAP *doubleBuffer, BITMAP *fondEcranPingouins, BITMAP *feu1, BITMAP *feu2, BITMAP *feu3, BITMAP *nuage1, BITMAP *nuage2, BITMAP *nuage3, FONT *police)
{
    int indicateur = 0;
    int i=20,j=700,k=1400;

    BITMAP *feu[3];

    feu[0] = feu1;
    feu[1] = feu2;
    feu[2] = feu3;

    while (!key[KEY_ESC]) //Boucle de déroulement tant que la touche ESCAPE n'est pas pressée
    {
        clear_bitmap(doubleBuffer); //Réinitialisation du double buffer

        blit(fondEcranPingouins,doubleBuffer,0,0,0,0,SCREEN_W,SCREEN_H); //Affiche du fond d'écran

        //Gestion de l'animation du feu
        feuAnim(feu,doubleBuffer,indicateur);
        indicateur = (indicateur+1)%3;

        //Gestion de l'animation des nuages
        nuagesAnim(nuage1,nuage2,nuage3,doubleBuffer,i,j,k);


        i = (i+3);//Augmentation de l'indicateur i
        if(i>=SCREEN_W)//Si i arrive au bout de l'écran, i revient au début
        {
            i=0-nuage1->w;
        }
        j = (j+2);//Augmentation de l'indicateur j
        if(j>=SCREEN_W)//Si j arrive au bout de l'écran, j revient au début
        {
            j=0-nuage2->w;
        }
        k = (k+4);//Augmentation de l'indicateur k
        if(k>=SCREEN_W)//Si k arrive au bout de l'écran, k revient au début
        {
            k=0-nuage3->w;
        }

        textout_centre_ex(doubleBuffer,police,"Victoire des pingouins",SCREEN_W/2,60,makecol(255,255,0),-1);//Affichage du texte "Victoire des pingouins"

        blit(doubleBuffer,screen,0,0,0,0,SCREEN_W,SCREEN_H); //Affichage du double buffer

        rest(100); //Permet de ralentir l'ensemble des animations
    }
}


/*
Sous-programme d'animation du feu dans l'écran de fin des pingouins
*/
void feuAnim(BITMAP *feu[3],BITMAP *doubleBuffer, int indicateur)
{
    //Affichage du feu en fonction de l'indicateur d'état
    if(indicateur == 0)
    {
        masked_blit(feu[0],doubleBuffer,0,0,570,510,feu[0]->w,feu[0]->h);
    }
    else if(indicateur == 1)
    {
        masked_blit(feu[1],doubleBuffer,0,0,570,510,feu[1]->w,feu[1]->h);
    }
    else if(indicateur == 2)
    {
        masked_blit(feu[2],doubleBuffer,0,0,570,510,feu[2]->w,feu[2]->h);
    }
}


/*
Sous-programme d'animation des nuages dans l'écran de fin des pingouins
*/
void nuagesAnim(BITMAP *nuage1, BITMAP *nuage2,BITMAP *nuage3, BITMAP *doubleBuffer,int i,int j, int k)
{
    masked_blit(nuage1,doubleBuffer,0,0,i,100,nuage1->w,nuage1->h); //Nuage 1

    masked_blit(nuage2,doubleBuffer,0,0,j,160,nuage2->w,nuage2->h);//Nuage 2

    masked_blit(nuage3,doubleBuffer,0,0,k,150,nuage3->w,nuage3->h); //Nuage 3
}


/*
Sous-programme de l'écran de fin du macareux
*/
void ecranFinMacareux(BITMAP *doubleBuffer, BITMAP *fondEcran, BITMAP *macareux, BITMAP *pingouinDos, BITMAP *pingouinDroite, BITMAP *pingouinFace, BITMAP *pingouinGauche, FONT *police)
{
    int i;
    int screenX, screenY; //Permettent le scrolling de l'image de fond
    int xInit = 200;
    int yInit = 455;
    int xFinal = 500;

    t_pingouin tabPingouins[3]; //Contient les trois pingouins ayant des mouvements aléatoires
    t_eau eau; //Représente l'eau que tire le macareux

    initPingouins(tabPingouins, pingouinDos, pingouinDroite, pingouinFace, pingouinGauche); //Initialisation des informations de chaque pingouin (position, direction, etc...)

    //Initialisation de la position de l'image de fond
    screenX = 0;
    screenY = 0;

    while (!key[KEY_ESC]) //Boucle de déroulement tant que la touche ESCAPE n'est pas pressée
    {
        clear_bitmap(doubleBuffer); //Réinitialisation du double buffer

        blit(fondEcran,doubleBuffer,screenX,screenY,0,0,SCREEN_W,SCREEN_H); //Affichage du fond d'écran dans le double buffer

        if(key[KEY_SPACE]) //Pression de la touche SPACE, tir d'eau du macareux
        {
            for(i=200; i<900; i+=40) //Animation de l'eau tirée
            {
                eauAnim(screen,i);
            }
        }



        for(eau.posX=xInit ; eau.posX<xFinal ; (eau.posX)++)
        {
            eau.posY = -(eau.posX)*(eau.posX)+5*(eau.posX)+yInit;

            circlefill(screen,eau.posX,eau.posY,20,makecol(0,200,200));
        }


        masked_stretch_blit(macareux,doubleBuffer,0,0,macareux->w,macareux->h,100,400,120,120); //Affichage du macareux

        //Déplacements du macareux (scrolling de l'image de fond)
        if (key[KEY_RIGHT]) //Déplacement vers la droite
        {
            screenX+=5;
        }
        else if (key[KEY_LEFT]) //Déplacement vers la gauche
        {
            screenX-=5;
        }
        else if (key[KEY_DOWN]) //Déplacement vers le bas
        {
            screenY+=5;
        }
        else if (key[KEY_UP]) //Déplacement vers le haut
        {
            screenY-=5;
        }

        //Fixation des limites du scrolling de l'image de fond
        if (screenX < 0) //Limite à gauche
        {
            screenX = 0;
        }
        if (screenX > fondEcran->w - SCREEN_W) //Limite à droite
        {
            screenX = fondEcran->w - SCREEN_W;
        }
        if (screenY < 0) //Limite en haut
        {
            screenY = 0;
        }
        if (screenY > fondEcran->h - SCREEN_H) //Limite en bas
        {
            screenY = fondEcran->h - SCREEN_H;
        }

        //Mise en mouvement des pingouins avec prise en compte des différentes collisions possibles
        actualisationTabPingouin(tabPingouins);

        //Affichage des pingouins
        dessinTabPingouin(doubleBuffer, tabPingouins);

        textout_centre_ex(doubleBuffer,police,"Victoire du macareux",SCREEN_W/2,60,makecol(255,255,0),-1); //Affichage du texte "Victoire des pingouins"

        blit(doubleBuffer,screen,0,0,0,0,SCREEN_W,SCREEN_H); //Affichage du double buffer sur l'écran

        rest(5); //Permet de ralentir l'ensemble des animations
    }
}


/*
Sous-programme de l'initialisation des pingouins
*/
void initPingouins(t_pingouin tabPingouins[3], BITMAP *pingouinDeDos, BITMAP *pingouinDeDroite, BITMAP *pingouinDeFace, BITMAP *pingouinDeGauche)
{
    int i; //Permet le bon fonctionnement de la boucle de remplissage du tableau de pingouins

    for(i=0 ; i<3 ; i++) //Boucle de remplissage du tableau des pingouins
    {
        //Initialisation des BITMAPs
        tabPingouins[i].pingouinDroite = pingouinDeDroite;
        tabPingouins[i].pingouinDos = pingouinDeDos;
        tabPingouins[i].pingouinFace = pingouinDeFace;
        tabPingouins[i].pingouinGauche = pingouinDeGauche;

        //Initialisation de la position de chaque pingouin
        tabPingouins[i].posX = rand()%((SCREEN_W - tabPingouins[i].pingouinDroite->w)-751)+750;
        tabPingouins[i].posY = rand()%((SCREEN_H - tabPingouins[i].pingouinDroite->h)-321)+320;

        //Initialisation de la direction de chaque pingouin
        tabPingouins[i].dirX = rand()%(4)+1;
        tabPingouins[i].dirY = rand()%(4)+1;
    }
}


/*
Sous-programme de l'animation de l'eau
*/
void eauAnim(BITMAP *screen, int i)
{
    rectfill(screen,i,455,i+50,460,makecol(0,200,200)); //Affichage du rectangle représentant l'eau
}


/*
Sous-programme de déplacement des pingouins
*/
void actualisationTabPingouin(t_pingouin tab[3])
{
    int i, j; //Permettent le bon fonctionnement des boucles d'actualisation
    int cote;

    // Contrôle des collisions : si on entre en collision avec un autre
    // et que la collision tend à nous rapprocher alors on rebondit
    for(i=0; i<3; i++)
    {
        for(j=i+1; j<3; j++)
        {
            if((cote=collisionActeurs(tab[i], tab[j])))
            {
                if((cote==1 && tab[i].dirX<0) || (cote==2 && tab[i].dirX>0))
                {
                    tab[i].dirX = -tab[i].dirX;
                }
                if((cote==3 && tab[i].dirY<0) || (cote==4 && tab[i].dirY>0))
                {
                    tab[i].dirY=-tab[i].dirY;
                }
                if((cote==1 && tab[j].dirX>0) || (cote==2 && tab[j].dirX<0))
                {
                    tab[j].dirX=-tab[j].dirX;
                }
                if((cote==3 && tab[j].dirY>0) || (cote==4 && tab[j].dirY<0))
                {
                    tab[j].dirY=-tab[j].dirY;
                }
            }
        }
    }

    //Mise en mouvement des pingouins et prise en compte des bordures de leurs limites spaciales
    actualisationPingouin(tab);
}


/*
Sous-programme de détermination du côté de collision en cas de collision
*/
int collisionActeurs(t_pingouin pingouin1, t_pingouin pingouin2)
{
    int retour;
    int m[4],imin,i;

    // Calcul des marges d'intersection (pingouin2 par rapport à pingouin1)
    m[0]=pingouin2.posX - pingouin1.posX + 90; // 0: à droite
    m[1]=pingouin1.posX - pingouin2.posX+ 90; // 1: à gauche
    m[2]=pingouin2.posY - pingouin1.posY+ 110; // 2: en bas
    m[3]=pingouin1.posY - pingouin2.posY+ 110; // 3: en haut

    //Recherche de l'indice de la plus petite marge
    imin=0;
    for (i=1; i<4; i++)
    {
        if (m[i]<m[imin])
        {
            imin=i;
        }
    }

    //A priori pas de collision
    retour=0;

    // Si la plus petite marge n'est pas strictement négative
    // alors c'est qu'on a une collision et cette collision est de ce coté
    if (m[imin]>=0)
    {
        retour=imin+1;  //On retourne l'indice du coté + 1 (car 0 signifie "pas de collision")
    }

    return retour;
}


void actualisationPingouin(t_pingouin tabPingouins[3])
{
    int i; //Permet le parcours du tableau contenant les pingouins

    for(i=0 ; i<3 ; i++) //Boucle de parcours du tableau contenant les informations sur les différents pingouins
    {
        //Changements de direction possibles en cas de collision avec les limites spaciales
        if(tabPingouins[i].posX > SCREEN_W - 100) //Bordure droite de l'écran
        {
            tabPingouins[i].dirX = -rand()%(4)+1;
        }
        if(tabPingouins[i].posX  < 750) //Bordure gauche de la zone de déplacements
        {
            tabPingouins[i].dirX = rand()%(4)+1;
        }
        if(tabPingouins[i].posY > SCREEN_H - 100) //Bordure basse de l'écran
        {
            tabPingouins[i].dirY = -rand()%(4)+1;
        }
        if(tabPingouins[i].posY  < 320) //Bordure haute de la zone de déplacements
        {
            tabPingouins[i].dirY = rand()%(4)+1;
        }

        //Application de la direction à la position actuelle du pingouin
        tabPingouins[i].posX += tabPingouins[i].dirX;
        tabPingouins[i].posY += tabPingouins[i].dirY;
    }
}


/*
Sous-programme d'affichage des pingouins*/
void dessinTabPingouin(BITMAP *doubleBuffer, t_pingouin tab[3])
{
    int i; //Permet le bon fonctionnement de la boucle de parcours du tableau contenant les pingouins

    for(i=0; i<3; i++) //Boucle de parcours du tableau contenant les informations sur les différents pingouins
    {
        //Affichage de la bonne orientation du pingouin en fonction de la direction de ce dernier
        if((tab[i].dirX==1 && tab[i].dirY>=-1 && tab[i].dirY <=1) || (tab[i].dirX==2 && tab[i].dirY>=-2 && tab[i].dirY <=2) || (tab[i].dirX==3 && tab[i].dirY>=-3 && tab[i].dirY <=3) || (tab[i].dirX==4 && tab[i].dirY>=-4 && tab[i].dirY <=4)) //Orientation droite du pingouin
        {
            masked_stretch_blit(tab[i].pingouinDroite,doubleBuffer,0,0,tab[i].pingouinDroite->w,tab[i].pingouinDroite->h,tab[i].posX,tab[i].posY,90,110);
        }
        else if((tab[i].dirX==-1 && tab[i].dirY>=-1 && tab[i].dirY <=1) || (tab[i].dirX==-2 && tab[i].dirY>=-2 && tab[i].dirY <=2) || (tab[i].dirX==-3 && tab[i].dirY>=-3 && tab[i].dirY <=3) || (tab[i].dirX==-4 && tab[i].dirY>=-4 && tab[i].dirY <=4)) //Orientation gauche du pingouin
        {
            masked_stretch_blit(tab[i].pingouinGauche,doubleBuffer,0,0,tab[i].pingouinGauche->w,tab[i].pingouinGauche->h,tab[i].posX,tab[i].posY,90,110);
        }
        else if((tab[i].dirY==2 && tab[i].dirX >=-2 && tab[i].dirX<=2)||(tab[i].dirY==3 && tab[i].dirX >=-3 && tab[i].dirX<=3)||(tab[i].dirY==4 && tab[i].dirX >=-4 && tab[i].dirX<=4)) //Orientation de face du pingouin
        {
            masked_stretch_blit(tab[i].pingouinFace,doubleBuffer,0,0,tab[i].pingouinFace->w,tab[i].pingouinFace->h,tab[i].posX,tab[i].posY,90,110);
        }
        else //Orientation de dos du pingouin
        {
            masked_stretch_blit(tab[i].pingouinDos,doubleBuffer,0,0,tab[i].pingouinDos->w,tab[i].pingouinDos->h,tab[i].posX,tab[i].posY,90,110);
        }
    }
}
