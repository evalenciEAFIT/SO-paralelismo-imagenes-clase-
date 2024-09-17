#ifndef PROCESADOR_IMAGEN_H
#define PROCESADOR_IMAGEN_H

#include "Archivo_jpeg.h"

class ProcesadorImagen {
public:
    static void invertirColores(Archivo_jpeg& archivo);
    static void invertirColoresOpenMP(Archivo_jpeg& archivo);
    static void binarizarImagen(Archivo_jpeg& archivo);
    static void binarizarImagenOpenMP(Archivo_jpeg& archivo);
    static void espejoImagen(Archivo_jpeg& archivo);
    static void espejoImagenOpenMP(Archivo_jpeg& archivo);
    static void aplicarKernelConvolucion(Archivo_jpeg& archivo);
    static void aplicarKernelConvolucionOpenMP(Archivo_jpeg& archivo);
    static void rotarImagen(Archivo_jpeg& archivo, double angulo);
    static void rotarImagenOpenMP(Archivo_jpeg& archivo, double angulo);

private:
    static void guardarTiempoProcesamiento(const std::string& nombreFuncion, double tiempo);
};

#endif // PROCESADOR_IMAGEN_H