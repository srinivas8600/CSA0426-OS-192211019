#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

typedef struct bloc_mem
{
    char etat_bloc;
    char etat_proc;
    char code_proc[10];
    int adrr_debu_bloc;
    int taille;
    struct bloc_mem *suiv;
    struct bloc_mem *prec;
}
bloc_mem;

typedef struct liste_bloc_mem
{
    bloc_mem *tete;
    int nb_proc_a_executer;
}
liste_bloc_mem;

typedef struct processus
{
    char code[10];
    char etat_alloc;//whether the process is allocated or not
    char etat_attente_mem;//whether the process is waiting for memory or not
    int date_arr;
    int tmp_exec;
    int esp_mem;
    struct processus *suiv;
    struct processus *prec;
}
processus;

typedef struct
{
    char cd[30];
}
code_proc;

typedef struct liste_processus
{
    processus *tete;
    processus *queue;
    int nb_proc_a_allouer;
    int nb_proc_en_att_mem;
}
liste_processus;

typedef struct elem_cmt
{
    char nv_proc_alloue[400];//1
    char proc_en_att[400];//2
    char proc_termine[400];//3
    char proc_elu[400];//4
    char proc_pret[400];//5
    int inst;
    struct elem_cmt *suiv;
}
elem_cmt;

typedef struct liste_cmt
{
    elem_cmt *tete;
}
liste_cmt;

void initialiser_LC_bloc_mem(liste_bloc_mem *mem,int taille_mem)
{
    bloc_mem *bm=(bloc_mem*)malloc(sizeof(bloc_mem));

    if(bm==NULL)
        printf("memoire insuffisante !!");
    else
    {
        mem->tete=bm;
        mem->nb_proc_a_executer=0;

        bm->etat_bloc='l';
        bm->taille=taille_mem;
        bm->adrr_debu_bloc=0;
        bm->suiv=NULL;
        bm->prec=NULL;
    }
}

 void initialiser_LC_cmt(liste_cmt *comentaire)
{
     comentaire->tete=NULL;
}

void initialiser_LC_proc(liste_processus *LC_proc)
{
    LC_proc->nb_proc_a_allouer=0;
    LC_proc->nb_proc_en_att_mem=0;
    LC_proc->queue=NULL;
    LC_proc->tete=NULL;
}

void Locate(int x,int y)
{
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD C;
    C.X=(SHORT)x;
    C.Y=(SHORT)y;
    SetConsoleCursorPosition(H,C);
}

int verif_bcp(char nom_fich[])
{
    FILE *p=fopen(nom_fich,"rt");
    int c,cmpt_poin_vrg=0,cmpt_lign=0;
    char ch[100];


    if(p==NULL)
    {
        printf("Unable to open the file !!");
        fclose(p);

        getch();

        return 0;
    }
    else
    {
        p=fopen(nom_fich,"rt");

        while(fgets(ch,sizeof(ch),p)!=NULL)
        {
            if(strcmp(ch,"\n")!=0)
                cmpt_lign++;
                else
                {
                    printf("Your file contains a line break that should be avoided !!");
                    getch();
                    return 0;
                }
        }
        fclose(p);

        p=fopen(nom_fich,"rt");

        while((c=fgetc(p))!=EOF)
        {
            if(c==';')
            {
                cmpt_poin_vrg++;
            }
            else
            {
                if(c==' ')
                {
                    printf("Your file contains spaces that should be avoided !!");
                    fclose(p);
                    getch();
                    return 0;
                }
            }
        }
        fclose(p);

        if(cmpt_lign==0)
        {
            printf("Your file is empty !!");
            getch();
            return 0;
        }
        else
        {
            if(cmpt_poin_vrg!=(4*cmpt_lign))
            {
                printf("Semicolons are misplaced in this file !!");
                getch();
                return 0;
            }
            else
                return 1;
        }


    }
}

int verif_occur_code(char nom_fich[])
{
    int cpt_ocur,j,i=0;
    code_proc cod[20];
    char *nom_code;
    FILE *p;

    p=fopen(nom_fich,"rt");//read if you want to read the file or write if you want to write to the file

    while(fscanf(p,"%s",cod[i].cd)!=EOF)
    {
        nom_code = strtok (cod[i].cd,";");

        strcpy(cod[i].cd,nom_code);

        i++;
    }
    strcpy(cod[i].cd,"stop");

    fclose(p);

    i=0;

    while(strcmp(cod[i].cd,"stop")!=0)
    {
        j=0;
        cpt_ocur=0;

        while(strcmp(cod[j].cd,"stop")!=0)
        {
            if(strcmp(cod[j].cd,cod[i].cd)==0)
                cpt_ocur++;
            j++;
        }

        if(cpt_ocur==1)
        {
            i++;
        }
        else
        {
            printf("\nError ! There are %d processes with the same code %s !!",cpt_ocur,cod[i].cd);
            getch();
            return 0;
        }
    }
    return 1;
}

int verif_nombre(char nb[],int max)
{
    char *p  ;
    int erreur=0,taille;

    p=nb;

    while (*p!=NULL)
    {
        if (isdigit(*p))//if the dotted character is a digital number
        {
            p++;
        }
        else
        {
            erreur=1;
            break;
        }
    }

    if(erreur==0)
    {
        taille=atoi(nb);
            if(taille==0||max>taille)
            {
                printf("\nGive a non-zero size of minimum %d KB !!",max);
                return 0;
            }
            else
                return atoi(nb);
    }
    else
    {
        printf("Give a positive and correct number !!");
        return 0;
    }
 }

void ajout_1ere_fois_LC_processus(liste_processus *l,char *chaine)
{
    char *part;
    processus *p=(processus*)malloc(sizeof(processus));

    if(p==NULL)
        printf("insufficient block !!");
    else
    {
        part = strtok (chaine,";");
        strcpy(p->code,part);

        part = strtok(NULL, ";");
        p->date_arr=atoi(part);

        part = strtok (NULL, ";");
        p->esp_mem=atoi(part);

        part = strtok (NULL, ";");
        p->tmp_exec=atoi(part);

        p->suiv=NULL;
        p->prec=NULL;
        p->etat_alloc='n';
        p->etat_attente_mem='n';

        l->tete=p;
        l->queue=p;
        l->nb_proc_a_allouer=1;
    }
}

void ajout_fin_LC_processus(liste_processus *l,char *chaine)
{
    char *part;
    processus *p=(processus*)malloc(sizeof(processus));

    if(p==NULL)
        printf("insufficient block !!");
    else
    {
        part = strtok (chaine,";");
        strcpy(p->code,part);

        part = strtok(NULL, ";");
        p->date_arr=atoi(part);

        part = strtok (NULL, ";");
        p->esp_mem=atoi(part);

        part = strtok (NULL, ";");
        p->tmp_exec=atoi(part);

        p->etat_alloc='n';
        p->etat_attente_mem='n';
        p->suiv=NULL;
        p->prec=l->queue;

        l->queue->suiv=p;
        l->queue=p;
        l->nb_proc_a_allouer++;
    }
}

