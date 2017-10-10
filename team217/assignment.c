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
#include "../fonts/font5x7_1.h"

#define LOOP_RATE 300

#define NULL 0
#define TRUE 1
#define FALSE 0
#define PACER_RATE 300
#define MESSAGE_RATE 25
#define CHAR_LENGTH 2
#define NUM_GAMES 1
#define READY_MESSAGE "Ready?\0"
#define GAMETEXT "PSR\0"
#define GAMETEXT_LEN 3
#define SELECTED "_ - Selected\0"
#define ZERO_i 0
#define WIN_MESSAGE "_"

#define WIN 2
#define DRAW 1
#define LOSS 0

static int win_flag = 0;
static int gamecount;
static char final_score = 48;
static char teammate_score = 48;
static int recv = FALSE;
static int sent = FALSE;
static uint8_t recv_char = NULL;
static uint8_t character = NULL;


//Function for displaying a single character
//SHOULD BE ABSTRACTED OUT
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
    
    

int main (void)
{
    for (gamecount=0; gamecount < NUM_GAMES; gamecount++) {
        char recv_char = NULL;
        char character = GAMETEXT[0];
       uint8_t ack = NULL;
        system_init ();
        tinygl_init (LOOP_RATE);
        tinygl_font_set (&font5x7_1);
        tinygl_text_speed_set (MESSAGE_RATE);
        tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
        
        navswitch_init ();
        ir_uart_init();
       
        pacer_init (PACER_RATE);
    
        char* gametext = GAMETEXT;
        display_character(gametext[0]);
        int i = ZERO_i;        //magic num
        
        while (1)
        {
            pacer_wait ();
            tinygl_update ();
            navswitch_update ();
            
            //Currently cycles through each of P S R to enable selection of paper scissors or rock
            if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
                if (i+1 > 2) {      //magic num
                    character = gametext[i];
                    i= ZERO_i;            //magic num
                    
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
                break;
            }
        }
        
        
        
        //MAYBE NOT
        tinygl_text_mode_set (TINYGL_TEXT_MODE_STEP);
        char* selected = SELECTED;
        selected[0] = character;    //Set the text to display what option was selected
        tinygl_text(selected);      //Display to the user what option they chose
        
       
       
       
       
        
        /*
        while (1)
        {
            pacer_wait();
            ir_uart_putc (character);
        }
       
       
       
        while (1)
        {
            pacer_wait();
            ir_uart_putc(character);
            if (ir_uart_read_ready_p ())
            {
                recv_char = ir_uart_getc ();
                recv = TRUE;
               // Process the received byte.
           }
        }
        
        
           */ 
        
        
        int exit = FALSE;
        /*Maybe*/
        while (1) {
            pacer_wait();
            tinygl_update();
            navswitch_update();
            
            if (!recv) {
                ir_uart_putc(character);
                sent = TRUE;
            }
            /*
            if (navswitch_push_event_p (NAVSWITCH_PUSH) && !sent)
                ir_uart_putc(character);
                sent = TRUE;
            */
            if (ir_uart_read_ready_p () && !recv)
            {
                
                recv_char = ir_uart_getc ();
                display_character(recv_char);
                recv = TRUE;
                ir_uart_putc('a');
                
            }
            
            
            while (sent && recv) {
                
                
                
                if (ir_uart_read_ready_p ()) 
                {
                    ack = ir_uart_getc ();
                    if (ack == 'a') {
                        exit = TRUE;
                        break;
                        display_character(recv_char);
                    }
                }
            }
            
            if (exit) {
                break;
            }
                    
                   
        }
        
        while(1) {
            display_character(character);
            tinygl_update();
        }
    }
    
}
            
            /*
        int winVal = test_for_win(character, recv_char);
        
        while (1) {
            pacer_wait();
            tinygl_update();
            navswitch_update();
            if (winVal == WIN) {
                display_character('W');
                win_flag = 1;
                
                
            } else if (winVal == LOSS){
                display_character('L');
                teammate_score += 1;
            
            } else {
                display_character('D');
            }
        }
        
    }
}
        /*
        while(1) {
            pacer_wait ();
            tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
            tinygl_update ();
            navswitch_update ();



            if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
                while (1) {
                    if (!sent) {
                        ir_uart_putc(character);
                    }
                    
                    if (ir_uart_read_ready_p () && recv == FALSE) {
                        recv_char = ir_uart_getc();
                        if (recv_char != NULL) {
                            recv = TRUE;
                        }
                        if (recv) {
                            ir_uart_putc('c');
                            
                            char test = ir_uart_getc();
                        
                            if (test == 'c') {
                                sent = TRUE;
                            }
                        }
                        
                        display_character(recv_char);
                        
                        if (sent == TRUE && recv == TRUE){
                            break;
                        }
                    }
                    
                }
                
            }
            
        }
        
        
        
        
        */
        
        
        
        /* Second while loop for sending and checking */
        
        
        /*
        while(1) {
            pacer_wait ();
            tinygl_update ();
            navswitch_update ();
            if (navswitch_push_event_p (NAVSWITCH_PUSH)){
                push = 1;
            }
            
            ir_uart_putc(character);
            if (push) {
                if (ir_uart_read_ready_p()) {
                    recv_char = ir_uart_getc();
                    display_character(recv_char);
                    //break;
                    
                }
            }
        }
    
        
    
        
        
        
        
        
        */
        
        /*
        //JUST TESTING WITH HARDCODED VALUES, will need to put each players values here
        int winVal = test_for_win(character, recv_char);
        while (1) {
            pacer_wait();
            tinygl_update();
            navswitch_update();
            
            //this text isnt scrolling
            if (winVal == WIN) {
                display_character('W');
                win_flag = 1;
                
                
            } else if (winVal == LOSS){
                display_character('L');
                teammate_score += 1;
            
            } else {
                display_character('D');
            }
            if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
                break;
            }
        }
    if (win_flag == 1) {
        final_score += 1;
        win_flag = 0;
    }
    } //end game loop by number of games
    
    //finish screen
    
    
    //abstract out
    char* end_msg;
    end_msg = "working";
    
    //lots of magic numbers
    switch(final_score) {
        case 48 :   //ascii 0
            end_msg = "Score: 0\0";
            break;
        case 49 :   //ascii 1
            end_msg = "Score: 1\0";
            break;
        case 50 :   //ascii 2
            end_msg = "Score: 2\0";
            break;
        case 51 :   //ascii 3
            end_msg = "Score: 3\0";
            break;
    }
    
    tinygl_text_mode_set(TINYGL_TEXT_MODE_SCROLL);
    tinygl_text(end_msg);
  
    //when game ends diplay results
    while (1) {
        pacer_wait();
        tinygl_update();        
    }
    return 0;
}
*/

