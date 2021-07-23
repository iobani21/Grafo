#ifdef __APPLE__
#include <GLUT/glut.h>          /* Open GL Util    APPLE */
#else
#include <GL/glut.h>            /* Open GL Util    OpenGL*/
#endif

#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include<math.h>

#define T 4

// Comando para compilar : gcc -fno-stack-protector Predictor.c -lm -lGL -lGLU -lglut
// Variables Globales
//-------------------

float deltaAngle=0.f;
int xOrigin =-1;
float radio = 30, calculo_x, calculo_y;
float posicion_x,posicion_y;
int window;
GLfloat xi=0;
GLfloat yi=0;
float x=100,y=600;
//-------------------
typedef struct elemento_nodo{
    char palabra[20];
    float PA;
    int posicion_x;
    int posicion_y;
    struct elemento_arista *arista;
    struct elemento_nodo *siguiente;
}eNodo;

typedef struct elemento_arista{
    struct elemento_nodo *nodo;
    struct elemento_arista *siguiente;
    float peso;
}eArista;

typedef struct elemento_lista{
    char palabra[20];
    struct elemento_lista *siguiente;
}elista;


//Funciones para Dibujo 
void inicializar_pantalla();
eNodo *grafo;
void display();
void reshape(int w, int h);
void movimiento(int key ,int x , int y);
void dibujar_nodos(eNodo *inicio, char num_nodo); // Dibuja en OPENGL los nodos Grafo ,con la Palabra y su Probabilidad , y dibuja las aristas con los pesos
void DibujarAristas(eNodo *inicio,eArista *arista,int posicionx,int posiciony); // Dibuja las lineas de la aristas en OPENGL
//----------------------------


// Funciones para Gestionar el Grafo
void crear_nodo(eNodo **inicio,FILE *flujo,int contador); // Crea Nodo Grafo
eArista* crear_arista(); //Crea Nodo Arista
void Calcular_Pesos(eArista *arista,float PA,int naristas); //Asigna los Pesos a las Aristas
void Recorrer_Grafo(eNodo *inicio);//Recorre  Los nodos del Grafo
void ordenar_nodos(eNodo **inicio,int matrix[T][T],int fila,int columna,eNodo *auxiliar,eNodo  *CopiaInicio); //Recorre el Grafo y Recorre la lista de palabras del texto
void apuntar_aristas(eArista **arista,eNodo *auxiliar,int columna_auxiliar); // Agrega el nodografo a la lista de Aristas del nodo grafo
void imprimir_grafo(eNodo *inicio); // Imprimer en consola el nodo grafo
void imprimir_aristas(eArista *arista); //Imprime en consola las aristas del nodo Grafo
void valores_ventana(); 

void LeerTexto(FILE *flujo, char *line_buf,size_t line_buf_size,int line_count,ssize_t line_size,struct elemento_lista **inicio); // Le el Texto.txt y separa el texto en parrafo, oraciones y palabras para insertarla en una lista del total de palabras
int Recorrer_lista_1( struct elemento_lista *inicio,int contador); // Cuenta el numero de palabras totaltes
void Insertar_lista1(struct elemento_lista **inicio, char *token); //Inserta una palabra en la lista de palabras totales;
struct elemento_lista* crear_nodo_lista1(char *palabra); // Crea un nodo lista
void Ordenar_Nodos(eNodo **inicio,struct elemento_lista *elemlista,eNodo *inicio_grafo);  //Recorre el Grafo
void Insertar_en_Grafo(eNodo **inicio, struct elemento_lista *elemlista,int npalabras); //Inserta  la palabra de la lista en el grafo y asigna PA al nodo
eNodo* crear_nodos(char *palabra); //Crea un nodo lista 
void Recorre_Lista(eNodo **inicio,struct elemento_lista *inicio_lista,int npalabras); //Recorre los elementos de la lista de palabras
void Buscar_palabras_siguientes( eNodo **inicio,struct elemento_lista *elemlista,eNodo *inicio_grafo); //Busca las palabras siguientes que existen en la lista apartir de la palabra del nodo recorrido 
eNodo *Buscar_en_Grafo( eNodo *inicio_grafo,char *palabra); //Busca en el Grafo una palabra
void Apuntar_Arista(eArista **arista,eNodo *auxiliar); // agrega el nodo grafo a la lista de aristas del nodo grafo
void Eliminar_Grafo(eNodo **inicio); // Elimina los nodos existentes en el grafo

