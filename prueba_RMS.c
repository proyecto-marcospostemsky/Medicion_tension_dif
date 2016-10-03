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
float tension, tension_RMS=0;
int pulso_timer=1,contador=0;
const long carga= 0xFD2D;

#INT_RTCC                // interrupcion para demora de 500 us
void interrtimer_0(){
    set_timer0(carga);   // se carga 29 para un desborde en 500 us teniendo en cuenta los tiempos en C
    pulso_timer=1;
   }

void main() {
    setup_timer_0( RTCC_INTERNAL | T0_DIV_2);
     enable_interrupts (INT_RTCC);
    enable_interrupts (GLOBAL);
    set_timer0(carga);
    lcd_init();
    lcd_gotoxy(1,1);
    printf (LCD_PUTC, "pruebtension ");
    set_tris_a(0xF9); 
    while(1){
    if (contador < 60 && pulso_timer==1 ){
    punto1= leer_ADC(2);
    tension=punto1;
    tension= (tension/1000)-2.54;
    tension_RMS=tension_RMS+ tension * tension;       //calcula tension eficaz
    contador++;
    pulso_timer=0;
    }
    
    if (contador==60){
        tension_RMS= sqrt(tension_RMS/60);
        lcd_gotoxy(1,2);
        printf (LCD_PUTC, "T=\%f V     ",tension_RMS);
        delay_ms(1000);
        
       /* punto1= leer_ADC(2);
        tension=punto1;
        tension= (tension/1000);
        lcd_gotoxy(1,2);
        printf (LCD_PUTC, "T=\%f V     ",tension);
        delay_ms(1000);*/
        contador=0;
    }
    }
}