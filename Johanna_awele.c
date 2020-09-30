

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define N 6  //Nombre de cases du plateau
#define I 4  //Nombre de graines initiales


//**** AFFICHE LES REGLES DU JEU  ****//
void regles()
{
  printf("\nL'awalé se joue sur un plateau composé de 2 rangées de 6 trous chacune, chaque trou recevant 4 graines au début du jeu.");
  printf("\nChaque joueur choisi une rangée qui devient alors son camp. Le but du jeu est de récolter plus de graines que son adversaire.");
  printf("\nChacun son tour, un joueur prend toutes les graines d'une des cases de son camp et les sèment une par une dans les cases suivantes dans le sens inverse de celui des aiguilles d'une montre.");
  printf("\nSi le nombre de graines prises dans un trou excède 11, on sème pendant un tour complet, on saute le trou de départ, puis on continue à semer dans les autres trous suivants.");
  printf("\nSi la dernière graine semée est déposée dans une case du camp adverse, dans laquelle il y a après le semage 2 ou 3 graines, le joueur ramasse toutes les graines de ce trou (y compris celle qu’il vient de semer).");
  printf("\nDe même, il ramasse les graines des cases précédentes du camp adverse si elles sont au nombre de 2 ou 3. Dans tous les autres cas il ne récolte rien.");
  printf("\nLorsque la récolte est possible, le grenier du joueur augmente du nombre de graines récoltées.\n\n");
}


//**** INITIALISATION DE L'AWELE  ****//
void initialisation(int M[2][N])
{
  int i;
  for (i=0;i<N;i++)
    {
      M[0][i]=I;  //Au début du jeu chaque case contient I graines
      M[1][i]=I;
    }
}


//**** AFFICHE LE SEPARATEUR  ****//
void separateur(int n)
{
  int i;
  for (i=0;i<n;i++)   
    {printf("\033[34;01m+---\033[00m");}
  puts("\033[34;01m+\033[00m");
}


//**** AFFICHE L'AWELE  ****//
void affiche(int M[2][N],int grenier_j,int grenier_o,int mode)
{
  int i,j;
  for (i=0;i<2;i++)   //Il y a deux lignes à afficher
  {
    separateur(N);
    for (j=0;j<N;j++)   //Il y a N colonnes à afficher
      {printf("|%3d",M[i][j]);}
    printf("\033[34;01m|\033[00m");
    if (i==0)   //Le 1er plateau est celui de l'ordinateur
    {
      if (mode==1) {printf("\033[32;01m\tPlateau de l'ordi\tGrenier : %d graines\n\033[00m",grenier_o);}
      if (mode==2) {printf("\033[32;01m\tPlateau de J1\tGrenier J1 : %d graines\n\033[00m",grenier_o);}
    }
    else   //Le 2eme plateau est celui du joueur
    {
      if (mode==1) printf("\033[35;01m\tVotre plateau\t\tGrenier : %d graines\n\033[00m",grenier_j);
      if (mode==2) {printf("\033[35;01m\tPlateau de J2\tGrenier J2 : %d graines\n\033[00m",grenier_j);}
    }
  }
  separateur(N);
}


//*** NOMBRE DE TOURS MAXIMUM VOULUS  ***//
int nb_de_tours()
{
  int nb;
  printf("\nCombien de tours voulez-vous faire ?\n");
  scanf("%d",&nb);
  return nb;
}


//**** CHOIX DE LA CASE JOUEUR  ****//
int choix_joueur(int M[2][N],int mode,int J)
{
  int choix_j;
  if (mode==1) {printf("Quelle case voulez-vous choisir pour semer ?\n");}
  if (mode==2) {printf("J%d : Quelle case voulez-vous choisir pour semer ?\n",J);}
  //Si le jeu est en mode 2 joueurs il faut préciser quel joueur joue
  scanf("%d",&choix_j);
  if (mode==1)
  {
    while ((choix_j<1 ) || (choix_j>N) || (M[1][choix_j-1]==0))
    {
      printf("Veuillez choisir une case non vide entre 1 et 6\n");
      scanf("%d",&choix_j);
    }
  }
  if (mode==2)
  {
    while ((choix_j<1 ) || (choix_j>N) || (M[J-1][choix_j-1]==0))
    {
      printf("Veuillez choisir une case non vide entre 1 et 6\n");
      scanf("%d",&choix_j);
    }
  }
  return choix_j;
}


