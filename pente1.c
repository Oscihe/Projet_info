#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
int slope_function(struct Grille grilles,double malloc_hauteur, int number_case){
// Le principe est de calculer la moyenne des gradients en x, en y sur une case
// qui est représentée par 4 différentes hauteurs. (Contrairement au malloc, on a
// 4 hauteurs par cases et 2 pentes.)
//as
  for (int i=0;i<number_case;i++){
    double value1 = malloc_hauteur[i] - malloc_hauteur[i+1];
    double value2 = malloc_hauteur[i+37] - malloc_hauteur[i+38];
    grilles.slope_x = ((value1+value2)/2)/200;//j'ai mis "value1" au lieu de "value"
    //j'ai divisé par 2 pour avoir une pente moyenne
    double value3 = malloc_hauteur[i] - malloc_hauteur[i+37];//j'ei inversé [i] et [i+37] pour avoir partout le même "ordre"
    double value4 = malloc_hauteur[i+1] - malloc_hauteur[i+38];
    grilles.slope_y = ((value3+value4)/2)/200;
  }
  return 0;
}