void constr_LC_processus(char *nomF,liste_processus *l)
{
    FILE *ptr_nomF=fopen(nomF,"rt");
    char chaine[30];
    l->nb_proc_a_allouer=0;

    if(ptr_nomF==NULL)
    {
        printf("Unable to open the file");
    }
    else
    {
        while(fgets(chaine,sizeof(chaine),ptr_nomF)!=NULL)
        {
            if(l->nb_proc_a_allouer==0)
                ajout_1ere_fois_LC_processus(l,chaine);
            else
                ajout_fin_LC_processus(l,chaine);
        }
    }
}

void trier_LC_proc(processus *lc_non_trie_tete)
{
    int permut,temp;
    char k[10];
    processus *T;
    do
    {
        T=lc_non_trie_tete;
        permut=0;

        while (T->suiv!=NULL)
        {
            if(T->date_arr>T->suiv->date_arr||(T->date_arr==T->suiv->date_arr&&strcmp(T->code,T->suiv->code)>0))
            {
                strcpy(k,T->code);
                strcpy(T->code,T->suiv->code);
                strcpy(T->suiv->code,k);

                temp=T->date_arr;
                T->date_arr=T->suiv->date_arr;
                T->suiv->date_arr=temp;

                temp=T->tmp_exec;
                T->tmp_exec=T->suiv->tmp_exec;
                T->suiv->tmp_exec=temp;

                temp=T->esp_mem;
                T->esp_mem=T->suiv->esp_mem;
                T->suiv->esp_mem=temp;

                permut=1;
            }
            T=T->suiv;
        }
    }
    while(permut!=0);
}

void inserer_cmt(liste_cmt *comentaire,int instant,char *code,int debut_partition,int fin_partition,int dur,bloc_mem tete,processus *first,int type)
{
    elem_cmt* elem;
    elem_cmt* nv_elem;
    char conv[10];
    char *q;
    char msg[80];

    int cpt=0;
    bloc_mem *bm;
    processus *proc;


    if(comentaire->tete==NULL)
    {

        nv_elem=(elem_cmt*)malloc(sizeof(elem_cmt));
        comentaire->tete=nv_elem;
        nv_elem->suiv=NULL;
        nv_elem->inst=instant;
                    strcpy(nv_elem->nv_proc_alloue,"not");
                    strcpy(nv_elem->proc_en_att,"not");
                    strcpy(nv_elem->proc_termine,"not");
                    strcpy(nv_elem->proc_elu,"not");
                    strcpy(nv_elem->proc_pret,"not");

    }
    else
    {

         for(elem=comentaire->tete; elem->suiv!=NULL&&elem->inst!=instant; elem=elem->suiv);

        if(elem->inst!=instant)
        {
            nv_elem=(elem_cmt*)malloc(sizeof(elem_cmt));
            elem->suiv=nv_elem;
            nv_elem->inst=instant;
            nv_elem->suiv=NULL;
             strcpy(nv_elem->nv_proc_alloue,"not");
                    strcpy(nv_elem->proc_en_att,"not");
                    strcpy(nv_elem->proc_termine,"not");
                    strcpy(nv_elem->proc_elu,"not");
                    strcpy(nv_elem->proc_pret,"not");
        }
        else
        {
            nv_elem=elem;
        }


    }

    if(type==1)
    {
            q=msg;
            strcpy(q,code);

       for(q=msg;*q!='\0';q++);
            strcpy(q," is loaded");

            for(q=msg;*q!='\0';q++);
            strcpy(q," in the part [");

            sprintf(conv, "%d", debut_partition); // Integer conversion
            for(q=msg;*q!='\0';q++);
            strcpy(q,conv);

            for(q=msg;*q!='\0';q++);
            strcpy(q,"Ko,");

          sprintf(conv, "%d", fin_partition); // Integer conversion
            for(q=msg;*q!='\0';q++);
            strcpy(q,conv);

            for(q=msg;*q!='\0';q++);
            strcpy(q,"Ko] " );

            for(q=nv_elem->nv_proc_alloue;*q!='\0';q++);
            strcpy(q,msg);

    }
    else
    {
        if(type==2)
        {
            for(proc=first; proc!=NULL; proc=proc->suiv)
                    {
                        if(proc->etat_attente_mem=='o')
                        {
                            q=msg;
                            strcpy(q,proc->code);

                            for(q=msg; *q!='\0'; q++);
                            strcpy(q," ");

                            cpt++;
                            break;
                        }

                        //printf("%s %c",proc->code,proc->etat_attente_mem);
                        //getch();

                    }

                    if(proc!=NULL)proc=proc->suiv;
                    while(proc!=NULL)
                    {
                        if(proc->etat_attente_mem=='o')
                        {
                            for(q=msg; *q!='\0'; q++);
                            strcpy(q,proc->code);

                            for(q=msg; *q!='\0'; q++);
                            strcpy(q," ");
                            cpt++;
                        }
                        proc=proc->suiv;
                    }

            if(cpt!=0)
            {
                if(cpt>1)
                {
                    for(q=msg; *q!='\0'; q++);
                    strcpy(q,"are put on hold");

                    for(q=nv_elem->proc_en_att; *q!='\0'; q++);
                    strcpy(q,msg);
                }
                else
                {

                    for(q=msg; *q!='\0'; q++);
                    strcpy(q,"is put on hold");

                    for(q=nv_elem->proc_en_att; *q!='\0'; q++);
                    strcpy(q,msg);
                }
            }
        }
        else
        {
            if(type==3)
            {
                  q=msg;
            strcpy(q,code);
            for(q=msg; *q!='\0'; q++);
            strcpy(q," is finished");

            for(q=msg;*q!='\0';q++);
            strcpy(q," ");

            for(q=nv_elem->proc_termine;*q!='\0';q++);
            strcpy(q,msg);
            }
            else
            {
                if(type==4)
                {

                    q=msg;
            strcpy(q,code);

       for(q=msg;*q!='\0';q++);
            strcpy(q," is ");

            for(q=msg;*q!='\0';q++);
            strcpy(q,"lu");

            for(q=msg;*q!='\0';q++);
            strcpy(q," and will be finished");

            for(q=msg;*q!='\0';q++);
            strcpy(q," ");

            for(q=msg;*q!='\0';q++);
            strcpy(q," T=");

            sprintf(conv, "%d",dur+instant); // Integer conversion
            for(q=msg;*q!='\0';q++);
            strcpy(q,conv);

            for(q=msg;*q!='\0';q++);
            strcpy(q," ");

            for(q=nv_elem->proc_elu;*q!='\0';q++);
            strcpy(q,msg);
                 }
                else
                {
                    for(bm=&tete; bm!=NULL; bm=bm->suiv)
                    {
                        if(bm->etat_proc=='p')
                        {
                            q=msg;
                            strcpy(q,bm->code_proc);

                            for(q=msg; *q!='\0'; q++);
                            strcpy(q," ");

                            cpt++;
                            break;
                        }
                    }
                    if(bm!=NULL)bm=bm->suiv;
                    while(bm!=NULL)
                    {
                        if(bm->etat_proc=='p')
                        {
                            for(q=msg; *q!='\0'; q++);
                            strcpy(q,bm->code_proc);

                            for(q=msg; *q!='\0'; q++);
                            strcpy(q," ");
                            cpt++;
                        }
                        bm=bm->suiv;
                    }

                    if(cpt!=0)
                    {
                        if(cpt>1)
                        {
                            for(q=msg; *q!='\0'; q++);
                            strcpy(q,"are ready");

                            for(q=nv_elem->proc_pret; *q!='\0'; q++);
                            strcpy(q,msg);
                        }
                        else
                        {

                            for(q=msg; *q!='\0'; q++);
                            strcpy(q,"is ready");

                            for(q=nv_elem->proc_pret; *q!='\0'; q++);
                            strcpy(q,msg);
                        }
                    }
                }
            }
        }
    }
}