void Predictor(eNodo **inicio,elista **iniciolista); // Actualiza el grafo con una oracion y palabra por palabra predice la palabra siguiente
void PalabrasSiguientes(eNodo *inicio,char palabra[]);//Muesta la lista de palabras siguientes en base a la palabra introducida
void MostrarPalabras(eArista *aux); //Busca e en la lista las aristas de la Palabra
void Eliminar_Lista(elista **inicio); // Elimina los nodos lista

// -----------------------------------



int main(int argc, char *argv[]){

    srand(time(NULL));
    FILE *arch_grafos_datos;
    FILE *flujo;
    int npalabras;  
    struct elemento_lista *inicio_lista=NULL;
    char *line_buf=NULL;
    size_t  line_buf_size=0;
    int line_count=0;
    ssize_t line_size;
    char archivo[20];
    eNodo *auxiliar;
    int w;
    int z;  
    w=0;
    while(w==0)
    {     
        printf("------------Menu-------------\n");
        printf("|1.-Cargar archivo txt|\n");
        printf("|2.Ingresar una Oracion |\n");
        printf("|3.Dibujar Grafo  ( Cerrar Ventana Grafo para continuar)|\n");
        printf("|4.Eliminar Grafo\n|");
        printf("|5.-Salir|\n");
        printf("-----------------------------\n");
        printf("Ingresa una opcion\n");
        scanf("%i",&z);
        system("clear");
        switch(z)
        {
            case 1:
                printf("Ingresar el nombre del archivo  (archivo.txt) \n");
                scanf("%s",archivo);
                printf("Cargando...\n");
                flujo = fopen(archivo, "r+");
                if (!flujo)
                {
                    printf("Error al abrir archivo 1\n");
                    break;      
                }
                LeerTexto(flujo,line_buf,line_buf_size,line_count,line_size,&inicio_lista);
                fclose(flujo);
                npalabras=Recorrer_lista_1(inicio_lista,0);
              
                Eliminar_Grafo(&grafo);
                grafo=NULL;
                x=100;
                y=600;
                Recorre_Lista(&(grafo),inicio_lista,npalabras);
                auxiliar = grafo;      
                Ordenar_Nodos(&(grafo),inicio_lista,auxiliar);                
                Recorrer_Grafo(grafo); 
                printf("Archivo Cargado  N°Palabras del Grafo=%i \n",npalabras );
            break;
            case 2:
            fflush(stdin);
                Predictor(&grafo,&inicio_lista);
                npalabras=Recorrer_lista_1(inicio_lista,0);
                grafo=NULL;
                x=100;
                y=600;
                Recorre_Lista(&(grafo),inicio_lista,npalabras);
                auxiliar = grafo;
                Ordenar_Nodos(&(grafo),inicio_lista,auxiliar);
                Recorrer_Grafo(grafo); 
            break;
            case 3:
                printf("*Mueve con las flechas de navegacion para ver todo el grafo *\n");
                printf("Dibujando Grafo...\n");
                glutInit (&argc, argv); 
                inicializar_pantalla();             
                glutSpecialFunc(movimiento);
                glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS ); 
                
                glutMainLoop();
            break;
            case 4:
                if(grafo==NULL && inicio_lista==NULL)
                {
                    printf("Grafo Vacio\n");
                    break;
                }

                Eliminar_Grafo(&grafo);
                Eliminar_Lista(&inicio_lista);
                inicio_lista=NULL;
                grafo=NULL;
                x=100;
                y=600;
                printf("Grafo Eliminado\n");
            break;
            case 5:
                w=1;
            break;  
        }
    }
 return 0;
}

void inicializar_pantalla(){
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);
    glutInitWindowSize(1200,800); //size de pantalla
    glutInitWindowPosition (100,100);
   glutCreateWindow ("El Predictivo");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
 
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    dibujar_nodos(grafo, '0');
    glutSwapBuffers();
}

void Eliminar_Lista(elista **inicio)
{
    if((*inicio)==NULL)
        return;
    elista *aux;
    aux=(*inicio);
    (*inicio)=(*inicio)->siguiente;
    free(aux);
    Eliminar_Lista(&(*inicio));
    return;
}

