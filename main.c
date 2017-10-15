#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/****************************************************************
******** PAR MOSTAFA ACHRAF LE 20 MARS 2016 - 12H : 39M, BOUZNIKA
****************************************************************/

/** ************************************************************************************************************************** **/
/** ****************************************************** STRUCTURES ******************************************************** **/
/** ************************************************************************************************************************** **/

/** La structure de la liste des distances **************************************************************************************/
typedef struct Dis
{
    char nomGroupe[16];
    float distance;
    struct Dis *suiv;
}Dis;

/** La structure des plantes ****************************************************************************************************/
typedef struct Plante
{
    float sepallength;
    float sepalwidth;
    float petallength;
    float petalwidth;
    char nom[20]; /*nom du groupe où elle appartient la plante*/
    struct Plante *next; /*next plante*/
}Plante;

/** La structure des groupes ****************************************************************************************************/
typedef struct Groupes
{
    char nomGroupe[16];
    int nbrPlProches; /*le nombre des plantes proches pardéfaut vaut 0*/
    struct Groupes *nextG; /*next groupe*/
    Plante *next; /*next plante*/
}Groupes;

/** ************************************************************************************************************************** **/
/** ****************************************************** FONCTIONS ********************************************************* **/
/** ************************************************************************************************************************** **/

/** Cette fonction test est-ce qu'un groupe est existe déjà pour éviter la redandance des groupes dans la liste des groupes *****/
int groupe_existe_deja(Groupes *l, char nomG[])
{
    Groupes *p;
    p = l;
    while(l != NULL)
    {
        if(strcmp(l->nomGroupe, nomG) == 0)
        {
            return (1);
        }
        l = l->nextG;
    }
    return (0);
}

/** Cette fonction construit une liste qui contient les noms des groupes existent et un pointeur sur la liste des plantes *******/
void CreerListeGroupes(Groupes **l, char nomG[])
{
    Groupes *temp, *p;

    temp = (Groupes*)malloc(sizeof(Groupes));

    strcpy(&temp->nomGroupe, nomG);
    temp->nextG = NULL;/*le groupe qui suit est nulle*/
    temp->next = NULL;/*la plante qui suit est nulle aussi*/
    temp->nbrPlProches = 0; /*initialiser à 0*/

    /*afficher le nom du groupe à insérer */
    printf(">>>>>>> %s\n", temp->nomGroupe);

    if(*l == NULL)
    {
        *l = temp;
    }

    else
    {
        p = *l;

        while(p->nextG != NULL)
        {
            p = p->nextG;
        }

        p->nextG = temp;
    }
}

/** Insérer une plante dans la liste **/
void InsererPlante(Groupes **tab, float sl, float sw, float pl, float pw, char nom[])
{
    Plante *elem, *p;
    Groupes *temp;
    int inserer = 0; /*la plante n'est pas encore insérer*/

    elem = (Plante*)malloc(sizeof(Plante));

    strcpy(&elem->nom, nom);
    elem->sepallength = sl;
    elem->sepalwidth = sw;
    elem->petallength = pl;
    elem->petalwidth = pw;
    elem->next = NULL;

    temp = *tab;

    /* insérer la plante */
    while(inserer == 0 && temp != NULL)
    {
        if(strcmp(nom, temp->nomGroupe) == 0)
        {
            if(temp->next == NULL)
            {
                temp->next = elem;
            }

             else
             {
                 /* p pointe sur la première plante */
                 p = temp->next;

                 while(p->next != NULL)
                 {
                     p = p->next;
                 }

                 p->next = elem;
             }
             inserer = 1; /* la plante est insérée */
        }
        temp = temp->nextG;
    }
}

/** Affichage de la liste globale qui contient tous le contenu du fichier *******************************************************/
void Afficher(Groupes *tab)
{
    Groupes *p;
    p = tab;
    Plante *temp;/*un pointeur qui va parcourir toutes les plantes*/
    while(p != NULL)
    {
        printf("\n>>>>>>>nom groupe : %s\n\n", p->nomGroupe);
        temp = p->next;
        while(temp != NULL)
        {
            printf("%.1f, %.1f, %.1f, %.1f, %s\n",temp->sepallength, temp->sepalwidth, temp->petallength, temp->petalwidth, temp->nom);
            temp = temp->next;
        }
        p = p->nextG;
    }
}

