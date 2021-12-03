#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
int slope_function(struct Carre carres,double malloc_hauteur, int number_carre){
	//calculer les pentes (en x et y) de chaque case
// Le principe est de calculer la moyenne des gradients en x, en y sur une case
// qui est représentée par 4 différentes hauteurs. (Contrairement au malloc, on a
// 4 hauteurs par cases (dans les angles) et 2 pentes.)
  for (int i=0;i<number_carre;i++){
	double value1 = malloc_hauteur[i] - malloc_hauteur[i+1];//si cette pente est positive, la goutte avancera dans la direction de l'axe x positif
	double value2 = malloc_hauteur[i+37] - malloc_hauteur[i+38];
	carres[i].slope_x = ((value1+value2)/2)/200;
	//j'ai divisé par 2 pour avoir une pente moyenne
	double value3 = malloc_hauteur[i+37] - malloc_hauteur[i];//si cette pente est positive, la goutte avancera dans la direction de l'axe y positif
	double value4 = malloc_hauteur[i+38] - malloc_hauteur[i+1];
	carres[i].slope_y = ((value3+value4)/2)/200;
  }
  return 0;
}

int velocity(struct Carre carres, int nbr_carre){
	//Déterminer la vitesse d'une goutte selon la pente en x et en y
	double a = 0.06;
	double b = 0.1;//vitesse minimale de la goutte
	//vitesse = a*pente + b
	//pour les rivières :
	double a_river = 1.8;
	double b_river = 1;//vitesse minimale de la goutte dans une rivière
	for(int i = 0; i<nbr_carre; i++){
		if(carres[i].riviere ==0){
			carres[i].vit_x = a*carres[i].slope_x+b;
			carres[i].vit_y = a*carres[i].slopey_y+b;
		} else{
			carres[i].vit_x = a_river*carres[i].slope_x+b_river;
			carres[i].vit_y = a_river*carres[i].slopey_y+b_river;
		}
	}
	return 0;
}

//Nouvelle fonction indexing pour le tableau de struct "carre", car il comprend une colonne et une ligne de moins que le tableau "grilles" (des altitudes)
//Attention : faire en sorte que le i soit un nbr entier et corresponde à la bonne case
//donc modifier la fct carreIndex
int carreIndex(int x,int y){
	//à modifier : comment réussir à créer la fct pour qu'elle nous dise dans quelle case CARRE est un point (x,y)
	int i=floor((103800-y)/200)*36+floor((x-592800)/200);
	//printf("%d\n",i);
	return i;
}

int avancer(struct Carre * carres, struct Goutte * gouttes){
	//Fonction pour faire avancer une goutte
	//on va effectuer cette ponction toutes les secondes (=> une fois par seconde)
	//on veut return (dans les struct) le temps que ça met jusqu'au lac, si la goutte y va
	
	//si ça va dans le lac, ça return 2
	//si la goutte continuera d'avancer, ça return 1
	//si la goutte s'arrête ou sort de notre zone, ça return 0
	
	
	//mettre des catch = 0 quand ça return 0
	if (gouttes.x <= 592800) return 0;
	if (gouttes.y >= 103800) return 0;
	if (gouttes.x >= 600000) return 0;
	if (gouttes.y <= 95200) return 0;
	
	if(inLac(gouttes.x, gouttes.y)==1){
		carres->catch = 1;
		return 2;//mettre le carres.catch du carre initial à 1 (dans la boucle faite dans main)
	} else if(grilles.slope_x ==0 && grilles.slope_y ==0){
		return 0;//Déterminer si la goutte reste sur place/s'arrête !!! => si les deux pentes sont égales à 0
		//à voir plus tard si risque que la goutte fasse des aller-retour entre 2 points (mais à mon avis c'est peu probable)
	} else{
		double new_x = gouttes.x + carres.vit_x*1;//le *1 et car c'est pendant 1 seconde
		double new_y = gouttes.y + carres.vit_y*1;
	
		gouttes->x = new_x;
		gouttes->y = new_y;
		goutte->time_tot += 1;//nbr de secondes que la goutte met jusqu'au lac (si elle y va)
		return 1;
	}
}
//si fct return 1 ou 2, ça correspond à true
//si elle return 0, ça correspond à false
//OU : return le time

//Attention à tjr ctrl l'index de la case "carre" où on est pour tjr avec les bonnes vitesses correspondantes
//compter le temps dans la struct goutte ou carre ?
