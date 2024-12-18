### ------ Setting up the compiling of C files ------ ###
# Specify the compiler for C files
CC = gcc

# Compiling flags
CFLAGS = -Wall -O2

# Libraries
LIBS = -lm

# Output
OUT = -o

### ------ Setting up the compiling of Python files ------ ###
# Specify the Python interpreter - use your own. 
PYTHON =  C:/Users/Titouan/Anaconda3/python.exe

# Specify file names and their relative paths
C_FILE_1= Modelisation2/resolution_modelisation2.c
C_FILE_2 = Modelisation2/main_humanimpact.c

# Specify the name of your Python files
PYTHON_FILE_1 = Modelisation2/visualisation_modele2.py
PYTHON_FILE_2 = Modelisation2/visualisation_pollution.py
PYTHON_FILE_3 = Modelisation2/visualisation_acidite.py
PYTHON_FILE_4 = Modelisation2/regressionlineaire.py


### ------ Default target -> order in which you want the files to be run this is what will actually happen in your terminal ------ ###
all: compile_c_1 run_python_1 compile_c_2 run_python_2 run_python_3 run_python_4 clean

### --- Line command (target) to compile the C file --- ###
compile_c_1: $(C_FILE_1)
	$(CC) $(C_FILE_1) $(CFLAGS) $(LIBS) $(OUT) $(basename $(C_FILE_1))

compile_c_2 : $(C_FILE_2)
	$(CC) $(C_FILE_2) $(CFLAGS) $(LIBS) $(OUT) $(basename $(C_FILE_2))

### --- Target to run the Python files --- ###
run_python_1:
	$(PYTHON) $(PYTHON_FILE_1)

run_python_2:
	$(PYTHON) $(PYTHON_FILE_2)

run_python_3:
	$(PYTHON) $(PYTHON_FILE_3)

run_python_4:
	$(PYTHON) $(PYTHON_FILE_4)



# Clean target to remove compiled files
clean:
	rm -f $(basename $(C_FILE))