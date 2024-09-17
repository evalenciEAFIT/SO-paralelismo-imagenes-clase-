#include "ProcesadorImagen.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <omp.h>

void ProcesadorImagen::invertirColores(Archivo_jpeg& archivo) {
    auto inicio = std::chrono::high_resolution_clock::now();
    
    std::cout << "Invirtiendo colores de la imagen: " << archivo.obtenerNombreArchivo() << std::endl;
    auto& datosImagen = archivo.obtenerDatosImagen();
    
    for (auto& pixel : datosImagen) {
        pixel = 255 - pixel;
    }
    
    archivo.guardarImagen("InvertirColores", false);
    
    auto fin = std::chrono::high_resolution_clock::now();
    double tiempo = std::chrono::duration<double, std::milli>(fin - inicio).count();
    guardarTiempoProcesamiento("invertirColores", tiempo);
    std::cout << "Tiempo de procesamiento (invertirColores): " << tiempo << " ms" << std::endl;
}

void ProcesadorImagen::invertirColoresOpenMP(Archivo_jpeg& archivo) {
    auto inicio = std::chrono::high_resolution_clock::now();
    
    std::cout << "Invirtiendo colores de la imagen (OpenMP): " << archivo.obtenerNombreArchivo() << std::endl;
    auto& datosImagen = archivo.obtenerDatosImagen();
    
    #pragma omp parallel for
    for (size_t i = 0; i < datosImagen.size(); ++i) {
        datosImagen[i] = 255 - datosImagen[i];
    }
    
    archivo.guardarImagen("InvertirColores", true);
    
    auto fin = std::chrono::high_resolution_clock::now();
    double tiempo = std::chrono::duration<double, std::milli>(fin - inicio).count();
    guardarTiempoProcesamiento("invertirColoresOpenMP", tiempo);
    std::cout << "Tiempo de procesamiento (invertirColoresOpenMP): " << tiempo << " ms" << std::endl;
}

void ProcesadorImagen::binarizarImagen(Archivo_jpeg& archivo) {
    auto inicio = std::chrono::high_resolution_clock::now();
    
    std::cout << "Binarizando la imagen: " << archivo.obtenerNombreArchivo() << std::endl;
    auto& datosImagen = archivo.obtenerDatosImagen();
    int ancho = archivo.obtenerAncho();
    int alto = archivo.obtenerAlto();
    int numComponentes = archivo.obtenerNumComponentes();
    
    for (int y = 0; y < alto; ++y) {
        for (int x = 0; x < ancho; ++x) {
            int promedio = 0;
            for (int c = 0; c < numComponentes; ++c) {
                promedio += datosImagen[(y * ancho + x) * numComponentes + c];
            }
            promedio /= numComponentes;
            
            unsigned char valor = (promedio > 128) ? 255 : 0;
            for (int c = 0; c < numComponentes; ++c) {
                datosImagen[(y * ancho + x) * numComponentes + c] = valor;
            }
        }
    }
    
    archivo.guardarImagen("BinarizarImagen", false);
    
    auto fin = std::chrono::high_resolution_clock::now();
    double tiempo = std::chrono::duration<double, std::milli>(fin - inicio).count();
    guardarTiempoProcesamiento("binarizarImagen", tiempo);
    std::cout << "Tiempo de procesamiento (binarizarImagen): " << tiempo << " ms" << std::endl;
}

void ProcesadorImagen::binarizarImagenOpenMP(Archivo_jpeg& archivo) {
    auto inicio = std::chrono::high_resolution_clock::now();
    
    std::cout << "Binarizando la imagen (OpenMP): " << archivo.obtenerNombreArchivo() << std::endl;
    auto& datosImagen = archivo.obtenerDatosImagen();
    int ancho = archivo.obtenerAncho();
    int alto = archivo.obtenerAlto();
    int numComponentes = archivo.obtenerNumComponentes();
    
    #pragma omp parallel for collapse(2)
    for (int y = 0; y < alto; ++y) {
        for (int x = 0; x < ancho; ++x) {
            int promedio = 0;
            for (int c = 0; c < numComponentes; ++c) {
                promedio += datosImagen[(y * ancho + x) * numComponentes + c];
            }
            promedio /= numComponentes;
            
            unsigned char valor = (promedio > 128) ? 255 : 0;
            for (int c = 0; c < numComponentes; ++c) {
                datosImagen[(y * ancho + x) * numComponentes + c] = valor;
            }
        }
    }
    
    archivo.guardarImagen("BinarizarImagen", true);
    
    auto fin = std::chrono::high_resolution_clock::now();
    double tiempo = std::chrono::duration<double, std::milli>(fin - inicio).count();
    guardarTiempoProcesamiento("binarizarImagenOpenMP", tiempo);
    std::cout << "Tiempo de procesamiento (binarizarImagenOpenMP): " << tiempo << " ms" << std::endl;
}

