#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matiere.h"
#include "../classes/classe.h"
#include "../notes/matiere.h"

void view_matiere()
{
	system("cls");
	printf("***Liste de toute les matiers***\n");
	FILE *file = fopen("./data/matieres.csv", "r");

	Matiere mat;

	if(file == NULL){
		printf("L'ouverture du fichier a echoue.\n");
		exit(EXIT_FAILURE);
	}

	printf("+-------------+------------------+---------------+\n");
	printf("|  Reference  |     Libellet     |  Coefficient  |\n");
	printf("+-------------+------------------+---------------+\n");

	do{
		fscanf(file, "%d,%[a-zA-Z ],%d\n", &mat.reference, mat.libelle, &mat.coefficient);
		printf("|    %3d      |  %-16s|  %3d          |\n", mat.reference, mat.libelle, mat.coefficient);
		printf("+-------------+------------------+---------------+\n");
	}while(!feof(file));
	fclose(file);
}

void add_matiere()
{
	system("cls");
	printf("***Ajout d'une nouvelle matiere***\n");

	FILE *file = fopen("./data/matieres.csv", "a");
	
	if(file == NULL){
		printf("L'ouverture du fichier a echoue.\n");
		exit(EXIT_FAILURE);
	}

	Matiere mat;
	char chaine[16];
	int entier = 0, bad = 0;

	printf("Entrer le libelle de la matiere : ");
	GRAB_LIBELLE_ADD:
	scanf("%[a-zA-Z0-9 ]", chaine);
	fflush(stdin);

	if(strlen(chaine) < 3){
		printf("Incorrect le libelle doit avoir plus que 3 carractere. Entrer le libelle : ");
		goto GRAB_LIBELLE_ADD;
	}
	strcpy(mat.libelle, chaine);

	printf("Entrer le coefficient de la matiere : ");
	GRAB_COEFICCIENT_ADD:
	bad = scanf("%d", &entier);
	fflush(stdin);

	if(bad != 1){
		printf("Incorrect le coefficient doit etre un entier. Entrer le coefficient : ");
		goto GRAB_COEFICCIENT_ADD;
	}
	mat.coefficient = entier;

	save_matiere(mat);
}

void edit_matiere()
{
	system("cls");
	printf("***Modification d'une matiere***\n");
	int entier = 0, bad = 0;
	char chaine[16];

	printf("Entrer le libelle de la matiere a modifier : ");
	GRAB_LIBELLE_EDIT:
	scanf("%[a-zA-Z0-9 ]", chaine);
	fflush(stdin);

	if(strcmp(chaine, "0") == 0)
		return;

	Matiere find = get_one_matiere_with(chaine);

	if(find.reference != 0){
		print_matiere(find);
	}else{
		printf("Cet libelle n'existe pas. Entrer un libelle svp ou 0 pour retourner : ");
		goto GRAB_LIBELLE_EDIT;
	}

	//Saisi du libelle
	printf("Entrer le nouveau libelle ou 0 pour ne pas modifier : ");
	GRAB_LIBELLE_EDIT_NEW:
	scanf("%[a-zA-Z0-9 ]", chaine);
	fflush(stdin);

	if(strcmp(chaine, "0") != 0){
		if(strlen(chaine) < 3){
			printf("Incorrect le libelle doit avoir plus que 3 carractere. Entrer le libelle : ");
			goto GRAB_LIBELLE_EDIT_NEW;
		}
		strcpy(find.libelle, chaine);
	}

	//Saisi du coefficient
	printf("Entrer le coefficient ou 0 pour ne pas modifier : ");
	GRAB_COEFICCIENT_EDIT_NEW:
	bad = scanf("%d", &entier);
	fflush(stdin);

	if(entier != 0 && bad == 1){
		find.coefficient = entier;
	}else{	
		if(entier != 0 || bad != 1){
			printf("Incorrect ceci n'est pas un coefficient. Entrer le coefficient : ");
			goto GRAB_COEFICCIENT_EDIT_NEW;
		}
	}

	print_matiere(find);
	update_matiere(find, 1);
}

void delate_matiere()
{
	system("cls");
	printf("***Suppression d'une matiere***\n");
	char chaine[31];


	printf("Entrer le libelle de la matiere a Supprimer : ");
	GRAB_LIBELLET_MATIERE_DELATE:
	scanf("%s", chaine);
	fflush(stdin);

	if(strcmp(chaine, "0") == 0) return;

	Matiere find = get_one_matiere_with(chaine);

	if(find.reference != 0){
		print_matiere(find);
		printf("Etes vous sure de bien vouloir supprimer cette matiere [o/N]? ");
	}else{
		printf("Cet libelle n'existe pas. Entrer un libelle svp ou 0 pour retourner : ");
		goto GRAB_LIBELLET_MATIERE_DELATE;
	}

	scanf("%s", chaine);
	if(strcmp(chaine, "o") == 0 || strcmp(chaine, "O") == 0){
		if(linked_to_classes(find.reference)){
			printf("Desole impossible de supprimer une matiere qui n'est pas vide. Supprimer la relation avec les classes et reessayer.\n");
			return;
		}
		update_matiere(find, 0);
		printf("\nLa matiere a ete supprimer avec succes.\n");
	}else{
		printf("\nLa matiere n'a pas ete supprimer.\n");
	}
}


void seach_matiere_future(Matiere mat)
{
	FILE *file = fopen("data/sefaire.csv", "r");
	int it = 0;
	Classe cl;
	Faire f;
	char niveau[10];

	if(file == NULL){
		printf("L'ouverture du fichier a echoue.\n");
		exit(EXIT_FAILURE);
	}

	printf("Liste des classes dans les quelles on trouve cette matiere.\n");
	printf("+------+---------------------+----------+\n");
	printf("| Code |  Nom                |  Niveau  |\n");
	printf("+------+---------------------+----------+\n");
	do{
		fscanf(file, "%d,%d\n", &f.id_matiere, &f.id_classe);
		if(f.id_matiere == mat.reference){
			it = 1;
			cl = find_classe_from_primary(f.id_classe);
			if(cl.niveau)
				strcpy(niveau, "MASTER");
			else
				strcpy(niveau, "LICENCE");
			printf("|  %3d | %-20s| %-9s|\n", cl.code, cl.nom, niveau);
			printf("+------+---------------------+----------+\n");
		}
	}while(!feof(file));
	fclose(file);
	if(!it){
		printf("Desole cette matiere n'a pas encore de classe.\n");
	}

	printf("1. Rajouter une classe.\n");
	printf("2. Retirer une classe.\n");
	printf("0. Retourner\n");

	int entier = 0;
	printf("Votre choix : ");
	scanf("%d", &entier);
	fflush(stdin);

	switch(entier){
		case 1:
		add_matiere_to_classe(mat);
		break;
		case 2:
		disadd_matiere_to_classe(mat);
		break;
		case 0:
		return;
		default:
		printf("L'option saisie n'est pas disponible.\n");
		exit(EXIT_FAILURE);
	}
}

void seach_matiere()
{
	system("cls");
	printf("***Seach Matiere***\n");
	char chaine[31];

	printf("Entrer le libelle libelle de la matiere a rechercher : ");
	scanf("%s", chaine);
	fflush(stdin);
	printf("\n");

	Matiere mat = get_one_matiere_with(chaine);
	if(mat.reference){
		print_matiere(mat);
	}else{
		printf("Desole cette matiere n'existe pas encore.\n");
		return;
	}

	seach_matiere_future(mat);
}