void entete(void)
{
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf("       ");
    putchar(218);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(194);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(194);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(194);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(191);
    printf("\n");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf("                 ");
    putchar(179);
    printf(" ");
    printf(" Process ");
    printf(" ");
    putchar(179);
    printf(" ");
    printf("Dat of arri");
    putchar(179);
    printf(" ");
    printf(" memory space  ");
    putchar(179);
    printf(" ");
    printf("Tim of execu");
    printf(" ");
    putchar(179);
    printf(" ");
    printf("\n");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf("                 ");
    putchar(195);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(197);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(197);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(197);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(180);
}

void milieu(char *a,int b,int c,int d)
{
    int aa=strlen(a),i;
    int bb;
    int cc;
    int dd;

    if(b<10)
        bb=1;
    else
    {
        if(b>=10&&b<100)
            bb=2;
        else
        {
            if(b>=100&&b<1000)
                bb=3;
            else
                bb=4;
        }
    }

    if(c<10)
        cc=1;
    else
    {
        if(c>=10&&c<100)
            cc=2;
        else
        {
            if(c>=100&&c<1000)
                cc=3;
            else
                cc=4;
        }
    }

     if(d<10)
        dd=1;
    else
    {
        if(d>=10&&d<100)
            dd=2;
        else
        {
            if(d>=100&&d<1000)
                dd=3;
            else
                dd=4;
        }
    }

    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf("       ");
    putchar(179);
    printf(" ");

    for(i=1; i<=(9-aa)/2; i++)
        printf(" ");
    printf("%s",a);
    for(i=1; i<=((9-aa)/2)+(9-aa)%2; i++)
        printf(" ");

    printf(" ");
    putchar(179);
    printf(" ");

    switch(bb)
    {
        case 1 : for(i=1; i<=5; i++) printf(" "); printf("%d",b); for(i=1; i<=4; i++) printf(" ");break;
        case 2 : for(i=1; i<=4; i++) printf(" "); printf("%d",b); for(i=1; i<=4; i++) printf(" ");break;
        case 3 : for(i=1; i<=3; i++) printf(" "); printf("%d",b); for(i=1; i<=4; i++) printf(" ");break;
        case 4 : for(i=1; i<=2; i++) printf(" "); printf("%d",b); for(i=1; i<=4; i++) printf(" ");break;
    }

    printf(" ");
    putchar(179);
    printf(" ");

  switch(cc)
    {
        case 1 : for(i=1; i<=5; i++) printf(" "); printf("%d",c); for(i=1; i<=6; i++) printf(" ");break;
        case 2 : for(i=1; i<=5; i++) printf(" "); printf("%d",c); for(i=1; i<=5; i++) printf(" ");break;
        case 3 : for(i=1; i<=5; i++) printf(" "); printf("%d",c); for(i=1; i<=4; i++) printf(" ");break;
        case 4 : for(i=1; i<=5; i++) printf(" "); printf("%d",c); for(i=1; i<=3; i++) printf(" ");break;
    }

    printf("   ");
    putchar(179);
    printf(" ");

  switch(dd)
    {
        case 1 : for(i=1; i<=5; i++) printf(" "); printf("%d",d); for(i=1; i<=5; i++) printf(" ");break;
        case 2 : for(i=1; i<=5; i++) printf(" "); printf("%d",d); for(i=1; i<=4; i++) printf(" ");break;
        case 3 : for(i=1; i<=5; i++) printf(" "); printf("%d",d); for(i=1; i<=3; i++) printf(" ");break;
        case 4 : for(i=1; i<=5; i++) printf(" "); printf("%d",d); for(i=1; i<=2; i++) printf(" ");break;
    }

    printf("  ");
    putchar(179);

    printf("\n");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf("                 ");
    putchar(195);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(197);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(197);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(197);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(180);
}

