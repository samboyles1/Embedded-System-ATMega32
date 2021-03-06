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
 * This module contains the functions required for transmission
 * */

#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include "system.h"

/**
 * Transmit the character you want to send over the ir blaster.
 * @param char_to_send The character you want to transmit
 */
void transmit(char char_to_send);

/**
 * Recieve a character from the IR receiver
 * @return The value read by the IR Receiver
 */
char receive(void);


#endif
