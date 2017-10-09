#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"


#define PACER_RATE 500
#define MESSAGE_RATE 10


void display_character (char character)
{
    char buffer[2];
    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}


int main (void)
{
    char character = 'P';

    system_init ();
    tinygl_init (PACER_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
   
    navswitch_init ();

    /* TODO: Initialise IR driver.  */
    ir_uart_init();

    pacer_init (PACER_RATE);
    char* options = "PSR\0";
    int i=0;
    while (1)
    {
        pacer_wait ();
        tinygl_update ();
        navswitch_update ();
        
        if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
            if (i+1 > 3) {
                i=0;
                character = options[i];
            } else {
                character = options[i++];
            }
            
        }

        if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
            if (i-1 < 0) {
                i=2;
                character = options[i];
            } else {
                character = options[i--];
            }
        }
        display_character(character);
        
    // ----------------------- NOT YET NECCESARY-------------------
        /* TODO: Transmit the character over IR on a NAVSWITCH_PUSH
           event.  */
        /*if (navswitch_push_event_p (NAVSWITCH_PUSH))
            ir_uart_putc(character);
            
        
        
        if (ir_uart_read_ready_p ())
        {
            char recv_char;
            recv_char = ir_uart_getc();
            display_character(recv_char);
        }
        * */
    // ----------------------- NOT YET NECCESARY-------------------
        
    }

    return 0;
}
