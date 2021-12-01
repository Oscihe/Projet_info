#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

//#include "fichier.c"
//mettre toutes les struct dans le fichier main
//et que des fichiers dans les autres doc (pas de main)

//dans goutte : num case départ (avec index peut retrouver les coord) et temps qu'elle prend
//case : pente en x et y, catch, lac

// Structure
struct GpsPoint {
	double latitude;
	double longitude;
	float altitude;
};

// Lire une ligne et remplir la structure
int lireLigne(char * ligne, struct GpsPoint * point) {
	char * virgule1 = strchr(ligne, ' ');
	if (virgule1 == NULL) return 0;
	char * virgule2 = strchr(virgule1 + 1, ' ');
	if (virgule2 == NULL) return 0;
	double v1=atof(ligne);
	double v2=atof(virgule1+1);
	double v3=atof(virgule2+1);
	if(v1 >= 592800.00 && v1 <= 600000.00 && v2 >= 095200.00 && v2 <= 103800.00){
		point->latitude = v1;
		point->longitude = v2;
		point->altitude = v3;
		//printf("%f, %f, %f\n",point->latitude,point->longitude,point->altitude);
		return 1;
	}
	return 0;
}

int lireFichier(char * nomFichier, struct GpsPoint * tableauARemplir, int longueur) {
	// Ouvrir le fichier
	FILE * file = fopen(nomFichier, "r");
	if (file == NULL) return -1;

	// Lire ligne par ligne
	int n = 0;
	char buffer[100];
	while (fgets(buffer, 100, file) != NULL) {
		if (n >= longueur) break;
		int ok = lireLigne(buffer, &tableauARemplir[n]);
		if (ok) {
			n = n + 1;
		}
	}

	// Fermer le fichier et renvoyer le nombre de lignes lues
	fclose(file);
	return n;
}

bool writeCsv(char * filename, int * values, int sizeX, int sizeY) {
	//j'ai fait en sorte que la fct prenne des int pour voir les catch
	//ATTENTION : remettre en double ensuite (dans arg de fct et dans le printf %f
	FILE * file = fopen(filename, "w");
	if (file == NULL) {
		fprintf(stderr, "File %s not found.", filename);
		return false;
	}

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			if (x > 0) fprintf(file, ", ");
			fprintf(file, "%d", values[y * sizeX + x]);
		}
		fprintf(file, "\n");
	}

	fclose(file);
	return true;
}

struct Grille {
	double lat;
	double longi;
	double pluie;
	double alt;
	int lac;
	int catch;
	double time;
	double pente;
};

int indexing(int x,int y){
	int i=(103800-y)/200*37+(x-592800)/200;
	//printf("%d\n",i);
	return i;
}

void initialisation(double *malloc_hauteur,int compteur,double quantite_precip,double lac_lat_max,double lac_lat_min,double lac_long_min,double lac_long_max,struct Grille *grilles){
	//int lenxp=37;
	//int lenyp=44;
	//int total=lenxp*lenyp;
	for (int k=0;k<compteur;k++){
		grilles[k].longi=103800-200*(k/37);
		grilles[k].lat=592800+200*(k%37);
		//x = latitude et y = longitude
		grilles[k].pluie=quantite_precip;
		grilles[k].alt=malloc_hauteur[k];
		//printf("%f, %f, %f\n",grilles[k].alt,grilles[k].lat,grilles[k].longi);

		double c1 = grilles[k].longi;
		double c2 = grilles[k].lat;
		if (c1<lac_long_max && c1>lac_long_min && lac_lat_max>c2 && lac_lat_min<c2){
			grilles[k].lac=1;
			grilles[k].catch=1;
		}
		else{
			grilles[k].lac=0;
		}
	}
}

struct Point {
    int x;
    int y;
};

struct Depart{
	int x;
	int y;
};