void fin_tab(char *a,int b,int c,int d)
{
        int aa=strlen(a),i;
    int bb;
    int cc;
    int dd;

    if(b<10)
        bb=1;
    else
    {
        if(b>=10&&b<100)
            bb=2;
        else
        {
            if(b>=100&&b<1000)
                bb=3;
            else
                bb=4;
        }
    }

    if(c<10)
        cc=1;
    else
    {
        if(c>=10&&c<100)
            cc=2;
        else
        {
            if(c>=100&&c<1000)
                cc=3;
            else
                cc=4;
        }
    }

     if(d<10)
        dd=1;
    else
    {
        if(d>=10&&d<100)
            dd=2;
        else
        {
            if(d>=100&&d<1000)
                dd=3;
            else
                dd=4;
        }
    }

    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf("       ");
    putchar(179);
    printf(" ");

    for(i=1; i<=(9-aa)/2; i++)
        printf(" ");
    printf("%s",a);
    for(i=1; i<=((9-aa)/2)+(9-aa)%2; i++)
        printf(" ");

    printf(" ");
    putchar(179);
    printf(" ");

    switch(bb)
    {
        case 1 : for(i=1; i<=5; i++) printf(" "); printf("%d",b); for(i=1; i<=4; i++) printf(" ");break;
        case 2 : for(i=1; i<=4; i++) printf(" "); printf("%d",b); for(i=1; i<=4; i++) printf(" ");break;
        case 3 : for(i=1; i<=3; i++) printf(" "); printf("%d",b); for(i=1; i<=4; i++) printf(" ");break;
        case 4 : for(i=1; i<=2; i++) printf(" "); printf("%d",b); for(i=1; i<=4; i++) printf(" ");break;
    }

    printf(" ");
    putchar(179);
    printf(" ");

  switch(cc)
    {
        case 1 : for(i=1; i<=5; i++) printf(" "); printf("%d",c); for(i=1; i<=6; i++) printf(" ");break;
        case 2 : for(i=1; i<=5; i++) printf(" "); printf("%d",c); for(i=1; i<=5; i++) printf(" ");break;
        case 3 : for(i=1; i<=5; i++) printf(" "); printf("%d",c); for(i=1; i<=4; i++) printf(" ");break;
        case 4 : for(i=1; i<=5; i++) printf(" "); printf("%d",c); for(i=1; i<=3; i++) printf(" ");break;
    }

    printf("   ");
    putchar(179);
    printf(" ");

  switch(dd)
    {
        case 1 : for(i=1; i<=5; i++) printf(" "); printf("%d",d); for(i=1; i<=5; i++) printf(" ");break;
        case 2 : for(i=1; i<=5; i++) printf(" "); printf("%d",d); for(i=1; i<=4; i++) printf(" ");break;
        case 3 : for(i=1; i<=5; i++) printf(" "); printf("%d",d); for(i=1; i<=3; i++) printf(" ");break;
        case 4 : for(i=1; i<=5; i++) printf(" "); printf("%d",d); for(i=1; i<=2; i++) printf(" ");break;
    }

    printf("  ");
    putchar(179);
    printf("\n");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf("                 ");
    putchar(192);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(193);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(193);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(193);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(217);
}

void title_first_fit(void)
{
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf("                            ");
    putchar(218);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);

    putchar(191);
    printf("\n");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf("                            ");
    putchar(179);
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf("First Fit");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    putchar(179);
    printf("\n");
    printf("                            ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    putchar(192);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(217);
    printf("\n");

}

void title_best_fit(void)
{
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf("                            ");
    putchar(218);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);

    putchar(191);
    printf("\n");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf("                            ");
    putchar(179);
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf("  ");
    printf("Best Fit");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    putchar(179);
    printf("\n");
    printf("                            ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    printf(" ");
    putchar(192);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(196);
    putchar(217);
    printf("\n");

}

void afficher_proc(processus *proc,int nbr,char nom_fich[])
{
    int boucle=1;
    int pos=1;
    processus *T=proc;

    printf("\n\t\t\t                 List of file data %s : \n",nom_fich);
    printf("\n\t  ");
    entete();

    do
    {
        if(pos!=nbr)
        {
            printf("\n\t  ");
            milieu(T->code,T->date_arr,T->esp_mem,T->tmp_exec);
        }
        else
        {
            printf("\n\t  ");
            fin_tab(T->code,T->date_arr,T->esp_mem,T->tmp_exec);
        }

        if(T->suiv!=NULL)
        {
            T=T->suiv;
        }
        else
            boucle=0;
        pos++;
    }
    while(boucle==1);

}

void supp_liste_proc(processus *proc)
{
    processus *T;
    int boucle=1;
    do
    {
        T=proc;
        if(T->suiv!=NULL)
        {
            proc=proc->suiv;
            free(T);
        }
        else
        {
            boucle=0;
            free(T);
        }
    }
    while(boucle==1);
}

bloc_mem* prem_esp_mem_lib(liste_bloc_mem mem,int taille)
{
    bloc_mem *bm;

    for(bm=mem.tete; bm!=NULL; bm=bm->suiv)
    {
        if(bm->etat_bloc=='l'&&bm->taille>=taille)
            return bm;
    }
    return NULL;

}

int max_proc_esp(liste_processus LC_prc)
{
    processus *proc;
    int max=LC_prc.tete->esp_mem;

    for(proc=LC_prc.tete;proc!=NULL;proc=proc->suiv)
    {
        if(proc->esp_mem>max)
            max=proc->esp_mem;
    }
    return max;
}

int taille_du_prem_proc_alloue(liste_processus lc_proc)
{
    processus *proc;
    for(proc=lc_proc.tete; proc!=NULL; proc=proc->suiv)
    {
        if(proc->etat_alloc=='o')
            return proc->tmp_exec;
    }
    return 0;
}

bloc_mem* chercher_esp_FF(processus pr,liste_bloc_mem mem)
{
    bloc_mem *bm;

    for(bm=mem.tete; bm!=NULL; bm=bm->suiv)
    {
        if(bm->etat_bloc=='l'&&bm->taille>=pr.esp_mem)
            return bm;
    }
    return NULL;
}

bloc_mem* chercher_esp_BF(processus pr,liste_bloc_mem mem)
{
    int min;
    bloc_mem *bm;
    bloc_mem *adr;

    adr=prem_esp_mem_lib(mem,pr.esp_mem);

    if(adr!=NULL)
    {
        min=adr->taille;
        for(bm=mem.tete; bm!=NULL; bm=bm->suiv)
        {
            if(bm->etat_bloc=='l'&&bm->taille>=pr.esp_mem&&min>bm->taille)
            {
                min=bm->taille;
                adr=bm;
            }
        }
    }

    return adr;
}

processus * chercher_proch_proc_a_executer_SJF(liste_processus *LC_prc,liste_bloc_mem *LC_bm,liste_cmt*comentaire,int instant)
{
    int min=taille_du_prem_proc_alloue(*LC_prc);
    int cpt_occur=0;
    processus *proc;
    bloc_mem *bm;
    processus *adr;

    for(proc=LC_prc->tete; proc!=NULL; proc=proc->suiv) //search for the shortest execution time
    {
        if(proc->etat_alloc=='o'&&min>proc->tmp_exec)
        {
            min=proc->tmp_exec;
        }
    }

    for(proc=LC_prc->tete; proc!=NULL; proc=proc->suiv)//check if there is another process with the same short execution time and at the same time point to the process to be executed
    {
        if(proc->etat_alloc=='o'&&proc->tmp_exec==min)
        {
            cpt_occur++;
            adr=proc;
        }
    }

    if(cpt_occur>1)
    {
        for(proc=LC_prc->tete; proc!=NULL; proc=proc->suiv)
        {
            if(proc->etat_alloc=='o'&&proc->tmp_exec==min)
            {
                adr=proc;
                break;
            }
        }
    }

    for(bm=LC_bm->tete; bm!=NULL; bm=bm->suiv)//give the elected state to the process to be carried out
        {
            if(strcmp(bm->code_proc,adr->code)==0)
            {
                bm->etat_proc='e';
                inserer_cmt(comentaire,instant,bm->code_proc,bm->adrr_debu_bloc,bm->adrr_debu_bloc+bm->taille,adr->tmp_exec,*LC_bm->tete,LC_prc->tete,4);
                inserer_cmt(comentaire,instant,bm->code_proc,bm->adrr_debu_bloc,bm->adrr_debu_bloc+bm->taille,adr->tmp_exec,*LC_bm->tete,LC_prc->tete,5);
                break;
            }
        }

    /*for(proc=LC_prc.tete; proc!=NULL; proc=proc->suiv)//donner l etat pret aux processus en attente
    {
        if(proc->etat_alloc=='o'&&proc!=adr)
        {
            proc->etat_cpu='p';
        }
    }*/

return adr;
}

