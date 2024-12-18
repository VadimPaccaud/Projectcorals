# Computational Methods and Tools - Project : Modeling the impact of temperature and anthropogenic activity on a coral ecosystem

## Project description

This program simulates in first the impact of temperature on a coral reef in Gili Island. In a second time, the program simulate a release of pollutants and how it impacts the corals.

The program will:
1. Solve a differential system equations and save the results in a CSV file (**resolution_modelisation.c**)
2. Plot the results of the CSV file (**visualisation_modele.py**)
3. Simulate the release of CO2 in atmosphere and how it's affect the pH of the ocean and save the results in 3 CSV file (**main_humanimpact.c**)
4. Plot the results of the two CSV file (**visualisation_pollution.py** and **visualisation_acidite.py** respectively)
5. Apply linear regression method on datas collected in one of the CSV file and plot it (**linearregression.py**)
6. All graphs results obtained, and key figures will be saved in "*Results*" as .png .

## Project structure !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

- As the data used were few in number, it was quicker to write them directly to the code. We mention them in the code. 
- "*Internal*" contains files used for passing information between C and Python. They are automatically edited by the program and should not be manually modified.
- "*Code*" contains program code.
- "*Results*" contains saved .jpg files of graphs and a summary .txt file.

### Inputs and Outputs

Inputs:
- As the data used were few in number, it was quicker to write them directly to the code. We mention them in the code.

Internal files:
- **Internal/ecosystem_simulation_results.csv** is a comma-delimited file.
- **Internal/CO2_terrain.csv** , 1 column and 10'000 rows file.
- **Internal/acidite_terrain.csv**, 1 column and 10'000 rows file.
- **Internal/valeurs_uniques_pH_sante.csv** , is a comma-delimited file.

Outputs:
- "*Results*" contains several image files, each has a custom name of format: "*Dissolved_Oxygen_{Month}_{Value}_percent.png*". You can save them or delete them as required.
- "*Results/Results.txt*" is a text file containing summarized information of the simulation, such as critical time and distance. Do **not** delete this file.
- **C02_atmosphere_figure.png** is an image file, showing the C02 concentration in the atmosphere
- **pH_water_figure.png** is an image file, showing the pH of water
- **observation_results.png** is an image file, plotting the datas collected
- **final_curve.png** is an image file, plotting the relation between coral health and the pH of water

## Implementation details

**Overview:**

- The simulation is handled by C. It directly outputs the results of computation into a CSV file.
- Python reads CSV file values, which contains parameters selected by the user and plot CSV contents.
- The simulation is handled by C. It directly outputs the results of computation into 3 other CSV file ( **Internal/CO2_terrain.csv** , **Internal/acidite_terrain.csv** , **Internal/valeurs_uniques_pH_sante.csv** ).
- 2 Python program reads  **Internal/CO2_terrain.csv** and **Internal/acidite_terrain.csv** respectively, which contains parameters selected by the user and plot CSV contents.
- The last Python program make the linear regression with the Numpy modul and plot the results.

**Structure**: In the directory "*Code/*" are located:
- "*RhineData.py*"
    - Reads in the CSVs located in "*Data*".
    - Computes average monthly values of river parameters : flowrate / temperature / dissolved oxygen...
- "*Parameterchoice.py*"
    - Imports "*RhineData.py*" as a module.
    - Writes all selected parameters ("*RhineData.py*" + pollution values) into a text file "*Internal/PythonParameters.txt*".
- "*Computations.c*"
    - Reads in "*Internal/PythonParameters.txt*".
    - Performs computations.
    - Exports results into the CSV "*Internal/CalculatedData.csv*".
- "*Visualisation.py*"
    - Executes the compiled C file.
    - Reads in the CSV "*Internal/CalculatedData.csv*".
    - Plots results in a separate window.
    - Saves key plots to directory "*Results*".
    - Writes a summary file to "*Results*".

## Instructions

To reproduce results in the report, four steps should be followed:
1. Navigate to the "*Parameterchoice.py*" function located in the "*Code*" directory. The function contains commented instructions for the user to modify input parameters. Modify as desired, and save the file.
2. Navigate to the makefile to ensure the Python interpreter selected is yours.
3. Open the terminal from the project root directory (location of this README.md file). You can check this is the case by typing:
    ```
    ls
    ```
    The terminal should return:
    ```
    Code    Data    Internal    README.md   Results    makefile
    ```
4. Run the following line in the terminal:
    ```
    make
    ```
The program will run automatically, and will open two plotting windows in full screen. Once you close them , the program cleans up compiled filed files and terminates automatically.

## Requirements

Versions of Python and C used are as follows.
```
$ python --version
Python 3.11.5

$ gcc --version
Apple clang version 14.0.0
```
The Python libraries utilized were the following:
```
pandas 2.1.4 py311hdb55bb0_0

numpy 1.26.0 py311h728a8a3_0  
numpy-base 1.26.0 py311h53bf9ac_0

subprocess 2.4

matplotlib 3.8.0 py311hecd8cb5_0  
matplotlib-base 3.8.0 py311h41a4f6b_0  
matplotlib-inline 0.1.6 py311hecd8cb5_0 

csv 1.0
```

## Credits

### Data

The data files "*Rhine_DO_Palmrainbrücke.csv*", "*Rhine_Flowrate_Rheinhalle.csv*"  and "*Rhine_Watertemperature_Palmrainbrucke.csv*" come from the [Swiss Federal Bureau for the Environment](https://www.bafu.admin.ch/bafu/en/home/topics/water/state/data/obtaining-monitoring-data-on-the-topic-of-water/hydrological-data-service-for-watercourses-and-lakes.html).

The data file "*Rhine_depth.csv*" comes from the [French eauFrance](hydro.eaufrance.fr).

The data file "*Rhine_windspeeds.txt*" comes from the [Swiss Federal Office of Meteorology](https://www.meteoswiss.admin.ch/services-and-publications/service/weather-and-climate-products/data-portal-for-teaching-and-research.html).

### Formulae

The function tracking dissolved oxygen relies on the [Streeter-Phelps](https://en.wikipedia.org/wiki/Streeter%E2%80%93Phelps_equation) equation, taken from Pearson's *[Introduction to Environmental Engineering and Science](https://www.pearson.com/en-gb/subject-catalog/p/introduction-to-environmental-engineering-and-science-pearson-new-international-edition/P200000004949/9781292025759)*.

The function tracking pollutant concentrations is based on the Van Genuchten equation, solved by Jin & al in *[Mitigation of impact of a major benzene spill into a river](https://www.sciencedirect.com/science/article/pii/S0043135420300257)*.
