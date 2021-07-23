#ifdef __APPLE__
#include <GLUT/glut.h>          /* Open GL Util    APPLE */
#else
#include <GL/glut.h>            /* Open GL Util    OpenGL*/
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include<math.h>

#define T 5

float radio = 40, calculo_x, calculo_y;
float posicion_x,posicion_y;

typedef struct elemento_nodo{
	int dato;
	int posicion_x;
	int posicion_y;
	struct elemento_arista *arista;
	struct elemento_nodo *siguiente;
}eNodo;

typedef struct elemento_arista{
	struct elemento_nodo *nodo;
	struct elemento_arista *siguiente;
}eArista;

eNodo *inicio = NULL;

void llenar_matrix(int matrix[T][T],FILE *arch_grafos_datos);
void crear_nodo(eNodo **inicio,int numero);
eNodo* crear_nodos(int numero);
eArista* crear_arista();
void ordenar_nodos(eNodo **inicio,int matrix[T][T],int fila,int columna,eNodo *auxiliar);
void apuntar_aristas(eArista **arista,eNodo *auxiliar,int columna_auxiliar);
void imprimir_grafo(eNodo *inicio);
void imprimir_aristas(eArista *arista);
void valores_ventana();
void dibujar_nodos(eNodo *inicio, char num_nodo);

void menucito(int opcion);
void insertar_2(eNodo **inicio, int bandera);
void mouse_control(int button,int state,int x,int y);

int main(int argc, char *argv[]){

	srand(time(NULL));

	FILE *arch_grafos_datos;
	int matrix[T][T];

	arch_grafos_datos = fopen("datos_grafo.txt", "r+");

	if (!arch_grafos_datos){
		printf("Error al abrir archivo 1\n");
		return 0;		
	}

	else{
		printf("Archivo abierto correctamente\n");
	}

	llenar_matrix(matrix, arch_grafos_datos);
	fclose(arch_grafos_datos);

	printf("\nDatos en Matriz: \n");
	for (int i = 0; i < 5; i++){
		for (int j = 0; j < 5; j++){
			printf("%i\n", matrix[i][j]);
		}
	}

	crear_nodo(&inicio,0);
	eNodo *auxiliar = inicio;
	ordenar_nodos(&inicio,matrix,0,0,auxiliar);
	imprimir_grafo(inicio);

	glutInit (&argc, argv);	
	glutInitDisplayMode (GLUT_RGBA|GLUT_SINGLE);
	glutInitWindowSize (600,600);	
	glutInitWindowPosition (100,100);
	glutCreateWindow ("grafos1");
	valores_ventana();

	glutCreateMenu(menucito);
	glutAddMenuEntry("Agregar Nuevo Nodo",0);
	glutAddMenuEntry("Salir",1);
	
	glutMouseFunc(mouse_control);

	dibujar_nodos(inicio, '0');
	glutMainLoop(); 

	return 0;
}

void llenar_matrix(int matrix[T][T],FILE *arch_grafos_datos){
	int dato;

	for(int i=0;i<T;i++){
		for(int j=0;j<T;j++){
			fscanf(arch_grafos_datos,"%d",&dato);
			printf("%d\n",dato);
			matrix[i][j] = dato;
		}
	}
	return;
}

void crear_nodo(eNodo **inicio,int numero){
	if(numero == 5){
		return;
	}
	if(!(*inicio)){
		(*inicio) = crear_nodos(numero);
		(*inicio)->arista = crear_arista();
	}

	numero+=1;
	crear_nodo(&(*inicio)->siguiente,numero);
}

eNodo* crear_nodos(int numero){
	eNodo *nuevo = (eNodo*)malloc(sizeof(eNodo));
	if(!nuevo){
		return NULL;
	}
	nuevo->dato = numero;
	nuevo->posicion_x = -480+rand()%(-480-480);
	nuevo->posicion_y = -480+rand()%(-480-480);
	nuevo->arista = NULL;
	nuevo->siguiente = NULL;
	return nuevo;
}

eArista* crear_arista(){
	eArista *nueva = (eArista*)malloc(sizeof(eArista));
	if(!nueva){
		return NULL;
	}
	nueva->nodo = NULL;
	nueva->siguiente = NULL;
	return nueva;
}

