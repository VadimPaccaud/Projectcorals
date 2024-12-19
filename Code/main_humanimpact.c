#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define taillex 100     // dimensions of water field
#define tailley 100

double randomDomaine = RAND_MAX + 1.0;

double randomNumber(double max) {
    return rand() / randomDomaine * max;
}

// Function to find an index in an array
int trouverIndice(double *tableau, int taille, double valeur) {
    for (int i = 0; i < taille; i++) {
        if (fabs(tableau[i] - valeur) < 1e-6) { 
            return i;
        }
    }
    return -1; // Return -1 if we don't find the value
}

// Function to collect unique values and datas associated
int collecterValeursUniquesEtSante(double *ph, double *sante, int taille, 
                                   double *phUniques, double *santeAssociee) {
    int compteur = 0;
    for (int i = 0; i < taille; i++) {
        int indice = trouverIndice(phUniques, compteur, ph[i]);
        if (indice == -1) { // If the ph is unique
            phUniques[compteur] = ph[i];
            santeAssociee[compteur] = sante[i];
            compteur++;
        }
    }
    return compteur; // Return the number of unique values
}


// Function to simulate packages of C02
void simulePollution(int x, int y, double r, double c, double * tableau) {
    for (int i = 0; i < 100; i++) {
        double ventX = 5;
        double ventY = 0.3*(x-2);

        double ax = (randomNumber(6) - 3);    //We assume here a random movement, can due to many factors as the passage of a boat
        double ay = (randomNumber(6) - 3);

        r = r + 20 / r;

        x += ventX + ax;
        y += ventY + ay;

        if ((x < 0 || x >= taillex) || (y < 0 || y >= tailley)) {    //We stop the simulation if the plume goes out of our land
            break;
        }
        if (c>275){
			c = 275;
		}

        // Propagation of C02 over space

        for (int dx = -r; dx <= r; dx++) {
            for (int dy = -r; dy <= r; dy++) {
                int nx = x + dx;
                int ny = y + dy;

                if (nx >= 0 && nx < taillex && ny >= 0 && ny < tailley) {	
                    double distance = sqrt(dx * dx + dy * dy);
                    if (distance <= r) {					// We look if the C02 is in the radius, or out. If its out, we apply diffusion
                        tableau[ny * taillex + nx] += c * (1 - distance / r);  // Further we are, further the diffusion is important
                    }
                }
            }
        }

    }
}

