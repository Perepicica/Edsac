#include "main.h"

bool check_instruction(char option, int which) { //проверка корректности введенных опций
    char *alph_first_check = "ASHVNTUCRLEGIOFXYZPQWYJKDMB";  //общий случай для всех возможных команд
    char *alph_second_check ="ASHVNTUCRLEGIOFXYZ";  //частный случай для команд до точки остановки программы
    char *alph;
    if (which) alph = alph_first_check;
    else alph = alph_second_check;
    int len = strlen(alph);
    int i;
    for (i = 0; i < len; ++i) {
        if (alph[i] == option)break;
    }
    if (i == len) return true;
    return false;
}

bool check_length(char length) {   //проверка корректности введенных длин
    if (length != 'S' && length != 'L')
        return true;
    return false;
}