void ProcesadorImagen::espejoImagen(Archivo_jpeg& archivo) {
    auto inicio = std::chrono::high_resolution_clock::now();
    
    std::cout << "Aplicando efecto espejo a la imagen: " << archivo.obtenerNombreArchivo() << std::endl;
    auto& datosImagen = archivo.obtenerDatosImagen();
    int ancho = archivo.obtenerAncho();
    int alto = archivo.obtenerAlto();
    int numComponentes = archivo.obtenerNumComponentes();
    
    for (int y = 0; y < alto; ++y) {
        for (int x = 0; x < ancho / 2; ++x) {
            for (int c = 0; c < numComponentes; ++c) {
                std::swap(datosImagen[(y * ancho + x) * numComponentes + c],
                          datosImagen[(y * ancho + (ancho - 1 - x)) * numComponentes + c]);
            }
        }
    }
    
    archivo.guardarImagen("EspejoImagen", false);
    
    auto fin = std::chrono::high_resolution_clock::now();
    double tiempo = std::chrono::duration<double, std::milli>(fin - inicio).count();
    guardarTiempoProcesamiento("espejoImagen", tiempo);
    std::cout << "Tiempo de procesamiento (espejoImagen): " << tiempo << " ms" << std::endl;
}

void ProcesadorImagen::espejoImagenOpenMP(Archivo_jpeg& archivo) {
    auto inicio = std::chrono::high_resolution_clock::now();
    
    std::cout << "Aplicando efecto espejo a la imagen (OpenMP): " << archivo.obtenerNombreArchivo() << std::endl;
    auto& datosImagen = archivo.obtenerDatosImagen();
    int ancho = archivo.obtenerAncho();
    int alto = archivo.obtenerAlto();
    int numComponentes = archivo.obtenerNumComponentes();
    
    #pragma omp parallel for
    for (int y = 0; y < alto; ++y) {
        for (int x = 0; x < ancho / 2; ++x) {
            for (int c = 0; c < numComponentes; ++c) {
                std::swap(datosImagen[(y * ancho + x) * numComponentes + c],
                          datosImagen[(y * ancho + (ancho - 1 - x)) * numComponentes + c]);
            }
        }
    }
    
    archivo.guardarImagen("EspejoImagen", true);
    
    auto fin = std::chrono::high_resolution_clock::now();
    double tiempo = std::chrono::duration<double, std::milli>(fin - inicio).count();
    guardarTiempoProcesamiento("espejoImagenOpenMP", tiempo);
    std::cout << "Tiempo de procesamiento (espejoImagenOpenMP): " << tiempo << " ms" << std::endl;
}

void ProcesadorImagen::aplicarKernelConvolucion(Archivo_jpeg& archivo) {
    auto inicio = std::chrono::high_resolution_clock::now();
    
    std::cout << "Aplicando kernel de convolución a la imagen: " << archivo.obtenerNombreArchivo() << std::endl;
    auto& datosImagen = archivo.obtenerDatosImagen();
    int ancho = archivo.obtenerAncho();
    int alto = archivo.obtenerAlto();
    int numComponentes = archivo.obtenerNumComponentes();
    
    // Kernel de ejemplo (detección de bordes)
    const int kernel[3][3] = {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    };
    
    std::vector<unsigned char> nuevaImagen(datosImagen.size());
    
    for (int y = 1; y < alto - 1; ++y) {
        for (int x = 1; x < ancho - 1; ++x) {
            for (int c = 0; c < numComponentes; ++c) {
                int suma = 0;
                for (int ky = -1; ky <= 1; ++ky) {
                    for (int kx = -1; kx <= 1; ++kx) {
                        int indice = ((y + ky) * ancho + (x + kx)) * numComponentes + c;
                        suma += datosImagen[indice] * kernel[ky + 1][kx + 1];
                    }
                }
                suma = std::max(0, std::min(suma, 255));
                nuevaImagen[(y * ancho + x) * numComponentes + c] = static_cast<unsigned char>(suma);
            }
        }
    }
    
    archivo.actualizarDatosImagen(nuevaImagen);
    archivo.guardarImagen("KernelConvolucion", false);
    
    auto fin = std::chrono::high_resolution_clock::now();
    double tiempo = std::chrono::duration<double, std::milli>(fin - inicio).count();
    guardarTiempoProcesamiento("aplicarKernelConvolucion", tiempo);
    std::cout << "Tiempo de procesamiento (aplicarKernelConvolucion): " << tiempo << " ms" << std::endl;
}