//Function to absorbate some C02 (by plants for example)
void absorption(double *tableau, int x_taille, int y_taille, double lambda, double delta_t) {
    double decay_factor = exp(-lambda * delta_t);
    for (int i = 0; i < x_taille; i++) {
        for (int j = 0; j < y_taille; j++) {
            tableau[j * x_taille + i] *= decay_factor;
        }
    }
}
int main() {
    srand(time(NULL));
    FILE* output2 = fopen("C02_terrain.csv", "w");    	 // File to collect the concentration of C02 in the air
    FILE* output3 = fopen("acidite_terrain.csv", "w");   // File to collect the pH of water
    

    if (output2 == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier de sortie.\n");
        return 1;
    }
    if (output3 == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier de sortie.\n");
        return 1;
    }
	
	
	// Initializing of data

    double* pollution = malloc(taillex * tailley * sizeof(double));  //We use dynamic memory allocation to change/increase the size of our field
    double* acidite = malloc(taillex * tailley * sizeof(double));
    double* sante = malloc(taillex * tailley * sizeof(double));
    
    for(int i = 0; i<taillex; i++){
		for(int j = 0; j<tailley; j++){
			acidite[j*taillex+i] = 8.1;       // We are initializing the pH of water field
		}
	} 
	      
    for (int i = 0; i < taillex; i++) {
        for (int j = 0; j < tailley; j++) {
            pollution[j * taillex + i] = 300;    // We initialize the concentration of C02[ppm]
        }
    }
    
    for(int i = 0; i<taillex; i++){
		for(int j = 0; j<tailley; j++){
			sante[j*taillex+i] = 7;           // We are initializing the health of corals, in kg.m⁻2
		}
	} 
    
    double depart_x = 0; // We start the simulation at the left down corner, we imagine that a port is at this position
    double depart_y = 0;
	
	//Simulation of throwing packages of C02 over 20 years
	double concentration_c02 = 275;      // Initializing of plume concentration[ppm]
	double r = 3;			     // Initial radius of plume: 3 meters.
	int years = 20;			     //Simulation on 20 years
	for(int i = 0; i<years; i++){
		for(int day = 0; day<365; day++){
			
			simulePollution(depart_x,depart_y,r,concentration_c02, pollution);
			absorption(pollution, taillex,tailley, 0.3, 1);
			
		}
		for(int x = 0; x<taillex; x++){
			for(int y = 0; y<tailley; y++){
				if(pollution[y*taillex+x]>350){
					acidite[y*taillex+x] -= 0.005;        // We assume here that only 30% of C02 concentration is absorbated by the water
				}
				else if(pollution[y*taillex+x]>400){
					acidite[y*taillex+x] -= 0.01;
				}
				else if(pollution[y*taillex+x]>450){
					acidite[y*taillex+x] -= 0.02;
				}
				else if(pollution[y*taillex+x]>500){
					acidite[y*taillex+x] -= 0.04;
				}
				else if(pollution[y*taillex+x]>550){
					acidite[y*taillex+x] -= 0.06;
				}
				else if(pollution[y*taillex+x]>600){
					acidite[y*taillex+x] -= 0.08;
				}
				else if(pollution[y*taillex+x]>650){
					acidite[y*taillex+x] -= 0.1;
				}
				else if(pollution[y*taillex+x]>700){
					acidite[y*taillex+x] -= 0.2;
				}
				else if(pollution[y*taillex+x]>750){
					acidite[y*taillex+x] -= 0.4;
				}

			}
		}

	}
		
	// Loop to simulate the coral health with a change in pH
	for(int x = 0; x<taillex; x++){
			for(int y = 0; y<tailley; y++){
				if((acidite[y*taillex+x]<=8.1) && (acidite[y*taillex+x]>8.08)){
					sante[y*taillex+x] -= (randomNumber(4)+2)/10;
				}
				else if((acidite[y*taillex+x]<=8.08) && (acidite[y*taillex+x]>8.06)){
					sante[y*taillex+x] -= (randomNumber(4)+6)/10;
				}
				
				else if((acidite[y*taillex+x]<=8.06) && (acidite[y*taillex+x]>8.04)){
					sante[y*taillex+x] -= (randomNumber(4)+8)/10 ;
				}
				else if((acidite[y*taillex+x]<=8.04) && (acidite[y*taillex+x]>8.02)){
					sante[y*taillex+x] -= (randomNumber(4)+10)/10;
				}
				else if((acidite[y*taillex+x]<=8.02) && (acidite[y*taillex+x]>=7.99)){
					sante[y*taillex+x] -= (randomNumber(4)+12)/10;
				}

			}
		}
	for(int i = 0; i<taillex; i++){
		for(int j = 0; j<tailley; j++){
			fprintf(output2,"%f\n", pollution[j*taillex+i]);
			fprintf(output3,"%f\n", acidite[j*taillex+i]);
		}
	}
    // Part to collect data to create a dataset for the linear regression, we want to have the same number of x and y. 
	
	 
    double valeursUniquesPH[taillex * tailley];                         // Arrays to stock unique values of coral health and ph
    double santeAssociee[taillex * tailley];

    int nombreValeursUniques = collecterValeursUniquesEtSante(
        acidite, sante, taillex * tailley, valeursUniquesPH, santeAssociee);

    
    FILE* outputUniques = fopen("valeurs_uniques_pH_sante.csv", "w");   // Save unique values of health and pH on a csv file
    if (outputUniques == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier des valeurs uniques.\n");
        return 1;
    }

    for (int i = 0; i < nombreValeursUniques; i++) {
        fprintf(outputUniques, "%f, %f\n", valeursUniquesPH[i], santeAssociee[i]);
    }

    printf("Valeurs uniques de pH collectées et sauvegardées.\n");
    printf("Simulation terminée\n");
    fclose(output2);
    fclose(output3);
    fclose(outputUniques);
    free(pollution);
    free(acidite);


    return 0;
}



