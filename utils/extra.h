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

#ifndef EXTRA_H
#define EXTRA_H

#include "system.h"

/** Display a simple start screen message. Game starts on button press */
void start_game(void);

/** 
 * Display the character on the 5x7 display
 * @param character The character to be displayed
 */
void display_character(uint8_t character);

/** Simple function which compares each players selections and returns 
 *  the result of the game
 *  @param player_me The selection that was made on this board
 *  @param player_them The selection made by the other player
 *  @return Win, Draw or Loss
 */
int test_for_win(uint8_t player_me, uint8_t player_them);

/** Cycle through the given string to allow a player to select their option,
 *  continues on button press
 *  @param gametext The string with which selections are to be chosen from
 *  @return The selected value
 */
char select_option(char* gametext);

/**
 * Transmit the character you want to send over the ir blaster.
 * @param char_to_send The character you want to transmit
 **/
void transmit(char char_to_send);

/**
 * Recieve a character from the IR receiver
 * @return The value read by the IR Receiver
 */
char receive(void);

/** 
 * Inverts the result so that it can be sent and displayed properly on
 * the other players board. For example, if the result is WIN, invert it 
 * to LOSS so that WIN displays on your board and LOSS displays on your
 * opponents board
 * @param recv_result The result to invert
 * @return The inverted result
 */
char invert_result(char recv_result);

/**
 * Set the text to display a string based on the result of the game
 * @param result The result of the game, a single char
 */
void display_result(char result);

#endif