/** Extraire les informations à partir du fichier *******************************************************************************/
void ExtraireInfo_Insert(Groupes **tab, char nomFichier[])
{
    FILE *fichier = NULL;
    float sl, sw, pl, pw;
    char nom[16];

    fichier = fopen(nomFichier, "r"); /* ouvrir le fichier et pointer un pointeur au début du fichier */

    if(fichier != NULL)
    {
        /*remplir la liste des groupes*/
        while(fscanf(fichier,"%f,%f,%f,%f,Iris-%s",&sl,&sw,&pl,&pw,nom)!=EOF)
        {
            if(groupe_existe_deja(*tab, nom) == 0) /*Si ce nom de groupe n'existe pas encore on l'insére dans la liste*/
            {
                CreerListeGroupes(tab, nom); /* créer une liste qui va contenir les groupes qu'on a dans le fichier */
            }

        }

        fichier = fopen("iris.arff", "r");

        /*remplir les listes des plantes pour chaque groupe*/
        while(fscanf(fichier,"%f,%f,%f,%f,Iris-%s",&sl,&sw,&pl,&pw,nom)!=EOF)
        {
            InsererPlante(tab, sl, sw, pl, pw, nom); /* insérer les plantes dans la liste selon le type du groupe */
        }
    }
}

/** Cette fonction crée une liste des distance calculées ************************************************************************/
void CreerListe(Dis **l, float val, char groupe[])
{
    Dis *elem, *temp;

    elem = (Dis*)malloc(sizeof(Dis));
    strcpy(&elem->nomGroupe, groupe);
    elem->distance = val;
    elem->suiv = NULL;

    if(*l == NULL)
    {
        *l = elem;
    }

    else
    {
        temp = *l;

        while(temp->suiv != NULL)
        {
            temp = temp->suiv;
        }

        temp->suiv = elem;
    }
}

/** Cette fonction affiche la liste des distance créee **************************************************************************/
void AfficherListe(Dis *l)
{
    Dis *p;

    p = l;

    while(p != NULL)
    {
        printf(">>>%f-%s\n", p->distance, p->nomGroupe);
        p = p->suiv;
    }
}

/** Trouver une plante (retourne un pointeur sur la position)********************************************************************/
void TrouverPlante(Groupes *tab, char nom[], float sl, float sw, float pl, float pw, Plante **pos)
{
    Plante *p = NULL; /*au début p ne point sur aucune plante*/

    *pos = NULL;

    /*pointer le pointeur p au début de la liste*/
    while(tab != NULL && p == NULL)
    {
        /*si on a pointer p sur la première plante c'est pas la peine de continuer la boucle while*/
        if(strcmp(nom, tab->nomGroupe)== 0)
        {
            p = tab->next; /*pointer p sur la première plante*/
        }
        tab = tab->nextG; /*passer au prochain groupe*/
    }

    /*rechercher la plante et repointer pos*/
    while(*pos == NULL && p != NULL)
    {
        if(p->sepallength == sl && p->sepalwidth == sw && p->petallength == pl && p->petalwidth == pw)
        {
            *pos = p;
        }

        p = p->next;
    }
}

/** Rechercher un plante et l'afficher s'elle existe ****************************************************************************/
void RechercherPlante(Groupes *tab)
{
    float sl, sw, pl, pw;
    char choix[20];
    Plante *pos;
    pos = NULL;

    /*si la liste globale est vide on recherche pas*/
    if(tab == NULL || tab->next == NULL)
    {
        printf("La liste est vide vous ne pouvez pas rechercher une plante!!!\n");
    }

    /*si non si la liste contient des plantes on fait la recherche*/
    else
    {
        /*Menu*/
        /*le choix du groupe où appartient la plante*/

        printf("\nA quel groupe appartient la plante recherchee ? ");
        scanf("%s", &choix);

        /*si non on demande les informations de la plante*/

        printf("\nEntrez sepallength : ");
        scanf("%f", &sl);
        printf("Entrez sepalwidth : ");
        scanf("%f", &sw);
        printf("Entrez petallength : ");
        scanf("%f", &pl);
        printf("Entrez petalwidth : ");
        scanf("%f", &pw);

        /*positionner un pointeur sur la plante désirée*/
        TrouverPlante(tab, choix, sl, sw, pl, pw, &pos);

        if(pos == NULL)
        {
            printf("La plante recherchee n'existe pas !!!\n");
        }

        else
        {
            printf("La plante recherchee existe\n");
        }
    }
}