//**** SEME CHEZ LE JOUEUR  ****//
int semer_joueur(int M[2][N],int choix,int reste, int S[2])
{
  int case_=choix;
  while ((case_<N)&&(reste>0))
  {
    M[1][case_]++;
    case_++;
    reste--;
    S[0]=1;       //La première case du tableau S contient le plateau où l'on sème
    S[1]=case_;   //La deuxième case du tableau S contient la case où l'on sème
  }
  return reste;
}


//**** SEME CHEZ L'ORDI  ****//
int semer_ordi(int M[2][N],int choix,int reste,int S[2])
{
  int case_=choix;
  while ((case_>1)&&(reste>0))
  {
    M[0][case_-2]++;
    case_--;
    reste--;
    S[0]=0;    //La première case du tableau S contient le plateau où l'on sème
    S[1]=case_;   //La deuxième case du tableau S contient la case où l'on sème
  }
  return reste;
}


//**** SEME LES GRAINES  ****//
void semer(int M[2][N],int choix,int plateau, int S[2], int grenier_j, int grenier_o, int test, int mode)
{
  int i,reste=M[plateau][choix-1];
  M[plateau][choix-1]=0; //La case choisie est remise à zéro
  if (plateau==1)
    {reste=semer_joueur(M,choix,reste,S);}  //On sème chez le joueur à partir de la case choisie
  else
    {reste=semer_ordi(M,choix,reste,S);}   //On sème chez l'ordinateur à partir de la case choisie

  for (i=0;i<(reste/N)+1+1;i++)  // (reste/N)+1 est le nombre de fois qu'on change de plateau
  {
    if ((i%2==0)&&(plateau==1)&&(reste>0)) 
      {reste=semer_ordi(M,7,reste,S);}    //On sème le reste des graines chez l'ordi
    if ((i%2==0)&&(plateau==0)&&(reste>0))
      {reste=semer_joueur(M,0,reste,S);}   //On sème le reste des graines chez le joueur
    if ((i%2!=0)&&(plateau==1)&&(reste>0))
      {reste=semer_joueur(M,0,reste,S);}
    if ((i%2!=0)&&(plateau==0)&&(reste>0))
      {reste=semer_ordi(M,7,reste,S);}
  }
  if (test==0)  //si test=1 on fait un test pour l'intelligence1 de l'ordinateur
  {
    affiche(M,grenier_j,grenier_o,mode);
    printf("\nLa case %d a été semée !\n",choix);
  }
}


//*** VERIFIE SI LE JOUEUR PEUT RECOLTER ***//
int recolte_joueur(int M[2][N],int S[2])
{
  if ((S[0]==0)&&(M[0][S[1]-1]==2)||(S[0]==0)&&(M[0][S[1]-1]==3))
    {return 1;}
  else
    {return 0;}
}


//*** VERIFIE SI L'ORDI PEUT RECOLTER ***//
int recolte_ordi(int M[2][N],int S[2])
{
  if ((S[0]==1)&&(M[1][S[1]-1]==2)||(S[0]==1)&&(M[1][S[1]-1]==3))
    {return 1;}
  else
    {return 0;}
}


//*** RECOLTE LES GRAINES ***//
int recolte(int M[2][N],int plateau, int S[2],int grenier_j, int grenier_o,int test,int mode)
{
  int gain=0;
  while ((plateau==1)&&(recolte_joueur(M,S)==1)&&(S[1]<=N))
  {
    if (test==0) {printf("\033[36;01mVous récoltez la case %d du plateau de l'ordinateur\n\033[00m",S[1]);}
    gain=gain+M[0][S[1]-1];
    M[0][S[1]-1]=0;
    S[1]++;
  }
  if ((gain>0)&&(test==0)) {affiche(M,grenier_j+gain,grenier_o,mode);} //On affiche l'awele si il y a un gain non nul et si on ne fait pas de test
  while ((plateau==0)&&(recolte_ordi(M,S)==1)&&(S[1]>=1))
  {
    if (test==0) {printf("\033[36;01mL'ordi récolte la case %d de votre plateau\n\033[00m",S[1]);}
    gain=gain+M[1][S[1]-1];
    M[1][S[1]-1]=0;
    S[1]--;
  }
  if ((gain>0)&&(plateau==0)&&(test==0)) {affiche(M,grenier_j,grenier_o+gain,mode);}
  return gain;
}


