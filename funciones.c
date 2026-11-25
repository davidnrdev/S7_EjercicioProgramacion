#include<stdio.h>
#include<string.h>
#include "funciones.h"

float validarRangoFloat(float a, float b){

    float num;
    int val;

    do{
        val = scanf(" %f", &num);
        while((getchar()) != '\n');
        
        if (num < a || num > b || val != 1){
            printf("Error: el valor ingresado es incorrecto\n");
        }

    } while (num < a || num > b || val != 1);
    
    return num;
}

float validarRangoFloatMinimo(float a){

    float numMinimo;
    int val;

    do{
        val = scanf(" %f", &numMinimo);
        while((getchar()) != '\n');
        
        if (val != 1 || numMinimo < a){
            printf("Error: el valor ingresado es incorrecto\n");
        }
    } while (val != 1 || numMinimo < a);

    return numMinimo;  
}

int validarRangoInt(int a){
    int num;
    int val, valido;
    int c; //almacena los caracteres que lee el getchar

    do{
        val = scanf(" %d", &num);
        valido = (val == 1);
        while((c = getchar()) != '\n' && c != EOF){ //end of file (en caso de que no lea el salto del línea) medida preventiva
            valido = 0; 
        };

        if (num < a || !valido){
            printf("Error: el valor ingresado es incorrecto\n");
        }

        if(c == EOF && !valido){
            return -1;
        }

    } while (num < a || !valido);

    return num;
}

int validarRangoIntMaxMin(int a, int b){
    int num;
    int val, valido;
    int c;

    do
    {
        val = scanf(" %d", &num);
        valido = (val == 1);
        while((c = getchar()) != '\n' && c != EOF){
            valido = 0;
        };

        if (!valido || num < a || num > b){
            printf("Error: el valor ingresado es incorrecto\n");
        }

        if(c == EOF && !valido){
            return -1;
        }

    } while (!valido || num < a || num > b);

    return num;
}

void leerCadenas(char *cadena, int n){
    if (fgets(cadena, n, stdin) != NULL) {
        int len = strcspn(cadena, "\n");
        if (cadena[len] == '\n') {
            cadena[len] = '\0';
        } else {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }

        int inicio = 0;
        while (cadena[inicio] == ' ' || cadena[inicio] == '\t' || cadena[inicio] == '\r') {
            inicio++;
        }

        int fin = strlen(cadena);
        while (fin > inicio && (cadena[fin - 1] == ' ' || cadena[fin - 1] == '\t' || cadena[fin - 1] == '\r')) {
            fin--;
        }

        if (inicio > 0) {
            memmove(cadena, cadena + inicio, fin - inicio);
        }
        cadena[fin - inicio] = '\0';
    }
}

void administrarInventario(int *encontrar, int *modificar, char nombresInsumos[5][40], char unidadMedida[5][20], char proveedor[5][40], float stockMinimo[5], char tipoPresentacion[5][20], float cantidadInsumo[5], char caducidad[5][20], char insumosTratamientos[5][5][40]){
    int opcion;

    do{
        printf("Seleccione una opcion:\n");
        printf("1. Registrar un nuevo insumo\n");
        printf("2. Mostrar insumos\n");
        printf("3. Buscar un insumo\n");
        printf("4. Editar insumos\n");
        printf("5. Registrar lote\n");
        printf("6. Hacer una reposicion o quitar insumos\n");
        printf("7. Mostrar lote\n");
        printf("8. Editar lote\n");
        printf("9. Regresar al menu principal\n");
        printf(">> ");
        opcion = validarRangoIntMaxMin(1,9);

        switch (opcion){
        case 1:
            registrarInsumos(nombresInsumos, unidadMedida, proveedor, stockMinimo);
            break;
        case 2:
            mostrarInsumos(nombresInsumos, unidadMedida, proveedor, stockMinimo);
            break;
        case 3:
            buscarInsumo(nombresInsumos, unidadMedida, proveedor, stockMinimo, encontrar, modificar);
            break;
        case 4:
            buscarInsumo(nombresInsumos, unidadMedida, proveedor, stockMinimo, encontrar, modificar);
            if (*encontrar == 1){
                editarInsumo(nombresInsumos, unidadMedida, proveedor, stockMinimo, *modificar, insumosTratamientos);
            }
            else{
                printf("No se puede editar, el insumo no existe.\n");
            }
            break;
        case 5:
            buscarInsumo(nombresInsumos, unidadMedida, proveedor, stockMinimo, encontrar, modificar);
            if (*encontrar == 1){
                registrarLote(modificar, tipoPresentacion, nombresInsumos, cantidadInsumo, caducidad);
            }
            else{
                printf("No se puede registrar, el insumo no existe.\n");
            }
            break;
        case 6:
            buscarInsumo(nombresInsumos, unidadMedida, proveedor, stockMinimo, encontrar, modificar);
            if (*encontrar == 1){
                if (strlen(tipoPresentacion[*modificar]) == 0){
                    printf("Primero debe registrar una presentacion para este insumo.\n");
                }
                else{
                    reposicion(modificar, cantidadInsumo);
                }
            }
            else{
                printf("No se puede modificar, el insumo no existe.\n");
            }
            break;
        case 7:
            mostrarLote(nombresInsumos, unidadMedida, tipoPresentacion, cantidadInsumo, caducidad);
            break;
        case 8:
            buscarInsumo(nombresInsumos, unidadMedida, proveedor, stockMinimo, encontrar, modificar);
            if (*encontrar == 1){
                if (strlen(tipoPresentacion[*modificar]) == 0){
                    printf("Este insumo todavia no tiene un lote registrado.\n");
                }
                else{
                    editarLote(tipoPresentacion, cantidadInsumo, caducidad, modificar);
                }
            }
            else{
                printf("No se puede editar el lote, el insumo no existe.\n");
            }
            break;
        case 9:
            printf("Regresando al menu principal >>\n");
            break;
        }

    } while (opcion != 9);
}

