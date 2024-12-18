#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "determination_temperature.h"
#include "implementation_equa_diff_modele2.c"


// Time steps and limits

#define DT 0.01
#define MAX_TIME 1000

#define facteur_climatique 0.15
#define nbr_month 12



int main() {

    double releve_temperatures1[] = {28.7, 28.8, 29.2, 29.7, 29.6, 28.8, 28.2, 27.8, 28.0, 28.7, 29.4, 29.3} ; // Temperature record for the Gilis Islands #1
    double releve_temperatures2[] = {29.3, 29, 29.5, 29.4, 29.3, 28.5, 27.5, 27.2, 27.8, 28.8, 29.5, 29.9} ; // Temperature record for the Gilis Islands #2
    double releve_temperatures3[] = {29, 29, 29, 29, 29, 28, 28, 27, 27, 28, 29, 29} ; // Temperature record for the Gilis Islands #3
    double releve_temperatures4[] = {29.3, 28.8, 29.3 , 29.4, 29.1, 28.5 , 27.7 , 27.4 , 27.6 , 28.8 , 29.7 , 29.7} ; // Temperature record for the Gilis Islands #4
    double time[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}; // Time (month)

    double coeff1[2], coeff2[2], coeff3[2], coeff4[2] ;

    calcule_regression_lineaire(releve_temperatures1,time, nbr_month, coeff1) ;
    calcule_regression_lineaire(releve_temperatures2,time, nbr_month, coeff2) ;
    calcule_regression_lineaire(releve_temperatures3,time, nbr_month, coeff3) ;
    calcule_regression_lineaire(releve_temperatures4,time,nbr_month, coeff4) ;

    static double mean_coefficients[2] ; 

    for ( int i = 0 ; i<2 ; i++) {
        mean_coefficients[i]= (coeff1[i] + coeff2[i] + coeff3[i] + coeff4[i]) / 4 ;
        printf("%f ", mean_coefficients[i]) ;
    }


    double percent_initial_population[] = {0.17, 0.03 , 0.35 , 0.01} ; // Percentages occupied by each population at initial conditions
    
    double densite_population[] = {0.01 , 0.02 , 7.0, 0.15} ;  // Typical biomass densities in [kg/m^2]
    
    simulate_ecosystem(MAX_TIME, DT, mean_coefficients, percent_initial_population, densite_population, facteur_climatique) ;

    

    return 0 ;
}