#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;



/*** Macros ***/

#define TAMBUF  1024  /* Tamanño para buffer de entrada */

/*** Definicion de tipos de datos personalizados ***/

/* Tipo de datos para la gestion del menu del programa */

typedef enum _OpcionMenu OpcionMenu;

enum _OpcionMenu
{
    MENU_LISTAR_DATOS = 1,

    MENU_AGREGAR_DATO,
    MENU_ELIMINAR_DATO,
    MENU_MODIFICAR_DATO,

    MENU_LEER_ARCHIVO,
    MENU_GUARDAR_ARCHIVO,

    MENU_SALIR,

    MENU_FIN  /* Elemento especial para marcar el final de la
               * enumeracion */
};


/* Tipo de datos que almacena valores y descripciones para el menu */

typedef struct _Menu Menu;

struct _Menu
{
    OpcionMenu valor;

    /* Le damos un tamanyo fijo, a sabiendas de que solo el
     * programador puede operar sobre este tipo de
     * estructuras. Asumimos que el programador sabe lo que hace,
     * cierto? */
    char descripcion[81];
};


typedef unsigned char uchar;  /* Alias para unsigned char */



/*** Prototipos de funciones ***/

void arreglar (char *);

void bit_definir   (uchar *, int, int);
int  bit_recuperar (uchar *, int);

int  bits_agregar   (uchar **, int);
int  bits_eliminar  (uchar **, int);
void bits_listar    (uchar *, int);
void bits_modificar (uchar *, int);

void bits_guardar   (uchar *, int);
int  bits_leer      (uchar **);

int leer_entero (void);



/*** Funcion principal ***/

int
main (void){
    static Menu datos_menu[] = {
        {MENU_LISTAR_DATOS,    "Listar datos"},
        {MENU_AGREGAR_DATO,    "Agregar dato"},
        {MENU_ELIMINAR_DATO,   "Eliminar dato"},
        {MENU_MODIFICAR_DATO,  "Modificar dato"},
        {MENU_LEER_ARCHIVO,    "Leer datos desde archivo"},
        {MENU_GUARDAR_ARCHIVO, "Guardar datos en archivo"},
        {MENU_SALIR,           "Salir"}};

    OpcionMenu opcion;
    int i;

    uchar *secuencia = NULL;  /* Secuencia de bits */
    int cantidad = 0;  /* Cantidad de bits que se estan manipulando */


    /* Mensaje de introduccion. Se siente un poco mejor cuando el
     * programa le da la bienvenida al usuario, y le dice brevemente
     * de que se trata el asunto. */

    printf ("\n""Bienvenido.\n" "Este programa realiza algunas manipulaciones simples con bits.\n\n"
                        "Por favor seleccione una opcion:\n\n");


    for (i = 0; i < MENU_FIN - 1; i++)
        printf ("  %d. %s\n", datos_menu[i].valor, datos_menu[i].descripcion);


    do {
        printf ("\nopcion> ");

//       opcion = leer_entero();

        switch (opcion) {
        case MENU_LISTAR_DATOS:
            bits_listar (secuencia, cantidad);
            break;

        case MENU_AGREGAR_DATO:
            cantidad = bits_agregar (&secuencia, cantidad);
            break;

        case MENU_ELIMINAR_DATO:
            cantidad = bits_eliminar (&secuencia, cantidad);
            break;

        case MENU_MODIFICAR_DATO:
            bits_modificar (secuencia, cantidad);
            break;

        case MENU_LEER_ARCHIVO:
            cantidad = bits_leer (&secuencia);
            break;

        case MENU_GUARDAR_ARCHIVO:
            bits_guardar (secuencia, cantidad);
            break;

        case MENU_SALIR:
            break;

        default:
            printf ("\nOpcion invalida.\n");
        }

    } while (opcion != MENU_SALIR);

    printf ("\nAdios pues..\n");

    return 0;
}



/*** Declaracion de funciones ***/


/* arreglar()
 *
 * Sencilla funcion que se encarga de asegurarse de que una cadena
 * solo contenga caracteres validos, de acuerdo a la funcion
 * isgraph(). Todos los caracteres no-validos son eliminados */