void registrarInsumos(char nombresInsumos[5][40], char unidadMedida[5][20], char proveedor[5][40], float stockMinimo[5]){
    int resp = 1, contadorInsumos = 0, i, repetido;
    char nombrePrueba[40];

    do{
        printf("¿En que espacio desea registrar el insumo? (0 - 4)\n");
        i = validarRangoIntMaxMin(0, 4);

        if (stockMinimo[i] == 0){
            do{
                printf("Ingrese el nombre del insumo numero %d:\n", i + 1);
                leerCadenas(nombrePrueba, 40);
                repetido = 0;

                for (int j = 0; j < 5; j++){
                    if (stockMinimo[j] > 0 && strcasecmp(nombrePrueba, nombresInsumos[j]) == 0){
                        printf("Ya existe un insumo con el mismo nombre.\n");
                        repetido = 1;
                        break;
                    }
                }
            } while (repetido == 1);

            strcpy(nombresInsumos[i], nombrePrueba);

            printf("Ingrese la unidad de medida del insumo numero %d:\n", i + 1);
            leerCadenas(unidadMedida[i], 20);
            printf("Ingrese el proveedor habitual del insumo numero %d:\n", i + 1);
            leerCadenas(proveedor[i], 40);
            printf("Ingrese el stock minimo del insumo numero %d:\n", i + 1);
            stockMinimo[i] = validarRangoFloatMinimo(0.0001);
            printf("Insumo registrado correctamente.\n");
        }
        else{
            printf("Error: el espacio seleccionado ya se encuentra ocupado.\n");
        }

        contadorInsumos = 0;
        for (int j = 0; j < 5; j++){
            if (stockMinimo[j] > 0){
                contadorInsumos++;
            }
        }

        if (contadorInsumos < 5){
            printf("¿Desea ingresar otro insumo? (1.si / 2.no)\n");
            resp = validarRangoIntMaxMin(1, 2);
        }
        else{
            printf("Se ha completado el registro de 5 insumos.\n");
            resp = 2;
        }
    } while (resp == 1);
}

void mostrarInsumos(char nombresInsumos[5][40], char unidadMedida[5][20], char proveedor[5][40], float stockMinimo[5]){
    int contador = 0;

    printf("INSUMOS REGISTRADOS:\n");
    printf("%-25s %-15s %-18s %s\n", "Insumo", "Stock minimo", "Unidad", "Proveedor");

    for (int i = 0; i < 5; i++){
        if (stockMinimo[i] > 0){
            printf("%-25s %-15.3f %-18s %s\n", nombresInsumos[i], stockMinimo[i], unidadMedida[i], proveedor[i]);
            contador++;
        }
    }

    if (contador == 0){
        printf("No existen insumos registrados.\n");
    }
}

void buscarInsumo(char nombresInsumos[5][40], char unidadMedida[5][20], char proveedor[5][40], float stockMinimo[5], int *encontrar, int *modificar){
    char nombreBuscar[40];
    *encontrar = 0, *modificar = 0;

    printf("Ingrese el nombre del insumo:\n");
    leerCadenas(nombreBuscar, 40);

    for (int i = 0; i < 5; i++){
        if (stockMinimo[i] > 0 && strcasecmp(nombreBuscar, nombresInsumos[i]) == 0){
            *modificar = i;
            *encontrar = 1;
            break;
        }
    }

    if (*encontrar == 1){
        printf("%-25s %-18s %-25s %s\n", "Insumo", "Unidad", "Proveedor", "Stock minimo");
        printf("%-25s %-18s %-25s %.3f\n", nombresInsumos[*modificar], unidadMedida[*modificar], proveedor[*modificar], stockMinimo[*modificar]);
        printf("El insumo se ha encontrado correctamente.\n");
    }
    else{
        printf("El insumo ingresado no existe.\n");
    }
}

void editarInsumo(char nombresInsumos[5][40], char unidadMedida[5][20], char proveedor[5][40], float stockMinimo[5], int modificar, char insumosTratamientos[5][5][40]){
    int opc, repetido;
    char nombreNuevo[40];
    char nombreAnterior[40];

    printf("¿Que desea modificar?\n");
    printf("1. Nombre del insumo\n");
    printf("2. Unidad de medida\n");
    printf("3. Proveedor\n");
    printf("4. Stock minimo\n");
    printf(">> ");
    opc = validarRangoIntMaxMin(1, 4);

    switch (opc){
    case 1:
        strcpy(nombreAnterior, nombresInsumos[modificar]);
        repetido = 0;
        do{
            printf("Ingrese el nuevo nombre:\n");
            leerCadenas(nombreNuevo, 40);
            repetido = 0;

            for (int i = 0; i < 5; i++){
                if (stockMinimo[i] > 0 && strcasecmp(nombreNuevo, nombresInsumos[i]) == 0){
                    repetido = 1;
                    printf("Ya existe un insumo con ese nombre.\n");
                    break;
                }
            }
        } while (repetido == 1);

        strcpy(nombresInsumos[modificar], nombreNuevo);

        for (int i = 0; i < 5; i++){
            for (int j = 0; j < 5; j++){
                if (strcasecmp(insumosTratamientos[i][j], nombreAnterior) == 0){
                    strcpy(insumosTratamientos[i][j], nombreNuevo);
                }
            }
        }
        printf("El cambio se ha realizado con exito.\n");
        break;
    case 2:
        printf("Ingrese la nueva unidad de medida:\n");
        leerCadenas(unidadMedida[modificar], 20);
        printf("El cambio se ha realizado con exito.\n");
        break;
    case 3:
        printf("Ingrese el nombre del nuevo proveedor:\n");
        leerCadenas(proveedor[modificar], 40);
        printf("El cambio se ha realizado con exito.\n");
        break;
    case 4:
        printf("Ingrese el nuevo stock minimo:\n");
        stockMinimo[modificar] = validarRangoFloatMinimo(0.0001f);
        printf("El cambio se ha realizado con exito.\n");
        break;
    }
}

