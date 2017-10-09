/*
 * ENCE260 Embedded Systems Assignment 2017
 * 
 * Sam Boyles 14560776
 * Jerry Yu {}
 * 
 * 9th October 2017
 * */
 
 /*
  * 
  * TODO:
  * -set up the ir communtications (need two boards to test)
  * -put entire game into a loop so they can play (3) games and then display final scores at the end
  * -get rid of 'magic numbers'
  * -abstract extra methods out into their own files
  * 
  * 
  * */
#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"


#define PACER_RATE 500
#define MESSAGE_RATE 25
#define CHAR_LENGTH 2
#define READY_MESSAGE "Ready?\0"
#define GAMETEXT "PSR\0"
#define GAMETEXT_LEN 3
#define SELECTED "_ - Selected\0"
#define ZERO_i 0

#define WIN 2
#define DRAW 1
#define LOSS 0









static int final_score = 0;
static int break_flag = 0;

//Function for displaying a single character
//SHOULD BE ABSTRACTED OUT
void display_character (char character)
{
    char buffer[CHAR_LENGTH];
    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}

// Returns 0 if you lose, 1 if a draw, 2 if you win
//SHOULD BE ABSTRACTED OUT
int test_for_win(char player_me, char player_them) {
    
    switch(player_me) {
        case 'P' :
            if (player_them == 'P') {
                return DRAW;
            } else if (player_them == 'S') {
                return LOSS;
            } else if (player_them == 'R') {
                return WIN;
            }
            break;
        case 'S' :
            if (player_them == 'P') {
                return WIN;
            } else if (player_them == 'S') {
                return DRAW;
            } else if (player_them == 'R') {
                return LOSS;
            }
            break;
        case 'R' :
            if (player_them == 'P') {
                return LOSS;
            } else if (player_them == 'S') {
                return WIN;
            } else if (player_them == 'R') {
                return DRAW;
            }
            break;
        }
    return 0;
}
    
    

int main (void)
{
    char character = GAMETEXT[0];
    system_init ();
    tinygl_init (PACER_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    navswitch_init ();

    ir_uart_init();

    pacer_init (PACER_RATE);
    
    char* gametext = GAMETEXT;
    display_character(gametext[0]);
    int i = ZERO_i;        //magic num
    
    while (break_flag == 0)
    {
        pacer_wait ();
        tinygl_update ();
        navswitch_update ();
        
        
        //Currently cycles through each of P S R to enable selection of paper scissors or rock
        if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
            if (i+1 > 2) {      //magic num
                character = gametext[i];
                i= ZERO_i;            //magic num
                
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
            break_flag = 1;
            break;
        }
    }
    break_flag = 0;
    char* selected = SELECTED;
    selected[0] = character;    //Set the text to display what option was selected
    tinygl_text(selected);      //Display to the user what option they chose
    
    
    /* Second while loop for sending and checking */
    while(break_flag == 0) {
        pacer_wait ();
        tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
        tinygl_update ();
        navswitch_update ();
        
        
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            ir_uart_putc(character);
            display_character('T');
            break_flag = 1;
            break;
        }
        
        /*
        if (ir_uart_read_ready_p ())
        {
            char recv_char;
            recv_char = ir_uart_getc();
            display_character(recv_char);
            break;
        }
        * */
        
        
    }
    //JUST TESTING WITH HARDCODED VALUES, will need to put each players values here
    int winVal = test_for_win('P', 'R');
    while (1) {
        pacer_wait();
        tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
        tinygl_update();
        navswitch_update();
        
        
        //this text isnt scrolling
        if (winVal == 2) {
            tinygl_text("YOU WIN!\0");
        }
    }

    return 0;
}
