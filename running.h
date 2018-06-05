#ifndef EDSAC_RUNNING_H
#define EDSAC_RUNNING_H

#include "main.h"

void print_output(FILE*out,struct Command *command,int ** binary_command,int *acc,int end_prog,int *multiplier);
void last_bits(short int num,int*a);

void fromDecToBin(int num,int*a,bool flag);

int num_for_shift(struct Command command);

void traslation(struct Command command,int*a);

void running(struct Command *command, int len,FILE *out);

void instruction_A(int *line, char len, int which ,int *acc);

void instruction_S(int *line, char len, int which ,int *acc);

void instruction_H(int *line, char len, int which, int *multiplier);

void instruction_T(int *line, char len, int which, int *acc);

void instruction_U(int *line, char len, int which, int *acc);

void instruction_C(int *line, char len, int which, int *multiplier, int *acc);

void instruction_R(int shift_len,int *acc);

void instruction_L(int shift_len,int *acc);

void instruction_O(FILE *out,int *line, int which);

void instruction_Y(int *acc);

int from_instr_to_dec(int *line,int which);

#endif //EDSAC_RUNNING_H
