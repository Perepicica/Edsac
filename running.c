#include <math.h>
#include "running.h"


void last_bits(short int num, int *a) { //переводит опцию(одну букву) в opcode
    int b = 4;                          //и записывает в бинарное представление всей инструкции
    for (int i = 0; i < 5; ++i) {
        int bit = (num >> i) & 1;
        a[b] = bit;
        b--;
    }
}

void fromDecToBin(int num, int *a, bool flag) { //переводит число из дес€тичной системы в двоичную
    if (num) {                                  //и записывает в 11 разр€дов бин представлени€ инструкции
        int bit;
        if (flag) bit = 15;
        else bit = 10;
        while (num != 1) {
            a[bit] = num % 2;
            num /= 2;
            bit--;
        }
        a[bit] = 1;
    }
}

int num_for_shift(struct Command command) { //определ€ет количество сдвигов дл€ опций L и R
    int a[12] = {0};
    int shift_len = 0;
    fromDecToBin(command.address, a, 0);
    if (command.length == 'S') a[11] = 0; else a[11] = 1;
    for (int i = 11; i >= 0; --i) {
        if (a[i]) return shift_len + 1;
        shift_len++;
    }
    return shift_len;
}

void traslation(struct Command command, int *a) { //преобразовывает инструкцию в 17 битное двоичное число
    for (int i = 0; i < 17; ++i) {
        a[i] = 0;
    }
    char *alph = "PQWERTYUIOJJSZKKKFFDDHNMMLXGABCV";
    for (short int j = 0; j < 32; ++j) {  //разр€ды 0-4
        if (command.instruction == alph[j]) {
            last_bits(j, a);
            break;
        }
    }
    fromDecToBin(command.address, a, 1); //разр€ды 5-15
    if (command.length == 'S') a[16] = 0; else a[16] = 1; //разр€д 16
}

void instruction_A(int *line, char len, int which, int *acc) { //прибавл€ет строку line к аккамул€тору
    int carry = 0;                                              //короткое слово,если len == 'S' и длинное если наоборот
    int intermid = 0;                                           //which oпредел€ет какую часть line приб. при len==S
    if (len == 'S') {
        if (which & 1) {
            for (int i = 16; i >= 0; --i) {
                intermid = acc[i] ^ line[i] ^ carry;
                carry = (acc[i] & carry) | (line[i] & carry) | (acc[i] & line[i]);
                acc[i] = intermid;
            }
        } else {
            for (int i = 16; i >= 0; --i) {
                intermid = acc[i] ^ line[i + 18] ^ carry;
                carry = (acc[i] & carry) | (line[i + 18] & carry) | (acc[i] & line[i + 18]);
                acc[i] = intermid;
            }
        }
    } else {
        for (int i = 34; i >= 0; --i) {
            intermid = acc[i] ^ line[i] ^ carry;
            carry = (acc[i] & carry) | (line[i] & carry) | (acc[i] & line[i]);
            acc[i] = intermid;
        }
    }
}

void instruction_S(int *line, char len, int which, int *acc) { //вычитает строку line из аккамул€тора
    int tmp_line[35] = {
            0};    //тут будет доп код               //короткое слово,если len == 'S' и длинное если наоборот
    int tmp_one[35] = {0};   //доп единица                     //which oпредел€ет какую часть line вычит. при len==S

    if (len == 'S') {
        if (which & 1) {
            tmp_one[16] = 1;
            for (int i = 0; i < 17; ++i) {
                tmp_line[i] = !line[i];
            }
            instruction_A(tmp_one, 'S', 1, tmp_line); //складываю единицу с обратным кодом line
            instruction_A(tmp_line, 'S', 1, acc);     //складываю доп код и акк
        } else {
            tmp_one[34] = 1;
            for (int i = 18; i < 35; ++i) {
                tmp_line[i] = !line[i];
            }
            int intermid = 0;
            int carry = 0;
            for (int i = 34; i >= 0; --i) {
                intermid = tmp_line[i] ^ tmp_one[i] ^ carry;
                carry = (tmp_line[i] & carry) | (tmp_one[i] & carry) | (tmp_line[i] & tmp_one[i]);
                tmp_line[i] = intermid;
            }
            instruction_A(tmp_line, 'S', 0, acc);
        }

    } else {
        tmp_one[34] = 1;
        for (int i = 0; i < 35; ++i) {
            tmp_line[i] = !line[i];
        }
        instruction_A(tmp_one, 'L', 0, tmp_line);
        instruction_A(tmp_line, 'L', 0, acc);
    }
}