void
arreglar (char *cadena)
{
    int i, j;
    int longitud;

    longitud = strlen (cadena);

    for (i = j = 0; i < longitud; i++) {
        if (isgraph(cadena[i])) {
            if (i != j)
                cadena[j++] = cadena[i];
            else
                j++;
        }
    }

    cadena[j] = '\0';
}


/* bit_definir()
 *
 * Define el valor de un bit determinado dentro de una secuencia de
 * bits, que se asume esta en orden little-endian. Recibe en orden: la
 * secuencia de bits, la posicion a definir, y el nuevo valor para el
 * bit */

void
bit_definir (uchar *sec, int pos, int val)
{
    uchar temp = 1 << (7 - (pos % 8));

    sec[pos / 8] = (uchar) (val ? sec[pos / 8] | ((uchar)temp) :
                            sec[pos / 8] & ((uchar)~temp));
}


/* bit_recuperar()
 *
 * Devuelve el valor de un bit determinado, en una secuencia de bytes
 * dada. Recibe la secuencia de bytes, y la posicion del bit que se
 * desea consultar. Asume que la secuencia esta en orden
 * little-endian. */

int
bit_recuperar (uchar *sec, int pos)
{
    return (int)((sec[pos / 8] >> (7 - (pos % 8))) & 1);
}


/* bits_agregar()
 *
 * Recibe una referencia a una secuencia de bits y la cantidad de bits
 * que contiene. Pide un nuevo valor al usuario y lo agrega al final
 * de la secuencia. Retorna el nuevo numero de elementos de la
 * secuencia. */

int
bits_agregar (uchar **sec, int n)
{
    int nuevo_bit;

    if (n == 0)
        *sec = (uchar *) malloc (sizeof (uchar));
    else if (n % 8 == 0)
        *sec = (uchar *) realloc (*sec, sizeof (uchar) * (n / 8 + 1));

    if (*sec == NULL) {
        printf ("Error de ubicacion de memoria.\n");
        exit (1);
    }

    printf ("\nIngrese el valor del nuevo bit:\n");

    do {
        printf ("> ");
        nuevo_bit = leer_entero ();

        if (nuevo_bit < 0 || nuevo_bit > 1)
            printf ("\nPor favor ingrese 0 o 1.\n");

    } while (nuevo_bit != 0 && nuevo_bit != 1);

    bit_definir (*sec, n, nuevo_bit);

    return n + 1;
}


/* bits_eliminar()
 *
 * Recibe una referencia a una secuencia de bits, y la cantidad de
 * bits que contiene. Esta funcion que pide al usuario una posicion
 * dentro de la secuencia y procede a eliminar el dato en tal
 * posicion. Retorna el nuevo numero de elementos de la secuencia. */

int
bits_eliminar (uchar **sec, int n)
{
    int pos;
    int i;

    if (n < 1) {
        printf ("\nNo hay elementos para eliminar.\n");
        return n;
    }

    else if (n == 1) {
        free (*sec);
        *sec = NULL;

        printf ("\nEl ultimo elemento disponible fue eliminado.\n");
        return 0;
    }

    printf ("\nIngrese la posicion a eliminar (valor entre 0 y %d)\n", n - 1);

    do {
        printf ("> ");
        pos = leer_entero ();

        if (pos < 0 || pos >= n)
            printf ("Ingrese un valor entre 0 y %d\n", n - 1);
    } while (pos < 0 || pos >= n);


    /* Aqui, por sencillez, optaremos por usar nuestras funciones
     * bit_recuperar() y bit_definir(), aunque este proceso de
     * eliminacion podria optimizarse considerablemente usando
     * translaciones directas de bits. */

    for (i = pos + 1; i < n; i++)
        bit_definir (*sec, i - 1, bit_recuperar (*sec, i));

    printf ("\nSe ha eliminado la posicion %d.\n", pos);

    return n - 1;
}


/* bits_listar()
 *
 * Funcion que lista el valor de una secuencia de bits. Recibe la
 * secuencia (en forma de apuntador a ``unsigned char'', y el numero
 * de bits a listar */

void
bits_listar (unsigned char *sec, int n)
{
    int i;

    if (n < 1) {
        printf ("\nNo hay datos en el momento.\n");
        return;
    }

    printf ("\nLa secuencia de bits actual es:\n\n");

    for (i = 0; i < n; i++)
        printf ("%d\t", bit_recuperar (sec, i));

    printf ("\n\nTotal de bits: %d\n", n);
}


