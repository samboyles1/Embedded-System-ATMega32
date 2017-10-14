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
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "pacer.h"

#define WELCOME_MSG "PSR. Press button to Start\0"
#define CHAR_LENGTH 2
#define WIN 'W'
#define DRAW 'D'
#define LOSS 'L'

/** 
 * Display the character on the 5x7 display
 * @param character The character to be displayed
 */
void display_character (uint8_t character)
{
    char buffer[CHAR_LENGTH];
    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}

/** Display a simple start screen message. Game starts on button press */
void start_game(void)
{
    tinygl_text(WELCOME_MSG);
    while (1) 
    {
        pacer_wait();
        tinygl_update();
        navswitch_update();
        
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            break;
        }
    }
    
}
/** Simple function which compares each players selections and returns 
 *  the result of the game
 *  @param player_me The selection that was made on this board
 *  @param player_them The selection made by the other player
 *  @return Win, Draw or Loss
 */
char test_for_win(char player_me, char player_them)
{
    
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
/** Cycle through the given string to allow a player to select their option,
 *  continues on button press
 *  @param gametext The string with which selections are to be chosen from
 *  @return The selected value
 */
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
            if (i+1 > 2) {      //magic num change to strlen
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

/**
 * Recieve a character from the IR receiver
 * @return The value read by the IR Receiver
 */
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

/** 
 * Inverts the result so that it can be sent and displayed properly on
 * the other players board. For example, if the result is WIN, invert it 
 * to LOSS so that WIN displays on your board and LOSS displays on your
 * opponents board
 * @param recv_result The result to invert
 * @return The inverted result
 */
char invert_result(char recv_result)
{
    if (recv_result == WIN) {
        return LOSS;
    } else if (recv_result == LOSS) {
        return WIN;
    } else if (recv_result == DRAW) {
        return DRAW;
    }
    return 0;
        
}
/**
 * Set the text to display a string based on the result of the game
 * @param result The result of the game, a single char
 */
void display_result(char result)
{
    if (result == WIN) {
        tinygl_text("Winner");
    } else if (result == DRAW) {
        tinygl_text("Draw");
    } else if (result == LOSS) {
        tinygl_text("Loser");
    }
    
    while (1) 
    {
        pacer_wait();
        tinygl_update();
    }
    
    
}

