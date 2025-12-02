#include <stdio.h>
#include <string.h>
#include <ctype.h> // Necesario para validar nombres (isalpha)

// Definición de constantes del sistema
#define MAX_PROD 5        // Cantidad de productos (Requerimiento: 5 productos)
#define LONG_NOMBRE 50    // Longitud máxima para nombres

// Prototipos de funciones (Programación Modular)
// Se usan punteros (*tiempos, *recursos) para modificar los arreglos originales
void ingresarProductos(char nombres[][LONG_NOMBRE], float *tiempos, float *recursos, int *demandas);
void mostrarInventario(char nombres[][LONG_NOMBRE], float *tiempos, float *recursos, int *demandas);
int buscarProducto(char nombres[][LONG_NOMBRE], char *nombreBuscado); // Búsqueda manual
void editarProducto(char nombres[][LONG_NOMBRE], float *tiempos, float *recursos, int *demandas);
void eliminarProducto(char nombres[][LONG_NOMBRE], float *tiempos, float *recursos, int *demandas);
void calcularViabilidad(float *tiempos, float *recursos, int *demandas);

// Función Auxiliar de Validación de Texto
// Retorna 1 si el texto solo contiene letras y espacios, 0 si contiene números o símbolos
int esNombreValido(char *nombre) {
    if (strlen(nombre) == 0) return 0; // No aceptar vacíos
    for (int i = 0; i < strlen(nombre); i++) {
        if (!isalpha(nombre[i]) && !isspace(nombre[i])) {
            return 0; // Encontró un número o símbolo inválido
        }
    }
    return 1;
}