/** calcule de la distance ******************************************************************************************************/
float Calcule(Plante *p1, Plante *p2)
{
    float d;

    d = sqrt(pow((p1->sepallength - p2->sepallength),2) + pow((p1->petallength - p2->petallength),2) + pow((p1->sepalwidth - p2->sepalwidth),2) + pow((p1->petalwidth - p2->petalwidth),2));
    return (d);
}

/** Trier une liste en ordre croissant ******************************************************************************************/
void TrieCroissant(Dis **l)
{
    Dis *p, *pa;
    float aux;

    p = *l;

    while(p->suiv != NULL)
    {
        pa = p->suiv;
        while(pa != NULL)
        {
            if(p->distance > pa->distance)
            {
            /*permuter*/
            aux = pa->distance;
            pa->distance = p->distance;
            p->distance = aux;
            }

            pa = pa->suiv;
        }

        p = p->suiv;
    }
}

/** Calculer la distance entre deux plantes *************************************************************************************/
void Distance(Groupes *tab)
{
    float distance, sl, sw, pl, pw;
    int i = 1;
    char choix[20]; /*va contenir le choix du groupe de l'utilisateur*/
    Plante *pos, *p;
    pos = NULL;

    /*le choix du groupe où appartient la plante*/
    printf("\n\nA quel groupe appartient la plante recherchee ? ");
    scanf("%s", &choix);

    /*les coordonnées de la plante*/
    printf("\nEntrez sepallength : ");
    scanf("%f", &sl);
    printf("Entrez sepalwidth : ");
    scanf("%f", &sw);
    printf("Entrez petallength : ");
    scanf("%f", &pl);
    printf("Entrez petalwidth : ");
    scanf("%f", &pw);

    TrouverPlante(tab, choix, sl, sw, pl, pw, &pos);

    if(pos != NULL)
    {
        /*calculer la distance maintenant*/
        while(tab != NULL)
        {
            p = tab->next; /*pointer sur la première plante*/

            printf("\n>>>>>>>Groupe de %s :\n\n", tab->nomGroupe);
            while(p != NULL)
            {
                if(p != pos)
                {
                    distance = Calcule(p, pos);
                    printf(">>>Distance avec (%.1f,%.1f,%.1f,%.1f) = %f\n", p->sepallength, p->sepalwidth,p->petallength, p->petalwidth, distance);
                }

                p = p->next;
            }
            tab = tab->nextG; /*passer au prochain groupe*/
        }
    }

    else
    {
        printf("\nPlante introuvable le groupe ou les informations de la plantes n'est pas correcte !!!\n");
    }
}

/** Cette fonction affiche le groupe majoritaire ********************************************************************************/
void AfficherGroupeMaj(Groupes *g)
{
    Groupes *max = NULL; /*un pointeur qui va pointer sur le groupe majoritaire*/
    int max_ = g->nbrPlProches;

    printf("\n");
    while(g != NULL)
    {
        if(g->nbrPlProches >= max_)
        {
            max_ = g->nbrPlProches; /*modifier la valeur maximale*/
            max = g; /*pointer le pointeur sur le groupe majoritaire*/
        }
        printf("%d de %s\n", g->nbrPlProches, g->nomGroupe);
        g = g->nextG;
    }

    printf("\n*************************************************\n");
    printf("*************Le groupe majoritaire :***************\n");
    printf("**************************************************\n\n");
    printf("\nLe groupe majoritaire est : %s\n", max->nomGroupe);
}

/** Cette focntion retourne un pointeur sur le groupe recherché *****************************************************************/
void Incrementer_nbr_pl(Groupes *tab, char nomG[])
{
    int inc = 0; /*le variable n'est pas encore incrémenter*/
    Groupes *p = tab;

    while(tab != NULL && inc == 0)
    {
        if(strcmp(tab->nomGroupe, nomG) == 0)
        {
           tab->nbrPlProches++; /*incréméntation*/
           inc = 1; /*l'incrémentation est faite donc il faut arreter le boucle*/
        }
        tab = tab->nextG;
    }
}

