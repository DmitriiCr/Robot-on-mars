#include "generation_terrains.h"
#include "terrain.h"
#include "robot.h"
#include "environnement.h"
#include "interprete.h"
#include "programme.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void gestion_erreur_terrain(erreur_terrain e) {
  switch (e) {
  case OK:
    break;
  case ERREUR_FICHIER:
    printf("Erreur lecture du terrain : erreur d'ouverture du fichier\n");
    exit(1);
  case ERREUR_LECTURE_LARGEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la largeur\n");
    exit(1);
  case ERREUR_LECTURE_HAUTEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la hauteur\n");
    exit(1);
  case ERREUR_LARGEUR_INCORRECTE:
    printf("Erreur lecture du terrain : largeur incorrecte\n");
    exit(1);
  case ERREUR_HAUTEUR_INCORRECTE:
    printf("Erreur lecture du terrain : hauteur incorrecte\n");
    exit(1);
  case ERREUR_CARACTERE_INCORRECT:
    printf("Erreur lecture du terrain : caractère incorrect\n");
    exit(1);
  case ERREUR_LIGNE_TROP_LONGUE:
    printf("Erreur lecture du terrain : ligne trop longue\n");
    exit(1);
  case ERREUR_LIGNE_TROP_COURTE:
    printf("Erreur lecture du terrain : ligne trop courte\n");
    exit(1);
  case ERREUR_LIGNES_MANQUANTES:
    printf("Erreur lecture du terrain : lignes manquantes\n");
    exit(1);
  case ERREUR_POSITION_ROBOT_MANQUANTE:
    printf(
        "Erreur lecture du terrain : position initiale du robot manquante\n");
    exit(1);
  }
}

void affichage_position_programme(erreur_programme e) {
  int i;
  printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
  printf("%s\n", e.ligne);
  /* Impression de e.num_colonne-1 espaces */
  for (i = 1; i < e.num_colonne; i++) {
    printf(" ");
  }
  /* Impression d'un curseur de position */
  printf("^\n");
}

void gestion_erreur_programme(erreur_programme e) {
  switch (e.type_err) {
  case OK_PROGRAMME:
    break;
  case ERREUR_FICHIER_PROGRAMME:
    printf("Erreur lecture du programme : erreur d'ouverture du fichier\n");
    exit(2);
  case ERREUR_BLOC_NON_FERME:
    printf("Erreur lecture du programme : bloc non fermé\n");
    exit(2);
  case ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
    printf("Erreur lecture du programme : fermeture de bloc excédentaire\n");
    affichage_position_programme(e);
    exit(2);
  case ERREUR_COMMANDE_INCORRECTE:
    printf("Erreur lecture du programme : commande incorrecte\n");
    affichage_position_programme(e);
    exit(2);
  }
}



int nb_cases_occupees(Terrain T) {
  int occupee = 0;
  int x, y;
  for (x = 0; x < largeur(T); x++) {
    for (y = 0; y < hauteur(T); y++) {
      if (T.tab[x][y] != LIBRE)
        occupee++;
    }
  }
  return occupee;
}

int exe_step(Terrain T,char *f_prog,int step)
{
  Environnement envt;
  Programme prog;
  erreur_terrain errt;
  erreur_programme errp;
  etat_inter etat;
  resultat_inter res;
  int retour = 1;
  int compt = 0;

  FILE* terrain_buff;
  terrain_buff = fopen("terrain_buff.txt","w");
  printf("posi_robot x %d :", (int)(T.largeur/2));
  ecrire_terrain(terrain_buff,T,(int)(T.largeur/2),(int)(hauteur(T)/2));
  fclose(terrain_buff);
  
  errt = initialise_environnement(&envt, "terrain_buff.txt");
  gestion_erreur_terrain(errt);

  errp = lire_programme(&prog, f_prog);
  gestion_erreur_programme(errp);

  init_etat(&etat);
  do {
    res = exec_pas(&prog, &envt, &etat);
    afficher_envt(&envt);
    compt++;
  } while(res == OK_ROBOT && compt < step);
  
switch(res) {

  case OK_ROBOT: 
	printf("Robot sur une case libre, programme non terminé (ne devrait pas arriver)\n");
	retour = 3; 
	break;
  
  case SORTIE_ROBOT: 
	printf("Le robot est sorti :-)\n"); 
	retour = 0;
	break;

  case ARRET_ROBOT: 
	printf("Robot sur une case libre, programme terminé :-/\n"); 
	retour = 2;
	break;

  case PLOUF_ROBOT: 
	printf("Le robot est tombé dans l'eau :-(\n"); 
	retour = 1;
	break;

  case CRASH_ROBOT: 
	printf("Le robot s'est écrasé sur un rocher X-(\n"); 
	retour = 1;
	break;

  case ERREUR_PILE_VIDE: 
	printf("ERREUR : pile vide\n"); 
	retour = 3;
	break;

  case ERREUR_ADRESSAGE: 
	printf("ERREUR : erreur d'adressage\n"); 
	retour = 3;
	break;

  case ERREUR_DIVISION_PAR_ZERO: 
	printf("ERREUR : division par 0\n"); 
	retour = 3;
	break;
  }
  if (compt >= step)
  {
      printf("Nombre de pas trop important \n");
      retour = 2;
  }
  printf("%d \n", retour);
    return retour;
}


int main(int argc, char **argv) {
  int N, L, H, step, graine;
  float d;
  char *f_prog; 
  FILE *fres;
  Terrain T;
  int compt=0;

  if (argc != 9) {
    printf(
        "Usage: %s <fichier_programme> <N> <L> <H> <d> <graine> <nb_step_max> <fichier_res>\n",
        argv[0]);
    return 1;
  }
  f_prog = argv[1];
  N = atoi(argv[2]);
  L =atoi(argv[3]);
  H = atoi(argv[4]);
  d = atof(argv[5]);
  graine = atoi(argv [6]);
  step = atoi(argv[7]);



  // test de l et h
  if (L > DIM_MAX || L % 2 == 0) {
    printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if (H > DIM_MAX || H % 2 == 0) {
    printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
    return 1;
  }
  if ((d > 1) || (d < 0)) {
    printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
    return 1;
  }

  srand(graine);

  int crash=0;
  int succ=0;
  int blo=0;

  fres = fopen(argv[8], "w");
  fprintf(fres,"%s %d \n","Le nombre de terrain tester est ",N);


  while(compt < N){

	generation_aleatoire(&T,L,H,d);

 
	if (existe_chemin_vers_sortie(T)){
		switch (exe_step(T,f_prog,step)){
			case 0:
				succ++;
				compt++;
				break;
			case 1:
				crash++;
				compt++;
				break;
			case 2:
				blo++;
				compt++;
				break;
			case 3:
				compt++;
				break;
		}
	}
  }		
		
  fprintf(fres,"%s %d \n","Le nombre de fois ou le robot est sorti est",succ);
  int moy_succ= (succ*100)/compt;
  fprintf(fres,"%s %d %c \n","Ce qui fais une moyenne de ",moy_succ,'%');
  fprintf(fres,"%s %d \n","Le nombre de fois ou le robot est resté bloqué est",blo);
  int moy_blo= (blo*100)/compt;
  fprintf(fres,"%s %d %c \n","Ce qui fais une moyenne de ",moy_blo,'%');
  fprintf(fres,"%s %d \n","Le nombre de fois ou le robot a renconté un obstacle est",crash);
  int moy_crash= (crash*100)/compt;
  fprintf(fres,"%s %d %c \n","Ce qui fais une moyenne de ",moy_crash,'%');
  // fermeture des fichiers
  fclose(fres);
  return 0;
}