void Eliminar_Grafo(eNodo **inicio)
{
    if((*inicio)==NULL)
        return;
    eNodo *aux;
    aux=(*inicio);
    (*inicio)=(*inicio)->siguiente;
    free(aux->arista);
    free(aux);
    Eliminar_Grafo(&(*inicio));
    return;
}

void Predictor(eNodo **inicio,elista **iniciolista)
{
 char palabra[30];
 char aux[50];
 memset(&aux,'\0',50);
 int x;
 int contador=0;
 char delimitador[]=" \n";
 printf("Ingresa el numero de palabras\n");
 scanf("%i",&x);
    while(contador<x)
    {
        printf("***Oracion: %s  ***\n",aux );  
        printf("Ingresa la palabra \n");
        scanf("%s",palabra);
        PalabrasSiguientes((*inicio),palabra);
        contador++;
        strcat(aux,palabra);
        if(contador<x)
        strcat(aux," ");        
        else
        strcat(aux,".");     
        }

 printf("Actualizando grafo\n");
 Eliminar_Grafo(&(*inicio));
 printf("Grafo eliminado\n");
 char *token=strtok(aux,delimitador);
    if(token!=NULL)
    {
        while(token!=NULL)
        {
            //printf("la palabra es %s \n",token );
            Insertar_lista1(&(*iniciolista),token);
            token=strtok(NULL,delimitador);
        }
    }
return;
}

void PalabrasSiguientes(eNodo *inicio,char palabra[])
{
    eNodo *aux= Buscar_en_Grafo(inicio,palabra);
    if(aux!=NULL)
    {
        printf("Palabras Siguientes %s \n",aux->palabra );
        printf("-------------------\n");
        MostrarPalabras(aux->arista);
        printf("-------------------\n");
        return;
    }
    printf("No existe palabra siguiente\n");
    return;
}

void MostrarPalabras(eArista *arista){
    if(arista==NULL)    
        return;

    printf("|%s|\n",arista->nodo->palabra);
    MostrarPalabras(arista->siguiente);
    return;
}




void LeerTexto(FILE *flujo, char *line_buf,size_t line_buf_size,int line_count,ssize_t line_size,struct elemento_lista **inicio)
{    
 line_size=getline(&line_buf,&line_buf_size,flujo);
 if(line_size>=0)
 {
    fflush(stdin);
    char delimitador[]=" \n";
    char *token;
    //printf("El parrafo  es %s  \n",line_buf );
    token=strtok(line_buf,delimitador);
    if(token!=NULL)
        {
            while(token!=NULL)
            {
                Insertar_lista1(&(*inicio),token);
                token=strtok(NULL,delimitador);                    
            }
        }
     line_count++;           
     LeerTexto(flujo,line_buf,line_buf_size,line_count,line_size,inicio);
    }
 return ;
}

int Contar_Aristas(eArista *arista,int contador){
    if(arista==NULL)
        return contador;
 contador++;
 return Contar_Aristas(arista->siguiente,contador);
}

void Recorrer_Grafo(eNodo *inicio)
{
 if(inicio==NULL)
    return;
 int x=Contar_Aristas(inicio->arista,0);
 Calcular_Pesos(inicio->arista,inicio->PA,x);
 Recorrer_Grafo(inicio->siguiente);
 return;
}

void Calcular_Pesos(eArista *arista,float PA,int naristas)
{
 if(arista==NULL)
    return;
 double PBA;
 if(PA!=0)
 {
    PBA=(1./naristas)/PA;
    arista->peso=(PBA*PA)/arista->nodo->PA;
 }
 Calcular_Pesos(arista->siguiente,PA,naristas);
 return;
}

int Recorrer_lista_1( struct elemento_lista *inicio,int contador)
{
    if(inicio==NULL)
        return contador;
    contador++;
    return Recorrer_lista_1(inicio->siguiente,contador);
}
void Insertar_lista1(struct elemento_lista **inicio, char *token)
{
    if((*inicio)==NULL)
    {
        (*inicio)=crear_nodo_lista1(token);
        return;
    }
    Insertar_lista1(&(*inicio)->siguiente,token);
    return;
}



