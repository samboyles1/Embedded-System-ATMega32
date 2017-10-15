/*
 * ENCE260 Embedded Systems Assignment 2017
 * 
 * Sam Boyles 14560776
 * Jerry Yu
 * 
 * 9th October 2017
 * 
 * A simple 'Paper, Scissors, Rock' game designed to be played using two ATMega32U2 Microcontrollers
 * */
 
#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "game.h"
#include "transmission.h"
#include "../fonts/font5x7_1.h"

#define LOOP_RATE 300

#define NULL 0
#define TRUE 1
#define FALSE 0

#define MESSAGE_RATE 30

#define GAMETEXT "PSR\0"
#define GAMETEXT_LEN 3
#define PLAYERTEXT "12\0"
#define PLAYERTEXT_LEN 2

#define PAPER 'P'
#define SCISSORS 'S'
#define ROCK 'R'

#define WIN 'W'
#define DRAW 'D'
#define LOSS 'L'

#define ACKNOWLEDGE 'A'
#define PLAYER1 '1' /** MASTER **/ 
#define PLAYER2 '2' /** SECONDARY **/

static char player = NULL;

int main (void)
{
    
    //Initialise the game
    system_init ();
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    navswitch_init ();
    ir_uart_init();
    pacer_init (LOOP_RATE);
    
    //Display start screen
    start_game();
    
    /** Select your player number **/
    player = select_option(PLAYERTEXT, PLAYERTEXT_LEN);
    
    while (1) {
        /** Initialse main and variables at the start of each new game**/
        pacer_wait();
        tinygl_update();
        navswitch_update();
        char recv_char = NULL;
        char recv_result = NULL;
        int flag = FALSE;
        char char_to_send = NULL;
        char result = NULL;
        char inverted_result = NULL;
        
        /** Select your game selection **/
        char_to_send = select_option(GAMETEXT, GAMETEXT_LEN);
        
        /** Game logic for Player 1 (The master board), which does the main comparisons **/
        if (player == PLAYER1) {
            while (!flag) {
                pacer_wait();
                recv_char = receive();
                if (recv_char == PAPER || recv_char == SCISSORS || recv_char == ROCK) {
                    result = test_for_win(char_to_send, recv_char);
                    flag = TRUE;
                }
            }
            pacer_wait();
            transmit(result);
            transmit(result);
            /** Displays the results on the LED matrix. Game can be restarted by pressing the nav button **/
            display_result(result);
        
        /** Game logic for Player 2 **/
        } else if (player == PLAYER2) {
            while (!flag) {
                pacer_wait();
                transmit(char_to_send);
                recv_result = receive();
                if (recv_result == WIN || recv_result == DRAW || recv_result == LOSS) { 
                    flag = TRUE;
                }
            }
            /** Reverse the result that player 2 received, otherwise both boards would show winner (eg) **/
            inverted_result = invert_result(recv_result);
            /** Displays the results on the LED matrix. Game can be restarted by pressing the nav button **/
            display_result(inverted_result);
        }
    }
}
    

    
    
   
   
   
   
   
