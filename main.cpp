#include "mbed.h"
 
#define botonPulsado 0
#define botonNoPulsado 1
#define tiempoMaximo 0.5
 
int horas=0;
int minutos=0;
int segundos=0;
 
 
DigitalOut led(LED1);
DigitalIn boton(PC_13);
Timer temporizador;
Timer tiempoPulsado;
 
enum estados {normal,esperandoSoltarConf,esperandoPulsarHoras,esperandoSoltarHoras,esperandoPulsarMin,esperandoSoltarMin};
estados estado;
 
void intrementarReloj()
{
    if(temporizador.read()>1.0) {
        temporizador.reset();
        segundos++;
        if (segundos==60) {
            segundos=0;
            minutos++;
            if(minutos==60) {
                minutos=0;
                horas++;
                if(horas==24) {
                    horas=0;
                }
            }
        }
    }
    printf("%02d:%02d:%02d\n",horas,minutos,segundos);
}
 
void funcionNormal()
{
    if(temporizador>1.0) {
        intrementarReloj();
    } else if(boton==botonPulsado) {
        tiempoPulsado.reset();
        tiempoPulsado.start();
        estado=esperandoSoltarConf;
    }
}
 
 
 
void funcionEsperandoSoltarConf()
{
    if(temporizador>1.0) {
        intrementarReloj();
    } else if (boton==botonNoPulsado) {
        if(tiempoPulsado>1) {
            estado=esperandoPulsarMin;
            printf("Configurar Minutos\n");
        } else {
            estado=normal;
        }
    }
 
}
 
 
void funcionEsperandoPulsarMin()
{
    if (boton==botonPulsado) {
        tiempoPulsado.reset();
        tiempoPulsado.start();
        estado=esperandoSoltarMin;
    }
}
 
void funcionEsperandoSoltarMin()
{
    if (boton==botonNoPulsado) {
        if(tiempoPulsado>1) {
            estado=esperandoPulsarHoras;
            printf("Configurar Horas\n");
        } else {
            minutos++;
            if (minutos==60) {
                minutos=0;
            }
 
            printf("%02d:%02d:%02d\n",horas,minutos,segundos);
            estado=esperandoPulsarMin;
        }
    }
}
 
void funcionEsperandoPulsarHoras()
{
    if (boton==botonPulsado) {
        tiempoPulsado.reset();
        tiempoPulsado.start();
        estado=esperandoSoltarHoras;
    }
}
 
void funcionEsperandoSoltarHoras()
{
    if (boton==botonNoPulsado) {
        if(tiempoPulsado>1) {
            estado=normal;
        } else {
            horas++;
            if (horas==24) {
                horas=0;
            }
 
            printf("%02d:%02d:%02d\n",horas,minutos,segundos);
            estado=esperandoPulsarHoras;
        }
    }
}
 
 
int main()
{
    estado=normal;
    temporizador.start();
    while(1) {
        switch(estado) {
            case normal:
                funcionNormal();
                break;
            case esperandoSoltarConf:
                funcionEsperandoSoltarConf();
                break;
            case esperandoPulsarHoras:
                funcionEsperandoPulsarHoras();
                break;
            case esperandoSoltarHoras:
                funcionEsperandoSoltarHoras();
                break;
            case esperandoPulsarMin:
                funcionEsperandoPulsarMin();
                break;
            case esperandoSoltarMin:
                funcionEsperandoSoltarMin();
                break;
        }
    }
}