void inserer_ds_RAM(processus *pr,bloc_mem *bm,liste_bloc_mem *mem,int instant,liste_cmt *comentaire,liste_processus *lp)
{
    bloc_mem *nv_bm;
    pr->etat_alloc='o';

    if(pr->esp_mem==bm->taille)//insert in the same box
    {
        strcpy(bm->code_proc,pr->code);
        bm->etat_bloc='o';
        bm->etat_proc='p';
    }
    else
    {
        nv_bm=(bloc_mem*)malloc(sizeof(bloc_mem));

        if(bm->prec!=NULL)//insert in the middle
        {
            nv_bm->suiv=bm;
            nv_bm->prec=bm->prec;
            strcpy(nv_bm->code_proc,pr->code);
            nv_bm->etat_bloc='o';
            nv_bm->etat_proc='p';
            nv_bm->taille=pr->esp_mem;
            nv_bm->adrr_debu_bloc=nv_bm->prec->adrr_debu_bloc+nv_bm->prec->taille;

            bm->prec->suiv=nv_bm;
            bm->prec=nv_bm;
            bm->taille=bm->taille-pr->esp_mem;
            bm->adrr_debu_bloc=bm->prec->adrr_debu_bloc+bm->prec->taille;

            bm=nv_bm;//to insert the comment
        }
        else//insert at the head
        {
            nv_bm->suiv=bm;
            nv_bm->prec=NULL;
            strcpy(nv_bm->code_proc,pr->code);
            nv_bm->etat_bloc='o';
            nv_bm->etat_proc='p';
            nv_bm->taille=pr->esp_mem;
            nv_bm->adrr_debu_bloc=0;
            mem->tete=nv_bm;

            bm->prec=nv_bm;
            bm->taille=bm->taille-pr->esp_mem;
            bm->adrr_debu_bloc=bm->prec->adrr_debu_bloc+bm->prec->taille;

            bm=nv_bm;//to insert the comment
        }
    }
   inserer_cmt(comentaire,instant,pr->code,bm->adrr_debu_bloc,bm->adrr_debu_bloc+bm->taille,pr->tmp_exec,*mem->tete,lp->tete,1);

}

void retirer_proc_de_LCproc(processus *proc,liste_processus *lp)
{
    if(proc->prec!=NULL&&proc->suiv!=NULL)//the process to be released is located in the middle of the LC of processes
    {
        proc->prec->suiv=proc->suiv;
        proc->suiv->prec=proc->prec;
        free(proc);
    }
    else
    {
        if(proc->prec==NULL&&proc->suiv!=NULL)//the process to be released is located at the head of the LC of processes
        {
        lp->tete=proc->suiv;
        proc->suiv->prec=NULL;
        free(proc);
        }
        else
        {
             if(proc->prec!=NULL&&proc->suiv==NULL)//the process to be released is located at the end of the LC of processes
             {
                lp->queue=proc->prec;
                proc->prec->suiv=NULL;
                free(proc);
             }
             else//the process to be released is the only one that remains in the LC of processes
             {
                lp->queue=NULL;
                lp->tete=NULL;
                free(proc);
             }
        }
    }
}

void retirer_proc_de_LCbm(char *code,liste_bloc_mem *lbm,liste_cmt *comentaire,int instant,liste_processus *lp)
{
    bloc_mem *BM,*q;
    for(BM=lbm->tete; BM!=NULL; BM=BM->suiv) //search the allocated process in the memory
    {
        if(BM->etat_bloc=='o'&&strcmp(BM->code_proc,code)==0)
            break;
    }


    if(BM->prec==NULL&&BM->suiv!=NULL)//the process to be released is at the head of the list of WBs
    {

        if(BM->suiv->etat_bloc=='o')//x A
        {

            BM->etat_bloc='l';
            lbm->nb_proc_a_executer--;
        }
        else//x L
        {

            BM->etat_bloc='l';
            BM->taille=BM->taille+BM->suiv->taille;

            if(BM->suiv->suiv==NULL)
            {
                free(BM->suiv);
                BM->suiv=NULL;
            }
            else
            {
                q=BM->suiv->suiv;
                free(BM->suiv);
                BM->suiv=q;
                q->prec=BM;
            }

            lbm->nb_proc_a_executer--;
        }
    }
    else
    {
        if(BM->prec!=NULL&&BM->suiv!=NULL)//the process to be released is in the middle of the list of WBs
        {

            if(BM->suiv->etat_bloc=='o'&&BM->prec->etat_bloc=='o')//A x B
            {

                BM->etat_bloc='l';
                lbm->nb_proc_a_executer--;
            }
            else
            {
                if(BM->suiv->etat_bloc=='o'&&BM->prec->etat_bloc=='l')// L x B
                {
                     BM->prec->taille=BM->prec->taille+BM->taille;
                    BM->prec->suiv=BM->suiv;
                    BM->suiv->prec=BM->prec;
                    //BM->suiv->adrr_debu_bloc=BM->prec->adrr_debu_bloc+BM->prec->taille;
                    free(BM);
                    lbm->nb_proc_a_executer--;
                }
                else
                {
                    if(BM->suiv->etat_bloc=='l'&&BM->prec->etat_bloc=='o')// A x L
                    {
                         BM->etat_bloc='l';
                        BM->taille=BM->taille+BM->suiv->taille;

                        if(BM->suiv->suiv==NULL)
                        {
                            free(BM->suiv);
                            BM->suiv=NULL;
                        }
                        else
                        {
                            BM->suiv->suiv->prec=BM;
                            q=BM->suiv->suiv;
                            free(BM->suiv);
                            BM->suiv=q;

                        }

                        lbm->nb_proc_a_executer--;
                    }
                    else
                    {
                         if(BM->suiv->etat_bloc=='l'&&BM->prec->etat_bloc=='l')// L x L
                        {

                            BM->prec->taille=BM->prec->taille+BM->taille+BM->suiv->taille;

                            if(BM->suiv->suiv==NULL)
                            {
                                BM->prec->suiv=NULL;
                            }
                            else
                            {
                                BM->prec->suiv=BM->suiv->suiv;
                                BM->suiv->suiv->prec=BM->prec;
                            }

                            free(BM->suiv);
                            free(BM);
                            lbm->nb_proc_a_executer--;
                        }
                    }
                }
            }
        }
        else
        {
            if(BM->prec!=NULL&&BM->suiv==NULL)//the process to be released is at the end of the list of WBs
            {

                if(BM->prec->etat_bloc=='o')
                {
                    BM->etat_bloc='l';
                    lbm->nb_proc_a_executer--;
                }
                else
                {
                    BM->prec->taille=BM->prec->taille+BM->taille;
                    BM->prec->suiv=NULL;
                    free(BM);

                    lbm->nb_proc_a_executer--;
                }
            }
            else//the process to be released is the only one that occupies all the memory.
            {
                BM->etat_bloc='l';
                lbm->nb_proc_a_executer--;
            }
        }
    }

        inserer_cmt(comentaire,instant,code,0,0,0,*lbm->tete,lp->tete,3);

}

