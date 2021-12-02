#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

bool inLac(double x, double y){
	//But : dire si un point (de coord x et y) est dans le lac ou non
	//Nous pouvons ensuite retrouver les coordonnées correspondantes avec la formule reliant les indice et les coordonnées
	
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
	
	
	//si le point en question est dans le lac => return true
	
	if(101400 <= y <= 103200){
		//si y est entre 34 et 56
		if(((y-inters35)/slope35) <= x <= ((y-inters46)/slope46)){
			return true;
		}
	} else if(98000 <= y <= 101400){
		//si y est entre 12 et 34
		if(((y-inters13)/slope13) <= x <= ((y-inters24)/slope24)){
			return true;
		}
	} else{
		return false;
	}
	
	/*
	//normalement, pas besoin d'utiliser ça :
	if(y==103200 && 597000<=x<=597600){
		//entre les angles 5 et 6
		return true;
	} else if(y==098000 && 597200<=x<=598000){
		//entre les angles 1 et 2
		return true;
	} else if(98000 <= slope13*x + inters13 <= 101400){
		//entre les angles 1 et 3
		if(){
			return true;
		}
	}else if(98000 <= slope24*x + inters24 <= 101400){
		//entre les angles 2 et 4
		if(){
			return true;
		}
	}else if(101400 <= slope35*x + inters35 <= 103200){
		//entre les angles 3 et 5
		if(){
			return true;
		}
	}else if(101400 <= slope46*x + inters46 <= 103200){
		//entre les angles 4 et 6
		if(){
			return true;
		}
	}else{
		//Sinon, la case n'est pas dans le lac des Dix
		return false;
	}
	*/
}
