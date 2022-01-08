#include "environnement.h"
#include "interprete.h"
#include "programme.h"
#include "robot.h"
#include "terrain.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void gestion_erreur_terrain(erreur_terrain e) {
  switch (e) {
  case OK:
    break;
  case ERREUR_FICHIER:
    printf(" Erreur lecture du terrain : erreur d'ouverture du fichier\n");
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
int main(int argc, char **argv) {

  assert(argc == 2);

  FILE *ftest = fopen(argv[1], "r");
  assert(ftest);
  int nombre_exec, x_test, y_test;           // 3 , 5 lignes du fichier
  char even_test, even_attendu, orientation; // 4,6 ligne du fichier

  char nomTerrain[256], nomProgramme[256];

  fgets(nomTerrain, sizeof(nomTerrain), ftest);
  nomTerrain[strlen(nomTerrain) - 1] = '\0';
  
  fgets(nomProgramme, sizeof(nomProgramme), ftest);
  nomProgramme[strlen(nomProgramme) - 1] = '\0';
  fscanf(ftest, "%d", &nombre_exec);
  fscanf(ftest, "\n");
  fscanf(ftest, "%c", &even_attendu);
  
  fscanf(ftest, "\n");
  
  
  // On lit pour l'instant jusqu'a l'evenement attendu pour voir apres

  Environnement envt;
  erreur_terrain errt;
  Programme prog;
  erreur_programme errp;
  etat_inter etat;
  resultat_inter res;

  /* Initialisation de l'environnement : lecture du terrain,
     initialisation de la position du robot */
     
  //printf("%s\n",nomTerrain);   
  errt = initialise_environnement(&envt, nomTerrain);
  gestion_erreur_terrain(errt);
  //printf("%s\n",nomTerrain); 
  /* Lecture du programme */
  errp = lire_programme(&prog, nomProgramme);
  gestion_erreur_programme(errp);

  /* Initialisation de l'état */
  init_etat(&etat);

  int i = 0;
  while (i < nombre_exec) {
    res = exec_pas(&prog, &envt, &etat);
    /*if (res == OK_ROBOT){
        even_test = 'N';
        break;
        }
    else if (res == ARRET_ROBOT){
        even_test = 'F' ;
        break;
        }*/
    i++;
    switch (res) {
    case OK_ROBOT:
      even_test = 'N';
      break;
    case ARRET_ROBOT:
      even_test = 'F';
      break;
    case SORTIE_ROBOT:
      even_test = 'S';
      break;
    case PLOUF_ROBOT:
      even_test = 'P';
      break;
    default:
      even_test = 'O';
      break;
    }
    
  }
  
  if (even_test == even_attendu) {
    
    // Il faut verifier si les corrdonnees correspond a ceux qui sont fourni
    // dans le fichier
    fscanf(ftest, "%d", &x_test);
    fscanf(ftest, " ");
    fscanf(ftest, "%d", &y_test);
    fscanf(ftest, "\n");
    fscanf(ftest, "%c", &orientation);

    Orientation o;
    switch (orientation) {
    case 'N':
      o = Nord;
      break;
    case 'E':
      o = Est;
      break;

    case 'S':
      o = Sud;
      break;

    case 'O':
      o = Ouest;
      break;
    }

    if (x_test == abscisse(envt.r) && y_test == ordonnee(envt.r) && o == orient(envt.r))
      printf("Ok");
    else {
      char or ;
      switch (orient(envt.r)) {
      case Nord:
        or = 'N';
        break;

      case Est:
        or = 'E';
        break;

      case Sud:
        or = 'S';
        break;
      case Ouest:
        or = 'O';
        break;
      }
      printf("La direction , (abscisse,ordonnee) du robot sont : %c,( %d, "
             "%d)\n mais vous avez : %c , ( %d, %d) ",
             even_attendu, x_test, y_test, or, abscisse(envt.r),
             ordonnee(envt.r));
    }
    
  }
  else
    printf("L'evenement attendu est %c mais le teste donne %c" , even_attendu , even_test); 

  return 0;
}