//**** CALCUL DU GAIN POUR LES N CASES DE L'ORDI ****//
void gain(int M[2][N],int G[N],int S[2])
{
  int i,j,k,C[2][N];
  for (k=0;k<N;k++)  //calculons le gain pour les N cases que l'ordi peut jouer
  {
    for (i=0;i<2;i++)
    {
      for (j=0;j<N;j++) {C[i][j]=M[i][j];}  //copie de l'awele M dans le tableau C
    }
    semer(C,k+1,0,S,0,0,1,1);
    //affiche(C,0,0);
    //printf("S[0]=%d S[1]=%d\n",S[0],S[1]);
    //printf("gain=%d \n",recolte(C,0,S,0,0,1));
    if (M[0][k]!=0)  {G[k]=recolte(C,0,S,0,0,1,1);}  //L'ordi ne peut pas semer une case vide
    else {G[k]=-100;}  //Si la case est nulle on affecte un gain négatif
    //printf("%d\n",G[k]);
  }
}



//**** CALCUL DU GAIN POUR LES N CASES DE L'ORDI ET LES N CASES DU JOUEUR ****//
void gain2(int M[2][N],int G[N],int S[2], int GG[N][N])
{
  int i,j,k,l,C[2][N],CC[2][N];
  for (k=0;k<N;k++)  //calculons le gain pour les N cases que l'ordi peut jouer
  {
    for (i=0;i<2;i++)
    {
      for (j=0;j<N;j++) {C[i][j]=M[i][j];}  //copie de l'awele M dans le tableau C
    }

    semer(C,k+1,0,S,0,0,1,1);  //On sème sur le tableau C pour ne pas modifier M
    if (M[0][k]!=0)  {G[k]=recolte(C,0,S,0,0,1,1);}  //L'ordi ne peut pas semer une case vide
    //G[k] est le gain obtenu par l'ordi lorsqu'il a joué la case k
    else {G[k]=-100;}  //Si la case est nulle on affecte un gain négatif
    
    for (l=0;l<N;l++)  //calculons le gain pour les N cases que le joueur peut jouer
    {
      for (i=0;i<2;i++)
      {
	for (j=0;j<N;j++) {CC[i][j]=C[i][j];}  //copie du tableau C dans le tableau CC
      }
      semer(CC,l+1,1,S,0,0,1,1);  //On sème sur le tableau CC pour ne pas modifier C
      if (C[1][l]!=0)  {GG[k][l]=G[k]-recolte(CC,1,S,0,0,1,1);}
      //gain total = gain ordi - gain joueur
      //GG[k][l] est le gain obtenu par l'ordi lorsqu'il joue la case k et que le joueur joue la case l
      else {GG[k][l]=G[k]-100;}  //Si la case est nulle on affecte un gain négatif
    }
  }
}


//**** CHERCHE LE MAXIMUM D'UN TABLEAU  ****//
int maximumT(int T[N])
{
  int i,max=0,rang=0;
  for (i=0;i<N;i++)
  {
    if (T[i]>=max)
    {
      max=T[i];
      rang=i;
    }
  }
  return rang;
}


//**** CHERCHE LE MAXIMUM D'UNE MATRICE  ****//
void maximumM(int T[N][N],int X[2])
{
  int i,j,max=0;
  for (i=0;i<N;i++)
  {
    for (j=0;j<N;j++)
    {
      if (T[i][j]>=max)
      {
	max=T[i][j];
	X[0]=i;
	X[1]=j;
      }
    }
  }
}


//**** CHOIX DE LA CASE ORDI  ****//
int choix_ordi(int M[2][N],int difficulte)
{
  int i,G[N],S[2],GG[N][N],Gmax,X[2],choix_o=rand()%N+1; //Nombre aléatoire entre 1 et N
  if (difficulte==1)  //Niveau débutant
  {
    while (M[0][choix_o-1]==0) {choix_o=rand()%N+1;}   //Impossible de choisir une case vide
  }
  if (difficulte==2)  //Niveau intermédiaire
  {
    gain(M,G,S);
    choix_o=maximumT(G)+1;
  }
  if (difficulte==3)  //Niveau expert
  {
    gain2(M,G,S,GG);
    maximumM(GG,X);
    choix_o=X[0]+1;
  }
  printf("\nL'ordi décide de semer la case %d de son plateau !\n",choix_o);
  return choix_o;
}


