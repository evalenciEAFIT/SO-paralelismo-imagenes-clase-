#include "Archivo_jpeg.h"
#include "ProcesadorImagen.h"
#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <functional>

void mostrarMenu() {
    std::cout << "\nMenú de opciones:" << std::endl;
    std::cout << "1. Invertir colores" << std::endl;
    std::cout << "2. Binarizar imagen" << std::endl;
    std::cout << "3. Aplicar efecto espejo" << std::endl;
    std::cout << "4. Aplicar kernel de convolución" << std::endl;
    std::cout << "5. Rotar imagen" << std::endl;
    std::cout << "0. Salir" << std::endl;
    std::cout << "Seleccione una opción: ";
}

void ejecutarYCompararFunciones(const std::string& nombreFuncion, 
                                std::function<void(Archivo_jpeg&)> funcNormal,
                                std::function<void(Archivo_jpeg&)> funcOpenMP,
                                Archivo_jpeg& archivo) {
    std::cout << "\nEjecutando " << nombreFuncion << "..." << std::endl;

    Archivo_jpeg archivoNormal = archivo;
    Archivo_jpeg archivoOpenMP = archivo;

    auto inicioNormal = std::chrono::high_resolution_clock::now();
    funcNormal(archivoNormal);
    auto finNormal = std::chrono::high_resolution_clock::now();
    double tiempoNormal = std::chrono::duration<double, std::milli>(finNormal - inicioNormal).count();

    auto inicioOpenMP = std::chrono::high_resolution_clock::now();
    funcOpenMP(archivoOpenMP);
    auto finOpenMP = std::chrono::high_resolution_clock::now();
    double tiempoOpenMP = std::chrono::duration<double, std::milli>(finOpenMP - inicioOpenMP).count();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Tiempo de ejecución normal: " << tiempoNormal << " ms" << std::endl;
    std::cout << "Tiempo de ejecución con OpenMP: " << tiempoOpenMP << " ms" << std::endl;
    double aceleracion = tiempoNormal / tiempoOpenMP;
    std::cout << "Aceleración: " << aceleracion << "x" << std::endl;
    std::cout << "Las imágenes resultantes se han guardado en la carpeta 'img_resultado'" << std::endl;
}

int main() {
    std::string nombreArchivo;
    std::cout << "Ingrese el nombre del archivo JPEG: ";
    std::cin >> nombreArchivo;

    Archivo_jpeg archivo(nombreArchivo);
    if (!archivo.abrirArchivo()) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return 1;
    }

    int opcion;
    do {
        mostrarMenu();
        std::cin >> opcion;

        switch(opcion) {
            case 1:
                ejecutarYCompararFunciones("InvertirColores", 
                    ProcesadorImagen::invertirColores, 
                    ProcesadorImagen::invertirColoresOpenMP, 
                    archivo);
                break;
            case 2:
                ejecutarYCompararFunciones("BinarizarImagen", 
                    ProcesadorImagen::binarizarImagen, 
                    ProcesadorImagen::binarizarImagenOpenMP, 
                    archivo);
                break;
            case 3:
                ejecutarYCompararFunciones("EspejoImagen", 
                    ProcesadorImagen::espejoImagen, 
                    ProcesadorImagen::espejoImagenOpenMP, 
                    archivo);
                break;
            case 4:
                ejecutarYCompararFunciones("KernelConvolucion", 
                    ProcesadorImagen::aplicarKernelConvolucion, 
                    ProcesadorImagen::aplicarKernelConvolucionOpenMP, 
                    archivo);
                break;
            case 5: {
                double angulo;
                std::cout << "Ingrese el ángulo de rotación en grados: ";
                std::cin >> angulo;
                ejecutarYCompararFunciones("Rotacion_" + std::to_string(static_cast<int>(angulo)),
                    [angulo](Archivo_jpeg& img) { ProcesadorImagen::rotarImagen(img, angulo); },
                    [angulo](Archivo_jpeg& img) { ProcesadorImagen::rotarImagenOpenMP(img, angulo); },
                    archivo);
                break;
            }
            case 0:
                std::cout << "Saliendo del programa..." << std::endl;
                break;
            default:
                std::cout << "Opción no válida. Intente de nuevo." << std::endl;
        }
    } while (opcion != 0);

    return 0;
}