int nb_proc_a_allouer(liste_processus LC_proc)
{
    processus *proc;
    int cpt=0;

    for(proc=LC_proc.tete;proc!=NULL;proc=proc->suiv)
    {
        if(proc->etat_alloc=='n')
            cpt++;
    }
    return cpt;
}

int nb_proc_en_att_mem(liste_processus LC_proc)
{
    processus *proc;
    int cpt=0;

    for(proc=LC_proc.tete;proc!=NULL;proc=proc->suiv)
    {
        if(proc->etat_attente_mem=='o')
            cpt++;
    }
    return cpt;
}

int som_esp_mem(liste_processus lc)
{
    processus *proc;
    int som=0;

    for(proc=lc.tete; proc!=NULL; proc=proc->suiv)
    {
        som=som+proc->esp_mem;
    }
    return som;
}

int tot_lign(liste_processus lc)
{
    processus *proc;
    int nb_lign=0;

    for(proc=lc.tete;proc!=NULL;proc=proc->suiv)
    {
            if(proc->esp_mem<200)
            {
                nb_lign=nb_lign+3;
            }
            else
            {
                if((proc->esp_mem)>=200&&(proc->esp_mem%100)<50)
                {
                    nb_lign=nb_lign+(proc->esp_mem/100)+1;
                }
                else
                {
                    nb_lign=nb_lign+(proc->esp_mem/100)+2;
                }
            }
    }
    return nb_lign;
}

int nb_ligne_standard(liste_processus lc,int taille_mem)
{
     return tot_lign(lc)+(taille_mem-som_esp_mem(lc))/100;
}

int allouer_fifo(liste_processus *lp,int instant,liste_bloc_mem *mem,int choix,liste_cmt *cmt)
{
    processus *proc=lp->tete;
    bloc_mem *adr;
    int nbr1=lp->nb_proc_a_allouer;
    int nbr2=lp->nb_proc_en_att_mem;
    int mnt=0;

    while(proc!=NULL)
    {
        if(proc->date_arr<=instant&&proc->etat_alloc=='n')//date of arrival and if the proc is already allocated (o=yes) in the memory or not (n=no)
        {
            if(choix==2)
                adr=chercher_esp_BF(*proc,*mem);
            else
                adr=chercher_esp_FF(*proc,*mem);

            if(adr!=NULL)
            {

                inserer_ds_RAM(proc,adr,mem,instant,cmt,lp);//insert such a process at the addr address of the memory mem

                lp->nb_proc_a_allouer--;
                if(proc->etat_attente_mem=='o')
                    {
                        lp->nb_proc_en_att_mem--;
                        proc->etat_attente_mem='n';
                    }
                mem->nb_proc_a_executer++;

                /*proc=proc->prec;
                if(proc!=NULL)//le pointeur l n'est pas avant la tete
                {
                    retirer_proc(proc->suiv,lp);
                }
                else//le processus a supprimer est a la tete
                {
                    retirer_proc(lp->tete,lp);
                }*/
            }
            else
            {
                if(proc->date_arr==instant)
                    {
                        proc->etat_attente_mem='o';
                        lp->nb_proc_en_att_mem++;
                        mnt=1;
                    }
            }
        }

        /*if(proc==NULL)
            proc=lp->tete;
        else*/
        proc=proc->suiv;
    }

    if(mnt==1)inserer_cmt(cmt,instant,proc->code,0,0,0,*mem->tete,lp->tete,2);

    if(nbr1!=nb_proc_a_allouer(*lp)||nbr2!=nb_proc_en_att_mem(*lp))
        return 1;
    else
        return 0;
}

void afficher_proc_en_att(liste_processus LC_proc,int col,int lig)
{
    processus *proc;

    for(proc=LC_proc.tete;proc!=NULL;proc=proc->suiv)
    {
        if(proc->etat_attente_mem=='o')
           {    Locate(col,lig);
               printf("%s",proc->code);
               lig++;
           }
    }
}

