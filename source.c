#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
//Voir comment importer d'autres fichiers c dans celui là
//#include "barrage.c"
#include "pente1.c"
#include "Water_travel.c"
#include "time.c"
//Ce fichier contient la fonction main pour tout notre projet
//Nous importons les autres fichiers c (contenant toutes les fonctions, structures, ...)

//coord et alt de chaque case
struct GpsPoint {
 double latitude;
 double longitude;
 float altitude;
};

//Pour les gouttes qui vont dans le lac, avec le temps qu'elles mettent jusqu'au lac
struct Goutte{
  double x;
  double y;
  double time_tot;
};

//caractéristiques de chaque case
struct Grille {
 int lac;
 int riviere;
 int catch;
 double slope_x;
 double slope_y;
 double time;
};
struct Grilles grilles[1628];
struct Goutte gouttes[1628];
struct Gpspoint






gv[]) {
  for (int i=0;i<36*44;i++){
    while (travel(i,i,point))
  }
  return 0;
}