void ProcesadorImagen::aplicarKernelConvolucionOpenMP(Archivo_jpeg& archivo) {
    auto inicio = std::chrono::high_resolution_clock::now();
    
    std::cout << "Aplicando kernel de convolución a la imagen (OpenMP): " << archivo.obtenerNombreArchivo() << std::endl;
    auto& datosImagen = archivo.obtenerDatosImagen();
    int ancho = archivo.obtenerAncho();
    int alto = archivo.obtenerAlto();
    int numComponentes = archivo.obtenerNumComponentes();
    
    // Kernel de ejemplo (detección de bordes)
    const int kernel[3][3] = {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    };
    
    std::vector<unsigned char> nuevaImagen(datosImagen.size());
    
    #pragma omp parallel for collapse(2)
    for (int y = 1; y < alto - 1; ++y) {
        for (int x = 1; x < ancho - 1; ++x) {
            for (int c = 0; c < numComponentes; ++c) {
                int suma = 0;
                for (int ky = -1; ky <= 1; ++ky) {
                    for (int kx = -1; kx <= 1; ++kx) {
                        int indice = ((y + ky) * ancho + (x + kx)) * numComponentes + c;
                        suma += datosImagen[indice] * kernel[ky + 1][kx + 1];
                    }
                }
                suma = std::max(0, std::min(suma, 255));
                nuevaImagen[(y * ancho + x) * numComponentes + c] = static_cast<unsigned char>(suma);
            }
        }
    }
    
    archivo.actualizarDatosImagen(nuevaImagen);
    archivo.guardarImagen("KernelConvolucion", true);
    
    auto fin = std::chrono::high_resolution_clock::now();
    double tiempo = std::chrono::duration<double, std::milli>(fin - inicio).count();
    guardarTiempoProcesamiento("aplicarKernelConvolucionOpenMP", tiempo);
    std::cout << "Tiempo de procesamiento (aplicarKernelConvolucionOpenMP): " << tiempo << " ms" << std::endl;
}

void ProcesadorImagen::rotarImagen(Archivo_jpeg& archivo, double angulo) {
    auto inicio = std::chrono::high_resolution_clock::now();
    
    std::cout << "Rotando la imagen: " << archivo.obtenerNombreArchivo() << " " << angulo << " grados" << std::endl;
    const auto& datosOriginales = archivo.obtenerDatosImagen();
    int ancho = archivo.obtenerAncho();
    int alto = archivo.obtenerAlto();
    int numComponentes = archivo.obtenerNumComponentes();

    double radianes = angulo * M_PI / 180.0;
    double coseno = std::cos(radianes);
    double seno = std::sin(radianes);

    // Calcular el tamaño del cuadrado envolvente
    int maxDimension = std::max(ancho, alto);
    int nuevoDimension = static_cast<int>(std::ceil(maxDimension * std::sqrt(2.0)));

    std::vector<unsigned char> datosRotados(nuevoDimension * nuevoDimension * numComponentes, 0);

    int centroXOriginal = ancho / 2;
    int centroYOriginal = alto / 2;
    int centroNuevo = nuevoDimension / 2;

    for (int y = 0; y < nuevoDimension; ++y) {
        for (int x = 0; x < nuevoDimension; ++x) {
            // Calcular la posición original antes de la rotación
            int xOriginal = static_cast<int>((x - centroNuevo) * coseno + (y - centroNuevo) * seno + centroXOriginal);
            int yOriginal = static_cast<int>(-(x - centroNuevo) * seno + (y - centroNuevo) * coseno + centroYOriginal);

            if (xOriginal >= 0 && xOriginal < ancho && yOriginal >= 0 && yOriginal < alto) {
                for (int c = 0; c < numComponentes; ++c) {
                    datosRotados[(y * nuevoDimension + x) * numComponentes + c] = 
                        datosOriginales[(yOriginal * ancho + xOriginal) * numComponentes + c];
                }
            }
        }
    }

    Archivo_jpeg archivoRotado = archivo;
    archivoRotado.actualizarDatosImagen(datosRotados);
    archivoRotado.guardarImagen("Rotacion_" + std::to_string(static_cast<int>(angulo)), false);
    
    auto fin = std::chrono::high_resolution_clock::now();
    double tiempo = std::chrono::duration<double, std::milli>(fin - inicio).count();
    guardarTiempoProcesamiento("rotarImagen", tiempo);
    std::cout << "Tiempo de procesamiento (rotarImagen): " << tiempo << " ms" << std::endl;
}

