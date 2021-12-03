#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
double indexing(double x,double y){
	double t=(103800-y)/200*37+(x-592800)/200;
  int i=floor(t);
	//printf("%d\n",i);
	return i;
}
//le total de cases est 36*44?
bool travel (int compteur, int compteur2, struct GpsPoint point, struct Grille grilles, int total_case, struct Goutte gouttes){
  int indice = indexing(point->latitude, point->longitude);
  gouttes[compteur2].time_tot += grilles[compteur].time;
  if (point[compteur].latitude == 592800) return false;
  if (point[compteur].longitude == 103800) return false;
  if (point[compteur].latitude == 600000) return false;
  if (point[compteur].longitude == 95200) return false;
  double pente_normalisée_x = (atan(grilles[compteur].slope_x))/(M_PI/2);
  double pente_normalisée_y = (atan(grilles[compteur].slope_y))/(M_PI/2);
  distance_x = pente_normalisée_x*200;
  distance_y = pente_normalisée_y*200;
  double distance_tot_y = point.longitude + 100 + distance_y;
  double distance_tot_x = point.latitude + 100 + distance_x;
  int g = indexing(distance_tot_y, distance_tot_x);
  if (grilles[g].lac==1){
    grilles[indice].catch=1;
    return false;
  }
	travel(g, compteur2, point, grilles, gouttes );
  return true;
}
