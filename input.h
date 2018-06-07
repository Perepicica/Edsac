

#ifndef EDSAC_MAIN_H
#define EDSAC_MAIN_H


#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>

void about_edsac();
void about_instructions();
bool check_instruction(char option,int which);
bool check_length(char length);

struct Command {
    int line;
    char instruction;
    int address;
    char length;
};

#endif //EDSAC_MAIN_H