int main() {
    // Implementación de ARREGLOS PARALELOS
    char nombres[MAX_PROD][LONG_NOMBRE];
    float tiempos[MAX_PROD] = {0};   
    float recursos[MAX_PROD] = {0};  
    int demandas[MAX_PROD] = {0};    

    // Inicialización para evitar basura en memoria
    for(int i=0; i<MAX_PROD; i++) strcpy(nombres[i], "");

    int opcion;
    int datosCargados = 0; // Bandera de control

    do {
        printf("\n--- SISTEMA DE PRODUCCION DE COMPONENTES ---\n");
        printf("1. Ingresar Datos (%d Productos)\n", MAX_PROD);
        printf("2. Mostrar Inventario Actual\n");
        printf("3. Editar un Producto\n");
        printf("4. Eliminar un Producto\n");
        printf("5. Calcular Viabilidad (Tiempo y Recursos)\n");
        printf("6. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar(); // Limpieza crítica del buffer

        switch (opcion) {
            case 1:
                ingresarProductos(nombres, tiempos, recursos, demandas);
                datosCargados = 1;
                break;
            case 2:
                if(datosCargados) mostrarInventario(nombres, tiempos, recursos, demandas);
                else printf("[!] Error: Primero debe cargar los productos (Opcion 1).\n");
                break;
            case 3:
                if(datosCargados) editarProducto(nombres, tiempos, recursos, demandas);
                else printf("[!] Error: No hay datos para editar.\n");
                break;
            case 4:
                if(datosCargados) eliminarProducto(nombres, tiempos, recursos, demandas);
                else printf("[!] Error: No hay datos para eliminar.\n");
                break;
            case 5:
                if(datosCargados) calcularViabilidad(tiempos, recursos, demandas);
                else printf("[!] Error: Faltan datos para el calculo.\n");
                break;
            case 6:
                printf("Cerrando sistema...\n");
                break;
            default:
                printf("Opcion no valida.\n");
        }
    } while (opcion != 6);

    return 0;
}

// 1. Función de Ingreso con Validaciones Robustas
void ingresarProductos(char nombres[][LONG_NOMBRE], float *tiempos, float *recursos, int *demandas) {
    printf("\n--- INGRESO DE DATOS ---\n");
    for (int i = 0; i < MAX_PROD; i++) {
        printf("\n>>> Producto #%d:\n", i + 1);
        
        // Validación: Nombre sin números
        do {
            printf("Nombre del producto (Solo letras): ");
            fgets(nombres[i], LONG_NOMBRE, stdin);
            nombres[i][strcspn(nombres[i], "\n")] = 0; // Eliminar salto de línea
            
            if (!esNombreValido(nombres[i])) {
                printf("[!] Error: El nombre contiene caracteres invalidos.\n");
            }
        } while (!esNombreValido(nombres[i]));

        // Validación: Tiempo positivo
        do {
            printf("Tiempo de fabricacion unitario (horas): ");
            scanf("%f", &tiempos[i]);
            if (tiempos[i] < 0) printf("[!] Error: El tiempo no puede ser negativo.\n");
        } while (tiempos[i] < 0);
        
        // Validación: Recursos positivos
        do {
            printf("Cantidad de recursos unitarios: ");
            scanf("%f", &recursos[i]);
            if (recursos[i] < 0) printf("[!] Error: Los recursos no pueden ser negativos.\n");
        } while (recursos[i] < 0);

        // Validación: Demanda positiva
        do {
            printf("Demanda solicitada (cantidad): ");
            scanf("%d", &demandas[i]);
            if (demandas[i] < 0) printf("[!] Error: La demanda no puede ser negativa.\n");
        } while (demandas[i] < 0);

        getchar(); // Limpiar buffer para el siguiente ciclo
    }
}

// Función auxiliar para visualizar datos
void mostrarInventario(char nombres[][LONG_NOMBRE], float *tiempos, float *recursos, int *demandas) {
    printf("\n%-20s %-10s %-10s %-10s\n", "PRODUCTO", "TIEMPO/U", "RECURSO/U", "DEMANDA");
    printf("------------------------------------------------------------\n");
    for(int i=0; i<MAX_PROD; i++) {
        if(strlen(nombres[i]) > 0) { // Solo mostrar si no está eliminado
            printf("%-20s %-10.2f %-10.2f %-10d\n", nombres[i], tiempos[i], recursos[i], demandas[i]);
        }
    }
}

// Lógica de búsqueda manual (Lineal)
int buscarProducto(char nombres[][LONG_NOMBRE], char *nombreBuscado) {
    for (int i = 0; i < MAX_PROD; i++) {
        // strcmp retorna 0 si las cadenas son idénticas
        if (strcmp(nombres[i], nombreBuscado) == 0) {
            return i; // Retorna el índice encontrado
        }
    }
    return -1; // No encontrado
}

// 2. Función de Edición
void editarProducto(char nombres[][LONG_NOMBRE], float *tiempos, float *recursos, int *demandas) {
    char buscar[LONG_NOMBRE];
    printf("\nIngrese el nombre exacto del producto a editar: ");
    fgets(buscar, LONG_NOMBRE, stdin);
    buscar[strcspn(buscar, "\n")] = 0;

    int index = buscarProducto(nombres, buscar);

    if (index != -1) {
        printf("--- Editando %s ---\n", nombres[index]);
        
        // Se repiten las validaciones durante la edición
        do {
            printf("Nuevo Nombre: ");
            fgets(nombres[index], LONG_NOMBRE, stdin);
            nombres[index][strcspn(nombres[index], "\n")] = 0;
            if (!esNombreValido(nombres[index])) printf("[!] Error: Solo letras.\n");
        } while (!esNombreValido(nombres[index]));

        do {
            printf("Nuevo Tiempo (>0): ");
            scanf("%f", &tiempos[index]);
        } while(tiempos[index] < 0);

        do {
            printf("Nuevos Recursos (>0): ");
            scanf("%f", &recursos[index]);
        } while(recursos[index] < 0);

        do {
            printf("Nueva Demanda (>0): ");
            scanf("%d", &demandas[index]);
        } while(demandas[index] < 0);

        getchar();
        printf("[OK] Producto actualizado.\n");
    } else {
        printf("[!] Error: Producto no encontrado.\n");
    }
}

// 3. Función de Eliminación
void eliminarProducto(char nombres[][LONG_NOMBRE], float *tiempos, float *recursos, int *demandas) {
    char buscar[LONG_NOMBRE];
    printf("\nIngrese el nombre del producto a eliminar: ");
    fgets(buscar, LONG_NOMBRE, stdin);
    buscar[strcspn(buscar, "\n")] = 0;

    int index = buscarProducto(nombres, buscar);

    if (index != -1) {
        // Eliminación lógica: Limpiamos los datos
        strcpy(nombres[index], ""); 
        tiempos[index] = 0;
        recursos[index] = 0;
        demandas[index] = 0;
        printf("[OK] Producto eliminado del registro.\n");
    } else {
        printf("[!] Error: Producto no encontrado.\n");
    }
}

// 4. Cálculo de Viabilidad
void calcularViabilidad(float *tiempos, float *recursos, int *demandas) {
    float tiempoDisponible, recursosDisponibles;
    float tiempoTotalRequerido = 0;
    float recursosTotalRequeridos = 0;

    printf("\n--- ANALISIS DE VIABILIDAD ---\n");
    
    // Validar entradas de límites de fábrica
    do {
        printf("Ingrese el TIEMPO TOTAL disponible en fabrica: ");
        scanf("%f", &tiempoDisponible);
        if(tiempoDisponible < 0) printf("[!] No puede ser negativo.\n");
    } while (tiempoDisponible < 0);

    do {
        printf("Ingrese la CANTIDAD TOTAL de recursos disponibles: ");
        scanf("%f", &recursosDisponibles);
        if(recursosDisponibles < 0) printf("[!] No puede ser negativo.\n");
    } while (recursosDisponibles < 0);

    // Sumatoria total usando arreglos
    for (int i = 0; i < MAX_PROD; i++) {
        tiempoTotalRequerido += tiempos[i] * demandas[i];
        recursosTotalRequeridos += recursos[i] * demandas[i];
    }

    printf("\n--- RESULTADO DEL ANALISIS ---\n");
    printf("TIEMPO:   Requerido %.2f vs Disponible %.2f\n", tiempoTotalRequerido, tiempoDisponible);
    printf("RECURSOS: Requeridos %.2f vs Disponibles %.2f\n", recursosTotalRequeridos, recursosDisponibles);

    int esViable = 1;

    if (tiempoTotalRequerido > tiempoDisponible) {
        printf("=> [NO VIABLE] Falta Tiempo: %.2f horas adicionales necesarias.\n", 
               tiempoTotalRequerido - tiempoDisponible);
        esViable = 0;
    }
    
    if (recursosTotalRequeridos > recursosDisponibles) {
        printf("=> [NO VIABLE] Faltan Recursos: %.2f unidades adicionales necesarias.\n", 
               recursosTotalRequeridos - recursosDisponibles);
        esViable = 0;
    }

    if (esViable) {
        printf("=> [EXITO] La produccion es totalmente VIABLE con los recursos actuales.\n");
    }

}