//*** COMPTE LE NOMBRE DE GRAINES RESTANTES ***//
int nombre_de_graines(int M[2][N],int plateau)
{
  int i,j,graines=0;
  for (i=0;i<N;i++)   //Il y a N colonnes à compter
    {graines=graines+M[plateau][i];}
  return graines;
}


//**** FIN DU JEU ****//
int fin_du_jeu(int M[2][N],int tour,int nb,int grenier_j,int grenier_o)
{
  int i,graines_j=nombre_de_graines(M,1),graines_o=nombre_de_graines(M,0);
  if ((grenier_j>=25)||(grenier_o>=25)||(graines_j==0)||(graines_o==0)||(graines_j+graines_o<=6)||(tour>nb))
    {return 1;}
  else
    {return 0;}
}


//**** EXPLIQUE L'ARRET DU JEU ****//
void arret(int M[2][N],int tour,int nb,int grenier_j,int grenier_o,int mode)
{
  int graines_j=nombre_de_graines(M,1),graines_o=nombre_de_graines(M,0);
  if (tour>=nb)
    {printf("\033[31;01m\n\tFIN DU JEU ! %d tour(s) joué(s) !\n\033[00m",tour-1);}
  if ((graines_j==0)&&(mode==1))
    {printf("\033[31;01m\n\tFIN DU JEU ! Il n'y a plus de graine sur votre plateau !\n\033[00m");}
  if ((graines_j==0)&&(mode==2))
    {printf("\033[31;01m\n\tFIN DU JEU ! Il n'y a plus de graine sur le plateau de J2 !\n\033[00m");}
  if ((graines_o==0)&&(mode==1))
    {printf("\033[31;01m\n\tFIN DU JEU ! Il n'y a plus de graine sur le plateau de l'ordinateur !\n\033[00m");}
  if ((graines_o==0)&&(mode==2))
    {printf("\033[31;01m\n\tFIN DU JEU ! Il n'y a plus de graine sur le plateau de J1 !\n\033[00m");}
  if ((grenier_j>=25)&&(mode==1))
    {printf("\033[31;01m\n\tFIN DU JEU ! Vous avez 25 graines ou plus dans votre grenier !\n\033[00m");}
  if ((grenier_j>=25)&&(mode==2))
    {printf("\033[31;01m\n\tFIN DU JEU ! J2 a 25 graines ou plus dans son grenier !\n\033[00m");}
  if ((grenier_o>=25)&&(mode==1))
    {printf("\033[31;01m\n\tFIN DU JEU ! L'ordinateur a 25 graines ou plus dans son grenier !\n\033[00m");}
  if ((grenier_o>=25)&&(mode==2))
    {printf("\033[31;01m\n\tFIN DU JEU ! J1 a 25 graines ou plus dans son grenier !\n\033[00m");}
  if (graines_j+graines_o<=6)
    {printf("\033[31;01m\n\tFIN DU JEU ! Il reste 6 graines ou moins sur le mancala !\n\033[00m");}
  if (grenier_j==grenier_o)
    {printf("\033[31;01m\tEgalité ! Personne ne gagne !\n\n\033[00m");}
  if ((grenier_j>grenier_o)&&(mode==1))
    {printf("\033[31;01m\tBRAVO ! Vous remportez la partie !\n\n\033[00m");}
  if ((grenier_j>grenier_o)&&(mode==2))
    {printf("\033[31;01m\tBRAVO J2 ! Vous remportez la partie !\n\n\033[00m");}
  if ((grenier_j<grenier_o)&&(mode==1))
    {printf("\033[31;01m\tL'ordinateur gagne la partie !\n\n\033[00m");}
  if ((grenier_j<grenier_o)&&(mode==2))
    {printf("\033[31;01m\tBRAVO J1 ! Vous remportez la partie !\n\n\033[00m");}
}