void registrarLote(int *modificar, char tipoPresentacion[5][20], char nombresInsumos[5][40], float cantidadInsumo[5], char caducidad[5][20]){
    if (strlen(tipoPresentacion[*modificar]) > 0){
        printf("Este insumo ya tiene una presentacion registrada\n");
        return;
    }

    printf("Ingrese el tipo de presentacion para %s (ejemplo: jeringa, caja, frasco):\n", nombresInsumos[*modificar]);
    leerCadenas(tipoPresentacion[*modificar], 20);

    printf("Ingrese la cantidad del lote:\n");
    cantidadInsumo[*modificar] = validarRangoFloatMinimo(0.0001f);

    printf("Ingrese la fecha de caducidad (formato dia/mes/anio):\n");
    leerCadenas(caducidad[*modificar], 20);

    printf("La presentacion se registro correctamente.\n");
}

void reposicion(int *modificar, float cantidadInsumo[5]){
    int resp;
    float cantidad;

    printf("¿Desea reponer o quitar cantidad?: (1.reponer / 2.quitar)\n");
    resp = validarRangoIntMaxMin(1, 2);

    switch (resp){
    case 1:
        printf("Ingrese la cantidad que desea reponer:\n");
        cantidad = validarRangoFloatMinimo(0.0001f);
        cantidadInsumo[*modificar] += cantidad;
        printf("La reposicion se realizo correctamente.\n");
        break;
    case 2:
        if (cantidadInsumo[*modificar] <= 0)
        {
            printf("No existe cantidad disponible para retirar.\n");
            break;
        }
        printf("Ingrese la cantidad que desea quitar:\n");
        cantidad = validarRangoFloat(0.0001f, cantidadInsumo[*modificar]);
        cantidadInsumo[*modificar] -= cantidad;
        printf("El retiro se realizo correctamente.\n");
        break;
    }
}

void mostrarLote(char nombresInsumos[5][40], char unidadMedida[5][20], char tipoPresentacion[5][20], float cantidadInsumo[5], char caducidad[5][20]) {
    int contador = 0;

    printf("LOTES:\n");
    printf("%-25s %-14s %-16s %-16s %s\n", "Insumo", "Cantidad", "Unidad", "Presentacion", "Caducidad");

    for (int i = 0; i < 5; i++){
        if (strlen(tipoPresentacion[i]) > 0){
            printf("%-25s %-14.3f %-16s %-16s %s\n", nombresInsumos[i], cantidadInsumo[i], unidadMedida[i], tipoPresentacion[i], caducidad[i]);
            contador++;
        }
    }

    if (contador == 0){
        printf("No existen lotes registrados.\n");
    }
}

void editarLote(char tipoPresentacion[5][20], float cantidadInsumo[5], char caducidad[5][20], int *modificar){
    int opc;

    printf("¿Que desea modificar?\n");
    printf("1. Tipo de presentacion\n");
    printf("2. Cantidad del lote\n");
    printf("3. Fecha de caducidad\n");
    printf(">> ");
    opc = validarRangoIntMaxMin(1, 3);

    switch (opc){
    case 1:
        printf("Ingrese el nuevo tipo de presentacion:\n");
        leerCadenas(tipoPresentacion[*modificar], 20);
        printf("El cambio se ha realizado con exito.\n");
        break;
    case 2:
        printf("Ingrese la nueva cantidad del lote:\n");
        cantidadInsumo[*modificar] = validarRangoFloatMinimo(0.0001f);
        printf("El cambio se ha realizado con exito.\n");
        break;
    case 3:
        printf("Ingrese la nueva fecha de caducidad:\n");
        leerCadenas(caducidad[*modificar], 20);
        printf("El cambio se ha realizado con exito.\n");
        break;
    }
}

void gestionTratamientos(char tratamientos[5][40], char nombresInsumos[5][40], char unidadMedida[5][20], float stockMinimo[5], char insumosTratamientos[5][5][40], float cantidadesTratamientos[5][5], int modalidadesTratamientos[5][5], int cantidadInsumosTratamiento[5]){
    int opcion;

    do{
        printf("Seleccione una opcion\n");
        printf("1. Registrar un tratamiento\n");
        printf("2. Agregar insumos a un tratamiento\n");
        printf("3. Mostrar tratamientos registrados\n");
        printf("4. Buscar tratamiento\n");
        printf("5. Editar tratamientos\n");
        printf("6. Regresar al menu principal\n");
        printf(">> ");
        opcion = validarRangoIntMaxMin(1, 6);

        switch (opcion){
        case 1:
            registrarTratamiento(tratamientos);
            break;
        case 2:
            agregarInsumoReceta(tratamientos, nombresInsumos, unidadMedida, stockMinimo, insumosTratamientos, cantidadesTratamientos, modalidadesTratamientos, cantidadInsumosTratamiento);
            break;
        case 3:
            mostrarTratamientos(tratamientos, nombresInsumos, unidadMedida, stockMinimo, insumosTratamientos, cantidadesTratamientos, modalidadesTratamientos, cantidadInsumosTratamiento);
            break;
        case 4:
            buscarTratamiento(tratamientos, nombresInsumos, unidadMedida, stockMinimo, insumosTratamientos, cantidadesTratamientos, modalidadesTratamientos, cantidadInsumosTratamiento);
            break;
        case 5:
            editarTratamiento(tratamientos, nombresInsumos, unidadMedida, stockMinimo, insumosTratamientos, cantidadesTratamientos, modalidadesTratamientos, cantidadInsumosTratamiento);
            break;
        case 6:
            printf("Regresando al menu principal >>\n");
            break;
        }
    } while (opcion != 6);
}