void ProcesadorImagen::rotarImagenOpenMP(Archivo_jpeg& archivo, double angulo) {
    auto inicio = std::chrono::high_resolution_clock::now();
    
    std::cout << "Rotando la imagen (OpenMP): " << archivo.obtenerNombreArchivo() << " " << angulo << " grados" << std::endl;
    const auto& datosOriginales = archivo.obtenerDatosImagen();
    int ancho = archivo.obtenerAncho();
    int alto = archivo.obtenerAlto();
    int numComponentes = archivo.obtenerNumComponentes();

    double radianes = angulo * M_PI / 180.0;
    double coseno = std::cos(radianes);
    double seno = std::sin(radianes);

    // Calcular el tamaño del cuadrado envolvente
    int maxDimension = std::max(ancho, alto);
    int nuevoDimension = static_cast<int>(std::ceil(maxDimension * std::sqrt(2.0)));

    std::vector<unsigned char> datosRotados(nuevoDimension * nuevoDimension * numComponentes, 0);

    int centroXOriginal = ancho / 2;
    int centroYOriginal = alto / 2;
    int centroNuevo = nuevoDimension / 2;

    /*
    ¿Cuándo usar collapse(2)?
      - Bucles anidados: Es ideal cuando tienes dos bucles for anidados y quieres paralelizar ambos niveles para aprovechar al máximo los recursos de 
        cómputo disponibles.
      - Mejor distribución de trabajo: Al combinar los bucles, puedes lograr una distribución más equilibrada de las iteraciones entre los hilos, lo que 
        puede mejorar el rendimiento en algunos casos.
      - Reducción de overhead: En ocasiones, paralelizar bucles individuales puede generar un cierto overhead. collapse(2) puede ayudar a 
        reducir este overhead al combinar los bucles.
    */

    #pragma omp parallel for collapse(2)
    for (int y = 0; y < nuevoDimension; ++y) {
        for (int x = 0; x < nuevoDimension; ++x) {
            // Calcular la posición original antes de la rotación
            int xOriginal = static_cast<int>((x - centroNuevo) * coseno + (y - centroNuevo) * seno + centroXOriginal);
            int yOriginal = static_cast<int>(-(x - centroNuevo) * seno + (y - centroNuevo) * coseno + centroYOriginal);

            if (xOriginal >= 0 && xOriginal < ancho && yOriginal >= 0 && yOriginal < alto) {
                for (int c = 0; c < numComponentes; ++c) {
                    datosRotados[(y * nuevoDimension + x) * numComponentes + c] = 
                        datosOriginales[(yOriginal * ancho + xOriginal) * numComponentes + c];
                }
            }
        }
    }

    Archivo_jpeg archivoRotado = archivo;
    archivoRotado.actualizarDatosImagen(datosRotados);
    archivoRotado.guardarImagen("Rotacion_" + std::to_string(static_cast<int>(angulo)), true);
    
    auto fin = std::chrono::high_resolution_clock::now();
    double tiempo = std::chrono::duration<double, std::milli>(fin - inicio).count();
    guardarTiempoProcesamiento("rotarImagenOpenMP", tiempo);
    std::cout << "Tiempo de procesamiento (rotarImagenOpenMP): " << tiempo << " ms" << std::endl;
}

void ProcesadorImagen::guardarTiempoProcesamiento(const std::string& nombreFuncion, double tiempo) {
    std::ofstream archivo("tiempos_procesamiento.txt", std::ios::app);
    if (archivo.is_open()) {
        archivo << nombreFuncion << ": " << tiempo << " ms" << std::endl;
        archivo.close();
    } else {
        std::cerr << "No se pudo abrir el archivo para guardar los tiempos de procesamiento." << std::endl;
    }
}