void instruction_H(int *line, char len, int which, int *multiplier) { //загружает строку line в умножитель
    if (len == 'S') {
        if (which & 1) {
            for (int i = 0; i < 17; ++i) {
                multiplier[i] = line[i];
            }
        } else {
            for (int i = 0; i < 17; ++i) {
                multiplier[i] = line[i + 18];
            }
        }
        for (int j = 17; j < 35; ++j) {
            multiplier[j] = 0;
        }

    } else {
        for (int i = 34; i >= 0; --i) {
            multiplier[i] = line[i];
        }
    }

}

void instruction_T(int *line, char len, int which, int *acc) { //выгружает значение акк в line и очищает его
    if (len == 'S') {
        for (int i = 0; i < 17; ++i) {
            if (which & 1)
                line[i] = acc[i];
            else line[i + 18] = acc[i];
        }
    } else {
        for (int i = 34; i >= 0; --i) {
            line[i] = acc[i];
        }
    }
    for (int j = 0; j < 71; ++j) {
        acc[j] = 0;
    }
}

void instruction_U(int *line, char len, int which, int *acc) { //выгружает значение акк в line
    if (len == 'S') {
        for (int i = 0; i < 17; ++i) {
            if (which & 1)
                line[i] = acc[i];
            else line[i + 18] = acc[i];
        }
    } else {
        for (int i = 34; i >= 0; --i) {
            line[i] = acc[i];
        }
    }
}

void
instruction_C(int *line, char len, int which, int *multiplier, int *acc) { //делает логическое 'и' умножител€ и line
    int acc_tmp[35] = {0};                                                     //результат записывает в акк

    if (len == 'S') {
        for (int i = 0; i < 17; ++i) {
            if (which & 1)
                acc_tmp[i] = line[i] & multiplier[i];
            else acc_tmp[i] = line[i + 18] & multiplier[i];
        }
    } else {
        for (int i = 0; i < 35; ++i) {
            acc_tmp[i] = line[i] & multiplier[i];
        }
    }
    int carry = 0;
    int intermid = 0;
    for (int i = 34; i >= 0; --i) {
        intermid = acc[i] ^ acc_tmp[i] ^ carry;
        carry = (acc[i] & carry) | (acc_tmp[i] & carry) | (acc[i] & acc_tmp[i]);
        acc[i] = intermid;
    }

}

void instruction_R(int shift_len, int *acc) { //сдвиг вправо
    for (int i = 70 - shift_len; i >= shift_len; --i) {
        acc[i + shift_len] = acc[i];

    }
    for (int j = 0; j < shift_len; ++j) {
        acc[j] = 0;
    }
}

void instruction_L(int shift_len, int *acc) { //сдвиг в лево
    for (int i = 0; i <= 70 - shift_len; ++i) {
        acc[i] = acc[i + shift_len];
    }
    for (int j = 71 - shift_len; j < 71; ++j) {
        acc[j] = 0;
    }
}


void instruction_O(FILE *out,int *line,int which) {
    int num=0;
    char *alph = "PQWERTYUIOJJSZKKKFFDDHNMMLXGABCV";
    int d = 1;
    if (which & 1) {
        num = line[4];
        for (int i = 3; i >= 0; --i) {
            d *= 2;
            if (line[i]) num += d;
        }
    } else {
        num = line[22];
        for (int i = 21; i >= 18; --i) {
            d *= 2;
            if (line[i]) num += d;
        }
    }
    fprintf(out,"%c",alph[num]);
}



void instruction_Y(int *acc) { //округл€ет значение акк до 34 бит
    if (acc[35] == 0) {
        for (int i = 35; i <71 ; ++i) {
            acc[i]=0;
        }
    } else {
        for (int i = 35; i <71 ; ++i) {
            acc[i]=0;
        }
        int intermid=0;
        int carry=0;
        int line[35]={0};
        line[34]=1;
        for (int i = 34; i >= 0; --i) {
            intermid = acc[i] ^ line[i] ^ carry;
            carry = (acc[i] & carry) | (line[i] & carry) | (acc[i] & line[i]);
            acc[i] = intermid;
        }
        //что тут со знаком???
    }
}

