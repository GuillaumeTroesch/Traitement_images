/**
 * \file main.c
 * \brief Traitement d'images
 * \author Lufau Killian & Troesch Guillaume
 * \version 0.1
 *
 * Programme de test pour l'objet de gestion des chaînes de caractères Str_t.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


struct image Lecture();
void Copie(struct image image1);
struct image Miroir(struct image image1);
struct image Inverser_Couleurs(struct image image1);
struct image Retoucher_Couleurs(struct image image1);
struct image Nuance_gris(struct image image1,int choix);
struct image Flou_pixelise(struct image image1);
struct image Contours(struct image image1);
struct image Tourner(struct image image1, int largeur1, int hauteur1, int *n);
struct image Luminosite(struct image image1);
struct image Flou_bruit(struct image image1,int intens);

int Demander_couleur();
int Demander_valeur();
int Demander_choix(char *demande, int nb_choix);
int k_max(int type);
unsigned char ***alloc_dyna(int largeur, int hauteur, int type);



struct image
{
    //**Nom
    int type;
    int largeur;
    int hauteur;
    int max_couleur;
    unsigned char ***pixels;
};

int main()
{
    struct image image1;
    int choix;
    int largeur1;
    int hauteur1;
    //On crée un pointeur sur la valeur de n, on l'initialise à 0. n sert dans la rotation

    int n = 0;

    image1 = Lecture();

    largeur1 = image1.largeur;
    hauteur1 = image1.hauteur;

    do
    {
        Copie(image1);
        printf("%d", n);
        printf("--INFOS SUR L'IMAGE--\nLargeur: %dpx | Hauteur: %dpx | Nombre de couleurs: %d  |  Pixels: %d\n\n", image1.largeur, image1.hauteur, image1.max_couleur, image1.largeur*image1.hauteur); //***
        choix = Demander_choix("\nQue voulez-vous faire ?\n1: Miroir\n2: Inverser les couleurs\n3: Retoucher les couleurs\n4: Nuancer en gris\n5: Pixeliser\n6: Detecter les contours\n7: Tourner\n8: Changer luminosite\n9: Flou Gaussien\n10: Quitter\n",10);
        switch(choix)
        {
            case 1: image1 = Miroir(image1);
                break;
            case 2: image1 = Inverser_Couleurs(image1);
                break;
            case 3: image1 = Retoucher_Couleurs(image1);
                break;
            case 4: image1 = Nuance_gris(image1,0);
                break;
            case 5: image1 = Flou_pixelise(image1);
                break;
            case 6: image1 = Contours(Flou_bruit(image1,1));
                break;
            case 7: image1 = Tourner(image1, largeur1, hauteur1, &n);
                break;
            case 8: image1 = Luminosite(image1);
                break;
            case 9: image1 = Flou_bruit(image1,0);
                break;

        }
    }while(choix != 10);

    //**Dernier message

    return 0;
}



struct image Lecture()
{
    char nom_fichier[20];
    int i, j, k;
    struct image image1;



    //Lecture du fichier
    FILE* fichier = NULL;
    while (fichier == NULL)
    {
        printf("Nom du fichier avec extension ou chemin (ex : image.ppm ou C:\\Users\\Utilisateur\\Images\\image.ppm) \n");
        printf("Si l'image est placee dans le dossier, simplement ecrire nom.extension : ");
        gets(nom_fichier);
        //PEU DANGEREUSE VU QUE C'EST CE QU'ON DEMANDE EN PREMIER?
        fichier = fopen(nom_fichier, "rb");
        //ON LIT NOTRE FICHIER EN BINAIRE
        if (fichier == NULL)
        {
            //SI POINTEUR NE POINTE SUR RIEN
            printf("Nom du fichier introuvable\n");
        }
    }
    fscanf(fichier, "P%d %d %d %d %*[\n]", &image1.type, &image1.largeur, &image1.hauteur, &image1.max_couleur); //** %*[\n]?
    //SCAN DE L'ENTETE

    image1.pixels = alloc_dyna(image1.largeur, image1.hauteur, k_max(image1.type));
    //CREATION DE LA MATRICE 3D, LA FONCTION ALLOC_DYNA CREE UNE MATRICE AUX DIMENSIONS DE L'IMAGE

    for(j=0; j<image1.hauteur; j++)
    {
        for(i=0; i<image1.largeur; i++)
        {
            for(k=0; k<k_max(image1.type); k++)
            {

                fscanf(fichier,"%c", &image1.pixels[i][j][k]);
            }
        }
    }

    fclose(fichier);
    system("cls");
    printf("-------Image correctement ouverte\n\n");
    return image1;
}



void Copie(struct image image1) //**Rapport
{
    int i, j, k;
    FILE* new_fichier=NULL;
    //**Laisser utilisateur choisir le nom
    if (image1.type == 6)
    {
        new_fichier=fopen("test.ppm", "wb+");
    }
    else if (image1.type == 5)
    {
        new_fichier=fopen("test.pgm", "wb+");
    }

    fprintf(new_fichier, "P%d\n%d %d\n%d\n",image1.type, image1.largeur, image1.hauteur, image1.max_couleur);

    for(j=0; j<image1.hauteur; j++)
    {
        for(i=0; i<image1.largeur; i++)
        {
            for(k=0; k<k_max(image1.type); k++)
            {
               fputc(image1.pixels[i][j][k], new_fichier);
            }
        }

    }

    fclose(new_fichier);


    if (image1.type == 6)
    {
        system("Irfanview.exe test.ppm");
        //SI L'IMAGE EST PPM, TYPE =6,ON OUVRE UN PPM
    }
    else if (image1.type == 5)
    {
        system("Irfanview.exe test.pgm");
        //SI L'IMAGE EST PGM, TYPE =5,ON OUVRE UN PGM
    }

}




struct image Miroir(struct image image1)
{
    int choix=0, i, j, k;
    char temp;
    //NECESSITE DE CREER UNE VARIABLE TEMP POUR SAUVEGARDER UNE CASE AVANT DE L'INTERCHANGER

    choix = Demander_choix("Creer un miroir de l'image dans quel sens ?\n1: Horizontal\n2: Vertical\n", 2);
    switch(choix)
    {
        //MIROIR HORIZONTAL
        case 1:

            for(i=0; i<image1.largeur/2; i++)
            {
                for (j=0; j<image1.hauteur; j++)
                {
                    for(k=0; k<k_max(image1.type); k++)
                    {
                        temp = image1.pixels[i][j][k];
                        image1.pixels[i][j][k] = image1.pixels[image1.largeur-i-1][j][k];
                        image1.pixels[image1.largeur-i-1][j][k] = temp;
                    }
                }
            }



            break;

        //MIROIR VERTICAL
        case 2:
            for(i=0; i<image1.largeur; i++)
            {
                for (j=0; j<image1.hauteur/2; j++)
                {
                    for(k=0; k<k_max(image1.type); k++)
                    {
                        temp = image1.pixels[i][j][k];
                        image1.pixels[i][j][k] = image1.pixels[i][image1.hauteur-j-1][k];
                        image1.pixels[i][image1.hauteur-j-1][k] = temp;
                    }
                }
            }
            break;
    }
    system("cls");
    printf("-------Miroir effectue\n\n");
    return image1;
}


struct image Inverser_Couleurs(struct image image1)
{
    int i,j,k;
    for(j=0; j<image1.hauteur; j++)
    {
        for (i=0; i<image1.largeur; i++)
        {
            for(k=0; k<k_max(image1.type); k++)
            {
                image1.pixels[i][j][k] = 255-image1.pixels[i][j][k];
            }
        }
    }
    system("cls");
    printf("-------Couleurs inversees\n\n");
    return image1;
}




struct image Retoucher_Couleurs(struct image image1)//**Rapport
{
    int choix=0;
    int couleur, valeur, i, j, k;
    choix = Demander_choix("Retouche des couleurs\n1: Ajouter une valeur\n2: Soustraire par une valeur\n3: Multiplier par une valeur\n4: Remplacer par une valeur\n", 4);
    switch(choix)
    {

        case 1: printf("Ajouter une valeur ");
            if (image1.type==6)
            {couleur = Demander_choix("a quelle couleur?\n1: Rouge\n2: Vert\n3: Bleu\n", 3) - 1;}
            else if(image1.type==5)
            {
                couleur=0; printf("\n");
                //CAS OU IMAGE PGM, ON INDIQUE AU CODE QU'ON VA MODIFIER UNIQUEMENT LES CASES [i][j][couleur] donc [i][j][0], ON A PAS LE CHOIX IL Y'A QU'UNE COMPOSANTE PAR PIXEL
            }

            valeur = Demander_valeur();
            for(j=0; j<image1.hauteur; j++)
            {
                for (i=0; i<image1.largeur; i++)
                {
                    if (image1.pixels[i][j][couleur] + valeur > 255)
                    {
                        image1.pixels[i][j][couleur] = 255;
                        //CAS OU CA DEPASSE 255, ON DOIT REGULER A 255=INTENSITE MAX
                    }
                    else if (image1.pixels[i][j][couleur] + valeur < 0 )
                    {
                        image1.pixels[i][j][couleur] = 0;
                        //MEME REGULATION SI CA DESCEND EN DESSOUS DE 0, ON MET A 0=INTENSITE MIN
                    }
                    else
                    {
                        image1.pixels[i][j][couleur] += valeur;
                        //CAS OU CA RESTE DANS L'INTERVALLE [0;255]
                    }
                }
            }
            break;



        case 2: printf("Soustraire une valeur ");
            if (image1.type==6)
            {couleur = Demander_choix("a quelle couleur?\n1: Rouge\n2: Vert\n3: Bleu\n", 3) - 1;}
            else if(image1.type==5)
            {couleur=0; printf("\n");}

            valeur = Demander_valeur();
            //FONCTION DEDIEE POUR DEMANDER UNE VALEUR

            for(j=0; j<image1.hauteur; j++)
            {
                for (i=0; i<image1.largeur; i++)
                {
                    if (image1.pixels[i][j][couleur] - valeur > 255)
                    {
                        image1.pixels[i][j][couleur] = 255;
                    }
                    else if (image1.pixels[i][j][couleur] - valeur < 0 )
                    {
                        image1.pixels[i][j][couleur] = 0;
                    }
                    else
                    {
                        image1.pixels[i][j][couleur] -= valeur;
                    }
                }
            }
            break;

        case 3: printf("Multiplier par une valeur ");
            if (image1.type==6)
            {couleur = Demander_choix("a quelle couleur?\n1: Rouge\n2: Vert\n3: Bleu\n", 3) - 1;}
            else if(image1.type==5)
            {couleur=0; printf("\n");}

            valeur = Demander_valeur();
            for(j=0; j<image1.hauteur; j++)
            {
                for (i=0; i<image1.largeur; i++)
                {
                    if (image1.pixels[i][j][couleur] * valeur > 255)
                    {
                        image1.pixels[i][j][couleur] = 255;
                    }
                    else if (image1.pixels[i][j][couleur] * valeur < 0 )
                    {
                        image1.pixels[i][j][couleur] = 0;
                    }
                    else
                    {
                        image1.pixels[i][j][couleur] *= valeur;
                    }
                }
            }
            break;

        case 4: printf("Remplacer par une constante\n");
            if (image1.type==6)
            {
                couleur = Demander_choix("Quelle couleur remplacer?\n1: Rouge\n2: Vert\n3: Bleu\n", 3) - 1;
            }
            else if(image1.type==5)
            {
                couleur=0; printf("\n");
            }

            valeur = Demander_valeur();

            for(j=0; j<image1.hauteur; j++)
            {
                for (i=0; i<image1.largeur; i++)
                {
                    image1.pixels[i][j][couleur] = valeur;
                }
            }
            break;

    }
    system("cls");
    printf("-------Couleurs retouchees\n\n");
    return image1;
}



struct image Nuance_gris(struct image image1,int choi)
{
    int i,j,k, choix;
    int tempR,tempV, tempB;
    if (image1.type == 5)
    {
        printf("L'image est deja en nuance de gris, essayez avec une image PPM\n");
        system("pause");
        system("cls");
        return image1;
    }

    if (choi==0)
    {
    choix = Demander_choix("Garder en format PPM ou convertir en format PGM ?\n1: Format PPM\n2: Format PGM\n", 2);
    }

    else
    //ICI ON UTILISE CETTE CONDITION POUR TRANSFORMER UNE PPM DIRECTEMENT EN PGM, ON APPELLE NUANCES_GRIS AVEC CHOI=2, DONC CHOIX=CHOI=2 ==> IMAGE1.TYPE=PGM (CONDITION EN DESSOUS)
    {
        choix=choi;
    }



    if(choix == 2)
    {
        image1.type = 5;
    }
    else if (choix==1)
    {
        image1.type = 6;
    }
    //PAS DE ELSE CAR IL NE CHANGE PAS SINON DONC ON NE TOUCHE PAS AU TYPE



    choix = Demander_choix("Par quel canal de couleur?\n1: Rouge \n2: Vert \n3: Bleu \n4: Moyenne des trois composantes\n", 4);


    if( (choix==1) || (choix==2) || (choix==3) )
    {
        for(j=0;j<image1.hauteur;j++)
        {
            for(i=0;i<image1.largeur;i++)
            {
                for(k=0; k<k_max(image1.type); k++)
                {
                    image1.pixels[i][j][k]=image1.pixels[i][j][choix-1];
                    //CHOIX-1 CAR DECALAGE AVEC ENTREE CLAVIER, PAR EX LE ROUGE=1 AU CLAVIER MAIS IL EST REPRESENTE DANS LA MATRICE PAR LES CASES [i][j][0]
                }
            }
        }
    }


    else
    //CAS OU L'UTILISATEUR VEUT FAIRE UNE MOYENNE DES 3 COMPOSANTES
    {
        for(j=0; j<image1.hauteur; j++)  //**Revenir là desus parce que le ppm ne se nuance pas en gris
        {
            for (i=0; i<image1.largeur; i++)
            {
                tempR=image1.pixels[i][j][0];
                tempV=image1.pixels[i][j][1];
                tempB=image1.pixels[i][j][2];
                //UTILISATION DE VARIABLES TEMPORAIRES POUR AVOIR EN MEMOIRE LES VALEURS NECESSAIRE DE [][][0] et [][][1] POUR [][][2] PAR EXEMPLE

                for(k=0;k<k_max(image1.type);k++)
                {

                    image1.pixels[i][j][k] = (tempR+tempV+tempB)/3;
                    //CHAQUE COMPOSANTE=MOYENNE DES TROIS AUTRES
                }


            }
        }
    }


    system("cls");
    printf("-------Nuance de gris effectuee\n\n");
    return image1;
}




struct image Flou_pixelise(struct image image1)
{
    int i, j, k, l, m, valeur;
    printf("Entrez le facteur de grossissement de pixelisation (plus le facteur est grand plus l'image est pixelisee)\n");
    valeur = Demander_valeur();

    for(j=0; j<image1.hauteur; j+=valeur)
    {
        for (i=0; i<image1.largeur; i+=valeur)
        {
            for(l=0; l<valeur; l++)
            {
                for(m=0; m<valeur; m++)
                {
                    for(k=0; k<k_max(image1.type); k++)
                    {
                        if(image1.largeur > i+l && image1.hauteur > j+m)
                        {
                            image1.pixels[i+l][j+m][k] = image1.pixels[i][j][k];
                        }
                    }
                }

            }
        }
    }

    system("cls");
    printf("-------Floutage effectue\n\n");
    return image1;
}





struct image Contours(struct image image1)
{
    int inter;
    printf("Intervalle de fluctuation de la detection (plus cette intervalle est proche de 0 moins il y aura de contours reperes) : ");
    scanf("%d",&inter);
    if(image1.type==6)
        {
            printf("Votre image etant en couleur, il faut d'abord la modifier pour la transformer en image PGM, choisissez par quelle methode ci dessous:\n");
            system("pause");
            image1=Nuance_gris(image1,2);
        }
    int b,c,d,e,f,g,h,l;
    //VARIABLES POUR LES PIXELS VOISINS

    int fact1,fact2,fact;
    //VARIABLES POUR LES GRADIENTS

    int i,j,valmax=0;

     for(j=1;j<image1.hauteur-1;j++)
    {
        for(i=1;i<image1.largeur-1;i++)
        {
           b=image1.pixels[i-1][j][0];
           c=image1.pixels[i][j+1][0];
           d=image1.pixels[i+1][j][0];
           e=image1.pixels[i][j-1][0];
           f=image1.pixels[i-1][j-1][0];
           g=image1.pixels[i-1][j+1][0];
           h=image1.pixels[i+1][j+1][0];
           l=image1.pixels[i+1][j-1][0];
           //CE SONT TOUS LES PIXELS VOISINS

           fact1=h+2*d+l-g-2*b-f;  //GRADX, OPERATEUR DE SOBEL EN X
           fact2=-h+l-2*c+2*e-g+f; //GRADY, OPERATEUR DE SOBEL EN Y
           fact=sqrt( pow(fact1,2)+pow(fact2,2) );  //GRADIENT TOTAL
           if(fact>valmax) {valmax=fact;}

        }
    }
    //CETTE BOUCLE A SERVI A TROUVER LE GRADIENT MAXI DE L'IMAGE, ON S'ADAPTE MIEUX

    for(j=1;j<image1.hauteur-1;j++)
    {
        //ON PARCOURT UNE IMAGE DE TAILLE (LARGEUR-1)*(HAUTEUR-1) CAR ON A PAS DE CONTOURS SUR LES BORDS
        for(i=1;i<image1.largeur-1;i++)
        {

           b=image1.pixels[i-1][j][0];
           c=image1.pixels[i][j+1][0];
           d=image1.pixels[i+1][j][0];
           e=image1.pixels[i][j-1][0];
           f=image1.pixels[i-1][j-1][0];
           g=image1.pixels[i-1][j+1][0];
           h=image1.pixels[i+1][j+1][0];
           l=image1.pixels[i+1][j-1][0];







           fact1=h+2*d+l-g-2*b-f;
           fact2=-h+l-2*c+2*e-g+f;
           fact=sqrt( pow(fact1,2)+pow(fact2,2) );

           //image1.pixels[i][j][0]=255*(fact/valmax);
           if ((fact>valmax-inter)&&(fact<valmax+inter)) {image1.pixels[i][j][0]=255;}
           else {image1.pixels[i][j][0]=0;}



        }

    }

   //AVEC CES DEUX BOUCLES ON REMPLIT LES BORDS DE NOIR
    for(i=0;i<image1.largeur;i++)
    {
        image1.pixels[i][0][0]=0;
        image1.pixels[i][image1.hauteur-1][0]=0;
    }
    for(j=0;j<image1.hauteur;j++)
    {
        image1.pixels[0][j][0]=0;
        image1.pixels[image1.largeur-1][j][0]=0;
    }
    system("cls");
    printf("-------Detection de contours effectuee\n");
    return image1;
}




struct image Tourner(struct image image1, int largeur1, int hauteur1, int *n)
{
    //On initialise la structure, le type et le nb de couleurs de l'image 2
    struct image image2;

    image2.type = image1.type;
    image2.max_couleur = image1.max_couleur;

    int i,j,k,choix = 0;

    choix = Demander_choix("Tourner dans quel sens ?\n1: 90 degres vers gauche\n2: 90 degres vers droite\n3: 45 degres vers gauche\n4: 45 degres vers droite\n", 4);

    if(choix == 1 || choix ==2) //Si on a demandé une rotation de 90°
    {
        //On inverse largeur et hauteur, on fait une allocation dynamique
        image2.largeur = image1.hauteur;
        image2.hauteur = image1.largeur;
        image2.pixels = alloc_dyna(image2.largeur, image2.hauteur, image2.type);
    }
    else if (*n%2==0) //Si on a demandé une rotation de 45° et qu'on est dans un cas où l'image deviendra penchée (pas les cas où l'image sera droite)
    {
        //On crée la nouvelle image carrée pour y inscrire l'image1 penchée de 45°
        image2.largeur = image1.largeur + image1.hauteur+1;
        image2.hauteur = image2.largeur;
        image2.pixels = alloc_dyna(image2.largeur, image2.hauteur, image2.type);
        //On remplit le tableau de noir
        for (j=0; j<image2.hauteur; j++)
        {
            for(i=0; i<image2.largeur; i++)
            {
                 for(k=0;k<k_max(image1.type);k++)
                 {
                     image2.pixels[i][j][k] = 0;
                 }
            }
        }
    }


    switch(choix)
    {
        case 1: //Si 90° à gauche
            for (j=0; j<image2.hauteur; j++)
            {
                for(i=0; i<image2.largeur; i++)
                {
                     for(k=0;k<k_max(image1.type);k++)
                     {
                        image2.pixels[i][j][k] = image1.pixels[image1.largeur-1-j][i][k];
                     }
                 }
            }
            //On modifie n
            *n+=2;
            break;
        case 2: //Si 90° à droite
            for (j=0; j<image2.hauteur; j++)
            {
                for(i=0; i<image2.largeur; i++)
                {
                    for(k=0; k<k_max(image1.type); k++)
                    {
                        image2.pixels[i][j][k] = image1.pixels[j][image1.hauteur-1-i][k];
                    }
                }
            }
            //On modifie n
            *n-=2;
            break;
        case 3: //Si 45° à gauche
            if (*n%2==0) //Si l'image1 sera penchée de 45° inscrite dans la grande image2
            {
                for (j=0; j<image1.hauteur; j++) //L'image2 est déjà initialisée précédement, on la rempli
                {
                    for(i=0; i<image1.largeur; i++)
                    {
                        for(k=0; k<k_max(image1.type); k++)
                        {
                                image2.pixels[i + j][image1.largeur - i + j][k] = image1.pixels[i][j][k];
                                image2.pixels[i + j][image1.largeur - i + j+1][k] = image1.pixels[i][j][k];
                        }
                    }
                }
            }
            if (*n%2==1|| *n%2==-1) //Si l'image sera de nouveau redressée
            {
                //On arrivera à n= 0, ou n = 2, ou n = 4, ou n = 6
                if (*n%4==1 || *n%4==-1){image2.hauteur = largeur1; //Si on arrivera à n = 2 ou n = 6
                image2.largeur = hauteur1;} //On inverse hauteur et largeur de l'image au lancement du programme
                if (*n%4==3 || *n%4==-3) {image2.hauteur = hauteur1; //Si on arrivera à n = 0 ou n = 4
                image2.largeur = largeur1;} //On reprend les même largeur et hauteur de l'image au lancement du programme
                image2.pixels = alloc_dyna(image2.largeur, image2.hauteur, image2.type);

                //On remplit complètement l'image2
                for (j=0; j<image2.hauteur; j++)
                {
                    for(i=0; i<image2.largeur; i++)
                    {
                        for(k=0; k<k_max(image1.type); k++)
                        {
                            image2.pixels[i][j][k] = image1.pixels[image2.hauteur + i -j-1][i+j+2][k];
                        }
                    }
                }
            }
            //On modifie n
            *n+=1;
            break;
        case 4: //Si 45° à droite
            if (*n%2==0) //Si l'image1 sera penchée de 45° inscrite dans la grande image2
            {
                for (j=0; j<image1.hauteur; j++) //L'image2 est déjà initialisée précédement, on la rempli
                {
                    for(i=0; i<image1.largeur; i++)
                    {
                        for(k=0; k<k_max(image1.type); k++)
                        {
                            image2.pixels[image1.hauteur + i -j][i+j][k] = image1.pixels[i][j][k];
                            image2.pixels[image1.hauteur + i -j][i+j+1][k] = image1.pixels[i][j][k];
                        }
                    }
                }
            }
            if (*n%2==1 || *n%2==-1) //Si l'image sera de nouveau redressée
            {
                //On arrivera à n= 0, ou n = 2, ou n = 4, ou n = 6
                if (*n%4==1 || *n%4==-1){image2.hauteur = hauteur1; //Si on arrivera à n = 2 ou n = 6
                image2.largeur = largeur1;} //On inverse hauteur et largeur de l'image au lancement du programme
                if (*n%4==3 || *n%4==-3) {image2.hauteur = largeur1; //Si on arrivera à n = 0 ou n = 4
                image2.largeur = hauteur1;} //On reprend les même largeur et hauteur de l'image au lancement du programme
                image2.pixels = alloc_dyna(image2.largeur, image2.hauteur, image2.type);

                //On remplit complètement l'image2
                for (j=0; j<image2.hauteur; j++)
                {
                    for(i=0; i<image2.largeur; i++)
                    {
                        for(k=0; k<k_max(image1.type); k++)
                        {
                            image2.pixels[i][j][k] = image1.pixels[i + j +1][image2.largeur - i + j][k];
                        }
                    }
                }
            }
            //On modifie n
            *n-=1;
            break;
    }

    //On modifie n s'il dépasse l'intervalle [0;8]
    if (*n<=-1) *n+=8;
    if(*n>=8) {*n-=8;}
    system("cls");
    printf("-------Rotation effectuee\n\n");

    free(image1.pixels);
    return image2;
}



struct image Luminosite(struct image image1)
{
    int i,j,k, valeur;
    printf("Entrez une valeur pour modifier la luminosite (entre -255 et 255) (negatif = plus sombre ; positif = plus clair)\n");
    valeur = Demander_valeur();
    for(j=0; j<image1.hauteur; j++)
    {
        for (i=0; i<image1.largeur; i++)
        {
            for(k=0; k<k_max(image1.type); k++)
            {
                //TOUJOURS LE MEME PROBLEME DE DEPASSEMENT DE VALEUR POSSIBLE

                if (image1.pixels[i][j][k] + valeur > 255)
                {
                    image1.pixels[i][j][k] = 255;
                }
                else if (image1.pixels[i][j][k] + valeur < 0 )
                {
                    image1.pixels[i][j][k] = 0;
                }
                else
                {
                    image1.pixels[i][j][k] += valeur;
                }

            }
        }
    }
    system("cls");
    printf("-------Luminosite modifiee\n\n");
    return image1;

}


struct image Flou_bruit(struct image image1,int intens)
{
    int i,j,k,intensite, compt1,compt2,val,comptval, choix;
    struct image image2;
    image2.type = image1.type;
    image2.max_couleur = image1.max_couleur;
    image2.largeur=image1.largeur;
    image2.hauteur=image1.hauteur;
    //L'IMAGE A LES MEMES DIMENSIONS ET EST IDENTIQUE A CELLE DE BASE

    image2.pixels = alloc_dyna(image2.largeur, image2.hauteur, image2.type);
    //ON ALLOUE LA PLACE POUR UNE DEUXIEME MATRICE

    if(intens==0)
    {
         choix=Demander_choix("1: Flou leger \n2: Flou moyen \n3: Flou intense \n4: Autre (preciser valeur)\n",4);
         //PAR DEFAUT DES PRESELECTION MAIS ON LAISSE LE CHOIX DE PRECISER MANUELLEMENT LE FLOU
         if(choix==4)
         {
             intensite=Demander_choix("INTENSITE?(Entier de 1 a 100) : ",100);
         }
         else
         {
             if(choix==1) intensite=2; //FLOU LEGER
             else if(choix==2) intensite=4;  //FLOU MOYEN
             else if(choix==3) intensite=9;   // FLOU INTENSE
             //CES VALEURS ONT ETE CHOISIES PAR NOUS MEME
         }
    }


    else {intensite=intens;}




    for(j=0;j<image2.hauteur;j++)
    {
        for(i=0;i<image2.largeur;i++)
        {

            for(k=0;k<k_max(image2.type);k++)
            {
               val=0;
               comptval=0;

                    for(compt2=-intensite;compt2<=intensite;compt2++)
                    {
                        //ON SE BALLADE SUR UN CARRE AUTOUT DU PIXEL POUR CALCULER LA MOYENNE DE TOUT CA
                        for(compt1=-intensite;compt1<=intensite;compt1++)
                        {

                            //SI CA DEPASSE HORS DU TABLEAU
                            if((i+compt1>=image2.largeur)||(i+compt1<0)||(j+compt2>=image2.hauteur)||(j+compt2<0)) {val=val;}

                            else
                            {
                                val+=image1.pixels[i+compt1][j+compt2][k];
                                comptval++;
                                //ON A UN COMPTEUR JUSTE POUR CONNAITRE LE NOMBRE DE VALEURS ET POUVOIR FAIRE LA MOYENNE, CA PEUT VARIER VU QU'ON NE PREND PAS EN COMPTE LES VAL EXT AU TABLEAU
                            }


                        }

                    }
                    image2.pixels[i][j][k]=val/comptval;
                    //MOYENNE
            }
        }

    }

    system("cls");
    printf("-------Flou applique\n\n");
    return image2;
}


int Demander_valeur()
{
    int choix;
    printf("Quelle est cette valeur?\n");
    scanf("%d", &choix);
    return choix;
}


int Demander_choix(char *demande, int nb_choix)
{
    int choix=0;
    while(choix < 1 || choix > nb_choix)
    {
        printf("%s", demande);
        scanf("%d", &choix);
        if(choix < 1 || choix > nb_choix)
        {
            printf("Choix incorrect\n");
        }
    }
    return choix;
}


unsigned char ***alloc_dyna(int largeur, int hauteur, int type)
{
    unsigned char ***pixels;
    int i,j;
    //ALLOUE LA MEMOIRE POUR LE TABLEAU
    pixels = (unsigned char***)malloc(largeur * sizeof(unsigned char**));

    if(pixels == NULL)
        //SI ERREUR
    {
        printf("Erreur\n");
        exit(EXIT_FAILURE);
    }

    for(i=0 ; i < largeur ; i++)
    {
        pixels[i] = (unsigned char**)malloc(hauteur * sizeof(unsigned char*));

        if(pixels[i] == NULL)
        {
            printf("Erreur\n");
            exit(EXIT_FAILURE);
        }

        for(j=0 ; j < hauteur ; j++)
        {
            pixels[i][j] = (unsigned char*)malloc(k_max(type) * sizeof(unsigned char));

            if(pixels[i][j] == NULL)
            {
                printf("Erreur\n");
                exit(EXIT_FAILURE);
            }

        }
    }
    return pixels;

}


int k_max(int type)
{
    if (type == 5)
    {
        return 1;
        //IMAGE PGM N'A BESOIN QUE D'UNE CASE PAR PIXEL
    }
    else if (type == 6)
    {
        return 3;
        //IMAGE PPM A BESOIN DE 3 CASES PAR PIXEL
    }
    else
    {
        return 0;
    }

}
