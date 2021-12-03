#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

//je refait la fct dans un nouveau fichier


//ATTENTION : s'assurer que le malloc est sous la bonne forme => que les altitudes et coordonnées sont dans les angles des carrés ; cf aussi ex terrain 


//ancienne fct time :

/*
void time ( struct Grille grilles, int number_case, double steadyflow_speed, struct GpsPoint point){
// Apres réflexion j'ai remarqué que pour normaliser la pente, on peut la considérer comme une valeur normalisée d'angle variant de [0,pi/2]
// Ainsi, slope_tot=tan(x*(pi/2)), en isolant x (l'angle), x=arctan(slope_tot)/(pi/2)
  int riviere_speed_factor=2;
  for (i=0;i<number_case;i++){
    double pente_normalisée_x = ((-1)*atan(grilles[i].slope_x))/(M_PI/2);//pas sûr que ça fonctionne : si pente = 5 => "pente_normalisée" = 50.1
    double pente_normalisée_y = ((-1)*atan(grilles[i].slope_y))/(M_PI/2);
    if (grilles.riviere[i]==1){
      //faire attention de prendre en compte le signe des pentes.
      //double distance_x = pente_normalisée_x*riviere_speed_factor*200;
      //double distance_y = pente_normalisée_y*riviere_speed_factor*200;
      //double distance_norme = sqrt(pow(distance_x,2)+pow(distance_y,2));
      
      //mais ça fait pas trop de sens de multiplier une pente pour accentuer la vitesse ?! (bien que l'effet est peut être le même : parcourir plus de distance dans le même temps
      grilles[i].time = riviere_speed_factor*pente_normalisée_x + riviere_speed_factor*pente_normalisée_y;//pourquoi le temps serait égale à la valeur d'une pente
      //double time = distance_norme/200;
    // ici je rajoute 100, car on pose les gouttes entre 2 cases de malloc qui font 200 metres.
      //double distance_tot_x = point.longitude[i] + 100 + distance_x;
      //double distance_tot_y = point.latitude[i] + 100 + distance_y;
    }
    else{
      grilles[i].time = steadyflow_speed*pente_normalisée_x + steadyflow_speed*pente_normalisée_y;
    }
  }
}
*/

//ou alors : pour trouver la pente (=> direction) finale : travailler avec les coordonnées réelles
//et travailler avec des pentes "ponctualisées" : la somme des 2 pentes = 1 => si presque même pente (si goutte va autant à droite qu'à gauche) => pente_norm = 0.5 pour chacune
//OU :
//dire que la plus grande pente = 1 et l'autre est proportionnelle
//if(pente_x>pente_y): px = 1 et py = y/x (ou alors px = 100, comme ça on est exactement sur le bord d'une case, et py = (100*y)/x)
//else py = 100 et px = 100*x/y

//mon idée :
/*
//pour le signe, mettre des "if(grilles[i].slope_x <0 et y >0){...=> px<0 et py>0} if else(grilles[i].slope_y <0 et x >0){...} if else(grilles[i].slope_x<0 && grilles[i].slope_y <0){...}"
if(abs(grilles[i].slope_x) >= abs(grilles[i].slope_y)){//considérer les valeurs absolues pour avoir la pente plus grande
	//attention : voir si le signe est bien respecté
	double px = 100;
	double py = (100*grilles[i].slope_y)/grilles[i].slope_x;
} else{
	//attention : voir si le signe est bien respecté
	double py = 100;
	double px = (100*grilles[i].slope_x)/grilles[i].slope_y;
}
( double distance_tot = sqrt(px**2 + py**2);//est ce que la fct sqrt existe en c ?! ) // finalement je n'utilise pas cette distance
double time = py/vitesse_y + px/vitesse_x;///!\ les 100m sont horizontaux et pas le long de la pente
//ça donnera un temps un plus long que si faisait le chemin direct (= distamce_tot)... mais je ne vois pas trop comment faire d'autre
//et d'un autre côté, en réalité, il y a des obstacles qui peuvent retenir l'eau, donc possibleque ça changere pas trop le temps du trajet
//attention : pour le temps, il faut calculer les vitesses en fonctions des pentes réelles et pas les px, py
//MAIS ça dépend si on part du principe que la goutte part tjr du milieu de la case ou non
double coord_x_finale = grilles[i].latitude + 100 + px;//+100, car la goutte part depuis le milieu de la case
double coord_y_finale = grilles[i].longitude + 100 + py;

//et après, pour savoir dans quelle case elle va => mettre des if/else pour savoir entre quelles coord x et y la goutte se trouve = dans quelle case elle entre
//et après, voir si on refait partir la goutte depuis le milieu de la case ou non
//par exemple, on pourrait refire partir la goutte depuis le milieu de la case, et pour ne pas trop fausser le temps,
//on double le temps que la goutte a mis pour traverser la moitié de la case précédente (= pour parcourir la moitié du chemin)

//par exemple : if(grilles[i].slope_x+(200/3) <= cord_x_finale <= grilles[i+1].slope_x - (200/3)){if(coord_y_finale == grilles[i].slope_y){goutte.x = grilles[i-37].latitude + 100 et goutte.y = grilles[i-37].longitude +100}}
//double time_tot = 2*time;
//j'ai mis les +/- 200/3 pour prendre en compte si la goutte change de case dans un angle ou sur un côté
//les "+100" sont pour mettre la goutte au milieu de la case suivante
*/

