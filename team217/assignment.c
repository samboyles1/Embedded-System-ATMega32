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
#include "extra.h"
#include "../fonts/font5x7_1.h"

#define LOOP_RATE 300

#define NULL 0
#define TRUE 1
#define FALSE 0
#define PACER_RATE 300
#define MESSAGE_RATE 25
#define NUM_GAMES 1
#define READY_MESSAGE "Ready?\0"
#define GAMETEXT "PSR\0"
#define GAMETEXT_LEN 3
#define SELECTED "_ - Selected\0"
#define PLAYER1 '1'
#define PLAYER2 '2'
#define WIN '2'
#define DRAW '1'
#define LOSS '0'


static int result = NULL;
static uint8_t recv_result = NULL;
static uint8_t recv_p2_selection = NULL;
static char char_to_send = NULL;
static char player = NULL;


int main (void)
{
    system_init ();
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
    
    navswitch_init ();
    ir_uart_init();
   
    pacer_init (PACER_RATE);
    
    
    
    
    //Cycle through players to select your number
    while (1)
    {
        pacer_wait();
        tinygl_update();
        if (player == NULL)
        {
            player = select_option ("12");
        }
        display_character(player);
        break;
    }
    
    char character = GAMETEXT[0];
    char* gametext = GAMETEXT;
    display_character(character);
    //cycle through PSR selection
    while(1)
    {
        pacer_wait();
        tinygl_update();
        if (char_to_send == NULL) {
            char_to_send = select_option(gametext);
            break;
        }
    }
    
    
    while (1) {
        pacer_wait();
        tinygl_update();
        navswitch_update();
        
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) 
        {
            if (player == PLAYER1) // THEY ARE MASTER BOARD
            {
                while(1)    //PLAYER 1's Game
                {
                    pacer_wait();
                    
                    tinygl_update();
                    //receive
                    if (recv_p2_selection == NULL) {
                        if (ir_uart_read_ready_p ())
                        {
                            recv_p2_selection = ir_uart_getc ();
                        } 
                    }
                    if (recv_p2_selection != NULL)
                    {
                        result = test_for_win(char_to_send, recv_p2_selection);
                    }
                    
                    //compare
                    display_character(result);
                    ir_uart_putc(char_to_send);
                    //transmit and display
                    
                    
                    
                    
                }
            }
                
                
            if (player == PLAYER2) //THEY ARE JUST A PLAYER
            {
                while(1)
                {
                    pacer_wait();
                    tinygl_update();
                    //transmit
                    while(1) {
                        ir_uart_putc(char_to_send);
                        if (ir_uart_read_ready_p ())
                        {
                            recv_result = ir_uart_getc ();
                        } 
                        if (recv_result == WIN || recv_result == DRAW || recv_result == LOSS)
                        {
                            break;
                        }
                    }
                    display_character(recv_result);
                    
                }
            }
        }
    }
}
        
    
