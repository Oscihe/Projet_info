#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

int inLac(double x, double y){
	//But : dire si un point (de coord x et y) est dans le lac ou non
	//Nous pouvons ensuite retrouver les coordonnées correspondantes avec la formule reliant les indice et les coordonnées

	//pour calculer les droites entre les points, je calcule des fonctions linéaires :
	double slope13 = (double)(-(101400-98000))/(597200-596200);
	double slope24 = (double)(-(101400-98000))/(598000-597000);
	double slope35 = (double)(103200-101400)/(597000-596200);
	double slope46 = (double)(103200-101400)/(597600-597000);
	double inters13 = 98000-(slope13*597200);
	//double coord13 = slope13*point[...].latitude + inters13;
	double inters24 = 98000-(slope24*598000);
	//double coord24 = slope24*point[...].latitude + inters24;
	double inters35 = 101400-(slope35*596200);
	//double coord35 = slope35*point[...].latitude + inters35;
	double inters46 = 101400-(slope46*597000);
	//double coord46 = slope46*point[...].latitude + inters46;

	//si le point en question est dans le lac => return true

	if(101400 <= y && y <= 103200){
		//si y est entre 34 et 56
		if(((y-inters35)/slope35) <= x && x <= ((y-inters46)/slope46)){
			return 1;
		} else{
			return 0;
		}
	} else if(98000 <= y && y <= 101400){
		//si y est entre 12 et 34
		if(((y-inters13)/slope13) <= x && x <= ((y-inters24)/slope24)){
			return 1;
		} else{
			return 0;
		}
	} else{
		return 0;
	}
}
