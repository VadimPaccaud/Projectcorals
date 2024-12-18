import csv
from operator import add

import matplotlib.pyplot as plt
import numpy as np
from scipy.signal.filter_design import append
from sympy import shape

#DATASET

x_liste = []
y_liste = []

#ph et sante
with open("valeurs_uniques_pH_sante.csv", mode="r") as file:
    reader = csv.reader(file)


    for row in reader:
        x_liste.append(float(row[0]))
        y_liste.append(float(row[1]))

x = np.array(x_liste)
y = np.array(y_liste)

x = x.reshape((len(x_liste),1))
y = y.reshape((len(y_liste),1))

plt.scatter(x,y)
plt.show()
#Normalize the x axe, the range is too small
x_mean = np.mean(x)
x_std = np.std(x)
x = (x - x_mean) / x_std

#MATRICE X

X = np.hstack((x, np.ones(x.shape)))

#MATRICE THETA

theta = np.ones((2,1))


#MODELE
def model(A1, A2):
    return A1@A2

#ERREUR QUADRATIQUE MOYENNE
def error_moyenne(X,y,theta):
    m = len(y)
    return 1/(2*m)*np.sum((model(X,theta)-y)**2)


#GRADIENTS

def gradient(X,theta,y):
    m = len(y)
    return (1/m)*np.transpose(X)@(model(X,theta)-y)

def gradient_descendant(X,theta, y,alpha, n):
    for i in range(0,n):
        theta = theta -alpha*gradient(X,theta,y)
        
    return theta

#Courbe finale

theta_final = gradient_descendant(X,theta,y, 0.001, 10000)


courbe_finale = model(X,theta_final)

# To make the x axe as the beginning
x = (x*x_std)+x_mean

print(theta_final)
plt.scatter(x,y)
plt.plot(x,courbe_finale, c = "r")
plt.show()

