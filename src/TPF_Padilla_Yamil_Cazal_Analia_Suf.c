/*
 ============================================================================
 Name        : tpf_Suffragetto_PadillaYamil.c
 Author      : Yamil Padilla
 Version     :
 Copyright   : 
 Description :
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include "Declaraciones.h"



GtkWidget *label_turno;
GtkWidget *label_estado;
GtkWidget *tablero;
GtkWidget *fichas;

void tablero_cb(GtkWidget *event_box, GdkEventButton *event, gpointer data){
	guint i,j;
	int posmov;
	static int cantmove;
	static int Modo = 0;
	static struct movimiento moves[8];
	i = (GUINT_FROM_LE(event->y) / pixel);
	j = (GUINT_FROM_LE(event->x) / pixel);
	struct posicion P;
	static struct posicion Pini;
	P.F = i;
	P.C = j;
	if (Modo == 0){
		if (Es_ficha_suya(P,jugador)){
			cantmove = guardar_movimientos(moves, P, -1);
			mostrar_movimientos(moves,P,cantmove);
			Pini.F = i;
			Pini.C = j;
			Modo = 1;
		}else{

		}
	}else{
		if (Modo == 1){
				posmov = verificar_movimiento(moves,Pini,P, cantmove);
				if(posmov > -1){
					hacer_movimiento(Pini,P);
					if(verificar_captura(moves[posmov],tablero_fichas[Pini.F][Pini.C])){
						hacer_captura(moves[posmov].FSaltada);
					}
					deseleccionar_mov(moves,Pini,cantmove);
					cantmove = guardar_movimientos(moves, P, moves[posmov].Saltos);
					mostrar_movimientos(moves,P,cantmove);
					if (cantmove == 0){
						Modo = 0;
						cambiar_jugador();
					}
					verificar_ganador();
				}
				if (posmov == -2){
					deseleccionar_mov(moves,Pini,cantmove);
					Modo = 0;
				}
		}

	}
}

gboolean agrandar_tablero (GtkWidget* menu_ampliar,GdkEventButton event,gpointer user_data){
	rescale_tablero(2);
	return 1;
}
gboolean reducir_tablero(GtkWidget* menu_reducir,GdkEventButton event,gpointer user_data){
	rescale_tablero(1);
	return 1;
}


void cambiar_jugador(){
	if (jugador == SUFFRAGETTO){
		jugador = POLICIA;
	}else{
		jugador = SUFFRAGETTO;
	}
}

GtkWidget *crear_tablero(){
	int i, j;
	GtkWidget *imagen; //auxiliar para cargar la imagen
	tablero = gtk_grid_new();
	for (i = 0; i < 17; i++) {
		for (j = 0; j < 17; j++) {
				if(tablero_paralelo[i][j] == ARENA){
					imagen = gtk_image_new_from_file("./img/arena.png");
				}else if(tablero_paralelo[i][j] == NEUTRALG){
					imagen = gtk_image_new_from_file("./img/neutral.png");
				}else if(tablero_paralelo[i][j] == HOSPITAL || tablero_paralelo[i][j] == PRISON){
					imagen = gtk_image_new_from_file("./img/dead.png");
				}else if(tablero_paralelo[i][j] == HGROUNDS || tablero_paralelo[i][j] == PYARD){
					imagen = gtk_image_new_from_file("./img/yard.png");
				}else if(tablero_paralelo[i][j] == ALBERTH || tablero_paralelo[i][j] == HOUSEC){
					imagen = gtk_image_new_from_file("./img/goal.png");
				}
				gtk_grid_attach(GTK_GRID(tablero), GTK_WIDGET(imagen), j, i, 1, 1);
		}
	}
	return tablero;
}

GtkWidget *crear_tablero_fichas(){
	int i, j;
	GtkWidget *imagenficha;
	GtkWidget *eventbox;
	eventbox = gtk_event_box_new();
	fichas = gtk_grid_new();
	for (i = 0; i < 18; i++) {
		for (j = 0; j < 18; j++) {
				if(tablero_fichas[i][j] == VACIO){
					imagenficha = gtk_image_new_from_file("./img/vacio.png");
				}else if(tablero_fichas[i][j] == SUFFRAGETTO){
					imagenficha = gtk_image_new_from_file("./img/suf.png");
				}else if(tablero_fichas[i][j] == POLICIA){
					imagenficha = gtk_image_new_from_file("./img/pol.png");
				}else if(tablero_fichas[i][j] == LIDERSU){
					imagenficha = gtk_image_new_from_file("./img/lidersu.png");
				}else if(tablero_fichas[i][j] == INSPECTORP){
					imagenficha = gtk_image_new_from_file("./img/inspectorp.png");
				}
				gtk_grid_attach(GTK_GRID(fichas), GTK_WIDGET(imagenficha), j, i, 1, 1);
	}
	}
	gtk_container_add(GTK_CONTAINER(eventbox), fichas);
	g_signal_connect(eventbox, "button-press-event", G_CALLBACK(tablero_cb), fichas);
	return eventbox;
}





void preparar_tablero(){
	int i,j;
	for(i = 0;i < 17;i++){
		for(j=0;j < 18;j++){
			if(j == 0 || (j ==  1 && i < 4)){
				tablero_paralelo[i][j] = PRISON;
			}
			else if((j == 1 && i > 3 && i < 11)|| j == 2 || j == 3 || j == 13 || j == 14|| (j == 15 && i > 5 && i < 13)){
				tablero_paralelo[i][j] = NEUTRALG;
			}
			else if(j > 3 && j < 13){
				if((j > 6 && j < 10)){
					if(i == 3 || i == 4){
						tablero_paralelo[i][j] = HOUSEC;
					}else if(i == 12 || i == 13){
						tablero_paralelo[i][j] = ALBERTH;
					}else{
						tablero_paralelo[i][j] = ARENA;
					}
				}else{
					tablero_paralelo[i][j] = ARENA;
				}
			}
			else if(j == 15 && i < 6){
				tablero_paralelo[i][j] = HGROUNDS;
			}
			else if(j == 1 && i > 10){
				tablero_paralelo[i][j] = PYARD;
			}
			else if(j == 16 || (j ==  15 && i > 12)){
				tablero_paralelo[i][j] = HOSPITAL;
			}
			else if(j == 17){
				tablero_paralelo[i][j] = '\0';
				tablero_fichas[i][j] = '\0';
			}
			if((i > 2 && i < 6)){
				if(i == 3 && (j == 6 || j== 10)){
					tablero_fichas[i][j] = SUFFRAGETTO;
				}else if(i == 4 &&((j > 3 && j < 7)||(j > 9 && j < 13))){
					if(j == 4 || j == 12){
						tablero_fichas[i][j] = LIDERSU;
					}else{
						tablero_fichas[i][j] = SUFFRAGETTO;
					}
				}else if(i == 5 && (j > 1 && j < 15)){
					if(j == 2 || j ==  8 || j == 14){
						tablero_fichas[i][j] = LIDERSU;
					}else{
						tablero_fichas[i][j] = SUFFRAGETTO;
					}
				}else {
					tablero_fichas[i][j] = VACIO;
				}
			}else if((i > 10 && i < 14)){
				if(i == 13 && (j == 6 || j== 10)){
					tablero_fichas[i][j] = POLICIA;
				}else if(i == 12 &&((j > 3 && j < 7)||(j > 9 && j < 13))){
					if(j == 4 || j == 12){
						tablero_fichas[i][j] = INSPECTORP;
					}else{
						tablero_fichas[i][j] = POLICIA;
					}
				}else if(i == 11 && (j > 1 && j < 15)){
					if(j == 2 || j ==  8 || j == 14){
						tablero_fichas[i][j] = INSPECTORP;
					}else{
						tablero_fichas[i][j] = POLICIA;
					}
				}else{
				tablero_fichas[i][j] = VACIO;
				}
			}else{
				tablero_fichas[i][j] = VACIO;
			}
		}
	}
}



int verificar_movimiento(struct movimiento moves[],struct posicion Pini, struct posicion Pfin, int cantmov){
	int i;
	if (Pini.F == Pfin.F && Pini.C == Pfin.C){ // se verifica si se selecciono la misma casilla
		i = -2;
	}
	if (i != -2){
		int band = 0;
		for(i = 0; i < cantmov ;i++){ // Se van recorriendo los movimientos posibles y se compara con la posicion a mover
			if(moves[i].p.F == Pfin.F && moves[i].p.C == Pfin.C){
				band = 1;
			}
			if (band == 1){
				break;
			}
		}
		if(band == 0){
			i = -1;
		}
	}
	return i;
}

int Es_ficha_suya(struct posicion pos,char jugador){
	int band = 0;
	if(Es_permitido(pos.F, pos.C)){ // verifica que la posicion este dentro del tablero
		if (jugador == SUFFRAGETTO){ //si busca un sufraggetto
			if (tablero_fichas[pos.F][pos.C]== SUFFRAGETTO || tablero_fichas[pos.F][pos.C]== LIDERSU){
				band = 1; //es verdadero si la ficha pertenece a los sufraggetto
			}
		}
		if (jugador == POLICIA){ // o si busca un policia
			if (tablero_fichas[pos.F][pos.C]== POLICIA || tablero_fichas[pos.F][pos.C]== INSPECTORP){
				band = 1; //es verdadero si la ficha pertenece a los policias
			}
		}
	}
	return band;
}


int guardar_movimientos(struct movimiento moves[], struct posicion Pini, int numsalto){
	int i,j,saltox,saltoy,cantsalto;
	int bandAHoHC = 0; // bandera de si la ficha esta en albert hall o house of commons
	int cantmov = 0; // guarda la cantidad de movimientos posibles
	char bando; // guarda el bando de la ficha seleccionada
	if(Es_ficha_suya(Pini, SUFFRAGETTO)){ // se verifica si es un sufraggetto o policia
		bando = SUFFRAGETTO;
	}else{
		bando = POLICIA;
	}

	//Se inicializa los saltos a -1
	for (i = 0; i < 8;i++){
		moves[i].Saltos = -1;
	}
	//Se verifica que la ficha esta en albert hall o house of commons
	if(tablero_paralelo[Pini.F][Pini.C] == ALBERTH || tablero_paralelo[Pini.F][Pini.C] == HOUSEC){
		bandAHoHC = 1;
	}

	if(numsalto < 3){ // Solo entra si la ficha salto menos de 3 veces
	for(i = Pini.F - 1; i < Pini.F +2;i++){
		for(j = Pini.C - 1; j < Pini.C +2;j++){  // Se recorre el cuadrado alrededor a la ficha
			if(!(i == Pini.F && j == Pini.C)){  // Se quita el espacio donde esta la ficha
				if(Es_permitido(i, j)){		// Se verifica que la ubicacion este dentro del tablero
					if (tablero_fichas[i][j] == VACIO){	// Si la casilla esta vacia
						if(numsalto == -1){ //Si no ha hecho ningun salto
							if(!(bandAHoHC)){ // en el caso que no este en las metas
								// Se guarda si el lugar es distinto a las metas propias
								if(bando == SUFFRAGETTO && tablero_paralelo[i][j] != ALBERTH){
									guarda_V_mov(moves, cantmov, i, j, 0);
									cantmov++;
								}else if(bando == POLICIA && tablero_paralelo[i][j] != HOUSEC){
									guarda_V_mov(moves, cantmov, i, j, 0);
									cantmov++;
								}
							}else{ // si esta en sus metas
								// se guarda solo si siguen en sus metas
								if(bando == SUFFRAGETTO && tablero_paralelo[i][j] == HOUSEC){
									guarda_V_mov(moves, cantmov, i, j, 0);
									cantmov++;
								}else if(bando == POLICIA && tablero_paralelo[i][j] == ALBERTH){
									guarda_V_mov(moves, cantmov, i, j, 0);
									cantmov++;
									}
							}
						}
					}else{
						if(numsalto != 0){ // si no se ha hecho un movimiento normal
						if (numsalto == -1){ // si es el primer salto que se hace
							cantsalto = 0; // se han hecho 0 saltos antes
						}
						else{
							cantsalto = numsalto; // si no se trae con la variable
						}
						saltox = posicion_salto(Pini.F,i); // se calcula la posicion donde quedaria
						saltoy = posicion_salto(Pini.C,j);
						if(Es_permitido(saltox,saltoy)){ // se verifica que es valida la posicion
							if(tablero_fichas[saltox][saltoy] == VACIO){ // se verifica que la posicion a saltar este vacia
							if(!(bandAHoHC)){ // si no estan en sus metas
								if(bando == SUFFRAGETTO && tablero_paralelo[i][j] != ALBERTH){ //se guarda si no entra a la meta propia
									guarda_V_mov(moves, cantmov, saltox, saltoy, cantsalto + 1);
									moves[cantmov].FSaltada.F = i;
									moves[cantmov].FSaltada.C = j;
									cantmov++;
									}else if(bando == POLICIA && tablero_paralelo[i][j] != HOUSEC){
										guarda_V_mov(moves, cantmov, saltox, saltoy, cantsalto + 1);
										moves[cantmov].FSaltada.F = i;
										moves[cantmov].FSaltada.C = j;
										cantmov++;
									}
								}else{// si estan en sus metas
									if(bando == SUFFRAGETTO && tablero_paralelo[i][j] == HOUSEC){ // se guarda si siguen en ellas
										guarda_V_mov(moves, cantmov, saltox, saltoy, cantsalto + 1);
										moves[cantmov].FSaltada.F = i;
										moves[cantmov].FSaltada.C = j;
										cantmov++;
									}else if(bando == POLICIA && tablero_paralelo[i][j] == ALBERTH){
										guarda_V_mov(moves, cantmov, saltox, saltoy, cantsalto + 1);
										moves[cantmov].FSaltada.F = i;
										moves[cantmov].FSaltada.C = j;
										cantmov++;
									}
								}
							}
						}
						}
						}

					}
				}
			}
		}
	}
	return cantmov;
}

void guarda_V_mov(struct movimiento moves[],int posVmov, int i,int j, int salto){
	moves[posVmov].p.F = i;
	moves[posVmov].p.C = j;
	moves[posVmov].Saltos = salto;
}

int posicion_salto(int ini,int fin){
	int Posicion;
		switch (ini-fin){
			case -1:{			//significa que esta una fila/ columna atras de la posicion final
				Posicion = ini+2;		// por lo tanto, el lugar de salto son dos filas/columnas adelante
				break;
			}
			case 0:{			//significa que esta en la misma fila/columna
				Posicion = ini;
				break;
			}
			case 1:{			//significa que esta una fila/ columna adelante de la posicion final
				Posicion = ini+2;
				break;
			}
		}
	return Posicion;
}

int Es_permitido(int Posx,int Posy){
	int band = 0;
	if(Posy > 0 && Posy < 16){
		if (Posy == 1){
			if (Posx > 3 && Posx < 11){
				band = 1;
			}
		}else if(Posy == 15){
			if(Posx > 5 && Posx < 13){
				band = 1;
			}
		}else if(Posx > -1 && Posx < 17){
			band = 1;
		}
	}
	return band;
}

int verificar_captura(struct movimiento mov,char ficha){
	int band = 0;
	if(tablero_paralelo[mov.FSaltada.F][mov.FSaltada.C]== ARENA){
		if(jugador == SUFFRAGETTO){
			if((tablero_fichas[mov.FSaltada.F][mov.FSaltada.C] == POLICIA||tablero_fichas[mov.FSaltada.F][mov.FSaltada.C] == INSPECTORP)){
				if(ficha == LIDERSU){
					band = 1;
				}else{
					if((mov.p.F - mov.FSaltada.F) != 0 && (mov.p.C - mov.FSaltada.C) != 0 ){
						band = 1;
					}
				}
			}
		}else{
			if((tablero_fichas[mov.FSaltada.F][mov.FSaltada.C] == SUFFRAGETTO||tablero_fichas[mov.FSaltada.F][mov.FSaltada.C] == LIDERSU)){
				if(ficha == INSPECTORP){
					band = 1;
				}else{
					if((mov.p.F - mov.FSaltada.F) != 0 && (mov.p.C - mov.FSaltada.C) != 0 ){
						band = 1;
				}
				}
			}
		}
	}
	return band;
}

void hacer_captura(struct posicion Psaltada){
	int i,j,band = 0;
	int posimg;
	if(jugador == SUFFRAGETTO){
		for(i = 0;i < 17; i++){
			for(j = 0;j < 2; j++){
				if(j == 0 || (j ==  1 && i < 4)){
					if(tablero_paralelo[i][j] == VACIO){
						tablero_fichas[i][j] = tablero_fichas[Psaltada.F][Psaltada.C];
						tablero_fichas[Psaltada.F][Psaltada.C] = VACIO;
						band = 1;
						break;
					}
				}
			}
			if(band == 1){
				break;
			}
		}
	}else{
		for(i = 0;i < 17; i++){
			for(j = 15;j < 17; j++){
				if(j == 16 || (j ==  15 && i > 12)){
					if(tablero_paralelo[i][j] == VACIO){
						tablero_fichas[i][j] = tablero_fichas[Psaltada.F][Psaltada.C];
						tablero_fichas[Psaltada.F][Psaltada.C] = VACIO;
						band = 1;
						break;
					}
				}
			}
			if(band == 1){
			break;
			}
		}
	}
	GdkPixbuf *pixbuf = NULL;
	posimg = obtener_pos_img(tablero_fichas[i][j]);
	pixbuf = gdk_pixbuf_new_from_file(imgfichas[posimg],NULL);
	pixbuf = gdk_pixbuf_scale_simple (pixbuf,pixel,pixel,GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(fichas),j,i)), pixbuf);

	posimg = obtener_pos_img(tablero_fichas[Psaltada.F][Psaltada.C]);
	pixbuf = gdk_pixbuf_new_from_file(imgfichas[posimg],NULL);
	pixbuf = gdk_pixbuf_scale_simple (pixbuf,pixel,pixel,GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(fichas),Psaltada.C,Psaltada.F)), pixbuf);
}

void hacer_movimiento(struct posicion Pini,struct posicion Pfin){
	tablero_fichas[Pfin.F][Pfin.C] = tablero_fichas[Pini.F][Pini.C];
	tablero_fichas[Pini.F][Pini.C] = VACIO;
	int posimg;
	GdkPixbuf *pixbuf = NULL;
	posimg = obtener_pos_img(tablero_fichas[Pini.F][Pini.C]);
	pixbuf = gdk_pixbuf_new_from_file(imgfichas[posimg],NULL);
	pixbuf = gdk_pixbuf_scale_simple (pixbuf,pixel,pixel,GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(fichas),Pini.C,Pini.F)), pixbuf);

	posimg = obtener_pos_img(tablero_fichas[Pfin.F][Pfin.C]);
	pixbuf = gdk_pixbuf_new_from_file(imgfichas[posimg],NULL);
	pixbuf = gdk_pixbuf_scale_simple (pixbuf,pixel,pixel,GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(fichas),Pfin.C,Pfin.F)), pixbuf);
}


void mostrar_movimientos(struct movimiento moves[],struct posicion P,int cantmove){
	int i;
	int posimg;
	GdkPixbuf *pixbuf = NULL;
	for (i = 0; i < cantmove; i++){
		posimg = obtener_pos_img(tablero_paralelo[moves[i].p.F][moves[i].p.C]);
		pixbuf = gdk_pixbuf_new_from_file(imgtableromov[posimg],NULL);
		pixbuf = gdk_pixbuf_scale_simple (pixbuf,pixel,pixel,GDK_INTERP_BILINEAR);
		gtk_image_set_from_pixbuf(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(tablero),moves[i].p.C,moves[i].p.F)), pixbuf);
	}
	if(tablero_paralelo[P.F][P.C] == ARENA){
		gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(tablero),P.C,P.F)), "./img/arenasel.png");
	}else if(tablero_paralelo [P.F][P.C]== NEUTRALG){
		gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(tablero),P.C,P.F)), "./img/neutralsel.png");
	}else if(tablero_paralelo [P.F][P.C] == ALBERTH || tablero_paralelo[P.F][P.C] == HOUSEC){
		gtk_image_set_from_file(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(tablero),P.C,P.F)), "./img/goalsel.png");
	}
	posimg = obtener_pos_img(tablero_paralelo [P.F][P.C]);
	pixbuf = gdk_pixbuf_new_from_file(imgtablerosel[posimg],NULL);
	pixbuf = gdk_pixbuf_scale_simple (pixbuf,pixel,pixel,GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(tablero),P.C,P.F)), pixbuf);
}

void deseleccionar_mov(struct movimiento moves[],struct posicion P,int cantmove){
	int i;
	int posimg;
	GdkPixbuf *pixbuf = NULL;
	for (i = 0; i < cantmove + 1; i++) {
			posimg = obtener_pos_img(tablero_paralelo[moves[i].p.F][moves[i].p.C]);
			pixbuf = gdk_pixbuf_new_from_file(imgtablero[posimg],NULL);
			pixbuf = gdk_pixbuf_scale_simple (pixbuf,pixel,pixel,GDK_INTERP_BILINEAR);
			gtk_image_set_from_pixbuf(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(tablero),moves[i].p.C,moves[i].p.F)), pixbuf);
	}
	posimg = obtener_pos_img(tablero_paralelo[P.F][P.C]);
	pixbuf = gdk_pixbuf_new_from_file(imgtablero[posimg],NULL);
	pixbuf = gdk_pixbuf_scale_simple (pixbuf,pixel,pixel,GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(tablero),P.C,P.F)), pixbuf);
}

int obtener_pos_img(char img){
	int pos;
	if(img == VACIO){
		pos = 0;
	}else if(img == SUFFRAGETTO){
		pos = 1;
	}else if(img == POLICIA){
		pos = 2;
	}else if(img == LIDERSU){
		pos = 3;
	}else if(img == INSPECTORP){
		pos = 4;
	}
	if(img == ARENA){
		pos = 0;
	}else if(img == NEUTRALG){
		pos = 1;
	}else if(img == ALBERTH || img == HOUSEC){
		pos = 2;
	}else if(img == HOSPITAL || img == PRISON){
		pos = 3;
	}else if(img == HGROUNDS || img == PYARD){
		pos = 4;
	}
	return pos;
}




//GtkWidget *widget,GdkEventButton *event, gpointer data
void rescale_tablero(int opcion){
	int i,j;
	int posimg;
	if(opcion == 1){
		pixel = pixel/1.35;
	}
	if(opcion == 2){
		pixel = pixel*1.35;
	}
	GdkPixbuf *pixbuf = NULL;
	for (i = 0; i < 18; i++) {
		for (j = 0; j < 18; j++) {
			posimg = obtener_pos_img(tablero_fichas[i][j]);
			pixbuf = gdk_pixbuf_new_from_file(imgfichas[posimg],NULL);
			pixbuf = gdk_pixbuf_scale_simple (pixbuf,pixel,pixel,GDK_INTERP_BILINEAR);
			gtk_image_set_from_pixbuf(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(fichas),j,i)), pixbuf);

			posimg = obtener_pos_img(tablero_paralelo[i][j]);
			pixbuf = gdk_pixbuf_new_from_file(imgtablero[posimg],NULL);
			pixbuf = gdk_pixbuf_scale_simple (pixbuf,pixel,pixel,GDK_INTERP_BILINEAR);
			gtk_image_set_from_pixbuf(GTK_IMAGE(gtk_grid_get_child_at(GTK_GRID(tablero),j,i)), pixbuf);
		}
	}
}

int verificar_ganador(){
	int band = 0;
	int conts = 0; //conts = contador de fichas sufragetto
	int contp = 0; // contp = contador de fichas de policia
	int contsHC = 0; //contsHC = contador de fichas sufragetto en House of commons
	int contpAH = 0; // contAH = contador de fichas de policia en Albert Hall
	struct posicion P; // Posicion en que se esta revisando si hay una ficha
	for (P.F = 0; P.F < 17;P.F++){
		for (P.C = 1; P.C < 16;P.C++){ // recorre el tablero
			if(tablero_paralelo[P.F][P.C]== ARENA || tablero_paralelo[P.F][P.C]== NEUTRALG){ //cuenta las fichas en posiciones validas
				if(Es_ficha_suya(P, SUFFRAGETTO)){
					conts++;
				}
				if(Es_ficha_suya(P, POLICIA)){
					contp++;
				}
			}
			else if(tablero_paralelo[P.F][P.C] == HOUSEC){ // cuenta las fichas en House of commons
				if(Es_ficha_suya(P, SUFFRAGETTO)){
					conts++;
					contsHC++;
				}
			}else if(tablero_paralelo[P.F][P.C] == ALBERTH){ // cuenta las fichas en alberth house
				if(Es_ficha_suya(P, POLICIA)){
					contp++;
					contpAH++;
				}
			}
		}
	}
	// se verifican las cantidad de fichas en alberth house y house of commons, y la cantidad de fichas totales
	if (contsHC == 6 || contp < 6){
		band = 1;  // indica que ganaron los suffragettos
	}
	if (contpAH == 6 || conts < 6){
		band = 2; // indica que ganaron los policias
	}
	return band;
}

int numero_random(int limsup){
	srand(time(NULL));
	int n = rand();
	n = n % limsup;
	return n;
}

void limpiar_buffer(){
	char ch;
	while ((ch = getchar()) != EOF && ch != '\n');
}

int pedir_numero_entero(int liminf, int limsup){
	int n; 				// número que el usuario ingresa
	int r; 				// resultado del scanf. El scanf retorna el número de datos de entrada asignados correctamente a las variables correspondientes.
	int ban = FALSE;  	// bandera para iteración
	do{
		printf("Ingrese numero entre %d y %d: ",liminf,limsup);
		r = scanf("%d", &n);
		limpiar_buffer();
		if (r == 1 && n >= liminf && n <= limsup){
			ban = TRUE;}
		else{
			printf("Numero no valido \n");}
	} while (ban == FALSE);
	printf("\n");
	return n;
}

int main (int argc, char *argv[]){
	 GtkWidget *window;
	 GtkWidget *box_tablero;
	 GtkWidget *overlay;
	 GtkWidget *box_fichas;
	 GtkWidget *menu_ampliar;
	 GtkWidget *menu_reducir;

	    //Para el GtkBuilder
	    guint ret;
	    GtkBuilder *builder;
	    GError *error = NULL;

	    preparar_tablero();

	    gtk_init (&argc, &argv);

		builder = gtk_builder_new();
		//Se carga el builder
		ret = gtk_builder_add_from_file(builder, "tablero.glade", &error);
		if (ret == 0) {
			g_print("Error en la función gtk_builder_add_from_file:\n%s", error->message);
			return 1;
		}

		//Ventana principal
		window = GTK_WIDGET(gtk_builder_get_object(builder, "ventana"));

		overlay = GTK_WIDGET(gtk_builder_get_object(builder, "overlay"));

		//Box donde estara el Tablero
		box_tablero = GTK_WIDGET(gtk_builder_get_object(builder, "box_tablero"));

		box_fichas = gtk_box_new (GTK_ORIENTATION_VERTICAL,0);

		menu_ampliar = GTK_WIDGET(gtk_builder_get_object(builder, "menu_ampliar"));
		g_signal_connect (menu_ampliar, "activate", G_CALLBACK(agrandar_tablero),NULL);
		menu_reducir = GTK_WIDGET(gtk_builder_get_object(builder, "menu_reducir"));
		g_signal_connect (menu_reducir, "activate", G_CALLBACK(reducir_tablero),NULL);

		gtk_box_pack_start(GTK_BOX(box_tablero), crear_tablero(), TRUE, FALSE, 20);

		gtk_box_pack_start(GTK_BOX(box_fichas), crear_tablero_fichas(), TRUE, FALSE, 20);

		gtk_overlay_add_overlay (GTK_OVERLAY(overlay),box_fichas);

	    /* Connect the destroy signal of the window to gtk_main_quit
	     * When the window is about to be destroyed we get a notification and
	     * stop the main GTK+ loop
	     */
	    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	    /* make sure that everything, window and label, are visible */
	    gtk_widget_show_all (window);

	    /* start the main loop, and let it rest there until the application is closed */
	    gtk_main ();

   return 0;
}

