# CC4102-Logaritmos

## Índice
- [Descripción](#descripción)
- [Ejecución](#ejecución)
- [Estructura del Proyecto](#estructura-del-proyecto)


## Descripción
El objetivo principal de este estudio es analizar y cuantificar las diferencias de desempeño entre árboles B y B+ en escenarios reales. Para ello, se desarrollaron en el lenguaje C algoritmos de inserción y búsqueda por rango que cumplan con las reglas del modelo de memoria externa. 

Para las pruebas se utilizaron datos reales de temperatura de la Estación Quinta Normal, en Santiago, entre enero de 2019 y julio de 2025. Estos datos contienen 67 millones de pares numéricos, conformados por una llave (representando el segundo en que se tomó la medición de temperatura) y un valor (representando la temperatura medida en el instante dado). Con esta información se hicieron inserciones de $N \in \{ 2^{15},...,2^{24} \}$ pares y búsquedas en rangos aleatorios de tamaño 604.800. 

## Ejecución

Primero se debe clonar el repositorio con: 

```bash
git clone https://github.com/FeliiC/CC4102-Logaritmos.git
```

En el archivo experimeto.c se encuentra la función main para ejecutar el experimento. Previamente es necesario descargar el archivo *datos.bin* desde el siguiente [enlace](https://github.com/claugaete/tarea1-cc4102-2025-2/releases/tag/Datos) y guardarlo en la carpeta datos_bin para poder acceder a los datos llave-valor. Además, es necesario crear los archivo binarios vacíos *arbolB.bin* y *arbolBPlus.bin* en la carpeta datos_bin para guardar y luego acceder a los árboles en disco.

Una vez hecho esto se puede compilar y ejecutar el experimeto con los comandos:

```bash
cd Tarea_1
cd src
gcc -o experimento experimento.c
./experimento.exe
```

## Estructura del proyecto
```bash
CC4102-LOGARITMOS/                          # Carpeta principal
├── .vscode/                                # Ambiente VS Code
├── Tarea_1/                                # Carpeta del proyecto
│   ├── datos_bin/                          # Archivos binarios
│   │   ├── arbolB.bin                      # Arbol B en disco
│   │   ├── arbolBPlus.bin                  # Arbol B+ en disco
│   │   ├── datos.bin                       # Pares llave-valor
│   │   └── datos_sample.txt                # Ejemplo de pares llave-valor
│   ├── src/                                # Archivos principales
│   │    ├──estructuras/                    # Estructuras del proyecto
│   │    │  └── nodo.c                      # Estructura de par y nodo
│   │    └──operaciones/                    # Operaciones del proyecto
│   │       ├── buscar.c                    # Busqueda en arbol
│   │       ├── crear_arbol.c               # Inicializacion arbol
│   │       ├── insertar.c                  # Insertar en arbol
│   │       ├── leer_pares.c                # Leer pares desde .bin
│   │       ├── split.c                     # Split de un nodo
│   │       └── stats.c                     # Calculo de IOs de operaciones
│   └── experimento.c                       # Experimento
├── .gitignore                              # Archivos ignorados por Git
└── README.md                               # Documentación del proyecto
```