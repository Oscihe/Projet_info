#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

//Ce fichier simule le déplacement des gouttes d'eau

//coordonnées et altitude de chaque point, les cases sont dans l'ordre en commençant en haut à gauche de notre zone
struct Grille {
	double lat;//latitude
	double longi;//longitude
	double alt;//altitude
};

//coordonnées et alt de chaque case, mais dans le désordre
struct GpsPoint {
	double latitude;
	double longitude;
	float altitude;
};

//Pour les gouttes qui vont dans le lac : coordonnées, avec le temps qu'elles mettent jusqu'au lac
struct Goutte{
	double x;//= latitude
	double y;//= longitude
	int time_tot;
};

//caractéristiques de chaque case
struct Carre {
	int lac;//= 0 ou 1 si la case est dans le lac
	int riviere;//= 1 si la case est dans une rivière
	int catch;//= 1 si la case mène au lac
	double slope_x;//pente le long de x
	double slope_y;
	double vit_x;//vitesse d'une goutte le long de la pente en x
	double vit_y;
	int time;
	double quantite;//quantité de pluie ou de goutte
};

//Les fonctions "LireLigne", "lireFichier" et "writeCsv" sont inspirée des exercices et du Midterm
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

//Ecrire un CSV avec des "double", tableau 2D
bool writeCsv(char * filename, double * values, int sizeX, int sizeY) {
	FILE * file = fopen(filename, "w");
	if (file == NULL) {
		fprintf(stderr, "File %s not found.", filename);
		return false;
	}

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			if (x > 0) fprintf(file, ", ");
			fprintf(file, "%f", values[y * sizeX + x]);
		}
		fprintf(file, "\n");
	}

	fclose(file);
	return true;
}

