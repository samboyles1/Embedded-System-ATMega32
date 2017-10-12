#ifndef EXTRA_H
#define EXTRA_H

#include "system.h"


void display_character(uint8_t character);

int test_for_win(uint8_t player_me, uint8_t player_them);

char select_option(char* gametext);

void transmit(char char_to_send);

char receive(void);


#endif
