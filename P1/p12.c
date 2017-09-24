#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <dos.h>

#define INTR 0x1C

int cantidadEj = 0;
int tiempo = 0;
int flag = 0;
int cantint = 0;
int contf = 0;
int valorL, valorH;
unsigned long int frec_base=1193180;    /* Frecuencia base: 1.193 MHz*/
int contador;
unsigned char contador_hi, contador_lo;
unsigned char ogcont, ogconthi;
int frec = 0;                       /* Frecuencia */
void interrupt (*vectoriginal)();

void interrupt manejador(){
  flag=1;
}

int main (){
	disable();
	vectoriginal = getvect(INTR);
	
	do{
		printf("Ingrese la frecuencia deseada: \n");
		scanf("%d", &frec);
	} while (frec < 19);
	
	contador=frec_base/frec;   /* valor del contador... */
	contador_hi= contador>>8;
	//contador_hi=contador/256;      /* ... su byte alto ...*/
	contador_lo=contador; 	/* ... y el bajo */
	outportb(0x43,0x36);           /* escribe byte de control */
	
	ogcont = inportb(0x40);
	ogconthi = inportb(0x40);
	outportb(0x40,contador_lo);    /* escribe el contador bajo */
	outportb(0x40,contador_hi);    /* y luego el alto f */
	
	setvect(INTR,manejador);
	enable();
	while (tiempo < 21) {
		if (flag ==1){				
			flag=0;
			cantint++;
			contf++;
			printf("Se produjo la interrupcion %d. \n", cantint);
			if (contf == frec){
				contf=0;
				tiempo++;
				printf("Paso el segundo %d.\n", tiempo);
			}
		}
	}
	
	
	/* Antes de salir, restaurar las condiciones originales */
	disable();                     /* desactiva interrupciones */
	setvect(INTR, vectoriginal);      /* restaura la vieja ISR */
	outportb(0x40, ogcont);          /* frecuencia 18.2 Hz */
	outportb(0x40, ogconthi);
	enable();                      /* habilita interrupciones */
	return 0;
}