void ordenar_nodos(eNodo **inicio,int matrix[T][T],int fila,int columna,eNodo *auxiliar){
	if(fila==T){
		return;
	}
	if(columna==T){
		ordenar_nodos(&(*inicio)->siguiente,matrix,fila+=1,columna=0,auxiliar);
	}
	if(matrix[fila][columna]==0){
		ordenar_nodos(&(*inicio),matrix,fila,columna+=1,auxiliar);
	}
	if(matrix[fila][columna]==1){
		int columna_auxiliar = columna;
		apuntar_aristas(&(*inicio)->arista,auxiliar,columna_auxiliar);
		matrix[fila][columna] = 0;
		ordenar_nodos(&(*inicio),matrix,fila,columna+=1,auxiliar);
	}
}

void apuntar_aristas(eArista **arista,eNodo *auxiliar,int columna_auxiliar){
	if(columna_auxiliar){
		if(!(*arista)){
			(*arista) = crear_arista();
		}
		if((*arista)->nodo == NULL){
			(*arista)->nodo = crear_nodos(auxiliar->dato);
			(*arista)->nodo->posicion_x = auxiliar->posicion_x;
			(*arista)->nodo->posicion_y = auxiliar->posicion_y;
			(*arista)->siguiente = NULL;
		}
		else{
			apuntar_aristas(&(*arista)->siguiente,auxiliar,columna_auxiliar);
		}
		return;
	}
	apuntar_aristas(&(*arista),auxiliar->siguiente,columna_auxiliar-=1);
}

void imprimir_grafo(eNodo *inicio){
	if(!inicio){
		return;
	}
	printf("\nNodo: %d\n",inicio->dato);
	printf("Posicion Nodo: %d\n",inicio->posicion_x);
	printf("Posicion Nodo: %d\n",inicio->posicion_y);
	imprimir_aristas(inicio->arista);
	imprimir_grafo(inicio->siguiente);
}

void imprimir_aristas(eArista *arista){
	if(!arista || !arista->nodo){
		return;
	}
	printf("Apunta a Nodo %d\n",arista->nodo->dato);
	printf("--------\n");
	imprimir_aristas(arista->siguiente);
}

void valores_ventana(){
	glClearColor (0.0, 0.0, 0.0, 0.0);  
	glMatrixMode(GL_PROJECTION); 
	gluOrtho2D(-500,500,-500,500);
}

void dibujar_nodos(eNodo *inicio, char num_nodo){

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
        calculo_x = radio*cos(i)+posicionx;
        calculo_y = radio*sin(i)+posiciony;
        glVertex2f(calculo_x,calculo_y);
    }  
    glEnd();    //termina de dibujar puntos

    glRasterPos2i(posicionx,posiciony);
	glColor3f(0.0,0.0,1.0);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, num_nodo);
    glEnd();    //termina de dibujar letras
    //-------------------------- dibuja las aristas de cada nodo ------------------
    do{
    	posicionx2 = inicio->arista->nodo->posicion_x;
    	posiciony2 = inicio->arista->nodo->posicion_y;

    	if (posicionx == posicionx2 && posiciony == posiciony2) //si la posicion inicial y final son iguales 
    	{														// esta apuntando a el mismo
    		glBegin(GL_POINTS); //le indicamos que va a dibujar puntos
    		glColor3f(1.0,0.0,0.0);	//color de la linea
    	    posicionx -= 40;
    	    posiciony += 40;

    		for(double i=0.0; i<4.7; i+=0.001)		
    		{
        		calculo_x = radio*cos(i)+posicionx;
        		calculo_y = radio*sin(i)+posiciony;
        		glVertex2f(calculo_x,calculo_y);
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
	dibujar_nodos(inicio->siguiente, num_nodo += 1);
}

void menucito(int opcion){
	switch(opcion){
		case 0:
		printf("\n¿Entro?\n");
		insertar_2(&inicio,0);
		break;
		case 1:
		exit(-1);
		break;
	}
}

void mouse_control(int button,int state,int x,int y){
	if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
		glutAttachMenu(GLUT_LEFT_BUTTON);
	}
}

void insertar_2(eNodo **inicio,int bandera){
	int numero;
	if(bandera==1){
		return;
	}
	numero = (*inicio)->dato;
	printf("%d\n",numero);

	if(!(*inicio)){
		printf("\n¿Entro?\n");
		numero += 1;
		(*inicio) = crear_nodos(numero);
		(*inicio)->arista = crear_arista();
		bandera = 1;
	}
	insertar_2(&(*inicio)->siguiente,bandera);
}