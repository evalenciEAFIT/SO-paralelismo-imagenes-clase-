# Compilador
CXX = g++

# Flags de compilación
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -fopenmp

# Flags de enlazado
LDFLAGS = -fopenmp

# Librerías
LIBS = -ljpeg

# Nombre del ejecutable
TARGET = procesador_imagen

# Archivos fuente
SOURCES = main.cpp Archivo_jpeg.cpp ProcesadorImagen.cpp

# Archivos objeto generados a partir de los fuentes
OBJECTS = $(SOURCES:.cpp=.o)

# Regla por defecto
all: $(TARGET)

# Regla para compilar el ejecutable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

# Regla para compilar los archivos objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados
clean:
	rm -f $(OBJECTS) $(TARGET)

# Regla para ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Regla para recompilar todo el proyecto
rebuild: clean all

# Phony targets
.PHONY: all clean run rebuild