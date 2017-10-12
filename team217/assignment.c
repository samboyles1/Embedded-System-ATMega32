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

#define MESSAGE_RATE 25

#define GAMETEXT "PSR\0"
#define PLAYERTEXT "12\0"


#define WIN 'W'
#define DRAW 'D'
#define LOSS 'L'

#define ACKNOWLEGDE 'A'
#define PLAYER1 '1'
#define PLAYER2 '2'




int main (void)
{
    char player = NULL;
    char recv_char = NULL;
    char recv_result = NULL;
    int flag = FALSE;
    int flag_ack = FALSE;
    char char_to_send = NULL;
    
    
    system_init ();
    tinygl_init (LOOP_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
    navswitch_init ();
    ir_uart_init();
    pacer_init (LOOP_RATE);
    /** Select your player number **/
    player = select_option(PLAYERTEXT);
    /** Select your game selection **/
    char_to_send = select_option(GAMETEXT);
    
    
    if (player == PLAYER1) {        //MASTER PLAYER WORKS {i think}
        while (!flag) {
            pacer_wait();
            recv_char = receive();
            if (recv_char == 'P' || recv_char == 'S' || recv_char == 'R') { //magic num
                transmit(ACKNOWLEGDE);
                flag = TRUE;
                
            }
        }
        
        char result = test_for_win(char_to_send, recv_char);
        
        //need to send before display
        /*
        while (!flag_ack) {
            pacer_wait();
            transmit(result);
            recv_char = receive();
            if (recv_char == ACKNOWLEGDE) { //magic num
                flag_ack = TRUE;
            }
        }
        */
        int i = 5;
        while (i > 0) {
            transmit(ACKNOWLEGDE);
            i--;
        }
        display_character(result);
        int num = 0;
        while (1) {
            pacer_wait();
            if (num) {
                tinygl_update();
            } else {
                transmit(result);
                
            }
            num = !num;
        }
       
       
       
       
       
    } else if (player == PLAYER2) {
        //recv the ack message
        while (!flag) {
            pacer_wait();
            transmit(char_to_send);
            recv_char = receive();
            if (recv_char == ACKNOWLEGDE) { //magic num
                flag = TRUE;
            }
        }
        
        while (!flag_ack) {
            pacer_wait();
            recv_result = receive();
            if (recv_result == WIN || recv_result == DRAW || recv_result == LOSS) { //magic num
                //transmit(ACKNOWLEGDE);
                flag_ack = TRUE;
            }
        }
        
        
        if (recv_result == WIN) {
            display_character(LOSS);
        } else if (recv_result == LOSS) {
            display_character(WIN);
        } else if (recv_result == DRAW) {
            display_character(DRAW);
        }
        
        while (1) {
            pacer_wait();
            tinygl_update();
        }
    
    }
}
    

    
    
   
   
   
   
   