void afficher_etat_mem(liste_bloc_mem mem,liste_processus lc,int T,int lig,int col,int taille_mem,int nb_lig_stdr)
{
    bloc_mem *m;
    int i,affich_etat_proc=0,affich_etat_bloc=0,fin,nb_lig=0;
    int stop;
     printf("\n\n");
        Locate(col,lig);
        putchar(218);putchar(196);putchar(196);putchar(196);putchar(196);putchar(196);putchar(196);
        putchar(196);putchar(191);printf("0");//printf("+-------+");

    for(m=mem.tete; m!=NULL; m=m->suiv)
    {
        stop=0;
        if(m->suiv!=NULL)
        {
            if(m->taille<200)
            {
                fin=2;
                nb_lig=nb_lig+fin+1;
            }
            else
            {
                if((m->taille)>=200&&(m->taille%100)<50)
                {
                    fin=(m->taille/100);
                    nb_lig=nb_lig+fin+1;
                }
                else
                {
                    fin=(m->taille/100)+1;
                    nb_lig=nb_lig+fin+1;
                }
            }
        }
        else
        {

            fin=nb_lig_stdr-nb_lig+1;

        }

        for (i=1; i<=fin; i++)
        {
            if(i==fin/2&&m->etat_bloc=='o'&&affich_etat_proc==0)
            {
                lig++;
                Locate(col,lig);
                putchar(179);
                if(m->etat_proc=='p')textcolor(14);
                else textcolor(10);
                cprintf("   %s   ",m->code_proc);
                putchar(179);
                affich_etat_proc=1;
            }
            else
            {
                if(i==fin/2&&m->etat_bloc=='l'&&affich_etat_bloc==0&&stop==0)
                {
                    lig++;

                Locate(col,lig);
                putchar(179);
                textcolor(11);
                    cprintf("  %d ",m->taille);
                    if(m->taille<1000&&m->taille>=100)//120
                    {
                        printf(" ");
                        putchar(179);
                    }
                    else
                    {
                        if(m->taille<100&&m->taille>=10)//90
                        {
                            textcolor(11);
                            cprintf("  ");
                            putchar(179);
                        }

                        else
                        {
                            putchar(179);
                        }//1000
                    }

                    affich_etat_bloc=1;
                }
                else
                {
                    lig++;
                Locate(col,lig);
                    if(affich_etat_proc==0&&affich_etat_bloc==0)
                    {
                        putchar(179);
                        printf("       ");
                        putchar(179);
                    }
                else
                    {
                        if(affich_etat_proc==1)
                        {
                            if(m->etat_proc=='p')
                            {
                            putchar(179);
                            textcolor(14);
                            cprintf(" Ready ");
                            putchar(179);
                            }

                            else
                            {
                                putchar(179);
                                textcolor(10);
                                cprintf("Elected");
                                putchar(179);
                            }
                                affich_etat_proc=0;
                        }
                        else
                        {
                               putchar(179);
                                textcolor(11);
                                cprintf("  Free ");
                                putchar(179);
                                affich_etat_bloc=0;
                                stop=1;
                        }
                    }
                }
            }
        }

        lig++;
        if(m->suiv!=NULL)
        {
        Locate(col,lig);
        putchar(195); putchar(196);putchar(196);putchar(196);putchar(196);putchar(196);putchar(196);
        putchar(196);putchar(180); printf("%d",m->suiv->adrr_debu_bloc);//printf("+-------+");
        }
    }
    Locate(col,lig);
    putchar(192);putchar(196);putchar(196);putchar(196);putchar(196);putchar(196);putchar(196);
    putchar(196);putchar(217);
    printf("%d",taille_mem);//printf("+-------+ %d",taille_mem);
    Locate(col+2,lig+1);
    printf(" T=%d",T);
    afficher_proc_en_att(lc,col+4,lig+2);

     /*for(m=mem.tete; m!=NULL; m=m->suiv)
    {
        printf("A T=%d ",T);
        if(m->etat_bloc=='l')
            printf("Etat du Bloc : %c Code : Rien Etat du processus : Rien Taille : %d Addr deb : %d ",m->etat_bloc,m->taille,m->adrr_debu_bloc);
        else
            printf("Etat du Bloc : %c Code : %s Etat du processus : %c Taille : %d Addr deb : %d ",m->etat_bloc,m->code_proc,m->etat_proc,m->taille,m->adrr_debu_bloc);
    printf("\n");

    }*/



}

void afficher_les_commentaires(liste_cmt comentaire)
{
    elem_cmt *cmt;

printf("\n\n");
     for(cmt=comentaire.tete;cmt!=NULL;cmt=cmt->suiv)
    {
        printf("\nT=%d :",cmt->inst);
        if(strcmp(cmt->nv_proc_alloue,"not")!=0)
            printf("\n-%s ",cmt->nv_proc_alloue+3);

            if(strcmp(cmt->proc_elu,"not")!=0)
            printf("\n-%s ",cmt->proc_elu+3);

           if(strcmp(cmt->proc_pret,"not")!=0)
            printf("\n-%s ",cmt->proc_pret+3);

            if(strcmp(cmt->proc_en_att,"not")!=0)
            printf("\n-%s ",cmt->proc_en_att+3);

             if(strcmp(cmt->proc_termine,"not")!=0)
            printf("\n-%s ",cmt->proc_termine+3);
   printf("\n");
    }

}

void titre(void)
{
    printf("                         -----------------------------------------------------\n");
    printf("                        |                   ~~ A2M BF-FF ~~                   |\n");
    printf("                        |              MEMORY ALLOCATION PROGRAM              |\n");
    printf("                         -----------------------------------------------------\n");
}

void introduction(void)
{
   printf("\n\n\n\t\t\t\t       Hello (^_^) !!\n\t Here is a memory management program according to the type of allocation chosen\n ");
    printf(" knowing that the memory queue is managed by FIFO and the processor queue is managed by SJF ");
    printf("\n\t                           Press any key to begin ");
}

int menu_prin(int enreg_don)
{
    int boucle=1,rep;

    printf(" \n                                     -------------------------------\n");
    printf("                                    |           MAIN MENU            |\n");
    printf("                                     -------------------------------\n");
    printf("                                    |   1 . Loading data             |\n");;
    printf("                                    |   2 . List of data             |\n");;
    printf("                                    |   3 . Choice of algorithm      |\n");
    printf("                                    |   4 . Exit                     |\n");
    printf("                                     -------------------------------\n");
    do
    {
        printf("\t\t\t\t\t\t    Your choice : ");
        rep=getch();


        if(rep!=49&&rep!=50&&rep!=51&&rep!=52)
        {
            printf("\nPlease make a choice between 1 and 4 !!\n");
            boucle=1;
        }
        else
        {
            if(enreg_don==1)
            {
                boucle=0;
            }
            else
            {
                if((rep==50||rep==51)&&enreg_don==0)
                    printf("\nYou need to upload your data first !!\n");
                else
                    boucle=0;
            }
        }
    }
    while(boucle==1);
    return rep;
}

int menu_ch_algo(void)
{
    int rep;

    titre();

    printf("\n\t\t                    --------------------------------\n");
    printf("\t\t                   |    ALLOCATION ALGORITHMS       |\n");
    printf("\t\t                    --------------------------------\n");
    printf("\t\t                   |          1. FIRST FIT          |\n");
    printf("\t\t                   |          2. BEST  FIT          |\n");
    printf("\t\t                    --------------------------------\n");

    printf("\nR : return to the main menu\n");
    printf("Your Choice : ");

    do
    {
        rep=getch();
    }
    while(rep!=49&&rep!=50&&rep!=114&&rep!=82);

    if(rep==114||rep==82)
        rep=-1;
    else
    {
        if(rep==49)
            rep=1;
        else
            rep=2;
    }

    return rep;
}

