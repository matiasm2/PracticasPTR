#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <graphics.h>

#define INTR 0x1C

int flag = 0;
unsigned char init;
float temp;
int gdriver=DETECT;
int bool=1;
int gmode, contador, contint;
unsigned char contador_hi, contador_lo;
unsigned char ogcont, ogconthi;

void interrupt (*vectoriginal)();

void interrupt manejador(){
  flag=1;
}

float temperatura (){
	unsigned char nhi=inportb(0x309);
	unsigned char nlw=inportb(0x308);
	float aux=nhi*256+nlw;
	float v=aux*5/4096; //(2**12                                      )
	return v*24;
}

int main (){
	disable();
	vectoriginal = getvect(INTR);
	setvect(INTR,manejador);
	outportb(0x300,0);

	contador=65535;   /* valor del contador... */
	contador_hi= contador>>8;
	contador_lo=contador; 	/* ... y el bajo */
	outportb(0x43,0x36);           /* escribe byte de control */

	ogcont = inportb(0x40);
	ogconthi = inportb(0x40);
	outportb(0x40,contador_lo);    /* escribe el contador bajo */
	outportb(0x40,contador_hi);    /* y luego el alto f */
	enable();

	initgraph(&gdriver,&gmode,"C:\\BORLANDC\\BGI");

	/*
	f() = -0.5*x+120
	85=-0.5*x+120 85-120/-0.5 = x = 70
	65=-0.5*x+120 65-120/-0.5 = x = 110
	*/
	outtextxy(contint,0, "120");
//	outtextxy(contint,60, "90");
	outtextxy(contint,70, "85");
	outtextxy(contint,110, "65");
//	outtextxy(contint,120, "60");
	outtextxy(contint,180, "30");
	outtextxy(contint,240, "0");
	outtextxy(contint+1,240,"0");
	outtextxy(100, 240, "5.49");
	outtextxy(200, 240, "10.98");
	outtextxy(300, 240, "16.48");
	outtextxy(400, 240, "21.97");
	outtextxy(500, 240, "27.47");
	outtextxy(600, 240, "32.96");
	outtextxy(275, 250, "Segundos");
	init=inportb(0x308);
	outportb(0x304,0x08);
	outportb(0x304,0x01);
	while(bool){
		if (flag==1){
			contint++;
			line(contint+2,(120-temp)*2,contint+1,(120-temp)*2);
			if(contint==600){
				bool=0;
			}
			flag=0;
			temp = temperatura();
//			printf("La temperatura es: %f\n",temp);
			//00001000 prende el 0 00000000 apaga el 0
			if(temp>=85){
				outportb(0x304,0);
				outportb(0x304,0x01);
			} else if (temp>65){
				outportb(0x304,0x08);
				outportb(0x304,0x09);
			} else {
				outportb(0x304,0x08);
				outportb(0x304,0x01);
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
