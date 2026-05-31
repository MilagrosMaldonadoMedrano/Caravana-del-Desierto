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
#define ENCONTRADO 7

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

typedef void (*tAccion)(const void* elem1,const void* elem2);

typedef int (*tCmp)(const void* a,const void* b);


void crearLista(tLista* pl);
void vaciarLista(tLista* pl);
int insertarAlComienzo(tLista* pl,const void* elem,unsigned cantBytes);
int insertarAlFinal(tLista* pl,const void* elem,unsigned cantBytes);
int listaVacia(tLista* pl);
int listaLlena(tLista* pl,void* elem,unsigned cantBytes);

int listaInsertarOrdenado(tLista* pl, const void* dato, unsigned tamElem,tCmp cmp,tAccion accion);
int eliminarListaOrdenadaPorClave(tLista* pl, void* dato, unsigned tamElem,tCmp cmp);
int eliminarListaDesordenadaPorClaveSinDup(tLista* pl, void* dato, unsigned tamElem, tCmp cmp);
int eliminarListaDesordenadaPorClave(tLista* pl, void* dato, unsigned tamElem,tCmp cmp,tAccion accion);
int listaInsertarAlFinal(tLista* pl, const void* dato, unsigned tamElem);
int listaEliminarPrimerElemento(tLista* pl, void* dato, unsigned tamElem);
void* buscarElementoLista(tLista* pl, void* dato,tCmp cmp);
int buscarElementoEnLista(tLista* pl, void* dato, unsigned tamElem, tCmp cmp, tAccion accion, const void* extra);
int recuperarUltElemOperadoEnLista(tLista* pl, void* dato, unsigned tamElem);

int recuperarElementoXPosLista(tLista* pl, void* dato, unsigned tamElem, unsigned posDato);

unsigned listaCantidadElementos(const tLista* pl);

void ordenarLista(tLista* pl,tCmp cmp);

int recorrerDeDerAIzq(tLista* pl,tAccion accion);
int recorrerDeIzqADer(tLista* pl, tAccion accion, const void* extra);

#endif // LISTA_H_INCLUDED
