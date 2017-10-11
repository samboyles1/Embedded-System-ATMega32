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
#define ZERO_i 0
#define WIN_MESSAGE "_"
#define ACKNOWLEGDE 'A'
#define PLAYER1 '1'
#define PLAYER2 '2'

static uint8_t recv_char = NULL;
static int recv_p1 = FALSE;
static int sent_p1 = FALSE;
static int recv_p2 = FALSE;
static int sent_p2 = FALSE;
static int flag = FALSE;
static uint8_t ack_p1 = NULL;
static uint8_t ack_p2 = NULL;
static char char_to_send = NULL;
static char player = NULL;


//Function for displaying a single character
//SHOULD BE ABSTRACTED OUT
/*
void display_character (uint8_t character)
{
    char buffer[CHAR_LENGTH];
    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}

// Returns 0 if you lose, 1 if a draw, 2 if you win
//SHOULD BE ABSTRACTED OUT
int test_for_win(uint8_t player_me, uint8_t player_them) {
    
    switch(player_me) {
        case 80 :
            if (player_them == 80) {
                return DRAW;
            } else if (player_them == 83) {
                return LOSS;
            } else if (player_them == 82) {
                return WIN;
            }
            break;
        case 83 :
            if (player_them == 80) {
                return WIN;
            } else if (player_them == 83) {
                return DRAW;
            } else if (player_them == 82) {
                return LOSS;
            }
            break;
        case 82 :
            if (player_them == 80) {
                return LOSS;
            } else if (player_them == 83) {
                return WIN;
            } else if (player_them == 82) {
                return DRAW;
            }
            break;
        }
    return 0;
}
    
*/

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
    
    
    
    while (1)
    {
        pacer_wait();
        tinygl_update();
        navswitch_update();
        
        while (1) {
            if (player == PLAYER1) {
                while (recv_char == NULL) { 
                    recv_char = receive();
                }
                if (recv_char != NULL && recv_char != ACKNOWLEGDE) {
                    transmit(ACKNOWLEGDE);
                }
                recv_p1 = TRUE;
                break;
                
            } else if (player == PLAYER2) {
                
                transmit(char_to_send);
                ack_p2 = receive();
                if (ack_p2 != NULL && ack_p2 != 0) {
                    sent_p2 = TRUE;
                    break;
                }
            }
        }
        
        while (1) {
            if (player == '1') {
                transmit(char_to_send);
                ack_p1 = receive();
                if (ack_p1 != NULL && ack_p1 != 0) {
                    sent_p1 = TRUE;
                    break;
                }
            
                
            } else if (player == '2') {
                
                while (recv_char == NULL) { 
                    recv_char = receive();
                }
                if (recv_char != NULL && recv_char != ACKNOWLEGDE) {
                    transmit(ACKNOWLEGDE);
                }
                recv_p2 = TRUE;
                break;
            }
        }
        
        
        //if ((sent_p1 && recv_p1 && ack_p1) || (sent_p2 && recv_p2 && ack_p2)) {
          //  break;
        //}
    
        tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
        char* result = test_for_win(char_to_send, recv_char);
        
        tinygl_text(result);
        //display_character(result);
        while (1) {
            while (!flag) {
                transmit(char_to_send);
                flag = TRUE;
            }
            pacer_wait();
            tinygl_update();
        }
    }
}
        