void Insertar_en_Grafo(eNodo **inicio, struct elemento_lista *elemlista,int npalabras)
{
    if((*inicio)==NULL)
    {
        //printf("nodo creado palabra :%s \n",elemlista->palabra );
        (*inicio)=crear_nodos(elemlista->palabra);
        (*inicio)->PA=(*inicio)->PA/npalabras;
        return;
    }
    char delimitador[]=" \n";
    char *token;
    char copia[20];
    if(strcmp((*inicio)->palabra,elemlista->palabra)==0)
    {
        //printf("palabra repetida %s \n" ,(*inicio)->palabra);    
        double aux;
        aux=1./npalabras;
        (*inicio)->PA=(*inicio)->PA+aux;
        return;
    }
    Insertar_en_Grafo(&(*inicio)->siguiente,elemlista,npalabras);
    return;
}

void Recorre_Lista(eNodo **inicio,struct elemento_lista *inicio_lista , int npalabras)
{
    if(inicio_lista==NULL)
    return;
    Insertar_en_Grafo(&(*inicio),inicio_lista,npalabras);
    Recorre_Lista(&(*inicio),inicio_lista->siguiente,npalabras);
    return;
}

struct elemento_lista* crear_nodo_lista1(char *palabra)
{
    struct elemento_lista *nuevo = (struct elemento_lista*)malloc(sizeof(struct elemento_lista));
    if(!nuevo){
        return NULL;
    }
    strcpy(nuevo->palabra,palabra);
    nuevo->siguiente = NULL;
    return nuevo;
}


eNodo* crear_nodos(char *palabra)
{
    eNodo *nuevo = (eNodo*)malloc(sizeof(eNodo));
    if(!nuevo){
        return NULL;
    }
    strcpy(nuevo->palabra,palabra);
    nuevo->PA=1;
    nuevo->posicion_x = x;
    nuevo->posicion_y = y;
    nuevo->arista = NULL;
    nuevo->siguiente = NULL;


    x=x+500;
    if(x>=1500)
    {
        x=100;
        y=y-100;
    }
    return nuevo;
}

eArista* crear_arista()
{
    eArista *nueva = (eArista*)malloc(sizeof(eArista));
    if(!nueva){
        return NULL;
    }
    nueva->nodo = NULL;
    nueva->siguiente = NULL;
    nueva->peso=0;
    return nueva;
}

void Ordenar_Nodos(eNodo **inicio,struct elemento_lista *elemlista,eNodo *inicio_grafo)
{
    if((*inicio)==NULL)
        return;
    //printf("Ordenando palabra %s \n",(*inicio)->palabra );
    Buscar_palabras_siguientes(&(*inicio),elemlista,inicio_grafo);
    Ordenar_Nodos(&(*inicio)->siguiente,elemlista,inicio_grafo);
    return;
}


void Buscar_palabras_siguientes( eNodo **inicio,struct elemento_lista *elemlista,eNodo *inicio_grafo)
{
    int band=0;
    int band2=0;
    char *recorre;
    if(elemlista==NULL)
        return;




        if(strcmp((*inicio)->palabra,elemlista->palabra)==0)
        {
            if(elemlista->siguiente!=NULL)
            {

                    recorre=&elemlista->palabra[0];
                    while(*recorre!='\0')
                  {
                if(*recorre=='.' || *recorre==',' || *recorre=='?' ||*recorre=='!' || *recorre==';' || *recorre==':')
                      band=1;
                 recorre++;
                }
    

     if(band==0 && band2==0)
    {
        
                eNodo *aux=Buscar_en_Grafo(inicio_grafo,elemlista->siguiente->palabra);
                Apuntar_Arista(&(*inicio)->arista,aux); 
    }

                 
            }
        }
    
    Buscar_palabras_siguientes(&(*inicio),elemlista->siguiente,inicio_grafo);
    return;
}

eNodo *Buscar_en_Grafo( eNodo *inicio_grafo,char *palabra)
{
    if(inicio_grafo==NULL)
        return NULL;

    if(strcmp(inicio_grafo->palabra,palabra)==0){
        return inicio_grafo;
    }
    return Buscar_en_Grafo(inicio_grafo->siguiente,palabra);    
}

void Apuntar_Arista(eArista **arista,eNodo *aux)
{
    if((*arista)==NULL)
    {
        (*arista)=crear_arista();
        (*arista)->nodo=aux;
        (*arista)->siguiente=NULL;
        //printf("arista vale %s \n",(*arista)->nodo->palabra );
        return;
    }
    if(strcmp((*arista)->nodo->palabra,aux->palabra)==0)
    return;

    Apuntar_Arista(&(*arista)->siguiente,aux);
    return;
}

