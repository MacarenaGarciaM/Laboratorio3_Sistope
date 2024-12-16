CC = gcc
FLAGS = -Wall -pthread -lm

all: main

# Compilar funciones.c en un archivo objeto
funciones.o: funciones.c funciones.h
	$(CC) $(FLAGS) -c funciones.c -o funciones.o

# Enlazar main con funciones.o para crear el ejecutable
main: lab3.c funciones.o
	$(CC) $(FLAGS) funciones.o lab3.c -o lab3

# Limpiar archivos temporales
clean:
	rm -f *.o lab3
