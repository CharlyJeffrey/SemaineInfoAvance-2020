/* 
Cinquieme itération du code:
    * Utilisation des structures HSV/RGB
    * Modifier la valeur retournée par Mandelbrot par 'iter'
    * Définition d'une fonction pour modifier la valeur retournée par Mandelbrot
    * Ajouter une fonction pour enregistrer l'image en greyscale
*/

#include <stdio.h>  
#include <stdlib.h> 
#include <fstream>
#include <iostream>
#include <string>

// À INCLURE
#include "Outils.cpp"


// Resolution:
//> 1080 x 720
//> 2048 x 1080
//> 3840 x 2160
#define DEPTH 3
#define WIDTH 3840
#define HEIGHT 2160
#define RATIO ((1.0*WIDTH)/HEIGHT)
// Valeur de normalisation pixel
#define NORMALIZATION_VALUE 255

// Mandelbrot variables
#define MAX_ITER 256
#define MAX_NORM 4.0

using namespace std;

/* Fonction pour la récursion de Mandelbrot */
int Mandelbrot(double, double);

/* Fonction pour modifier la valeur obtenue par Mandelbrot */
double Normalize(int);

/* Fonctions pour allouer de la mémoire */
int**  AllocateMemory(int, int);
int*** AllocateMemory(int, int, int);

/* Fonction pour sauvegarder un array en image */
void SaveArrayAsImage(int**, int, int, string);
void SaveArrayAsImage(int***, int, int, int, string);
void SaveArrayAsGreyImage(int***, int, int, int, string);

int main() {
    // Nom de l'image
    string file_name = "Images/image_5.ppm";
    string file_name_GS = "Images/image_5_gs.ppm";

    // Centre de l'image
    double centre_x = -0.75;
    double centre_y = 0.0;
    // Zoom
    double zoom = 1.0;
    // Range et Aspect ratio
    double range_x = 2.0 / zoom;
    double range_y = range_x / RATIO;

    // Position (x,y) dans le domaine
    double x, y;
    // Variable pour stocker la valeur de Mandelbrot
    int val;

    // Structures pour stocker la couleur d'un pixel
    RGB pixelRGB;
    HSV pixelHSV;

    // Initialise la couleur
    pixelHSV.s = 1.0;
    pixelHSV.v = 1.0;

    // Initialise le array
    int *** array;
    // Alloue de la mémoire
    array = AllocateMemory(DEPTH, HEIGHT, WIDTH);
    
    // Boucle sur les rangés
    for (int i = 0; i < HEIGHT; i++) {
        // Obtient la position réelle à partir du pixel
        y = centre_y + (1.0 - ((2.0 * i)/HEIGHT)) * range_y;
        // Boucle sur les colonnes
        for (int j = 0; j < WIDTH; j++) {
            // Obtient la position réelle à partir du pixel
            x = centre_x + (((2.0 * j)/WIDTH) - 1.0) * range_x;
            // Obtient la valeur associée au nombre 'C = x + iy'
            val = Mandelbrot(x, y);
            // Colorie le pixel selon la valeur obtenue
            if (val != 0) {
                // Obtient la valeur entre 0.0 et 1.0
                pixelHSV.h = 360.0 * Normalize(val);
                // Obtient les valeurs RGB
                HSVtoRGB(&pixelHSV, &pixelRGB);
                // Colorie le pixel
                array[0][i][j] = pixelRGB.r;
                array[1][i][j] = pixelRGB.g;
                array[2][i][j] = pixelRGB.b;
            } else {
                // Colorie le pixel en noir
                array[0][i][j] = 0;
                array[1][i][j] = 0;
                array[2][i][j] = 0;
            }
        }
    }
    // Crée l'image
    SaveArrayAsImage(array, DEPTH, WIDTH, HEIGHT, file_name);
    SaveArrayAsGreyImage(array, DEPTH, WIDTH, HEIGHT, file_name_GS);

    // FIN
    return 0;
}

// Fonction de Mandelbrot
int Mandelbrot(double x, double y) {
    // Nombre complexe 'C'
    double c_re = x;
    double c_im = y;
    // Valeur initiale du nombre complexe 'Z'
    double z_re = 0.0;
    double z_im = 0.0;
    // Variables 'tampons'
    double re = 0.0;
    double im = 0.0;
    // Boucle
    int iter = 1;
    while (iter < MAX_ITER) {
        // Nouvelle valeur réelle/imaginaire
        re = z_re*z_re - z_im*z_im + c_re;
        im = 2*z_re*z_im + c_im;
        // Condition sur la norme
        if (re*re + im*im >= MAX_NORM) return iter;
        // Met à jour le nombre 'Z'
        z_re = re;
        z_im = im;
        // Augmente la variable itérative
        iter++;
    }
    return 0;
}

// Fonction pour normaliser la valeur
double Normalize(int val) {
    return sqrt( (1.0 * val) / MAX_ITER );
}


// Fonction pour allouer de la mémoire pour un array 2D
int** AllocateMemory(int w, int h) {
    int ** arr = (int**) malloc(w * sizeof(int*));
    for (int i = 0; i < w; i++) {
        arr[i] = (int*) malloc(h * sizeof(int));
        for (int j = 0; j < h; j++)
            arr[i][j] = 0;
    }
    return arr;
}

// Fonction pour allouer de la mémoire pour un array 3D
int*** AllocateMemory(int d, int w, int h) {
    int *** arr = (int***) malloc(d * sizeof(int**));
    for (int k = 0; k < d; k++) 
        arr[k] = AllocateMemory(w, h);
    
    return arr;
}

void FunkyFunction(int ** array, int w, int h) {
    for (unsigned int i = 0; i < w; i++) 
        for (int j = 0; j < h; j++) 
            array[i][j] = (i*i + j*j) % NORMALIZATION_VALUE;
}


// Fonction pour sauvegarder le array en image (B&W)
void SaveArrayAsImage(int ** arr, int width, int height, string file_name) {
    // Initialise l'image 
    ofstream image(file_name);
    // Écrit l'en-tête
    image << "P2\n";
    image << width << " " << height << endl;
    image << NORMALIZATION_VALUE << endl;
    // Écrit les valeurs de chaque pixel
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) image << arr[i][j] << " ";
        image << endl;
    }
}

// Fonction pour sauvegarder le array en image (RGB)
void SaveArrayAsImage(int *** arr, int depth, int width, int height, string file_name) {
    // Initialise l'image
    ofstream image(file_name);
    // Écrit l'en-tête
    image << "P3\n";
    image << width << " " << height << endl;
    image << NORMALIZATION_VALUE << endl;
    // Écrit les valeurs de chaque pixel (R G B)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
            for (int d = 0; d < depth; d++)     
                image << arr[d][i][j] << " ";
        image << endl;
    }
}

// Fonction pour sauvegarder l'image en greyscale
void SaveArrayAsGreyImage(int *** arr, int depth, int width, int height, string file_name) {
    // Initialise l'image
    ofstream image(file_name);
    // Écrit l'en-tête
    image << "P2\n";
    image << width << " " << height << endl;
    image << NORMALIZATION_VALUE << endl;
    // Variable pour obtenir la moyenne des couleurs
    int grey = 0;
    // Écrit les valeurs de chaque pixel (R G B)
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int d = 0; d < depth; d++) grey = grey + arr[d][i][j];
            grey = grey / depth;
            image << grey  << " ";
            grey = 0;
        }
        image << endl;
    }
}