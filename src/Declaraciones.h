

#define SUFFRAGETTO 's'
#define LIDERSU 'l'
#define POLICIA 'p'
#define INSPECTORP 'i'
#define VACIO 'v'

#define ARENA 'A'
#define NEUTRALG 'N'
#define HOSPITAL 'H'
#define HGROUNDS 'G'
#define PRISON 'P'
#define PYARD 'Y'
#define ALBERTH 'h'
#define HOUSEC 'c'

char imgfichas [][25] = {"./img/vacio.png",
		"./img/suf.png",
		"./img/pol.png",
		"./img/lidersu.png",
		"./img/inspectorp.png"};

char imgtablero [][25] = {"./img/arena.png",
		"./img/neutral.png",
		"./img/goal.png",
		"./img/dead.png",
		"./img/yard.png"};

char imgtableromov [][25] = {"./img/arenamov.png",
		"./img/neutralmov.png",
		"./img/goalmov.png"};

char imgtablerosel [][25] = {"./img/arenasel.png",
		"./img/neutralsel.png",
		"./img/goalsel.png"};


struct posicion {
	int F;
	int C;
	};

struct movimiento {
	struct posicion p;
	struct posicion FSaltada; //ficha saltada
	int Saltos;	// numero de saltos
	};

int pixel = 100;

char tablero_fichas[17][18];

char tablero_paralelo[17][18];

char jugador = SUFFRAGETTO;




// coloca el tablero en la posicion inicial del juego
void preparar_tablero();

void mostrar_movimientos(struct movimiento moves[],struct posicion P,int cantmove);

void deseleccionar_mov(struct movimiento moves[],struct posicion P,int cantmove);


int guardar_movimientos(struct movimiento moves[], struct posicion Pini, int numsalto);

gboolean agrandar_tablero (GtkWidget* self,GdkEventButton event,gpointer user_data);
gboolean reducir_tablero(GtkWidget* self,GdkEventButton event,gpointer user_data);

void rescale_tablero(int opcion);

//Pide las cordenadas del movimiento y ejecuta las funciones del turno
// jugador = identificador de quien es el turno
void turno(char jugador);

// Pide un numero entero entre los limites incluyendolos, retorna ese numero
int pedir_numero_entero(int liminf, int limsup);

//Se encarga de ver si el movimiento que selecciono esta dentro del vector moves con los movimientos posibles
//Siendo Pini la posicion inicial, Pfin la posicion final y cantmov la cantidad de movimientos que puede hacer
//retorna 1 si se puede hacer el movimiento, 0 si no y -1 si es que se eligio la misma casilla inicial
int verificar_movimiento(struct movimiento moves[],struct posicion Pini, struct posicion Pfin, int cantmov);

//Determina si la ficha que selecciona es valida para hacer un movimiento
// Retorna TRUE o FALSE
int Es_ficha_suya(struct posicion pos,char jugador);

//Determina si existe un salto a la ficha seleccionada
// Retorna TRUE o FALSE
int Existe_salto_posible(int Posx,int Posy);

//cambia de turno, si era suffragetto ahora policia, y al reves
void cambiar_jugador();

//Determina que la posicion este en la arena, Neutral grounds, albert hall o house of commons
int Es_permitido(int Posxf,int Posyf);

//Determina si hubo una captura de ficha
// Retorna TRUE o FALSE
//ficha = tipo de ficha que se mueve
int verificar_captura(struct movimiento mov,char ficha);

//Mueve la ficha capturada al hospital o prision
void hacer_captura(struct posicion Psaltada);

//Mueve la ficha a la posicion seleccionada
void hacer_movimiento(struct posicion Pini,struct posicion Pfin);

//se obtiene la posicion del vector de direcciones de imagen del char img correspondiente
int obtener_pos_img(char img);

//Determina si hay un ganador
//Retorna 0 si no hay ganador, 1 si ganaron los sufraggetos y 2 si ganaron los policias
int verificar_ganador();

// guarda el movimiento a hacer en el vector de movimientos
void guarda_V_mov(struct movimiento moves[],int posVmov, int i,int j, int salto);

int posicion_salto(int ini,int fin);

