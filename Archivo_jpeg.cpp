#include "Archivo_jpeg.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <cmath>

namespace fs = std::filesystem;

Archivo_jpeg::Archivo_jpeg(const std::string& nombre) : nombreArchivo(nombre), ancho(0), alto(0), numComponentes(0) {}

Archivo_jpeg::Archivo_jpeg(const Archivo_jpeg& otro)
    : nombreArchivo(otro.nombreArchivo),
      ancho(otro.ancho),
      alto(otro.alto),
      numComponentes(otro.numComponentes),
      datosImagen(otro.datosImagen),
      informacionCabecera(otro.informacionCabecera) {}

Archivo_jpeg::~Archivo_jpeg() {}

bool Archivo_jpeg::abrirArchivo() {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE* infile;

    if ((infile = fopen(nombreArchivo.c_str(), "rb")) == NULL) {
        std::cerr << "No se puede abrir el archivo: " << nombreArchivo << std::endl;
        return false;
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);

    std::ostringstream oss;
    oss << "Ancho: " << cinfo.image_width << "\n";
    oss << "Alto: " << cinfo.image_height << "\n";
    oss << "Componentes de color: " << cinfo.num_components << "\n";
    oss << "Precisión de color: " << cinfo.data_precision << " bits\n";
    informacionCabecera = oss.str();

    jpeg_start_decompress(&cinfo);

    ancho = cinfo.output_width;
    alto = cinfo.output_height;
    numComponentes = cinfo.output_components;

    datosImagen.resize(ancho * alto * numComponentes);

    while (cinfo.output_scanline < cinfo.output_height) {
        unsigned char* buffer_array[1];
        buffer_array[0] = &datosImagen[cinfo.output_scanline * ancho * numComponentes];
        jpeg_read_scanlines(&cinfo, buffer_array, 1);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);

    std::cout << "Imagen cargada: " << ancho << "x" << alto << " con " << numComponentes << " componentes." << std::endl;
    return true;
}

void Archivo_jpeg::crearDirectorioSiNoExiste(const std::string& ruta) const {
    fs::path dirPath(ruta);
    if (!fs::exists(dirPath)) {
        fs::create_directory(dirPath);
    }
}

bool Archivo_jpeg::guardarImagen(const std::string& funcion, bool esOpenMP) {
    std::string nombreBase = obtenerNombreSinExtension();
    std::string sufijo = esOpenMP ? "_OpenMP" : "";
    std::string carpetaResultado = "img_resultado";
    crearDirectorioSiNoExiste(carpetaResultado);
    
    std::string nuevoNombre = carpetaResultado + "/" + nombreBase + "_" + funcion + sufijo + ".jpg";

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE* outfile;

    if ((outfile = fopen(nuevoNombre.c_str(), "wb")) == NULL) {
        std::cerr << "No se puede crear el archivo: " << nuevoNombre << std::endl;
        return false;
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = ancho;
    cinfo.image_height = alto;
    cinfo.input_components = numComponentes;
    cinfo.in_color_space = numComponentes == 3 ? JCS_RGB : JCS_GRAYSCALE;

    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, 90, TRUE);
    jpeg_start_compress(&cinfo, TRUE);

    while (cinfo.next_scanline < cinfo.image_height) {
        JSAMPROW row_pointer[1];
        row_pointer[0] = &datosImagen[cinfo.next_scanline * ancho * numComponentes];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&cinfo);

    std::cout << "Imagen guardada como: " << nuevoNombre << std::endl;
    return true;
}

std::vector<unsigned char>& Archivo_jpeg::obtenerDatosImagen() {
    return datosImagen;
}

const std::vector<unsigned char>& Archivo_jpeg::obtenerDatosImagen() const {
    return datosImagen;
}

void Archivo_jpeg::actualizarDatosImagen(const std::vector<unsigned char>& nuevosDatos) {
    datosImagen = nuevosDatos;
    // Actualizar las dimensiones si es necesario
    if (datosImagen.size() != static_cast<size_t>(ancho * alto * numComponentes)) {
        int nuevoTamanio = datosImagen.size() / numComponentes;
        int nuevoAncho = static_cast<int>(std::sqrt(nuevoTamanio));
        int nuevoAlto = nuevoTamanio / nuevoAncho;
        redimensionar(nuevoAncho, nuevoAlto);
    }
}

void Archivo_jpeg::redimensionar(int nuevoAncho, int nuevoAlto) {
    if (nuevoAncho <= 0 || nuevoAlto <= 0) {
        throw std::invalid_argument("Las nuevas dimensiones deben ser positivas");
    }
    ancho = nuevoAncho;
    alto = nuevoAlto;
    // No es necesario redimensionar datosImagen aquí, ya que se hace en actualizarDatosImagen
}

std::string Archivo_jpeg::verDatosCabecera() const {
    return informacionCabecera;
}

std::vector<std::vector<std::vector<int>>> Archivo_jpeg::verSegmentoMatrizPixeles(int x1, int y1, int x2, int y2) const {
    if (x1 < 0 || y1 < 0 || x2 >= ancho || y2 >= alto || x1 > x2 || y1 > y2) {
        throw std::out_of_range("Coordenadas fuera de rango");
    }

    std::vector<std::vector<std::vector<int>>> segmento;
    for (int y = y1; y <= y2; ++y) {
        std::vector<std::vector<int>> fila;
        for (int x = x1; x <= x2; ++x) {
            std::vector<int> pixel;
            for (int c = 0; c < numComponentes; ++c) {
                int indice = (y * ancho + x) * numComponentes + c;
                pixel.push_back(static_cast<int>(datosImagen[indice]));
            }
            fila.push_back(pixel);
        }
        segmento.push_back(fila);
    }

    return segmento;
}

std::string Archivo_jpeg::obtenerNombreSinExtension() const {
    size_t lastdot = nombreArchivo.find_last_of(".");
    if (lastdot == std::string::npos) return nombreArchivo;
    return nombreArchivo.substr(0, lastdot);
}