int main()
{
    liste_processus LC_prc;
    liste_bloc_mem LC_bm;
    processus *proc_elu;
    char nom_fich[20],cpu,code_proc[10],taille[10];
    int taille_mem,instant,choix,maj,rep=-1,enreg_don=0;
    int lig,col;
    int nb_lig_stdr;
    int cpt_affich_maj;
    liste_cmt comentaire;

    titre();
    introduction();
    getch();
    system("cls");

    do
    {
        if(rep==-1)
        {
            if(enreg_don==1)
                system("cls");

            titre();
            rep=menu_prin(enreg_don);

            if(rep==52)
            {
                printf("\n\n\t\t     Thank you for using this program (^_^) see you soon !!\n\n");
                getch();
            }
        }
        else
        {
            if(rep==49)
            {
                if(enreg_don==0)
                {
                    printf("\n\nGive the title of the file to be loaded with the .TXT extension : ");
                    scanf("%s",nom_fich);

                    if(verif_bcp(nom_fich)==0)
                    {
                        rep=-1;
                        system("cls");
                    }
                    else
                    {
                        if(verif_occur_code(nom_fich)==0)
                        {
                            enreg_don=0;
                            rep=-1;
                            system("cls");
                        }
                        else
                        {
                            initialiser_LC_proc(&LC_prc);
                            constr_LC_processus(nom_fich,&LC_prc);
                            trier_LC_proc(LC_prc.tete);
                            rep=-1;
                            enreg_don=1;
                            printf("Successful data loading !! \n");
                            getch();
                        }
                    }
                }
                else
                {
                    printf("\nData are already loaded ! Do you want to overwrite them ? O/N : ");

                    do
                    {
                        rep=getch();
                    }
                    while(rep!=111&&rep!=110&&rep!=78&&rep!=79);

                    if(rep==111||rep==79)
                    {
                        supp_liste_proc(LC_prc.tete);
                        printf("\nSuccessful deletion of old data !!");

                        printf("\nGive the title of the new file to load with the .TXT extension : ");
                        scanf("%s",nom_fich);

                        if(verif_bcp(nom_fich)==0)
                        {
                            rep=-1;
                            enreg_don=0;
                            system("cls");
                        }
                        else
                        {
                            if(verif_occur_code(nom_fich)==0)
                            {
                                enreg_don=0;
                                rep=-1;
                                system("cls");
                            }
                            else
                            {
                                initialiser_LC_proc(&LC_prc);
                                constr_LC_processus(nom_fich,&LC_prc);
                                trier_LC_proc(LC_prc.tete);

                                rep=-1;
                                enreg_don=1;
                                printf("Successful data loading !! \n");
                                getch();
                            }
                        }
                    }
                    else
                    {
                        printf("\nNo task were performed\n");
                        getch();
                        rep=-1;
                    }
                }
            }
            else
            {
                if(rep==50)
                {
                    system("cls");
                    titre();
                    afficher_proc(LC_prc.tete,LC_prc.nb_proc_a_allouer,nom_fich);
                    printf("\n\nR : return to the main menu ");
                    do
                    {
                        rep=getch();
                    }
                    while(rep!=114&&rep!=82);

                    if(rep==114||rep==82)
                        rep=-1;
                }
                else
                {
                    if(rep==51)
                    {
                        system("cls");
                        choix=menu_ch_algo();

                        if(choix==1||choix==2)
                        {
                        do
                        {
                            printf("\nGive the size of the RAM : ");
                            scanf("%s",taille);
                        }
                        while(verif_nombre(taille,max_proc_esp(LC_prc))==0);

                        taille_mem=atoi(taille);
                        initialiser_LC_bloc_mem(&LC_bm,taille_mem);
                        comentaire.tete=NULL;
                        nb_lig_stdr=nb_ligne_standard(LC_prc,taille_mem)+1;
                        cpt_affich_maj=0;
                        cpu='l'; instant=0;maj=0;

                        lig=(LC_prc.nb_proc_a_allouer*2)+17;
                        col=2;
                        //if(LC_prc.nb_proc_a_allouer>10||taille_mem>3200)max_affich_par_lig=8;else max_affich_par_lig=8;


                        system("cls");
                        titre();
                        printf("\n");
                        if(choix==1)
                        title_first_fit();
                        else
                        title_best_fit();
                        printf("\n");
                        afficher_proc(LC_prc.tete,LC_prc.nb_proc_a_allouer,nom_fich);
                        afficher_etat_mem(LC_bm,LC_prc,instant,lig,col,taille_mem,nb_lig_stdr);
                        instant=LC_prc.tete->date_arr;
                        proc_elu->tmp_exec=0;

                        do
                        {
                            if(LC_prc.nb_proc_a_allouer!=0&&proc_elu->tmp_exec!=1)
                            {
                                maj=allouer_fifo(&LC_prc,instant,&LC_bm,choix,&comentaire);//the function sends 1 in 2 cases: allocation of a process or waiting for a process, i.e. 0 if there is no change to be made.
                            }

                            if(cpu=='l'&&LC_bm.nb_proc_a_executer!=0)
                            {
                                proc_elu=chercher_proch_proc_a_executer_SJF(&LC_prc,&LC_bm,&comentaire,instant);
                                cpu='o';
                                maj=1;
                            }
                            else
                            {
                                proc_elu->tmp_exec--;

                                if(proc_elu->tmp_exec==0)
                                {
                                    strcpy(code_proc,proc_elu->code);
                                    retirer_proc_de_LCproc(proc_elu,&LC_prc);
                                    retirer_proc_de_LCbm(code_proc,&LC_bm,&comentaire,instant,&LC_prc);

                                    if(LC_prc.nb_proc_a_allouer!=0)
                                    {
                                        allouer_fifo(&LC_prc,instant,&LC_bm,choix,&comentaire);
                                    }

                                    if(LC_bm.nb_proc_a_executer!=0)
                                    {
                                        proc_elu=chercher_proch_proc_a_executer_SJF(&LC_prc,&LC_bm,&comentaire,instant);
                                        cpu='o';
                                    }
                                    else
                                        cpu='l';

                                    maj=1;
                                }
                            }

                            if(maj==1)
                            {
                                cpt_affich_maj++;

                                if(cpt_affich_maj==8)
                                {
                                    lig=lig+nb_lig_stdr+nb_proc_en_att_mem(LC_prc)+5;
                                    col=-11;
                                    cpt_affich_maj=0;
                                }

                                col=col+14;
                                afficher_etat_mem(LC_bm,LC_prc,instant,lig,col,taille_mem,nb_lig_stdr);
                                maj=0;

                            }
                            instant++;
                        }
                        while(LC_prc.nb_proc_a_allouer!=0||LC_bm.nb_proc_a_executer!=0);

                        afficher_les_commentaires(comentaire);
                        initialiser_LC_proc(&LC_prc);
                        constr_LC_processus(nom_fich,&LC_prc);
                        trier_LC_proc(LC_prc.tete);

                    printf("\n\nR : return to the main menu\t\t\t\t\t    M : return to the menu choice of algorithms");
                    do
                    {
                        rep=getch();
                    }
                    while(rep!=114&&rep!=82&&rep!=109&&rep!=77);

                    if(rep==114||rep==82)
                    rep=-1;
                    else
                    rep=51;
                    }
                    else
                        rep=-1;
                    }
                }
            }
        }
    }
    while(rep!=52);

    return 0;
}
