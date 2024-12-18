import numpy as np
import matplotlib.pyplot as plt

# Chargement des données depuis le fichier
filename = "C02_terrain.csv"
data = np.loadtxt(filename)

# Reshape des données en un tableau 100x100
terrain = data.reshape((100, 100))

# Création du graphique
plt.figure(figsize=(8, 8))
im = plt.imshow(terrain, cmap='viridis', origin='lower', aspect='equal')

# Ajout d'une barre de couleur avec une étiquette
cbar = plt.colorbar(im)
cbar.set_label("concentration of C02 [ppm]", fontsize=12) 

# Ajout de labels et d'un titre
plt.title("C02 in atmosphere", fontsize=14)
plt.xlabel("X", fontsize=12)
plt.ylabel("Y", fontsize=12)

# Personnalisation des ticks
plt.xticks(fontsize=10)
plt.yticks(fontsize=10)

# Enregistrement (optionnel)
# plt.savefig("acidite_terrain.png", dpi=300)

# Affichage
plt.show()