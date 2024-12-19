#ifndef DETERMINATION_TEMPERATURE_H
#define DETERMINATION_TEMPERATURE_H

void calcule_regression_lineaire(double *temperatures, double *times, int n, double *coefficients) ;
double temperature(double t, double a, double b,double facteur_climatique) ;
double facteur_temperature_w(double t, double a, double b, double facteur_climatique) ;
double facteur_temperature_a(double t, double a, double b, double facteur_climatique) ;
double facteur_temperature_c(double t, double a, double b, double facteur_climatique) ;
double facteur_temperature_s(double t, double a, double b, double facteur_climatique) ;

#endif
