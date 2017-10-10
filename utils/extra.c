#include "system.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "pacer.h"


#define CHAR_LENGTH 2
#define WIN "Win\0"
#define DRAW "Draw\0"
#define LOSS "Loss\0"

void display_character (uint8_t character)
{
    char buffer[CHAR_LENGTH];
    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}

char* test_for_win(uint8_t player_me, uint8_t player_them)
{
    char* returnVal;
    switch(player_me) {
        case 80 :
            if (player_them == 80) {
                returnVal = DRAW;
            } else if (player_them == 83) {
                returnVal = LOSS;
            } else if (player_them == 82) {
                returnVal = WIN;
            }
            return returnVal;
            break;
        case 83 :
            if (player_them == 80) {
                returnVal = WIN;
            } else if (player_them == 83) {
                returnVal = DRAW;
            } else if (player_them == 82) {
                returnVal = LOSS;
            }
            return returnVal;
            break;
        case 82 :
            if (player_them == 80) {
                returnVal = LOSS;
            } else if (player_them == 83) {
                returnVal = WIN;
            } else if (player_them == 82) {
                returnVal = DRAW;
            }
            return returnVal;
            break;
        }
    return 0;
}

char select_option(char* gametext)
{
    int i = 0;
    char character = gametext[0];
    while (1) 
    {
        pacer_wait();
        tinygl_update();
        navswitch_update();
        //Currently cycles through each of P S R to enable selection of paper scissors or rock
        if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
            if (i+1 > 2) {      //magic num
                character = gametext[i];
                i= 0;            //magic num
                
            } else {
                character = gametext[i++];
            }
            
        }

        if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
            if (i-1 < 0) {      //magic num
                character = gametext[i];
                i=2;            //magic num
                
            } else {
                character = gametext[i--];
            }
        }
        display_character(character);
        
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            return character;
        }
    }
}

/**
 * Transmit the character you want to send over the ir blaster.
 * @param char_to_send The character you want to transmit
 **/
void transmit(char char_to_send)
{
    ir_uart_putc(char_to_send);
}

char receive(void)
{
    if (ir_uart_read_ready_p ())
    {
        uint8_t data;
        data = ir_uart_getc ();
        return data;
    } 
    return 0;
    
}

    