//Nouvelle fonction (cellle que je viens de décrire) :

//ne pas oublier si une pente = 0 et pas l'autre !

//dans la fonction, on rentre chaque struct de chaque case (et PAS tout le tableau de struct), car on va faire une boucle while pour renddre ça réciproque
//idem pour les gouttes et point

//les arguments de la fonction seront les "goutte[...].latitude" et "goutte[...].longitude" ... => ce ne sont pas des struct, mais ce sont des coord
//et juste avant de lancer la fct récursive, => initialiser la struct goutte[...] avec les coord du point[...] (gps) correspondant

//Exemple pour appeler la fct : travel(grilles[i], goutte[i])
//Attention : la struct "grilles" (=> la case initiale) reste tjr la même => ne pas la modifier tant qu'on a tjr la même goutte

bool travel(struct Carre grilles, struct Goutte * gouttes){
	//Si la goutte arrive à moins de 100m d'un bord, on arrête la fonction
	//Nous avons défini notre zone d'étude de façon a n'avoir que des pentes qui "sortent" de la zone lorsqu'on arrive sur le bord
	//Donc une goutte ne va pas longer le bord de la zone
	if (gouttes.x <= 592900) return false;
	if (gouttes.y >= 103700) return false;
	if (gouttes.x >= 599900) return false;
	if (gouttes.y <= 95100) return false;
	
	
	
	//est-ce qu'il faut travailler avec la struct point ou bien goutte ?! je pense goutte (pour tout ce qui concerne la position qu'on modifie)
	//"normaliser" les pentes en x et y : la pente la plus grande est amenée à 100 et la plus petite est proportionnelle à la grande
	double px = 0;
	double py = 0;
	if(grilles[i].slope_x==0 && grilles[i].slope_y==0){
		double px = 0;
		double py = 0;
		//Dans ce cas, la goutte de bougera pas
	} else if(grilles[i].slope_x <=0 && grilles[i].slope_y >=0){
		if(grilles[i].slope_x ==0 && grilles[i].slope_y >0){
			double px = 0;
			double py = 100;
		} else if(grilles[i].slope_x<0 && grilles[i].slope_y==0){
			double px = -100;
			double py = 0;
		} else if(fabs(grilles[i].slope_x) > grilles[i].slope_y){
			double px = -100;//comme ça on est exactement sur le bord d'une case ; est-ce qu'on écrit bien comme ça les nbr négatifs ?!
			double py = fabs((100*grilles.slope_y)/grilles.slope_x);
		} else{
			double py = 100;
			double px = (100*grilles.slope_x)/grilles.slope_y;//px est déjà négatif, car une slope est >0 et l'autre <0 => la division est <0
			//mêmes remarques pour les autres cas
		}
	} else if(grilles[i].slope_y <=0 && grilles[i].slope_x >=0){
		if(grilles[i].slope_x ==0 && grilles[i].slope_y<0){
			double px = 0;
			double py = -100;
		} else if(grilles[i].slope_x>0 && grilles[i].slope_y==0){
			double px = 100;
			double py = 0;
		} else if(grilles[i].slope_x > fabs(grilles[i].slope_y)){
			double px = 100;
			double py = (100*grilles[i].slope_y)/grilles[i].slope_x;
		} else{
			double py = -100;
			double px = fabs((100*grilles[i].slope_x)/grilles[i].slope_y);
		}
	} else if(grilles[i].slope_x<0 && grilles[i].slope_y<0){
		if(grilles[i].slope_x ==0 && grilles[i].slope_y<0){
			double px = 0;
			double py = -100;
		} else if(grilles[i].slope_x<0 && grilles[i].slope_y==0){
			double px = -100;
			double py = 0;
		} else if(fabs(grilles[i].slope_x) > fabs(grilles[i].slope_y)){
			double px = -100;
			double py = -((100*grilles[i].slope_y)/grilles[i].slope_x);
		} else{
			double py = -100;
			double px =-((100*grilles[i].slope_x)/grilles[i].slope_y);
		}
	} else if(grilles.slope_x>0 && grilles.slope_y>0){
		if(grilles.slope_x ==0 && grilles.slope_y>0){
			double px = 0;
			double py = 100;
		} else if(grilles.slope_x>0 && grilles.slope_y==0){
			double px = 100;
			double py = 0;
		} else if(grilles.slope_x > grilles.slope_y){
			double px = 100;
			double py = (100*grilles.slope_y)/grilles.slope_x;
		} else{
			double py = 100;
			double px =(100*grilles.slope_x)/grilles.slope_y;
		}
	}
	//il faut encore définir la fonction pour les vitesses (je dirait de les calculer en fonction de l'angle ou de la pente en %)
	//si je prends ces choix arbitraires : 
	//pente de 20% => angle = 11.31 degrés => vitesse = 0.2 m/s
	//pente de 50% => angle = 26.565 degrés => vitesse = 0.5 m/s
	double slope_fct_vitesse = (0.5-0.2)/(atan(0.5)-atan(0.2));
	double inters_fct_vitesse = 0.5-(slope_fct_vitesse*atan(0.5);
	//on choisit que quand l'eau est dans une rivière, elle avance à 2 m/s
	
	//Nous faisons d'abord tourner le code sans tenir compte des rivières
	//double vitesse_riviere = 2;
	/*
	if(grilles.river==1){
		//calculer pour chaque case de chaque riviere : à quelle distance elle est, vitesse = 2 m/s et cbn de temps elle met 
	}
	//ajouter encore si les températures sont en dessous de 0 => eau gèle/neige
	//ajouter encore la pénétration de l'eau dans le sol
	*/
	
	double vitesse_x = slope_fct_vitesse*atan(grilles.slope_x)+inters_fct_vitesse;
	double vitesse_y = slope_fct_vitesse*atan(grilles.slope_y)+inters_fct_vitesse;
	//ATTENTION : les 100m sont horizontaux et pas le long de la pente => on a pris la distance réelle de la pente
	double time1 = (fabs(py)/cos(atan(grilles.slope_y)))/vitesse_y + (fabs(px)/cos(atan(grilles.slope_x)))/vitesse_x;
	//Attention : pour le temps, il faut calculer les vitesses en fonctions des pentes réelles et pas les px, py
	//est-ce qu'on met les pentes en % pour calculer la fonction des vitesses (et donc calculer les vitesses) ?
	//comme ça c'est uniforme (partout la même échelle donc on peut comparer)
	//ou calculer les pente avec les angles ?! ce serait peut être mieux, car tout est entre 0 et +/-90 degrés ?!
	//j'ai calculé les pentes avec les angles
	
	//mettre des conditions if/else pour voir dans quelle case la goutte va
	double new_x = goutte.latitude + px;
	double new_y = goutte.longitude + py;
	if(grilles[i]
	
	
	//gouttes->x = goutte.latitude + 100 + px;//+100, car la goutte part depuis le milieu de la case
	//gouttes->y = goutte.longitude + 100 + py;
	
	//if(grilles.slope_x+(200/3) <= cord_x_finale <= grilles[i+1].slope_x - (200/3)){if(coord_y_finale == grilles[i].slope_y){goutte.x = grilles[i-37].latitude + 100 et goutte.y = grilles[i-37].longitude +100}}
	grilles.time += 2*time1;
	//Attention : avant de lancer la fct => initialiser toutes les grilles.time à 0
	
	//utiliser la fct inLac qui dit si un point (de coord x et y) est dans le lac
	//=> if gouttes.x et gouttes.y sont dans le lac => catch de la case de départ = 1
	if(inLac(gouttes.x, gouttes.y)){
		grilles.catch = 1;
		return false;
	}
}


//on pourrait aussi dire : si une case initiale mène au lac, alors chaque case qui a été traversée par la goutte mène aussi au lac
//mais ça nous prendrait trop de temps a coder, car là on travaille avec les coordonnées réelle et pas les indice des cases
//ici, on ne fait pas attention dans quelle case on passe