void registrarTratamiento(char tratamientos[5][40]) {
    int resp = 1, contadorTratamientos = 0, i, repetido;
    char nombrePrueba[40];
    do{
        printf("¿En que espacio desea registrar el tratamiento? (0 - 4)\n");
        i = validarRangoIntMaxMin(0, 4);

        if (strcmp(tratamientos[i], "-") == 0){
            do{
                printf("Ingrese el nombre del tratamiento numero %d:\n", i + 1);
                leerCadenas(nombrePrueba, 40);
                repetido = 0;

                for (int j = 0; j < 5; j++){
                    if (strcasecmp(nombrePrueba, tratamientos[j]) == 0) {
                        printf("Ya existe un tratamiento con el mismo nombre.\n");
                        repetido = 1;
                        break;
                    }
                }
            } while (repetido == 1);
            strcpy(tratamientos[i], nombrePrueba);
            printf("Tratamiento registrado correctamente.\n");
        } else{
            printf("Error: el espacio seleccionado ya se encuentra ocupado.\n");
        }

        contadorTratamientos = 0;
        for (int j = 0; j < 5; j++){
            if (strcmp(tratamientos[j], "-") != 0) {
                contadorTratamientos++;
            }
        }

        if (contadorTratamientos < 5) {
            printf("¿Desea ingresar otro tratamiento? (1.si / 2.no)\n");
            resp = validarRangoIntMaxMin(1, 2);
        } else {
            printf("Se ha completado el registro de 5 tratamientos.\n");
            resp = 2;
        }
    } while (resp == 1);
}

void agregarInsumoReceta(char tratamientos[5][40], char nombresInsumos[5][40], char unidadMedida[5][20], float stockMinimo[5], char insumosTratamientos[5][5][40], float cantidadesTratamientos[5][5], int modalidadesTratamientos[5][5], int cantidadInsumosTratamiento[5]) {
    int posicionTratamiento, posicionInsumo, contTratamientos = 0, contInsumos = 0, repetido = 0, columna = 0, hayEspacio = 0, modalidad;
    float cantidad;

    printf("Tratamientos disponibles:\n");
    for (int i = 0; i < 5; i++){
        if (strcmp(tratamientos[i], "-") != 0){
            printf("%d. %s\n", i, tratamientos[i]);
            contTratamientos++;
        }
    }

    if (contTratamientos == 0){
        printf("No existen tratamientos registrados.\n");
        return;
    }

    printf("Ingrese la posicion del tratamiento que desea configurar (0 - 4):\n");
    posicionTratamiento = validarRangoIntMaxMin(0, 4);

    if (strcmp(tratamientos[posicionTratamiento], "-") == 0){
        printf("El espacio seleccionado no contiene un tratamiento.\n");
        return;
    }

    printf("Tratamiento seleccionado: %s\n", tratamientos[posicionTratamiento]);

    if (cantidadInsumosTratamiento[posicionTratamiento] >= 5) {
        printf("Este tratamiento ya tiene registrados 5 insumos.\n");
        return;
    }

    printf("Insumos Disponibles\n");
    for (int i = 0; i < 5; i++){
        if (stockMinimo[i] > 0) {
            printf("%d. %s\n", i, nombresInsumos[i]);
            contInsumos++;
        }
    }
    if (contInsumos == 0){
        printf("No hay insumos registrados.\n");
        return;
    }

    printf("Ingrese la posicion del insumo que desea agregar (0 - 4):\n");
    posicionInsumo = validarRangoIntMaxMin(0, 4);

    if (stockMinimo[posicionInsumo] == 0){
        printf("El espacio seleccionado no contiene un insumo registrado.\n");
        return;
    }

    repetido = 0;
    for (int i = 0; i < 5; i++){
        if (strcmp(insumosTratamientos[posicionTratamiento][i], "-") != 0 && strcasecmp(insumosTratamientos[posicionTratamiento][i], nombresInsumos[posicionInsumo]) == 0){
            printf("Este insumo ya se encuentra asociado al tratamiento.\n");
            repetido = 1;
            return;
        }
    }

    for (int i = 0; i < 5; i++){
        if (strcmp(insumosTratamientos[posicionTratamiento][i], "-") == 0){
            columna = i;
            hayEspacio = 1;
            break;
        }
    }

    if (hayEspacio == 0){
        printf("No existen espacios disponibles para agregar otro insumo.\n");
        return;
    }

    printf("Ingrese la cantidad de consumo del insumo:\n");
    cantidad = validarRangoFloatMinimo(0.0001f);

    printf("Seleccione la modalidad de consumo:\n");
    printf("1. Por diente\n");
    printf("2. Por procedimiento\n");
    printf(">> ");
    modalidad = validarRangoIntMaxMin(1, 2);

    strcpy(insumosTratamientos[posicionTratamiento][columna], nombresInsumos[posicionInsumo]);
    cantidadesTratamientos[posicionTratamiento][columna] = cantidad;
    modalidadesTratamientos[posicionTratamiento][columna] = modalidad;
    cantidadInsumosTratamiento[posicionTratamiento]++;

    printf("Se ha agregado el insumo correctamente.\n");
    printf("Tratamiento: %s\n", tratamientos[posicionTratamiento]);
    printf("Insumo: %s\n", nombresInsumos[posicionInsumo]);
    printf("Cantidad: %.3f %s\n", cantidad, unidadMedida[posicionInsumo]);

    if (modalidad == 1){
        printf("Modalidad: Por diente\n");
    } else{
        printf("Modalidad: Por procedimiento\n");
    }
}