//Volume d'eau total (obtenu par la pluie) ajouté au lac
/*
struct Volume{
	double quantite;
};
*/
bool avancerEau(struct Point * point, struct Grille * grilles, struct Depart * depart) {
    // Vérifier si on est au bord
    if (point->x == 592800) return false;
    if (point->y == 103800) return false;
    if (point->x == 600000) return false;
    if (point->y == 95200) return false;

	int indice = indexing(depart->x, depart->y);
	//printf("%d, %d\n", depart->x, depart->y);
	//les coordonnées chgt au début et reste après tjr à 597400, 102800 => indice 208
	//printf("%d\n", indice);
	//c'est l'indice de la case de départ (la case où l'eau est tombée)
	//ça me print tjr le même indice alors que les coord chgt...

	//x=latitude et y=longitude
	//printf("%f\n", grilles[indice].alt);
    // Chercher le minimum dans un voisinage de 3x3 cases
    double min = 10000;
    int minX = 0;
    int minY = 0;
    for (int y = -200; y <= 200; y=y+200) {
        for (int x = -200; x <= 200; x=x+200) {
            int i = indexing(point->x + x, point->y + y);
            double altitude = grilles[i].alt;
            //ou grilles.alt[i]
            //printf("%f\n", altitude);
            //ça print des altitudes bizarres et plusieurs fois les mêmes...
            if (altitude < min) {
                min = altitude;
                minX = x;
                minY = y;
                //printf("%d, %d\n", minX, minY);
            }
            /*
            if (grilles[i].lac==1){
				grilles[indice].catch=1;
				//printf("%d\n",grilles[indice].catch);
				//printf("%d\n", indice);
				//ça print tjr "133"... -> ça a été corrigé !
				//printf("ok");
				return true;
			}
            if (minX == 0 && minY == 0){
				if (grilles[i].lac==1){
					grilles[indice].catch=1;
					//printf("%d\n",grilles->catch);
					//printf("ok");
					return true;
				}
				return false;
			}
			*/
        }
    }

    // Si le minimum se trouve au centre, on a un "trou" (lac des Dix, ou un autre lac, ou erreur dans les données)
    if (minX == 0 && minY == 0){
		if (grilles[indexing(point->x, point->y)].lac==1){
			grilles[indice].catch=1;
			//printf("%d\n",grilles->catch);
			//printf("ok");
			return false;
		}
		return false;
	}

    /*
    if (minX == 0 && minY == 0 && grilles[indexing(point->x, point->y)].lac!=1){
		return false;
	}
	//ajouter la quantité dans qui tombe dans le lac
	if(grilles[indexing(point->x, point->y)].lac==1){
		grilles[indice].catch=1;
		volume->quantite += grilles[indice].pluie;
		//printf("%d\n",grilles->catch);
		//printf("ok");
		return true;
	}
	*/

    // Avancer le point vers le minimum
    point->x += minX;
    point->y += minY;
    if (grilles[indexing(point->x, point->y)].lac==1){
		grilles[indice].catch=1;
		//printf("%d\n",grilles[indice].catch);
		//printf("%d, %d, %d\n", indice, grilles[indice].lac, grilles[indice].catch);
		//printf("ok");
		return false;
	}
    return true;
}

void simulerEau(int x, int y, struct Grille * grilles, struct Depart* depart) {
    struct Point point = {x, y};
    //int i = indexing(point.x, point.y);
    //printf("%d\n", indice);
    //int i = indexing(grilles.lat, grilles.longi);
    //sim.gouttes[i] += 1;
    while (avancerEau(&point, grilles, depart)) {
        //int i = indexing(grilles.lat, grilles.longi);
        //sim.gouttes[i] += 1;
        //printf("ok");
    }
}

/*
double  accumulation (int compteur1,struct Grille *grilles) {
	//int z=indexing(grilles[compteur1].long,grilles[compteur1].lat)
	int pos_x=grilles[compteur1].lat;
	int pos_y=grilles[compteur1].longi;
	//x=latitude et y=longitude
	int y_min = 0;
	int x_min = 0;
	int h_min=0;
	while(pos_x!=592800||pos_y!=95200||pos_x!=600000||pos_y != 103800){
		//printf("ok");
		//print que 1 ok
		for (int x= -200; x<=200; x=x+200){
			for (int y= -200; y<=200; y=y+200){
				int i=indexing(pos_x+x,pos_y+y);
				//printf("%f\n",grilles[i].alt);
				if (h_min > grilles[i].alt){
					h_min = grilles[i].alt;
					x_min = grilles[i].lat;
					y_min = grilles[i].longi;
					//printf("%d",i);
					//printf("ok");
					//ça ne print pas "ok"
				}

				pos_x = x_min;
				pos_y = y_min;

				if (grilles[indexing(x_min,y_min)].lac==1){
					grilles[compteur1].catch=1;
					//printf("%d\n",grilles[compteur1].catch);
					//printf("ok");
					//ne print pas
				}
			}
		}
		//pos_x = x_min;
		//pos_y = y_min;
	}
	//if (grilles[indexing(x_min,y_min)].lac==1){
		//grilles[compteur1].catch=1;
		//printf("%d\n",grilles[compteur1].catch);
		//printf("ok");
		//ne print pas et comme si boucle infinie...
	//}
	return 1;
}
*/

