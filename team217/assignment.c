/*
 * ENCE260 Embedded Systems Assignment 2017
 * 
 * Sam Boyles 14560776
 * Jerry Yu {}
 * 
 * 9th October 2017
 * 
 * A simple 'Paper, Scissors, Rock' game designed to be played using two ATMega32U2 Microcontrollers
 * */
 
 /*
  * 
  * TODO:
  * -
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

#define MESSAGE_RATE 25

#define GAMETEXT "PSR\0"
#define PLAYERTEXT "12\0"

#define PAPER 'P'
#define SCISSORS 'S'
#define ROCK 'R'

#define WIN 'W'
#define DRAW 'D'
#define LOSS 'L'

#define ACKNOWLEDGE 'A'
#define PLAYER1 '1' /** MASTER **/ 
#define PLAYER2 '2' /** SECONDARY **/




int main (void)
{
    char player = NULL;
    char recv_char = NULL;
    char recv_result = NULL;
    int flag = FALSE;
    char char_to_send = NULL;
    char result = NULL;
    char inverted_result = NULL;
    
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
    player = select_option(PLAYERTEXT);
    /** Select your game selection **/
    char_to_send = select_option(GAMETEXT);
    
    
    
    
    
    
    
    if (player == PLAYER1) {
        while (!flag) {
            pacer_wait();
            recv_char = receive();
            if (recv_char == PAPER || recv_char == SCISSORS || recv_char == ROCK) {
                result = test_for_win(char_to_send, recv_char);
                flag = TRUE;
            }
        }
        
        for (int i = 0; i < 5; i++) {
            transmit(result);
        }
        
        
        display_result(result);
       
    } else if (player == PLAYER2) {
        while (!flag) {
            pacer_wait();
            transmit(char_to_send);
            recv_result = receive();
            if (recv_result == WIN || recv_result == DRAW || recv_result == LOSS) { 
                flag = TRUE;
            }
        }
        inverted_result = invert_result(recv_result);
        display_result(inverted_result);
    
    }
}
    

    
    
   
   
   
   
   