void imprimir_grafo(eNodo *inicio)
{
    if(!inicio){
        return;
    }
    printf("\nNodo: %s \n",inicio->palabra);
    printf("Posicion Nodo: %d\n",inicio->posicion_x);
    printf("Posicion Nodo: %d\n",inicio->posicion_y);
    printf("Repeticiones %f \n",inicio->PA );
    imprimir_aristas(inicio->arista);
    imprimir_grafo(inicio->siguiente);
}

void imprimir_aristas(eArista *arista)
{
    if(!arista || !arista->nodo){
        return;
    }
    printf("Apunta a Nodo %s\n",arista->nodo->palabra);
    printf("PESO %f\n",arista->peso );
    printf("--------\n");
    imprimir_aristas(arista->siguiente);
}

void valores_ventana(eNodo *inicio)
{
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glutPostRedisplay();
    glFlush();
}

void reshape(int w, int h){
    if(w==0||h==0){
        return;
    }
    glViewport(0,0,w,h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluOrtho2D(0,w,0,h);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
}

void dibujar_nodos(eNodo *inicio, char num_nodo)
{
 
    if (!inicio){
        glEnd();    //termina de dibujar puntos
        glFlush (); //para poder usar la ventana
        return;
    }
    // printf("dibujando %s \n",inicio->palabra );
    int posicionx = inicio->posicion_x;
    int posiciony = inicio->posicion_y;
    int posicionx2, posiciony2;
   // glClear(GL_COLOR_BUFFER_BIT); //limpiar ventana en el color del clear color definido en glClearColor()
    glBegin(GL_POINTS); //le indicamos que va a dibujar puntos
    glColor3f(0.0,1.0,0.0);

    glRasterPos2i(0,0);
    for(double i=0.0; i<100; i+=0.001)
    {
        calculo_x = radio*cos(i)+posicionx;
        calculo_y = radio*sin(i)+posiciony;
        glVertex2f(calculo_x,calculo_y);
    }  
    glEnd();    //termina de dibujar puntos
    glRasterPos2i(posicionx,posiciony);
    glColor3f(0.0,0.0,1.0);

    char cadena[20];
    char PA[10];
    strcpy(cadena,inicio->palabra);
    char delimitador[]=".,;?!-\n";
    char *token;
    token=strtok(cadena,delimitador);

    sprintf(PA,"%f",inicio->PA);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, token);
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, "/");
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10,PA);  
    glEnd();    //termina de dibujar letras
    //-------------------------- dibuja las aristas de cada nodo ------------------
    if(inicio->arista!=NULL)
    {
        DibujarAristas(inicio,inicio->arista,posicionx,posiciony);
    //----------------------------------------------------------------
    }   
  
    dibujar_nodos(inicio->siguiente, num_nodo += 1);
    return;
}

void DibujarAristas(eNodo *inicio,eArista *arista,int posicionx,int posiciony)
{
    if(arista==NULL)
        {
        glEnd();    //termina de dibujar puntos
        glFlush (); //para poder usar la ventana
        return;
    }
    int posicionx2 = arista->nodo->posicion_x;
    int posiciony2 = arista->nodo->posicion_y;
    int z=(posicionx+posicionx2)/2;
    int w=(posiciony+posiciony2)/2; 
    char peso[10];


        glBegin(GL_LINES); //se define que se dibujaran lineas
        glColor3f(1.0,0.0,0.0); //color de la linear_congruential_engine
        glVertex2i(posicionx,posiciony); glVertex2i(posicionx2,posiciony2); 
        glEnd();
        glColor3f(0.0,1.0,0.0); 
        sprintf(peso,"%f",arista->peso);         
        glRasterPos2i(z,w);    
        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10,peso);
        glEnd();  

    
    DibujarAristas(inicio,arista->siguiente,posicionx,posiciony);
    
    
    return;
}

void movimiento(int key ,int x , int y){
    if(key==GLUT_KEY_UP){
        glTranslatef(0.0,-10.0,0.0);
    }
    if(key==GLUT_KEY_DOWN){
        glTranslatef(0.0,+10.0,0.0);
    }

       if(key==GLUT_KEY_LEFT){
        glTranslatef(+10.0,0.0,0.0);
    }
     if(key==GLUT_KEY_RIGHT){
        glTranslatef(-10.0,0.0,0.0);
    }

  glutPostRedisplay();
    return;
}