void running(struct Command *command, int len, FILE *out) {
    int **binary_command; //массив, внутри которого бинарвый эквивалент обычных инструкций из command
    int multiplier[35] = {0};
    int acc[71] = {0};
    int a[17] = {0};
    //кажда€ строка состоит их 2 инструкций как в Long Tank
    binary_command = (int **) calloc((command[0].address - 31) / 2, sizeof(int *));
    for (int l = 0; l < (command[0].address - 31) / 2; ++l) {
        binary_command[l] = (int *) calloc(35, sizeof(int));
    }

    if (binary_command == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }

    int tmp = 1; //счЄтчик дл€ заполнени€
    for (int k = 0; k < (command[0].address - 31) / 2; ++k) {   //заполн€ю его
        traslation(command[tmp], a);
        for (int i = 18; i < 35; ++i) {
            binary_command[k][i] = a[i - 18];
        }
        binary_command[k][17] = 0;
        tmp++;
        traslation(command[tmp], a);
        for (int i = 0; i < 17; ++i) {
            binary_command[k][i] = a[i];
        }
        tmp++;
    }

    bool debug = false;
    int i = 1; //выполн€ю инструкции со второй до точки останова, счЄтчик
    while (i != len) {
        switch (command[i].instruction) {
            case 'A':
                instruction_A(binary_command[(command[i].address - 30) / 2 - 1], command[i].length,
                              command[i].address, acc);
                i++;
                break;
            case 'S':
                instruction_S(binary_command[(command[i].address - 30) / 2 - 1], command[i].length,
                              command[i].address, acc);
                i++;
                break;
            case 'H':
                instruction_H(binary_command[(command[i].address - 30) / 2 - 1], command[i].length,
                              command[i].address, multiplier);
                i++;
                break;
            case 'T':
                instruction_T(binary_command[(command[i].address - 30) / 2 - 1], command[i].length,
                              command[i].address, acc);
                i++;
                break;
            case 'U':
                instruction_U(binary_command[(command[i].address - 30) / 2 - 1], command[i].length,
                              command[i].address, acc);
                i++;
                break;
            case 'C':
                instruction_C(binary_command[(command[i].address - 30) / 2 - 1],
                              command[i].length, command[i].address, multiplier, acc);
                i++;
                break;
            case 'R':
                instruction_R(num_for_shift(command[i]), acc);
                i++;
                break;
            case 'L':
                instruction_L(num_for_shift(command[i]), acc);
                i++;
                break;
            case 'E':
                if (!acc[0]) i = command[i].address - 31;
                else i++;
                break;
            case 'G':
                if (acc[0]) i = command[i].address - 31;
                else i++;
                break;
            case 'O':
                if(out==NULL) break;
                instruction_O(out,binary_command[(command[i].address - 30) / 2 - 1],command[i].address);
                i++;
                break;
            case 'X':
                i++;
                break;
            case 'Y':
                instruction_Y(acc);
                i++;
                break;
            case 'Z':
                debug = 1;
                i++;
                break;

        }
        if (debug) {
            if (command[i - 1].instruction == 'Z') {
                printf("DEBUG\nClick ENTER to make one step\n\"run\" to go to the end\n\"stop\" to end the program\n");
            }
            printf("\nLine %d Instruction %c%d%c", command[i - 1].line, command[i - 1].instruction,
                   command[i - 1].address, command[i - 1].length);
            print_output(out, command, binary_command, acc, 0, multiplier); //печатаю все необходимые данные
            int key; //дл€ возможного нажати€ ENTER
            char comand[10]; // в противном случае сюда будет записана команда
            while (1) {
                key = getc(stdin);
                if (key == 10) break;
                ungetc(key, stdin); //возвращаю символ в потом, чтобы потом считать всЄ слово
                scanf(" %s", comand);
                fflush(stdin); //очистка буфера
                if (!strcmp(comand, "stop")) {
                    printf("Enforced program termination");
                    fclose(out);
                    exit(0);
                }
                if (!strcmp(comand, "run")) {
                    debug = false;
                    break;
                } else printf("Enter right instruction\n");
            }
        }
    }

    print_output(out, command, binary_command, acc, 1, multiplier);
    for (int m = 0; m < (command[0].address - 31) / 2; ++m) {
        free(binary_command[m]);
    }
    free(binary_command);
    free(command);
}