//Ecrire un CSV avec des "int", tableau 2D
bool intwriteCsv(char * filename, int * values, int sizeX, int sizeY) {

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

//pour avoir l'indice de la case "grilles" correspondante
int indexing(int x,int y){
	int i=(103800-y)/200*37+(x-592800)/200;
	return i;
}

//Pour initialiser quelques variables des struct
void initialisation(double *malloc_hauteur,struct Grille *grilles, struct Goutte * gouttes, struct Carre * carres){
	for (int k=0;k<1628;k++){
		grilles[k].longi=103800-200*(k/37);
		grilles[k].lat=592800+200*(k%37);
		//x = latitude et y = longitude
		grilles[k].alt=malloc_hauteur[k];
	}

	//initialiser les coordonnées des gouttes et le temps
	//Nous plaçons chaque goutte exactement entre 4 cases "grilles", ce qui correspond au milieu d'une case "carres"
	//et on initialise une partie des structures "carres"
	for(int i=0; i<1548; i++){
		gouttes[i].x = 592900+200*(i%36);
		gouttes[i].y = 103700-(floor(i/36)*200);
		gouttes[i].time_tot = 0;

		carres[i].lac = 0;
		carres[i].riviere = 0;
		carres[i].catch = 0;
		carres[i].time = 0;
		carres[i].quantite = 1;
	}
}

int inLac(double x, double y){
	//But : dire si un point (de coord x et y) est dans le lac ou non
	//Nous pouvons ensuite retrouver les coordonnées correspondantes avec la formule reliant les indices et les coordonnées

	//pour calculer les droites entre les points, on calcule des fonctions linéaires :
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

	//si le point en question est dans le lac => return 1

	if(101400 <= y && y <= 103200){
		//si y est entre les points 34 et 56 (cf. schéma sur papier)
		if(((y-inters35)/slope35) <= x && x <= ((y-inters46)/slope46)){
			return 1;
		} else{
			return 0;
		}
	} else if(98000 <= y && y <= 101400){
		//si y est entre les points 12 et 34 (cf. schéma sur papier)
		if(((y-inters13)/slope13) <= x && x <= ((y-inters24)/slope24)){
			return 1;
		} else{
			return 0;
		}
	} else{
		return 0;
	}
}

int slope_function(struct Carre * carres,double * malloc_hauteur, int number_carre){
	//Calculer les pentes (en x et y) de chaque case
	// Le principe est de calculer la moyenne des gradients en x, en y sur une case
	// qui est représentée par 4 différentes hauteurs. (Contrairement au malloc, on a 4 hauteurs par cases (dans les angles) et 2 pentes.)
	for (int i=0;i<number_carre;i++){
		double value1 = malloc_hauteur[i+(int)(i/36)] - malloc_hauteur[i+1+(int)(i/36)];//si cette pente est positive, la goutte avancera dans la direction de l'axe x positif
		double value2 = malloc_hauteur[i+37+(int)(i/36)] - malloc_hauteur[i+38+(int)(i/36)];
		carres[i].slope_x = ((value1+value2)/2)/200;
		//On a divisé par 2 pour avoir une pente moyenne
		double value3 = malloc_hauteur[i+37+(int)(i/36)] - malloc_hauteur[i+(int)(i/36)];//si cette pente est positive, la goutte avancera dans la direction de l'axe y positif
		double value4 = malloc_hauteur[i+38+(int)(i/36)] - malloc_hauteur[i+1+(int)(i/36)];
		carres[i].slope_y = ((value3+value4)/2)/200;

		//On fait "+(int)(i/36)" parce que le tableau des alt est 37x44 alors que celui des carres est de 36x43
	}
	return 0;
}

int velocity(struct Carre * carres, int nbr_carre){
	//Déterminer la vitesse d'une goutte selon la pente en x et en y
	double a = 0.06;
	double b = 0.1;//vitesse minimale de la goutte
	//On estime que la pente maximale est de 5 (= 1000m/200m) et que la vitesse correspondante serait de 0.4 m/s
	//vitesse = a*pente + b
	//pour les rivières :
	double a_river = 1.8;
	double b_river = 1;//vitesse minimale de la goutte dans une rivière
	for(int i = 0; i<nbr_carre; i++){
		//Prendre en compte si la pente est positive (dans le sens de l'axe) ou négative, pour adapter les vitesses (positives ou négatives)
		if(carres[i].slope_x>=0 && carres[i].slope_y>=0){
			if(carres[i].riviere ==0){
				carres[i].vit_x = a*carres[i].slope_x+b;
				carres[i].vit_y = a*carres[i].slope_y+b;
			} else if(carres[i].riviere ==1){
				carres[i].vit_x = a_river*carres[i].slope_x+b_river;
				carres[i].vit_y = a_river*carres[i].slope_y+b_river;
			}
		} else if(carres[i].slope_x>=0 && carres[i].slope_y<=0){
			if(carres[i].riviere ==0){
				carres[i].vit_x = a*carres[i].slope_x+b;
				carres[i].vit_y = a*carres[i].slope_y-b;
			} else if(carres[i].riviere ==1){
				carres[i].vit_x = a_river*carres[i].slope_x+b_river;
				carres[i].vit_y = a_river*carres[i].slope_y-b_river;
			}
		} else if(carres[i].slope_x<=0 && carres[i].slope_y>=0){
			if(carres[i].riviere ==0){
				carres[i].vit_x = a*carres[i].slope_x-b;
				carres[i].vit_y = a*carres[i].slope_y+b;
			} else if(carres[i].riviere ==1){
				carres[i].vit_x = a_river*carres[i].slope_x-b_river;
				carres[i].vit_y = a_river*carres[i].slope_y+b_river;
			}
		} else if(carres[i].slope_x<=0 && carres[i].slope_y<=0){
			if(carres[i].riviere ==0){
				carres[i].vit_x = a*carres[i].slope_x-b;
				carres[i].vit_y = a*carres[i].slope_y-b;
			} else if(carres[i].riviere ==1){
				carres[i].vit_x = a_river*carres[i].slope_x-b_river;
				carres[i].vit_y = a_river*carres[i].slope_y-b_river;
			}
		}
	}
	return 0;
}

//Nouvelle fonction indexing pour le tableau de struct "carre", car il comprend une colonne et une ligne de moins que le tableau "grilles" (des altitudes)
int carreIndex(int x,int y){
	//Créer la fct pour qu'elle nous dise dans quelle case CARRE est un point (x,y) => nous donne l'indice de la case (=> un nombre entier)
	int i=floor(fabs(103799-y)/200)*36+floor(fabs(x-592801)/200);
	//Dans ce cas, si la goutte est sur un bord (entre 2 cases), elle est considérée comme étant sur la case précédente (sauf si c'est le bord du tableau)
	return i;
}

void river(struct Carre * carres){
	//Initialiser les cases où il y a une rivière, donc où les gouttes d'eau se déplacerons plus vite
	//Nous le faisons seulement pour les deux rivières qui ont les pentes les plus douces
	//(car sinon, il y aurait beaucoup trop de rivières à définir autour du lac des Dix)
	//Pour définir dans quelles cases passent les rivières, nous les approximons avec des fonctions (des droites)

	//Rivière se trouvant au Sud-Ouest du lac :
	//Elle va du point (596706.9 ; 097052.3) vers le point (597276.4 ; 097714.8)
	double slopeSO = (double)(97714.8-97052.3)/(597276.4-596706.9);
	double intersSO = 97052.3-(slopeSO*596706.9);

	//Initialiser les structures carres qui contiennent cette rivière
	for(int x=596706; x<597276; x = x+50){
		for(int i=0; i<1548; i++){
			if(carreIndex(x,(slopeSO*x + intersSO))==i){
				carres[i].riviere = 1;
			} else{
				continue;
			}
		}
	}

	//Rivière se trouvant au Sud-Est du lac :
	//Elle va du point (598544.5 ; 097272.9) vers le point (598039.0 ; 097946.3)
	double slopeSE = (double)(-(97946.3-97272.9))/(598544.5-598039.0);
	double intersSE = 97272.9-(slopeSE*598544.5);

	//Initialiser les structures carres qui contiennent cette rivière
	for(int x=598039; x<598544; x = x+50){
		for(int i=0; i<1548; i++){
			if(carreIndex(x,(slopeSE*x + intersSE)) == i){
				carres[i].riviere = 1;
			} else{
				continue;
			}
		}
	}

	//La suite de la rivière se trouvant au Sud-Est du lac :
	//Elle va du point (598460.0 ; 096746.2) vers le point (598544.5 ; 097272.9)
	double slopeSE2 = (double)(97272.9-96746.2)/(598544.5-598460);
	double intersSE2 = 96746.2-(slopeSE2*598460);

	//Initialiser les structures carres qui contiennent cette rivière
	for(int x=598460; x<598544; x = x+50){
		for(int i=0; i<1548; i++){
			if(carreIndex(x,(slopeSE2*x + intersSE2)) == i){
				carres[i].riviere = 1;
			} else{
				continue;
			}
		}
	}
}

int avancer(struct Carre * carres, struct Goutte * gouttes){
	//Fonction pour faire avancer une goutte
	//on va effectuer cette fonction toutes les secondes (=> une fois par seconde)
	//on veut return (dans les struct) le temps que ça met jusqu'au lac, si la goutte y va

	//si ça va dans le lac, ça return 2 (qui est aussi "true")
	//si la goutte continuera d'avancer, ça return 1 (qui est aussi "true")
	//si la goutte s'arrête ou sort de notre zone, ça return 0 (qui est aussi "false")

	//Mettre des catch = 0 quand ça return 0 => dans le main, car ici la case carres changera, ce ne sera pas toujours la case initiale,
	//et on veut mettre carres.catch = 0 dans la case "carres" initiale (donc celle où la goutte est tombée)
	if (gouttes->x <= 592800) return 0;
	if (gouttes->y >= 103800) return 0;
	if (gouttes->x >= 600000) return 0;
	if (gouttes->y <= 95200) return 0;

	if(inLac(gouttes->x, gouttes->y)==1){
		//On mettra le carres.catch du carre initial à 1 (dans la boucle faite dans "main")
		return 2;
	} else if(carres->slope_x ==0 && carres->slope_y ==0){
		//Déterminer si la goutte reste sur place/s'arrête => si les deux pentes sont égales à 0
		return 0;
	} else{
		gouttes->x += carres->vit_x*1;
		gouttes->y += carres->vit_y*1;
		//le "*1" est là parce qu'on ajoute aux coordonnées la distance que la goutte parcours pendant 1 seconde

		//nbr de secondes que la goutte met jusqu'au lac (si elle y va)
		gouttes->time_tot += 1;
		return 1;
	}
}

void meteo(double latitude, double longitude){
	//Pour avoir les données météo obtenues dans Meteoblue
	char site[1000];
	sprintf(site,"curl \"https://my.meteoblue.com/packages/basic-1h?apikey=8265088095b8&lat=%f&lon=%f&asl=453&tz=Europe%%2FZurich&city=Lausanne\" -o quantite_precipitation1.json",latitude,longitude);
	
	system(site);
}

int main(int argc, char * argv[]) {
	//tableaux de structures
	struct Carre carres[1548];
	struct Goutte gouttes[1548];
	struct GpsPoint points[1628];
	struct Grille grilles[1628];
	
	//Pour avoir les donées météo
	meteo(46.079468, 7.400798);
	
	//Récupérer les altitudes
	int nbPoints = lireFichier("DHM200.xyz", points, 1628*sizeof(double));
	
	if (nbPoints == -1) {
		printf("Erreur: Le fichier n'existe pas ou n'est pas accessible.\n");
		return 1;
	} else if (nbPoints == 0) {
		printf("Erreur: Le fichier est vide ou pas dans le bon format.\n");
		return 1;
	}
	//Déterminer le x minimum et le y maximum
	double xmin = points[0].latitude;
	double ymax = points[0].longitude;
	//(info : en fait, il s'agit déjà des xmin et ymax)
	for(int i=1; i<nbPoints; i++){
		if(points[i].latitude<xmin){
			xmin=points[i].latitude;
		}
	}
	for(int i=1; i<nbPoints; i++){
		if(points[i].longitude>ymax){
			ymax=points[i].longitude;
		}
	}

	//Création du malloc contenant les altitudes :
	double * altitudes = malloc(1628*sizeof(double));
	for(int i=0; i<nbPoints; i++){
		altitudes[(((int) ymax- (int) points[i].longitude)/200)*37 + (((int) points[i].latitude- (int) xmin)/200)] = points[i].altitude;
	}

	writeCsv("altitudes.csv", altitudes, 37, 44);


	//Initialiser certaines variables des structures
	initialisation(altitudes, grilles, gouttes, carres);
	slope_function(carres, altitudes, 1548);
	river(carres);
	velocity(carres, 1548);

	//Créer un tableau avec la zone des riviere = 1
	int * rivieres = malloc(1548*sizeof(int));
	for(int i=0; i<1548; i++){
		rivieres[i] = carres[i].riviere;
	}
	intwriteCsv("rivieres.csv", rivieres, 36, 43);

	/*
	//Code pour lancer une seule goutte :

	//Si nous mettons une goutte dans la case 134:
	int i = 134;
	int k = 134;

	//On applique la fonction "avancer" sur la goutte qui est à sa position initiale, pour pouvoir ensuite commencer la boucle while
	int result = avancer(&carres[k], &gouttes[i]);
	//Donc maintenant, gouttes[i].time_tot = 1 seconde

	while(result){
		//Donc "tant que result = 1 ou 2" => "tant que result = true"

		//Pour ajouter à la case initiale le temps que la goutte met jusqu'au lac :
		carres[i].time = gouttes[i].time_tot;
		//printf("i= %d, k= %d\n", i, k);
		//printf("%f, %f\n", gouttes[i].x,gouttes[i].y);

		if(result==2){
			//printf("La goutte va dans le lac des Dix et met %d secondes\n", carres[i].time);
			carres[i].catch = 1;
			carres[k].lac = 1;

			break;
		}

		if(carreIndex(gouttes[i].x,gouttes[i].y)!=k){
			//contrôler si la goutte change de case "carres" ou non
			k = carreIndex(gouttes[i].x,gouttes[i].y);
		}
		result = avancer(&carres[k], &gouttes[i]);
	}
	*/

	//Pour savoir le nombres de gouttes qui vont dans le lac :
	int ok = 0;


	//Simulation d'une goutte d'eau qui tombe dans chaque case "carres" et qui se déplace

	//Pour pouvoir ensuite créer une animation avec le parcours des gouttes :
	//Créer un malloc (et un csv) avec les coordonnées des gouttes toutes les 2 min
	//Pour pouvoir ensuite créer des graphiques et une animation de l'avancement des gouttes
	//SANS les rivières, le temps maximum qu'une goutte met jusqu'au lac est de plus de 64 heures
	//AVEC les rivières, le temps maximum qu'une goutte met jusqu'au lac est de 150685 secondes (41.86 heures)
	//Nous considérons la simulation AVEC les rivières
	//Ensuite, lancer chaque goutte et sauvegarder les coordonnées toutes les 2 min (donc en fonction de goutte[i].time) donc toutes les 120 secondes
	//Donc il y a 1257 lignes (la 1ère ligne commence à 0 seconde)(150685/120 = 1255.7), chaque ligne correspond à un intervalle de 2 min
	//et il y a 3096 colonnes (1548 gouttes * 2, car chaque goutte a 2 coordonnées)
	//1ère colonne = coord_x de goutte1 ; 2ème = coord_y de goutte1 ; 3ème = coord_x de goutte2 ; ...
	//Donc, pour l'animation, nous aurons 1257 graphes à afficher à la suite

	double * animation = malloc(3096*1257*sizeof(double));
	//Dans l'animation, on veut le parcours de toutes les gouttes, et pas seulement de celles qui vont dans le lac
	//Mais, nous arrêtons la simulation une fois que toutes les gouttes qui vont dans le lac sont dans le lac
	//Ce qui veut dire que nous fixons un temps maximum de 150720 secondes (= 1256*120)

	//Pour la simulation des gouttes :
	int num_colonne = 0;
	//L'indice, dans le malloc animation, est "num_colonne + num_ligne*3096"

	//pour parcourir les gouttes
	for(int i=0; i<1548; i++){
		//pour changer de cases carres (et donc changer les pentes et les vitesses)
		int k = i;

		//Pour l'animation, nous sauvegardons les coordonnées de la goutte au temps 0
		int num_ligne = 0;
		animation[num_colonne + num_ligne*3096]=gouttes[i].x;
		animation[num_colonne + 1 + num_ligne*3096]=gouttes[i].y;
		num_ligne += 1;

		//On applique la fonction "avancer" sur la goutte qui est à sa position initiale, pour pouvoir ensuite commencer la boucle while
		int result = avancer(&carres[k], &gouttes[i]);
		//Donc maintenant, gouttes[i].time_tot = 1 seconde donc le premier "modulo 120" ne sera pas "= 0"

		//Mais attention : on ne veut pas changer de goutte tant qu'elle n'est pas dans le lac (ou arrêtée)
		while(result){
			//Donc "tant que result = 1 ou 2" => "tant que result = true"

			//Pour ajouter à la case initiale le temps que la goutte met jusqu'au lac :
			carres[i].time = gouttes[i].time_tot;

			//Sauvegarder les coordonnées de la goutte toutes les 120 secondes pour faire l'animation
			if(gouttes[i].time_tot%120==0 && gouttes[i].time_tot<=150720){
				//remplir les 2 colonnes (correspondantes à cette goutte) avec ses coordonnées x et y
				animation[num_colonne + num_ligne*3096]=gouttes[i].x;
				animation[num_colonne + 1 + num_ligne*3096]=gouttes[i].y;
				num_ligne += 1;
			}

			if(result==2){
				ok +=1;
				//printf("%d goutte(s) vont dans le lac des Dix, %d secondes pour que la goutte aille dans le lac, catchment case %d\n", ok, carres[i].time, i);
				carres[i].catch = 1;
				carres[k].lac = 1;

				break;
			}

			if(carreIndex(gouttes[i].x,gouttes[i].y)!=k){
				//contrôler si la goutte change de case "carres" ou non
				k = carreIndex(gouttes[i].x,gouttes[i].y);
			}
			result = avancer(&carres[k], &gouttes[i]);
			
			if(result==0 && gouttes[i].time_tot<=150720){
				//Si result = 0, alors on ne rentrera pas dans le while et nous sauvegardons les dernières coordonnées de la goutte (pour l'animation)
				animation[num_colonne + num_ligne*3096]=gouttes[i].x;
				animation[num_colonne + 1 + num_ligne*3096]=gouttes[i].y;
			}
		}
		//Dans le malloc animation, aller 2 colonnes plus loins, car nous changeons de goutte
		num_colonne += 2;
	}
	printf("%d cases menent au lac des Dix\n", ok);

	//Créer un tableau avec la zone des catch = 1
	int * catchment = malloc(1548*sizeof(int));
	for(int i=0; i<1548; i++){
		catchment[i] = carres[i].catch;
	}
	intwriteCsv("catchment.csv", catchment, 36, 43);

	//Créer un csv avec le temps que met chaque goutte jusqu'au lac
	//En tout, 931 gouttes arrivent dans le lac
	int * malloc_time = malloc(931*sizeof(int));
	int increment = 0;
	for(int i=0;i<1548;i++){
		if(carres[i].catch==1){
			malloc_time[increment]=carres[i].time;
			increment +=1;
		}
	}
	intwriteCsv("time.csv", malloc_time, 1,931);

	//Créer le malloc pour l'animation
	writeCsv("animation.csv", animation, 3096,1257);

	free(altitudes);
	free(catchment);
	free(malloc_time);
	free(animation);
	free(rivieres);
	return 0;
}