void mostrarInsumosTratamiento(int posicionTratamiento, char nombresInsumos[5][40], char unidadMedida[5][20], float stockMinimo[5], char insumosTratamientos[5][5][40], float cantidadesTratamientos[5][5], int modalidadesTratamientos[5][5], int cantidadInsumosTratamiento[5]) {
    int posicionInsumo, encontradoInsumo, hayInsumos = 0;

    printf("Insumos del tratamiento:\n");

    if (cantidadInsumosTratamiento[posicionTratamiento] == 0) {
        printf("Este tratamiento no tiene insumos asociados.\n");
        return;
    }

    for (int i = 0; i < 5; i++){
        if (strcmp(insumosTratamientos[posicionTratamiento][i], "-") != 0){
            hayInsumos++, posicionInsumo = 0, encontradoInsumo = 0;

            for (int j = 0; j < 5; j++){
                if (stockMinimo[j] > 0 && strcasecmp(nombresInsumos[j], insumosTratamientos[posicionTratamiento][i]) == 0) {
                    posicionInsumo = j;
                    encontradoInsumo = 1;
                    break;
                }
            }

            printf("\nPosicion %d: %s\n", i, insumosTratamientos[posicionTratamiento][i]);

            if (encontradoInsumo == 1) {
                printf("Cantidad: %.3f %s\n", cantidadesTratamientos[posicionTratamiento][i], unidadMedida[posicionInsumo]);
            }

            if (modalidadesTratamientos[posicionTratamiento][i] == 1) {
                printf("Modalidad: Por diente\n");
            } else if (modalidadesTratamientos[posicionTratamiento][i] == 2) {
                printf("Modalidad: Por procedimiento\n");
            }
        }
    }

    if (hayInsumos == 0) {
        printf("Este tratamiento no tiene insumos asociados.\n");
    }
}

void mostrarTratamientos(char tratamientos[5][40], char nombresInsumos[5][40], char unidadMedida[5][20], float stockMinimo[5], char insumosTratamientos[5][5][40], float cantidadesTratamientos[5][5], int modalidadesTratamientos[5][5], int cantidadInsumosTratamiento[5]) {
    int contTratamientos = 0, posicionInsumo, encontradoInsumo;

    printf("Tratamientos:\n");

    for (int i = 0; i < 5; i++) {
        if (strcmp(tratamientos[i], "-") != 0){
            contTratamientos++;
            printf("\nTRATAMIENTO: %s\n", tratamientos[i]);

            if (cantidadInsumosTratamiento[i] == 0){
                printf("Este tratamiento todavia no tiene insumos asociados.\n");
            } else {
                printf("INSUMOS ASOCIADOS:\n");

                for (int j = 0; j < 5; j++){
                    if (strcmp(insumosTratamientos[i][j], "-") != 0) {
                        posicionInsumo = 0;
                        encontradoInsumo = 0;

                        for (int k = 0; k < 5; k++){
                            if (stockMinimo[k] > 0 && strcasecmp(nombresInsumos[k], insumosTratamientos[i][j]) == 0){
                                posicionInsumo = k;
                                encontradoInsumo = 1;
                                break;
                            }
                        }

                        printf("\n%d. %s\n", j, insumosTratamientos[i][j]);

                        if (encontradoInsumo == 1){
                            printf("Cantidad: %.3f %s\n", cantidadesTratamientos[i][j], unidadMedida[posicionInsumo]);
                        } else{
                            printf("Cantidad: %.3f (unidad no disponible en el catalogo)\n", cantidadesTratamientos[i][j]);
                        }

                        if (modalidadesTratamientos[i][j] == 1){
                            printf("Modalidad: Por diente\n");
                        } else if (modalidadesTratamientos[i][j] == 2){
                            printf("Modalidad: Por procedimiento\n");
                        }
                    }
                }
            }
        }
    }
    if (contTratamientos == 0) {
        printf("No existen tratamientos registrados.\n");
    }
}

void buscarTratamiento(char tratamientos[5][40], char nombresInsumos[5][40], char unidadMedida[5][20], float stockMinimo[5], char insumosTratamientos[5][5][40], float cantidadesTratamientos[5][5], int modalidadesTratamientos[5][5], int cantidadInsumosTratamiento[5]) {
    char nombreBuscar[40];
    int posicionTratamiento = 0, encontradoTratamiento = 0;

    printf("Ingrese el nombre del tratamiento:\n");
    leerCadenas(nombreBuscar, 40);

    for (int i = 0; i < 5; i++) {
        if (strcmp(tratamientos[i], "-") != 0 && strcasecmp(nombreBuscar, tratamientos[i]) == 0) {
            posicionTratamiento = i;
            encontradoTratamiento = 1;
            break;
        }
    }

    if (encontradoTratamiento == 0) {
        printf("El tratamiento ingresado no existe.\n");
        return;
    }

    printf("Tratamiento encontrado con exito\n");
    printf("Nombre: %s\n", tratamientos[posicionTratamiento]);
    mostrarInsumosTratamiento(posicionTratamiento, nombresInsumos, unidadMedida, stockMinimo, insumosTratamientos, cantidadesTratamientos, modalidadesTratamientos, cantidadInsumosTratamiento);
}