struct Grille grilles[37*44];

int main(int argc, char * argv[]) {
	struct GpsPoint points[1628];
	int nbPoints = lireFichier("DHM200.xyz", points, 1628*sizeof(double));
	//printf("%d",nbPoints);
	if (nbPoints == -1) {
		printf("Erreur: Le fichier n'existe pas ou n'est pas accessible.\n");
		return 1;
	} else if (nbPoints == 0) {
		printf("Erreur: Le fichier est vide ou pas dans le bon format.\n");
		return 1;
	}
	double xmin = points[0].latitude;
	double ymax = points[0].longitude;
	for(int i=1; i<nbPoints; i++){
		if(points[i].latitude<xmin){
			xmin=points[i].latitude;
			//printf("ok");
			//ne print pas, normal car il s'agit déjà de xmin !
		}
	}
	for(int i=1; i<nbPoints; i++){
		//print = ok
		if(points[i].longitude>ymax){
			ymax=points[i].longitude;
			//printf("ok");
			//ne print pas, normal car il s'agit déjà de ymax !
		}
	}

	//Création du malloc contenant les altitudes :
	double * altitudes = malloc(1628*sizeof(double));
	for(int i=0; i<nbPoints; i++){
		altitudes[(((int) ymax- (int) points[i].longitude)/200)*37 + (((int) points[i].latitude- (int) xmin)/200)] = points[i].altitude;
		//printf("%f\n", points[i].altitude);
		//printf("%d\n", (((int) ymax- (int) points[i].longitude)/200)*37 + (((int) points[i].latitude- (int) xmin)/200));
		//printf("%d\n", indexing(points[i].latitude, points[i].longitude));
	}

	//writeCsv("altitudes.csv", altitudes, 37, 44);


	//Imprimer les valeurs des altitudes se trouvant dans le malloc

	//printf("%d\n", nbPoints);
	for(int i=0; i<nbPoints; i++){
		//printf("%f\n", altitudes[i]);
	}


	//int lenxp=37;
	//int lenyp=44;
	//int total=lenxp*lenyp;
	//double quantite=0;
	//struct Grille grilles[total];
	//for (int l=0;l<total;l++){
	initialisation(altitudes,1628,1,598000,596200,98000,103200,grilles);
	//latitude = x et longitude = y

	//Simuler 1 goutte
	//simulerEau(592800, 100000, grilles);
	/*
	struct Volume volume;
	volume.quantite = 0;
	*/
	// Simuler beaucoup de gouttes

    for (int y = 103800; y >= 95200; y=y-200) {
        for (int x = 592800; x <= 600000; x=x+200) {
			struct Depart depart;
			depart.x = x;
			depart.y = y;

			//printf("%d, %d\n", depart.x, depart.y);
            simulerEau(x, y, grilles, &depart);
        }
    }

	int * eau = malloc(1628*sizeof(int));
	for(int i=0; i<nbPoints; i++){
		eau[i] = grilles[i].catch;
		//printf("%f\n", points[i].altitude);
		//printf("%d\n", (((int) ymax- (int) points[i].longitude)/200)*37 + (((int) points[i].latitude- (int) xmin)/200));
		//printf("%d\n", indexing(points[i].latitude, points[i].longitude));
	}
	writeCsv("eau.csv", eau, 37, 44);
	int salut=0;
	for (int i=0;i<1628;i++){
		if (grilles[i].catch==1){
			salut+=1;
		}
	}
	printf("%d",salut);
	int * lac = malloc(1628*sizeof(int));
	for(int i=0; i<nbPoints; i++){
		lac[i] = grilles[i].lac;
		//printf("%f\n", points[i].altitude);
		//printf("%d\n", (((int) ymax- (int) points[i].longitude)/200)*37 + (((int) points[i].latitude- (int) xmin)/200));
		//printf("%d\n", indexing(points[i].latitude, points[i].longitude));
	}
	writeCsv("lac.csv", lac, 37, 44);

	/*
	for(int m=0;m<1628;m++){
		//printf("ok");
		//ça print que 2 ok...
		//accumulation(m,grilles);
		//printf("ok");
		//ça print que 1 ok
		if (grilles[m].catch==1){
			quantite=quantite+grilles[m].pluie;
			printf("%f",quantite);
			//printf("ok");
			//ne print pas
		}
	}
	*/

	//double volume_tot=(volume.quantite/1000)*200;
	//printf("Le volume totale d'eau est: %f",volume_tot);

	free(altitudes);
	free(eau);
	free(lac);
	return 0;
}
