#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <GLUT/glut.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
#endif
# include<math.h>
float  radio = 40, calculox, calculoy;
float posicionx, posiciony;
//*****************************************
typedef struct Nodo
{
	int dato;
	int posicion_x;
	int posicion_y;
	struct Aristas *arista;
	struct Nodo *siguiente_nodo;
}nodo;

typedef struct Aristas
{
	struct Nodo *nodo_apuntado;
	struct Aristas *siguiente;
}arist;
//******************************************
void acomodar_nodos(nodo **inicio, int matriz[5][5], int fila, int columna, nodo *auxiliar);
void crear(nodo **inicio, int cantidad);
void apuntar_aristas(struct Aristas **arista, nodo *auxiliar, int comodin);
void imprimir_grafo(nodo *inicio);
void mostrar_aristas(arist *arista);
arist *crear_arista();
nodo *crear_nodos(int cantidad);
void llenar_archivo(FILE *datos_grafos, int contador);
void llenar_matriz(int matriz[5][5], FILE *datos_grafos);
//********************************************
void valores_ventana(){
	glClearColor (0.0, 0.0, 0.0, 0.0);  // color de la pantalla
	glMatrixMode(GL_PROJECTION); //hacer uso de la pantalla
	gluOrtho2D(-500,500,-500,500); //rango del label
}
//************************************************
void dibujar_nodos(nodo *inicio, char num_nodo){

	if (!inicio){
	    glEnd();    //termina de dibujar puntos
		glFlush (); //para poder usar la ventana
		return;
	}

    int posicionx = inicio->posicion_x;
    int posiciony = inicio->posicion_y;
    int posicionx2, posiciony2;

    glClear(GL_COLOR_BUFFER_BIT); //limpiar ventana en el color del clear color definido en glClearColor()
    glBegin(GL_POINTS); //le indicamos que va a dibujar puntos
    glColor3f(0.0,1.0,0.0);

    for(double i=0.0; i<100; i+=0.001)
    {
        calculox = radio*cos(i)+posicionx;
        calculoy = radio*sin(i)+posiciony;
        glVertex2f(calculox,calculoy);
    }  
    glEnd();    //termina de dibujar puntos

    glRasterPos2i(posicionx,posiciony);
	glColor3f(0.0,0.0,1.0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, num_nodo);
    glEnd();    //termina de dibujar letras
    //-------------------------- dibuja las aristas de cada nodo ------------------
    do{
    	posicionx2 = inicio->arista->nodo_apuntado->posicion_x;
    	posiciony2 = inicio->arista->nodo_apuntado->posicion_y;

    	if (posicionx == posicionx2 && posiciony == posiciony2) //si la posicion inicial y final son iguales 
    	{														// esta apuntando a el mismo
    		glBegin(GL_POINTS); //le indicamos que va a dibujar puntos
    		glColor3f(1.0,0.0,0.0);	//color de la linea
    	    posicionx -= 40;
    	    posiciony += 40;

    		for(double i=0.0; i<4.7; i+=0.001)		
    		{
        		calculox = radio*cos(i)+posicionx;
        		calculoy = radio*sin(i)+posiciony;
        		glVertex2f(calculox,calculoy);
    		}	
   			glEnd();    //termina de dibujar puntos
   			posicionx += 40, posiciony -= 40;
    	}

    	else{
			glBegin(GL_LINES); //se define que se dibujaran lineas
    		glColor3f(1.0,0.0,0.0);	//color de la linear_congruential_engine
	    	glVertex2i(posicionx,posiciony); glVertex2i(posicionx2,posiciony2);
    	}
	    
	    inicio->arista = inicio->arista->siguiente;	
    }while(inicio->arista != NULL);
    //----------------------------------------------------------------
	dibujar_nodos(inicio->siguiente_nodo, num_nodo += 1);
}
//******************************************
int main(int argc, char **argv)
{
	srand(time(NULL));
	nodo *inicio = NULL;
	FILE *datos_grafos;
	int matriz[5][5];

	datos_grafos = fopen("grafos.txt", "r+");

	if (!datos_grafos){
		printf("Error al abrir archivo 1\n");		
		return 0;
	}

	else
		printf("Archivo abierto correctamente\n");

	llenar_matriz(matriz, datos_grafos);
	fclose(datos_grafos);

	for (int i = 0; i < 5; i++){
		for (int j = 0; j < 5; j++){
			printf("%i\n", matriz[i][j]);
		}
	}

	//------------------------------
	crear(&inicio, 0);
	nodo *auxiliar = inicio;
	acomodar_nodos(&inicio, matriz, 0, 0, auxiliar);
	imprimir_grafo(inicio);

	//----------Grafico-------------

	glutInit (&argc, argv);	//inicializa las funciones de opengl
	glutInitDisplayMode (GLUT_RGBA|GLUT_SINGLE);//propiedades y atributos de la pantalla
	glutInitWindowSize (600,600);	//tamaño de la ventana
	glutInitWindowPosition (100,100);	//posicion de la ventana
	glutCreateWindow ("Grafos 1");	//nombre que tendra la ventana
	valores_ventana();
	dibujar_nodos(inicio, '0');
	glutMainLoop(); //cierra el programa hasta que cerremos la pantalla

	//------------------------------

	return 0;
}
//********************************************
void llenar_matriz(int matriz[5][5], FILE *datos_grafos){
	int dato;

	for (int i = 0; i < 5; i++){
		for (int j = 0; j < 5; j++){
			fscanf(datos_grafos, "%i", &dato);
			printf("%i\n", dato);
			matriz[i][j] = dato;
		}
	}
	printf("----------------------\n");
	return;
}
//********************************************
void llenar_archivo(FILE *datos_grafos, int contador){
	int dato;

	if (contador == 25)
		return;
	dato = 0 + rand()% (0 - 2);
	fprintf(datos_grafos, "%i\n", dato);
	llenar_archivo(datos_grafos, contador += 1);
}
//********************************************
nodo *crear_nodos(int cantidad){

	nodo *nuevo_n  = (nodo*)malloc(sizeof(nodo));

	if (!nuevo_n){
		printf("Desbordamiento de memoria\n");
		return nuevo_n = NULL;
	}

	nuevo_n->dato           = cantidad;
	nuevo_n->posicion_x		= -480 + rand()% (-480 - 480);
	nuevo_n->posicion_y		= -480 + rand()% (-480 - 480);
	nuevo_n->arista         = NULL;
	nuevo_n->siguiente_nodo = NULL;

	return nuevo_n;
}
//********************************************
arist *crear_arista(){

	arist *nueva_a = (arist*)malloc(sizeof(arist));

	if (!nueva_a){
		printf("Desbordamiento de memoria\n");
		return nueva_a = NULL;
	}

	nueva_a->nodo_apuntado  = NULL;
	nueva_a->siguiente      = NULL;	

	return nueva_a;
}
//********************************************
void crear(nodo **inicio, int cantidad){

	if (cantidad == 5)
		return;

	if (!(*inicio))
	{
		(*inicio) = crear_nodos(cantidad);
		(*inicio)->arista = crear_arista();
	}

	cantidad += 1;
	crear(&(*inicio)->siguiente_nodo, cantidad);
}
//******************************************
void mostrar_aristas(arist *arista){

	if (!arista || !arista->nodo_apuntado)
		return;

	printf("Apunta a nodo %i\n", arista->nodo_apuntado->dato);
	printf("--------\n");	
	mostrar_aristas(arista->siguiente);
}
//******************************************
void imprimir_grafo(nodo *inicio){

	if (!inicio)
		return;

	printf("\nNodo %i\n", inicio->dato);
	printf("posicion nodo %i\n", inicio->posicion_x);
	printf("posicion nodo %i\n", inicio->posicion_y);
	mostrar_aristas(inicio->arista);
	imprimir_grafo(inicio->siguiente_nodo);
}
//******************************************
void acomodar_nodos(nodo **inicio, int matriz[5][5], int fila, int columna, nodo *auxiliar){

	if (fila == 5)
		return;
	
	if (columna == 5)
		acomodar_nodos(&(*inicio)->siguiente_nodo, matriz, fila += 1, columna = 0, auxiliar);

	if (matriz[fila][columna] == 0)
		acomodar_nodos(&(*inicio), matriz, fila, columna += 1, auxiliar);

	if (matriz[fila][columna] == 1)
	{
		int comodin = columna;
		apuntar_aristas(&(*inicio)->arista, auxiliar, comodin);
		matriz[fila][columna] = 0;
		acomodar_nodos(&(*inicio), matriz, fila, columna += 1, auxiliar);
	}
}
//*********************************************
void apuntar_aristas(struct Aristas **arista, nodo *auxiliar, int comodin){

	if (comodin == 0){

		if (!(*arista))
			(*arista) = crear_arista();
		
		if ((*arista)->nodo_apuntado == NULL)
		{
			(*arista)->nodo_apuntado = crear_nodos(auxiliar->dato);
			(*arista)->nodo_apuntado->posicion_x = auxiliar->posicion_x;
			(*arista)->nodo_apuntado->posicion_y = auxiliar->posicion_y;
			(*arista)->siguiente = NULL;
		}
		else
			apuntar_aristas(&(*arista)->siguiente, auxiliar, comodin);

		return;
	}

	apuntar_aristas(&(*arista), auxiliar->siguiente_nodo, comodin -= 1);
}