void editarTratamiento(char tratamientos[5][40], char nombresInsumos[5][40], char unidadMedida[5][20], float stockMinimo[5], char insumosTratamientos[5][5][40], float cantidadesTratamientos[5][5], int modalidadesTratamientos[5][5], int cantidadInsumosTratamiento[5]){
    char nombreBuscar[40], nombreNuevo[40];
    int posicionTratamiento = 0, encontradoTratamiento = 0, posicionInsumo, posicionDetalle, opcion, repetido, modalidad;
    float nuevaCantidad;

    printf("Tratamientos:\n");
    for (int i = 0; i < 5; i++){
        if (strcmp(tratamientos[i], "-") != 0){
            printf("%d. %s\n", i, tratamientos[i]);
        }
    }

    printf("Ingrese el nombre del tratamiento que desea editar:\n");
    leerCadenas(nombreBuscar, 40);

    for (int i = 0; i < 5; i++){
        if (strcmp(tratamientos[i], "-") != 0 && strcasecmp(nombreBuscar, tratamientos[i]) == 0){
            posicionTratamiento = i;
            encontradoTratamiento = 1;
            break;
        }
    }

    if (encontradoTratamiento == 0){
        printf("El tratamiento ingresado no existe.\n");
        return;
    }
    printf("Tratamiento seleccionado: %s\n", tratamientos[posicionTratamiento]);
    do{
        printf("¿Que desea modificar?\n");
        printf("1. Nombre del tratamiento\n");
        printf("2. Cambiar un insumo asociado\n");
        printf("3. Modificar cantidad de consumo\n");
        printf("4. Modificar modalidad de consumo\n");
        printf("5. Eliminar un insumo de la receta\n");
        printf("6. Regresar\n");
        printf(">> ");
        opcion = validarRangoIntMaxMin(1, 6);

        switch (opcion){
        case 1:
            repetido = 0;
            do{
                printf("Ingrese el nuevo nombre del tratamiento:\n");
                leerCadenas(nombreNuevo, 40);
                repetido = 0;

                if (strlen(nombreNuevo) == 0 || strcmp(nombreNuevo, "-") == 0){
                    printf("Debe ingresar un nombre valido.\n");
                    repetido = 1;
                }

                for (int i = 0; i < 5; i++){
                    if (i != posicionTratamiento && strcmp(tratamientos[i], "-") != 0 && strcasecmp(nombreNuevo, tratamientos[i]) == 0){
                        printf("Ya existe un tratamiento con ese nombre.\n");
                        repetido = 1;
                        break;
                    }
                }
            } while (repetido == 1);

            strcpy(tratamientos[posicionTratamiento], nombreNuevo);
            printf("El nombre del tratamiento se modifico correctamente.\n");
            break;

        case 2:
            if (cantidadInsumosTratamiento[posicionTratamiento] == 0){
                printf("Este tratamiento no tiene insumos registrados.\n");
                break;
            }

            mostrarInsumosTratamiento(posicionTratamiento, nombresInsumos, unidadMedida, stockMinimo, insumosTratamientos, cantidadesTratamientos, modalidadesTratamientos, cantidadInsumosTratamiento);

            printf("Seleccione la posicion del insumo que desea cambiar (0 - 4):\n");
            posicionDetalle = validarRangoIntMaxMin(0, 4);

            if (strcmp(insumosTratamientos[posicionTratamiento][posicionDetalle], "-") == 0){
                printf("La posicion seleccionada no contiene ningun insumo.\n");
                break;
            }

            printf("Insumos disponibles:\n");
            for (int i = 0; i < 5; i++){
                if (stockMinimo[i] > 0){
                    printf("%d. %s\n", i, nombresInsumos[i]);
                }
            }

            printf("Seleccione el nuevo insumo (0 - 4):\n");
            posicionInsumo = validarRangoIntMaxMin(0, 4);

            if (stockMinimo[posicionInsumo] == 0){
                printf("El espacio seleccionado no contiene un insumo registrado.\n");
                break;
            }

            repetido = 0;
            for (int i = 0; i < 5; i++){
                if (i != posicionDetalle && strcmp(insumosTratamientos[posicionTratamiento][i], "-") != 0 && strcasecmp(insumosTratamientos[posicionTratamiento][i], nombresInsumos[posicionInsumo]) == 0){
                    printf("Este insumo ya se encuentra en la receta.\n");
                    repetido = 1;
                    break;
                }
            }

            if (repetido == 1){
                break;
            }

            printf("Ingrese la nueva cantidad de consumo:\n");
            nuevaCantidad = validarRangoFloatMinimo(0.0001f);

            printf("Seleccione la nueva modalidad:\n");
            printf("1. Por diente\n");
            printf("2. Por procedimiento\n");
            modalidad = validarRangoIntMaxMin(1, 2);

            strcpy(insumosTratamientos[posicionTratamiento][posicionDetalle], nombresInsumos[posicionInsumo]);
            cantidadesTratamientos[posicionTratamiento][posicionDetalle] = nuevaCantidad;
            modalidadesTratamientos[posicionTratamiento][posicionDetalle] = modalidad;
            printf("El insumo se modifico correctamente.\n");
            break;

        case 3:
            if (cantidadInsumosTratamiento[posicionTratamiento] == 0){
                printf("Este tratamiento no tiene insumos registrados.\n");
                break;
            }

            mostrarInsumosTratamiento(posicionTratamiento, nombresInsumos, unidadMedida, stockMinimo, insumosTratamientos, cantidadesTratamientos, modalidadesTratamientos, cantidadInsumosTratamiento);

            printf("Seleccione la posicion del insumo que desea modificar (0 - 4):\n");
            posicionDetalle = validarRangoIntMaxMin(0, 4);

            if (strcmp(insumosTratamientos[posicionTratamiento][posicionDetalle], "-") == 0){
                printf("La posicion seleccionada no contiene ningun insumo.\n");
                break;
            }

            printf("Ingrese la nueva cantidad de consumo:\n");
            nuevaCantidad = validarRangoFloatMinimo(0.0001f);
            cantidadesTratamientos[posicionTratamiento][posicionDetalle] = nuevaCantidad;
            printf("La cantidad se modifico correctamente.\n");
            break;

        case 4:
            if (cantidadInsumosTratamiento[posicionTratamiento] == 0){
                printf("Este tratamiento no tiene insumos registrados.\n");
                break;
            }

            mostrarInsumosTratamiento(posicionTratamiento, nombresInsumos, unidadMedida, stockMinimo, insumosTratamientos, cantidadesTratamientos, modalidadesTratamientos, cantidadInsumosTratamiento);

            printf("Seleccione la posicion del insumo que desea modificar (0 - 4):\n");
            posicionDetalle = validarRangoIntMaxMin(0, 4);

            if (strcmp(insumosTratamientos[posicionTratamiento][posicionDetalle], "-") == 0){
                printf("La posicion seleccionada no contiene ningun insumo.\n");
                break;
            }

            printf("Seleccione la nueva modalidad:\n");
            printf("1. Por diente\n");
            printf("2. Por procedimiento\n");
            modalidad = validarRangoIntMaxMin(1, 2);

            modalidadesTratamientos[posicionTratamiento][posicionDetalle] = modalidad;
            printf("La modalidad se modifico correctamente.\n");
            break;

        case 5:
            if (cantidadInsumosTratamiento[posicionTratamiento] == 0){
                printf("Este tratamiento no tiene insumos registrados.\n");
                break;
            }

            mostrarInsumosTratamiento(posicionTratamiento, nombresInsumos, unidadMedida, stockMinimo, insumosTratamientos, cantidadesTratamientos, modalidadesTratamientos, cantidadInsumosTratamiento);

            printf("Seleccione la posicion del insumo que desea eliminar (0 - 4):\n");
            posicionDetalle = validarRangoIntMaxMin(0, 4);

            if (strcmp(insumosTratamientos[posicionTratamiento][posicionDetalle], "-") == 0){
                printf("La posicion seleccionada no contiene ningun insumo.\n");
                break;
            }

            strcpy(insumosTratamientos[posicionTratamiento][posicionDetalle], "-");
            cantidadesTratamientos[posicionTratamiento][posicionDetalle] = 0;
            modalidadesTratamientos[posicionTratamiento][posicionDetalle] = 0;
            cantidadInsumosTratamiento[posicionTratamiento]--;

            printf("El insumo se elimino correctamente.\n");
            break;

        case 6:
            printf("Regresando al menu de tratamientos...\n");
            break;
        }
    } while (opcion != 6);
}

