#include "environnement.h"
#include "programme.h"
#include "interprete.h"

#include <stdio.h>
#include <stdlib.h>


void gestion_erreur_terrain(erreur_terrain e) {
  switch(e) {

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
   	printf("Erreur lecture du terrain : position initiale du robot manquante\n");
    	exit(1);
  }
}

void affichage_position_programme(erreur_programme e) {
  int i;
  printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
  printf("%s\n", e.ligne);
  for(i = 1; i < e.num_colonne; i++) {
    printf(" ");
  }
  printf("^\n");
}

void gestion_erreur_programme(erreur_programme e) {
  switch(e.type_err) {

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

int main(int argc, char ** argv) {
  Environnement envt;
  Programme prog;
  erreur_terrain errt;
  erreur_programme errp;
  etat_inter etat;
  resultat_inter res;

  
  if (argc < 3) {
    printf("Usage: %s <terrain> <programme>\n", argv[0]);
    return 1;
  }

  errt = initialise_environnement(&envt, argv[1]);
  gestion_erreur_terrain(errt);

  errp = lire_programme(&prog, argv[2]);
  gestion_erreur_programme(errp);


  init_etat(&etat);
  do {
    res = exec_pas(&prog, &envt, &etat);

    afficher_envt(&envt);
  }while(res == OK_ROBOT);

  switch(res) {

 	 case OK_ROBOT: 
		printf("Robot sur une case libre, programme non terminé (ne devrait pas arriver)\n"); 
		break;

 	 case SORTIE_ROBOT: 
		printf("Le robot est sorti :-)\n"); 
		break;

  	 case ARRET_ROBOT: 
		printf("Robot sur une case libre, programme terminé :-/\n"); 
		break;

  	 case PLOUF_ROBOT: 
		printf("Le robot est tombé dans l'eau :-(\n"); 
		break;
 	 case CRASH_ROBOT: 
		printf("Le robot s'est écrasé sur un rocher X-(\n"); 
		break;

 	 case ERREUR_PILE_VIDE: 
		printf("ERREUR : pile vide\n"); 
		break;

  	 case ERREUR_ADRESSAGE: 
		printf("ERREUR : erreur d'adressage\n"); 
		break;

  	 case ERREUR_DIVISION_PAR_ZERO: 
		printf("ERREUR : division par 0\n"); 
		break;
  }

  switch(envt.obs){

  	case ERR : 
		printf("Compte Rendu observateur : Erreur \n "); 
		break;

  	case MES : 
		printf("Compte Rendu observateur : Ok \n"); 
		break;

  	case INIT : 
		printf("Compte Rendu observateur : Ok \n"); 
		break;
  }
}