#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#define VERDADERO 1
#define FALSO 0


#define TODO_OK 1
#define LISTA_LLENA 2
#define LISTA_VACIA 3
#define LISTA_NO_VACIA 4
#define LISTA_NO_LLENA 5
#define DUPLICADO 6

#define NO_ENCONTRADO -1
#define ERROR_MEM -2
typedef struct sNodoD
{
    void* info;
    unsigned tamElem;
    struct sNodoD* sig;
    struct sNodoD* ant;
}tNodoD;

typedef tNodoD* tLista;

typedef void (*Accion)(const void* elem1,const void* elem2);

typedef int (*Cmp)(const void* a,const void* b);


void crearLista(tLista* pl);
void vaciarLista(tLista* pl);
int insertarAlFinal(tLista* pl,const void* elem,unsigned cantBytes);
int listaVacia(tLista* pl);
int listaLlena(tLista* pl,void* elem,unsigned cantBytes);

int listaInsertarOrdenado(tLista* pl, const void* dato, unsigned tamElem,Cmp cmp,Accion accion);
int eliminarListaOrdenadaPorClave(tLista* pl, void* dato, unsigned tamElem,Cmp cmp);
int eliminarListaDesordenadaPorClave(tLista* pl, void* dato, unsigned tamElem,Cmp cmp,Accion accion);
int listaInsertarAlFinal(tLista* pl, const void* dato, unsigned tamElem);
int listaEliminarPrimerElemento(tLista* pl, void* dato, unsigned tamElem);
void* buscarElementoLista(tLista* pl, void* dato,Cmp cmp);

unsigned listaCantidadElementos(const tLista* pl);

void ordenarLista(tLista* pl,Cmp cmp);

int recorrerDeDerAIzq(tLista* pl,Accion accion);
int recorrerDeIzqADer(tLista* pl, Accion accion, const void* extra);

#endif // LISTA_H_INCLUDED
