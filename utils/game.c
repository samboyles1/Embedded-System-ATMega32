/*
 * ENCE260 Embedded Systems Assignment 2017
 * 
 * Sam Boyles 14560776
 * Jerry Yu 
 * 
 * 9th October 2017
 * 
 * A simple 'Paper, Scissors, Rock' game designed to be played using two ATMega32U2 Microcontrollers
 * 
 * This module contains functions that assist in the function of the game Paper Scissors Rock
 * */

#include "system.h"
#include "navswitch.h"
#include "tinygl.h"
#include "pacer.h"
#include <string.h>

#define WELCOME_MSG "PSR. Press button to Start\0"
#define CHAR_LENGTH 2
#define WIN 'W'
#define DRAW 'D'
#define LOSS 'L'

#define PAPER 'P'
#define SCISSORS 'S'
#define ROCK 'R'

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
        case PAPER :
            if (player_them == PAPER) {
                return DRAW;
            } else if (player_them == SCISSORS) {
                return LOSS;
            } else if (player_them == ROCK) {
                return WIN;
            }
            break;
        case SCISSORS :
            if (player_them == PAPER) {
                return WIN;
            } else if (player_them == SCISSORS) {
                return DRAW;
            } else if (player_them == ROCK) {
                return LOSS;
            }
            break;
        case ROCK :
            if (player_them == PAPER) {
                return LOSS;
            } else if (player_them == SCISSORS) {
                return WIN;
            } else if (player_them == ROCK) {
                return DRAW;
            }
            break;
        }
    return 0;
}
/** Cycle through the given string to allow a player to select their option,
 *  continues on button press
 *  @param gametext The string with which selections are to be chosen from
 *  @param gametext_len The length of the given game string
 *  @return The selected value
 */
char select_option(char* gametext, int gametext_len)
{
    int position = 0; 
    char character = gametext[0];
    while (1) 
    {
        pacer_wait();
        tinygl_update();
        navswitch_update();
        //Currently cycles through each of the characters in gametext to enable selection
        if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
            if (position+1 > gametext_len-1) {      
                character = gametext[position];
                position = 0;            
                
            } else {
                character = gametext[position++];
            }
            
        }

        if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
            if (position-1 < 0) {      //start of gametext
                character = gametext[position];
                position = strlen(gametext)-1;            
                
            } else {
                character = gametext[position--];
            }
        }
        display_character(character);
        
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            return character;
        }
    }
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
        tinygl_text("Winner! Press to play again");
    } else if (result == DRAW) {
        tinygl_text("Draw! Press to play again");
    } else if (result == LOSS) {
        tinygl_text("Loser! Press to play again");
    }
    
    while (1) 
    {
        pacer_wait();
        tinygl_update();
        navswitch_update();
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            break;
        }
    }
    
    
}

