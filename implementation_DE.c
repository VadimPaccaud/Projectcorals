#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "determination_temperature.c"

// Maximum density of all the species [kg.m^-2]

#define MAX_DENSITE_W 35.0
#define MAX_DENSITE_A 15.0
#define MAX_DENSITE_C 50.0
#define MAX_DENSITE_S 18.0

#define FACTEUR_REDUCTION 0.95 // If species reach these maximum densities, they are reduced by this factor 

// Global parameters of the model

double PHI1 = 0.1, PHI2 = 0.05, PHI3 = 0.03, PHI4 = 0.02 ;
double THETA1 = 0.5, THETA2 = 0.4 ;
double MU = 0.02, DELTA1 = 0.01, DELTA2 = 0.005 ;
double U = 0.001, p = 0.1, q = 0.15 ;

// Function to save results in a CSV file

void save_to_csv(const char *filename, double *t, double *w, double *a, double *c, double *s, double *T, int size) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erreur d'ouverture du fichier") ;
        return ;
    }
    fprintf(file, "Time,W,A,C,S,Temperature\n") ;
    for (int i = 0 ; i < size ; i++) {
        fprintf(file, "%f,%f,%f,%f,%f,%f\n", t[i], w[i], a[i], c[i], s[i], T[i]) ;
    }
    fclose(file) ;
}

// Function for simulate the ecosystem

void simulate_ecosystem(double max_time, double dt, double *mean_coeff, double *percent_init_pop, double *densite_pop, double facteur_climatique) {
    int steps = (int)(max_time / dt) ;

    double *time = malloc(steps * sizeof(double)) ;
    double *w = malloc(steps * sizeof(double)) ;
    double *a = malloc(steps * sizeof(double)) ;
    double *c = malloc(steps * sizeof(double)) ;
    double *s = malloc(steps * sizeof(double)) ;
    double *T = malloc(steps * sizeof(double)) ;

    if (!time || !w || !a || !c || !s || !T) {
        printf("Erreur : allocation mémoire échouée.\n") ;
        return ;
    }

    double surface = 3e6 * 1.2;                                 // Corrected projected surface area
    double initial_surface[4], biomasses[4], densite[4];

    for (int i = 0; i < 4; i++) {
        initial_surface[i] = surface * percent_init_pop[i];
        biomasses[i] = initial_surface[i] * densite_pop[i];
        densite[i] = biomasses[i] / surface;
    }

    for (int i = 0; i < steps; i++) {
        time[i] = i * dt;
        w[i] = densite[0];
        a[i] = densite[1];
        c[i] = densite[2];
        s[i] = densite[3];

        T[i] = temperature(time[i], mean_coeff[0], mean_coeff[1], facteur_climatique);
        T[i] = fmin(fmax(T[i], 25), 35);  // Limitation of température


        // Updating the impact of temperature

        double gamma_w = facteur_temperature_w(time[i], mean_coeff[0], mean_coeff[1], facteur_climatique) ;
        double gamma_a = facteur_temperature_a(time[i], mean_coeff[0], mean_coeff[1], facteur_climatique) ;
        double gamma_c = facteur_temperature_acropora(time[i], mean_coeff[0], mean_coeff[1], facteur_climatique) ;
        double gamma_s = facteur_temperature_s(time[i], mean_coeff[0], mean_coeff[1], facteur_climatique) ;

        // Gamma limitation at 1.0

        gamma_w = fmin(gamma_w, 1.0) ;
        gamma_a = fmin(gamma_a, 1.0) ;
        gamma_c = fmin(gamma_c, 1.0) ;
        gamma_s = fmin(gamma_s, 1.0) ;

        double dw = densite[0] * (PHI1 * (1 - (densite[0] / (1 + THETA1 * densite[2]))) + DELTA1 * densite[3]) - MU * densite[0] * densite[1] - gamma_w * T[i] ;
        double da = densite[1] * (PHI2 * (1 - (densite[1] / (1 + THETA2 * densite[2]))) - MU * densite[0] * densite[1]) - gamma_a * T[i] ;
        double dc = densite[2] * (PHI3 * (1 - densite[2]) - (U * densite[3]) / (densite[2] + p)) - gamma_c * T[i] ;
        double ds = densite[3] * (PHI4 * (1 - densite[3]) - DELTA2 * densite[0] - (densite[2] / (densite[2] + q))) - gamma_s * T[i] ;

        densite[0] += dw * dt;
        densite[1] += da * dt;
        densite[2] += dc * dt;
        densite[3] += ds * dt;

        // Checking thresholds

        if (densite[0] > MAX_DENSITE_W) densite[0] *= FACTEUR_REDUCTION ;
        if (densite[1] > MAX_DENSITE_A) densite[1] *= FACTEUR_REDUCTION ;
        if (densite[2] > MAX_DENSITE_C) densite[2] *= FACTEUR_REDUCTION ;
        if (densite[3] > MAX_DENSITE_S) densite[3] *= FACTEUR_REDUCTION ;

        // Extinction if negative density

        for (int j = 0; j < 4; j++) {
            if (densite[j] < 0) densite[j] = 0;
        }
    }

    save_to_csv("simulation_modelisation2_results.csv", time, w, a, c, s, T, steps);

    free(time);
    free(w);
    free(a);
    free(c);
    free(s);
    free(T);

    printf("Simulation terminée. Résultats sauvegardés dans 'simulation_modelisation2_results.csv'.\n");
}