void capacidadTratamientos(char tratamientos[5][40], char nombresInsumos[5][40], char unidadMedida[5][20], float stockMinimo[5], char tipoPresentacion[5][20], float cantidadInsumo[5], char caducidad[5][20], char insumosTratamientos[5][5][40], float cantidadesTratamientos[5][5], int modalidadesTratamientos[5][5], int cantidadInsumosTratamiento[5]) {

    int contInsumos = 0, contTratamientos = 0, contRecetas = 0, posicionTratamiento, posicionInsumo = 0, encontradoInsumo, cantidadProcedimientos, cantidadDientes, tieneDiente, resp = 2, tratamientosPlanificados = 0, recetaValida, tieneReceta, i, j, k, contInsuficientes = 0;
    float demandaInsumos[5] = {0}, consumo, faltante;

    for (i = 0; i < 5; i++){
        if (stockMinimo[i] > 0){
            contInsumos++;
        }
    }

    if (contInsumos == 0){
        printf("No existen insumos registrados\n");
        return;
    }

    for (i = 0; i < 5; i++){
        if (strcmp(tratamientos[i], "-") != 0){
            contTratamientos++;
        }
    }

    if (contTratamientos == 0){
        printf("No existen tratamientos registrados\n");
        return;
    }

    for (i = 0; i < 5; i++){
        for (j = 0; j < 5; j++){
            if (strcmp(insumosTratamientos[i][j], "-") != 0){
                contRecetas++;
            }
        }
    }

    if (contRecetas == 0){
        printf("No existen recetas registradas.\n");
        printf("Primero debe agregar insumos a los tratamientos.\n");
        return;
    }

    do{
        printf("Tratamientos\n");

        for (i = 0; i < 5; i++) {
            if (strcmp(tratamientos[i], "-") != 0) {
                printf("%d. %s\n", i, tratamientos[i]);
            }
        }

        printf("Seleccione la posicion del tratamiento que desea realizar (0 - 4):\n");
        posicionTratamiento = validarRangoIntMaxMin(0, 4);

        if (strcmp(tratamientos[posicionTratamiento], "-") == 0) {
            printf("El espacio seleccionado no contiene un tratamiento registrado.\n");
            break;
        }

        tieneReceta = 0;

        for (j = 0; j < 5; j++) {
            if (strcmp(insumosTratamientos[posicionTratamiento][j], "-") != 0) {
                tieneReceta = 1;
            }
        }

        if (tieneReceta == 0) {
            printf("El tratamiento seleccionado no tiene insumos asociados\n");
            break;
        }

        recetaValida = 1;

        for (j = 0; j < 5; j++){
            if (strcmp(insumosTratamientos[posicionTratamiento][j], "-") != 0){

                posicionInsumo = 0;
                encontradoInsumo = 0;

                for (k = 0; k < 5; k++){
                    if (stockMinimo[k] > 0 && strcasecmp(nombresInsumos[k], insumosTratamientos[posicionTratamiento][j]) == 0){
                        posicionInsumo = k;
                        encontradoInsumo = 1;
                        break;
                    }
                }

                if (encontradoInsumo == 0){
                    printf("El insumo %s no existe en el inventario.\n", insumosTratamientos[posicionTratamiento][j]);
                    recetaValida = 0;
                    break;
                }

                if (strlen(tipoPresentacion[posicionInsumo]) == 0 || strlen(caducidad[posicionInsumo]) == 0) {
                    printf("El insumo %s no tiene una presentacion completa registrada.\n", nombresInsumos[posicionInsumo]);
                    printf("Primero debe registrar su presentacion, cantidad y caducidad.\n");
                    recetaValida = 0;
                    break;
                }

                if (cantidadesTratamientos[posicionTratamiento][j] <= 0 || modalidadesTratamientos[posicionTratamiento][j] < 1 || modalidadesTratamientos[posicionTratamiento][j] > 2) {
                    printf("La receta del insumo %s tiene datos de consumo incorrectos.\n", nombresInsumos[posicionInsumo]);
                    recetaValida = 0;
                    break;
                }
            }
        }

        if (recetaValida == 0) {
            break;
        }

        printf("Tratamiento seleccionado: %s\n", tratamientos[posicionTratamiento]);

        printf("Ingrese la cantidad de tratamientos que desea realizar:\n");
        cantidadProcedimientos = validarRangoInt(1);

        if (cantidadProcedimientos == -1) {
            break;
        }

        tieneDiente = 0;

        for (j = 0; j < 5; j++){

            if (strcmp(insumosTratamientos[posicionTratamiento][j], "-") != 0){

                if (modalidadesTratamientos[posicionTratamiento][j] == 1){
                    tieneDiente = 1;
                }
            }
        }

        cantidadDientes = 0;

        if (tieneDiente == 1) {
            printf("Ingrese la cantidad total de dientes que se intervendran:\n");
            cantidadDientes = validarRangoInt(1);
        }

        for (j = 0; j < 5; j++){
            if (strcmp(insumosTratamientos[posicionTratamiento][j], "-") != 0){

                posicionInsumo = 0;
                encontradoInsumo = 0;

                for (k = 0; k < 5; k++){
                    if (stockMinimo[k] > 0 && strcasecmp(nombresInsumos[k], insumosTratamientos[posicionTratamiento][j]) == 0) {
                        posicionInsumo = k;
                        encontradoInsumo = 1;
                        break;
                    }
                }

                if (encontradoInsumo == 1){

                    consumo = 0;

                    if (modalidadesTratamientos[posicionTratamiento][j] == 1) {
                        consumo = cantidadesTratamientos[posicionTratamiento][j] * cantidadDientes;
                    }
                    else if (modalidadesTratamientos[posicionTratamiento][j] == 2) {
                        consumo = cantidadesTratamientos[posicionTratamiento][j] * cantidadProcedimientos;
                    }

                    demandaInsumos[posicionInsumo] += consumo;
                }
            }
        }

        tratamientosPlanificados++;

        printf("El tratamiento se agrego correctamente\n");

        printf("¿Desea agregar otro tratamiento?\n");
        printf("1. Si\n");
        printf("2. No\n");
        resp = validarRangoIntMaxMin(1, 2);

    } while (resp == 1);

    
    if (tratamientosPlanificados > 0) {
        char demandaTexto[40], cantidadTexto[40];
        printf("Resultados:\n");
        printf("%-25s %-14s %-14s %-15s\n", "Insumo", "Demanda", "Cantidad", "Capacidad");

        contInsuficientes = 0;

        for (i = 0; i < 5; i++) {
            if (demandaInsumos[i] > 0) {
                snprintf(demandaTexto, sizeof(demandaTexto), "%.3f %s", demandaInsumos[i], unidadMedida[i]);
                snprintf(cantidadTexto, sizeof(cantidadTexto), "%.3f %s", cantidadInsumo[i], unidadMedida[i]);
                printf("%-25s %-14.3f %-14.3f ", nombresInsumos[i], demandaInsumos[i], cantidadInsumo[i]);

                if (cantidadInsumo[i] >= demandaInsumos[i]){
                    printf("%-15s\n", "SUFICIENTE");
                }
                else{
                    faltante = demandaInsumos[i] - cantidadInsumo[i];
                    printf("%-15s\n", "INSUFICIENTE");
                    printf("Faltan %.3f %s de %s.\n", faltante, unidadMedida[i], nombresInsumos[i]);
                    contInsuficientes++;
                }
            }
        }
    }   
}