//**** JEU CONTRE L'ORDINATEUR****//
void jeu(int M[2][N])
{
  int choix_j,choix_o,grenier_j=0,grenier_o=0,nb,S[2],tour=1,difficulte;
  printf("\n\nNiveau de l'ordinateur :\n1-Niveau débutant \t 2-Niveau intermédiaire \t 3-Niveau expert\n");
  scanf("%d",&difficulte);
  while ((difficulte!=1) && (difficulte!=2) && (difficulte!=3))
  {
    printf("Veuillez choisir une difficulté entre 1 et 3\n");
    scanf("%d",&difficulte);
  }
  nb = nb_de_tours(); //nombre de tours maximum voulus par l'utilisateur
  if (nb!=0)
    {printf("\nCommençons le jeu ! A vous l'honneur...\n");}
  while (fin_du_jeu(M,tour,nb,grenier_j,grenier_o)==0)
  {
    printf("\033[33;01m\n\n-------------------------Tour n°%d-------------------------\n\n\033[00m",tour);
    choix_j=choix_joueur(M,1,2);
    semer(M,choix_j,1,S,grenier_j,grenier_o,0,1);
    grenier_j=grenier_j+recolte(M,1,S,grenier_j,grenier_o,0,1);
    if (fin_du_jeu(M,tour,nb,grenier_j,grenier_o)==0)
    {
      choix_o=choix_ordi(M,difficulte);
      semer(M,choix_o,0,S,grenier_j,grenier_o,0,1);
      grenier_o=grenier_o+recolte(M,0,S,grenier_j,grenier_o,0,1);
    }
    tour=tour+1;
  }
  arret(M,tour,nb,grenier_j,grenier_o,1);
}


//**** JEU MODE : DEUX UTILISATEURS****//
void jeu2(int M[2][N])
{
  int choix_j1,choix_j2,grenier_j1=0,grenier_j2=0,nb,S[2],tour=1;
  nb = nb_de_tours(); //nombre de tours maximum voulus par l'utilisateur
  if (nb!=0)
    {printf("\nCommençons le jeu ! \n");}
  while (fin_du_jeu(M,tour,nb,grenier_j2,grenier_j1)==0)
  {
    printf("\033[33;01m\n\n-------------------------Tour n°%d-------------------------\n\n\033[00m",tour);
    choix_j1=choix_joueur(M,2,1);
    semer(M,choix_j1,0,S,grenier_j2,grenier_j1,0,2);
    grenier_j1=grenier_j1+recolte(M,0,S,grenier_j2,grenier_j1,0,2);
    if (fin_du_jeu(M,tour,nb,grenier_j2,grenier_j1)==0)
    {
      choix_j2=choix_joueur(M,2,2);
      semer(M,choix_j2,1,S,grenier_j2,grenier_j1,0,2);
      grenier_j2=grenier_j2+recolte(M,1,S,grenier_j2,grenier_j1,0,2);
    }
    tour=tour+1;
  }
  arret(M,tour,nb,grenier_j2,grenier_j1,2);
}


//**** MAIN ****//
int main()
{
  int M[2][N],nb,choix_o,choix_j,refaire,mode,regle;
  srand(time(NULL)); //Initialisation de rand
  printf("\033[36;01m \n\t\tBienvenue ! •ᴗ• Jouons à un jeu : l'AWELE !\n \033[00m");
  printf("\n1-Affichage des règles du jeu\t2-Passer\n");
  scanf("%d",&regle);
  if (regle==1) {regles();}
  printf("\n1-Mode solo \t 2-Mode 2 joueurs\n");
  scanf("%d",&mode);
  printf("\033[36;01m \n\t\tVoilà le Mancala :\n\n \033[00m");
  initialisation(M);
  affiche(M,0,0,mode);  //Il n'y a pas de graine dans les deux greniers au début du jeu
  if (mode==1) {jeu(M);}    //mode solo
  if (mode==2) {jeu2(M);}   //mode 2 joueurs
  printf("0-Arrêter de jouer \t 1-Refaire une partie\n");
  scanf("%d",&refaire);
  while (refaire!=0) //L'utilisateur joue autant de fois qu'il le souhaite
  {
    printf("\033[36;01m\nEn avant pour une nouvelle partie !\n\n\033[00m");
    printf("\n1-Mode solo \t 2-Mode 2 joueurs\n");
    scanf("%d",&mode);
    printf("\033[36;01m \n\t\tVoilà le Mancala :\n\n \033[00m");
    initialisation(M);
    affiche(M,0,0,mode);  //Il n'y a pas de graine dans les deux greniers au début du jeu
    if (mode==1) {jeu(M);}
    if (mode==2) {jeu2(M);}
    printf("0-Arrêter de jouer \t 1-Refaire une partie\n");
    scanf("%d",&refaire);
  }
  return 0;
}