/* bits_modificar() Recibe una secuencia de bits y la cantidad de
 * valores en la secuencia. Pide al usuario una posicion a modificar y
 * el nuevo valor para tal posicion, para realizar asi la
 * modificacion. */

void
bits_modificar (uchar *sec, int n)
{
    int nuevo_bit;
    int pos;

    if (n < 1) {
        printf ("\nNo hay datos para modificar.\n");
        return;
    }

    if (n == 1)
        pos = 0;
    else {
        printf ("\nIngrese la posicion a modificar (valor entre 0 y %d)\n",
                n - 1);

        do {
            printf ("> ");
            pos = leer_entero ();

            if (pos < 0 || pos >= n)
                printf ("Ingrese un valor entre 0 y %d\n", n - 1);
        } while (pos < 0 || pos >= n);
    }

    printf ("\nIngrese el nuevo valor del bit No. %d:\n", pos);

    do {
        printf ("> ");
        nuevo_bit = leer_entero ();

        if (nuevo_bit < 0 || nuevo_bit > 1)
            printf ("\nPor favor ingrese 0 o 1.\n");

    } while (nuevo_bit != 0 && nuevo_bit != 1);

    bit_definir (sec, pos, nuevo_bit);
}


/* leer_entero()
 *
 * Funcion que se limita a leer un valor entero desde el teclado */

int
leer_entero (void)
{
    char bufer[TAMBUF];

    fgets (bufer, TAMBUF, stdin);

    return atoi (bufer);
}

/* bits_guardar()
 *
 * Guardar una secuencia de bits en un archivo de disco especificado
 * por el usuario. */

void
bits_guardar (uchar *sec, int n)
{
    FILE *archivo;

    char nombre[TAMBUF];  /* Nombre de archivo */
    int i;

    if (n < 1) {
        printf ("\nNo hay datos para almacenar.\n");
        return;
    }

    printf ("\nIndique el nombre del archivo para almacenar los datos:\n> ");

    fgets (nombre, TAMBUF, stdin);
    arreglar (nombre);

    archivo = fopen (nombre, "wb");

    if (archivo == NULL) {
        printf ("\nNo ha podido abrirse el archivo `%s' para escritura.\n",
                nombre);

        return;
    }

    for (i = 0; i < (n - 1) / 8 + 1; i++)
        fprintf (archivo, "%c", sec[i]);

    if (fclose (archivo) != 0) {
        printf ("\nHa ocurrido un error al cerrar el archivo.\n");
        return;
    }

    printf ("\nSe escribieron %d bits en el archivo.\n", n);
}


/* bits_leer()
 *
 * Funcion que lee desde un archivo especificado por el usuario, un
 * numero de bits dado. Recibe una referencia a la secuencia en donde
 * deben almacenarse los datos, y se devuelve el numero de bits leidos
 * desde el archivo. */

int
bits_leer (uchar **sec)
{
    FILE *archivo;

    char nombre[TAMBUF];  /* Nombre de archivo */
    int i, n;

    printf ("\nIndique el nombre del archivo desde donde leer los datos:\n> ");

    fgets (nombre, TAMBUF, stdin);
    arreglar (nombre);

    printf ("\nIndique el numero de bits a leer:\n> ");
    n = leer_entero ();

    if (n < 1) {
        printf ("No se va a leer menos de 1 bit desde el archivo.\n");
        return 0;
    }

    if (*sec == NULL)
        *sec = (uchar *) malloc (sizeof (uchar) * ((n - 1) / 8 + 1));
    else
        *sec = (uchar *) realloc (*sec, sizeof (uchar) * ((n - 1) / 8 + 1));


    if (*sec == NULL) {
        printf ("Error al asignar memoria.\n");
        exit (1);
    }

    archivo = fopen (nombre, "rb");

    if (archivo == NULL) {
        printf ("\nNo ha podido abrirse el archivo `%s' para lectura.\n",
                nombre);

        exit (1);
    }

    for (i = 0; i < (n - 1) / 8 + 1; i++)
        fscanf (archivo, "%c", &(*sec[i]));

    if (fclose (archivo) != 0) {
        printf ("\nHa ocurrido un error al cerrar el archivo.\n");
        exit (1);
    }

    printf ("\nSe han leido %d bits desde el archivo.\n", n);

    return n;
}
