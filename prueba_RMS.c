#include <18F4550.h>

#fuses NOWDT,MCLR,HS,NOUSBDIV,NOIESO,            //Selecciona el oscilador externo
#use delay(clock=12 Mhz, crystal= 12 MHz)   // Selecciona la velocidad del oscilador interno
#use i2c(Master,Fast=100000, sda=PIN_D6, scl=PIN_D7,force_sw)
#include <stdio.h>
#include <stdlib.h>
#include <i2c_Flex_LCD.h>
#include <MAX191.h>
#include <math.h>


#use fast_io(A)
#use fast_io(B)

/*unsigned long punto1, punto2;
int8 pos_V,pos_I,pos_V_A,pos_I_A;
int control_V, control_I;
int desfase;*/
long punto1;
float tension, tension_RMS;
int pulso_timer=1,contador;


#INT_RTCC                // interrupcion para demora de 500 us
void interrtimer_0(){
    set_timer0(0x1D);   // se carga 29 para un desborde en 500 us teniendo en cuenta los tiempos en C
    pulso_timer++;
   }

void main() {
    //lcd_init();
    set_tris_a(0xF9);
    
    while (contador < 60 && pulso_timer==1 ){
    punto1= leer_ADC(2);
    tension=punto1;
    tension= (tension*5)/4096;
    tension_RMS=tension_RMS+ tension * tension;       //calcula tension eficaz
    contador++;
    pulso_timer=0;
    }
    if (contador==60){
        tension_RMS= SQRT(tension_RMS/60);
        lcd_gotoxy(1,1);
        printf (LCD_PUTC, "T=\%f V     ",tension_RMS);
        delay_ms(40);
        contador=0;
    }

}