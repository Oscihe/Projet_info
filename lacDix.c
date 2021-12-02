#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

int lacDix(struct Grille grilles, struct GpsPoint point, int total_case_zone, int malloc_lac){
	//définir les indices du tableau malloc qui correspondent à la zone du lac des Dix, puis en faire un csv
	//Nous pouvons ensuite retrouver les coordonnées correspondantes avec la formule reliant les indice et les coordonnées
	int k =0;
	//pour calculer les droites entre les points, je calcule des fonctions linéaires :
	double slope13 = (-(101400-98000))/(597200-596200);//est-ce que pour mettre en négatif, ça fonctionne de mettre "-" devant ?
	double slope24 = (-(101400-98000))/(598000-597000);
	double slope35 = (103200-101400)/(597000-596200);
	double slope46 = (103200-101400)/(597600-597000);
	double inters13 = 98000-(slope13*597200);
	//double coord13 = slope13*point[...].latitude + inters13;
	double inters24 = 98000-(slope24*598000);
	//double coord24 = slope24*point[...].latitude + inters24;
	double inters35 = 101400-(slope35*596200);
	//double coord35 = slope35*point[...].latitude + inters35;
	double inters46 = 101400-(slope46*597000);
	//double coord46 = slope46*point[...].latitude + inters46;
	
	
	//ATTENTION : s'assurer que les i des struct GpsPoint suivent bien les i du malloc (=> des cases) !
	
	for(int i=0; i<total_case_zone; i++){
		if(point[i].longitude==103200 && 597000<=point[i].latitude<=597600){
			//entre les angles 5 et 6
			malloc_lac[k]=i;
			grilles[i].lac = 1;
			grilles[i].catch = 1;
			k += 1;
		} else if(point[i].longitude==098000 && 597200<=point[i].latitude<=598000){
			//entre les angles 1 et 2
			malloc_lac[k]=i;
			grilles[i].lac = 1;
			grilles[i].catch = 1;
			k += 1;
		} else if(98000 <= slope13*point[i].latitude + inters13 <= 101400){
			//entre les angles 1 et 3
			malloc_lac[k]=i;
			grilles[i].lac = 1;
			grilles[i].catch = 1;
			k += 1;
		}else if(98000 <= slope24*point[i].latitude + inters24 <= 101400){
			//entre les angles 2 et 4
			malloc_lac[k]=i;
			grilles[i].lac = 1;
			grilles[i].catch = 1;
			k += 1;
		}else if(101400 <= slope35*point[i].latitude + inters35 <= 103200){
			//entre les angles 3 et 5
			malloc_lac[k]=i;
			grilles[i].lac = 1;
			grilles[i].catch = 1;
			k += 1;
		}else if(101400 <= slope46*point[i].latitude + inters46 <= 103200){
			//entre les angles 4 et 6
			malloc_lac[k]=i;
			grilles[i].lac = 1;
			grilles[i].catch = 1;
			k += 1;
		}else{
			//Sinon, la case n'est pas dans le lac des Dix
			grilles[i].lac = 0;
		}
	}
	
	//ATTENTION : on n'a pas les coordonnées qui sont au milieu du lac => il faut les rajouter
	//je vais finir ce code plus tard
	return 0;
}