/** Les dix plantes les plus proches et le groupe majoritaire *******************************************************************/
void PlantesProches(Groupes *tab)
{
    float distance, sl, sw, pl, pw;
    char choix[20];
    int k = 0; /*la précision : les dix plantes plus proches*/
    Plante *pos = NULL, *p;
    Dis *l = NULL, *temp;
    Groupes *racine1, *racine2;
    racine1 = tab;
    racine2 = tab;

    /*le choix du groupe où appartient la plante*/
    printf("\n\nA quel groupe appartient la plante recherchee ? ");
    scanf("%s", &choix);

    /*les coordonnées de la plante*/
    printf("\nEntrez sepallength : ");
    scanf("%f", &sl);
    printf("Entrez sepalwidth : ");
    scanf("%f", &sw);
    printf("Entrez petallength : ");
    scanf("%f", &pl);
    printf("Entrez petalwidth : ");
    scanf("%f", &pw);

    printf("\n");
    TrouverPlante(tab, choix, sl, sw, pl, pw, &pos);

    if(pos != NULL)
    {
        /*calculer la distance maintenant*/
        while(tab != NULL)
        {
            p = tab->next; /* p pointe sur la première plante*/

            while(p != NULL)
            {
                if(p != pos)
                {
                    distance = Calcule(p, pos);
                    /*Insérer les distances dans la liste */
                    CreerListe(&l, distance, p->nom);
                }

                p = p->next;
            }
            tab = tab->nextG;
        }

        TrieCroissant(&l); /*trier la liste en ordre croissant*/

        printf("\n*************************************************\n");
        printf("*************Les distances triée :***************\n");
        printf("*************************************************\n");

        AfficherListe(l); /*afficher les distances après trie croissant*/

        /** trouver le groupe majoritaire **/

        temp = l; /*temp pointe sur le premier distance de la liste*/

        while(l != NULL && k < 10) /*afficher les dix plantes les plus proche k = 10*/
        {
            Incrementer_nbr_pl(racine1, l->nomGroupe); /*incréménter le nombre des plantes selon le groupe*/
            l = l->suiv;
            k++;
        }

        /* afficher le groupe majoritaire */
        AfficherGroupeMaj(racine2);
    }

    else
    {
        printf("\nPlante introuvable !!!\n");
    }
}

/** Cette fonction affiche un menu à l'utilisateur ******************************************************************************/
void Menu(char nomFichier[])
{
    Groupes *tab = NULL;
    int choix, continuer = 1;

    /*créer la liste globale et afficher les groupes existants */
    printf("Les groupes disponibles sont :\n\n");
    ExtraireInfo_Insert(&tab, nomFichier);
    printf("\n*************************************************\n");
    printf("************Le contenu du fichier :**************\n");
    printf("*************************************************\n");
    Afficher(tab); /* afficher le contenu */

    while(continuer == 1)/* répéter tant l'utilisateur tape 1 */
    {

        do
        {
            printf("\n*************************************************\n");
            printf("*********************Menu :**********************\n");
            printf("*************************************************\n\n");
            printf("Tapez (1) pour rechercher une plante dans le fichier.\n");
            printf("Tapez (2) pour calculer la distance entre une plante et toutes les autres dans le fichier.\n");
            printf("Tapez (3) pour Afficher le groupe majoritaire et les dix plantes les plus proches d'une plante donnée.\n");
            printf("Tapez (4) pour Réafficher le contenu du fichier.\n\n");
            printf("Veuillez entre un choix valide : ");
            scanf("%d", &choix);

            switch(choix)
            {
                case (1):
                    printf("\n*************************************************\n");
                    printf("************Rechercher une plante :**************\n");
                    printf("*************************************************\n");
                    RechercherPlante(tab);
                    break;

                case (2):
                    printf("\n*************************************************\n");
                    printf("******La distyance avec les autres plantes :*****\n");
                    printf("*************************************************\n");
                    Distance(tab);
                    break;

                case (3):
                    printf("\n*************************************************\n");
                    printf("*******Les dix plantes les plus proches :*******\n");
                    printf("*************************************************\n");
                    PlantesProches(tab);
                    break;

                case (4):
                    printf("\n*************************************************\n");
                    printf("************Le contenu du fichier :**************\n");
                    printf("*************************************************\n");
                    Afficher(tab); /* afficher le contenu */

                default : printf("Erreur!!! choix invalide!!!\n");
            }

        }while(choix > 4 || choix < 1);

        printf("\nPour continuer tapez 1 : ");
        scanf("%d", &continuer);
    }

    printf("\n||||||||||||||||||||||||||||||||||||||\n");
    printf("||||||||||PAR MOSTAFA ACHRAF||||||||||\n");
    printf("||||||||||||||||||||||||||||||||||||||\n");
}

/** ************************************************************************************************************************** **/
/** ********************************************************* MAIN *********************************************************** **/
/** ************************************************************************************************************************** **/
int main()
{

    Menu("iris.arff");

    return 0;
}
