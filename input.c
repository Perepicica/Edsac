#include "running.h"

#define INIT_ORD_LINES 30  //����������� ��������� ������� ����������� � ������������ ������ EDSAC

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, ".1251");
    FILE *in;
    FILE *out;

    if (argc == 1) {
        about_edsac();  //������ � ���������
        exit(1);
    }

    if (argc == 2 && strcmp(argv[1], "help_instructions") == 0) {
        about_instructions();  //�������� ���� ����� EDSAC
        exit(1);
    }

    if (argc == 3) {
        in = fopen(argv[1], " rt");
        out = fopen(argv[2], "wt");
        if (in == NULL) {
            printf("Incorrect path to input file %s", argv[1]);
            exit(10);
        }
        if (out == NULL) {
            printf("Incorrect path to output file");
        }
    }

    if (argc > 3) {
        printf("Incorrect number of arguments");
        exit(11);
    }

    int i = 0;  // ������� ��� ���������� ������� ��������
    int ret;    //��� fscanf

    int line;   //��������������� ���������� ��� ������ �� ����� � ���������� ������� ��������
    char instruction;
    int address;
    char length;
    int previousLine = INIT_ORD_LINES;  //���������� ��� �������� ������������ ������� �����
    int lastLine = 1; //����������, � ������� ���������� ����� ���������� ������ � �� ������� ����� �������� ����

    bool f = false;  // ��� �������� ������� �������,����������� ���� ��� �� 1 ������� � ���������� ������ ���������� �����
    struct Command *command;  //������ ��������, � ������� �������� ���

    while (lastLine != 0) {   // ������ �� ����� � ���������� ������� ��������
        ret = fscanf(in, " %d %c %d %c", &line, &instruction, &address, &length);

        if (ret == EOF) {
            printf("Incorrect address in first line");
            exit(12);  //����� ����� ��������� ������ ���� ����� ���������� ��������� ��������� ������ ������ ��� ����� ��������� ������ � ����������������
        }
        if (ret != 4) {
            printf("Incorrect data : line %d \n", previousLine + 1);
            exit(13);
        }
        if (line != previousLine + 1) {
            printf("Incorrect line number: line %d \n ", previousLine + 1);
            exit(14);
        }
        if (check_instruction(instruction, 1)) {
            printf("Incorrect instruction: line %d \n", previousLine + 1);
            exit(15);
        }
        if (check_length(length)) {
            printf("Incorrect length: line %d \n", previousLine + 1);
            exit(16);
        }

        if (!f) {
            if (address < 32) {
                printf("Incorrect address in first line");
                exit(12);
            }
            f = !f; //����������� ���� ��� �� 1 ������� � ���������� ������ ���������� �����
            command = (struct Command *) malloc((address - (INIT_ORD_LINES )) * sizeof(struct Command));
            lastLine = address - (INIT_ORD_LINES + 1);
            if (command == NULL) {
                printf("Memory allocation error");
                exit(17);
            }
        }
        command[i].line = line;
        command[i].instruction = instruction;
        command[i].address = address;
        command[i].length = length;
        i++;
        previousLine = line;
        lastLine--;
    }
    //����� ��������� �������� ������ �������� ������, ����� ��� ��������� � ���(���� �����) �� ���� ���������
    command[command[0].address-31].line = command[0].address;
    command[command[0].address-31].instruction = 'P';
    command[command[0].address-31].address = 0;
    command[command[0].address-31].length = 'S';

    ret = fscanf(in, " %d %c %d %c", &line, &instruction, &address, &length);
    if (ret != EOF) {
        printf("Incorrect address in first line");
        exit(12);  //����� ����� ��������� ������ ���� ����� ���������� ��������� ��������� ������ ������ ��� ����� ��������� ������ � ����������������
    }

    int stop_point = command[0].address -
                     32; //����� ��������� ����������, ���� ��������� ����� ������ ��� ��������� �������������
    while (command[stop_point].instruction != 'Z' &&
           stop_point != 0) {//��� ����� ��������� ���������, �� ���� ���������� Z(���������)
        stop_point--;
    }
    if (stop_point == 0) {
        printf("No end of the program");
        exit(18);
    }

    //��������, ��������� �� ������ � ����� ����������� ����������
    for (int k = 1; k < stop_point; ++k) {
        if (command[k].address >= command[0].address || command[k].address < 31 ||
            check_instruction(command[k].instruction, 0)) {
            if (command[k].instruction != 'L' && command[k].instruction != 'R' && command[k].instruction != 'X' &&
                command[k].instruction != 'Z') {
                printf("Incorrect address or instruction : line %d", k + 31);
                exit(19);
            }
        }
    }
    //��������, ��������� �� ����� �������� � �������� ������
    for (int k = 1; k < stop_point; ++k) {
        if ((command[k].instruction == 'E' && command[k].address > stop_point+31) ||
            (command[k].instruction == 'G' && command[k].address >  stop_point+31)) {
            printf("Incorrect address : line %d", k + 31);
            exit(20);
        }

    }


    running(command, stop_point,out); //���������� ���������
    fclose(in);
}

void about_edsac() {
    printf("����� : ��������� ����������\n"
                   "������ ��������� ���������� ������ ��� EDSAC, ������������� � 1949-1950-�� �����\n"
                   "��� ��������� ���������� ��� ���� ����������� ���������� ��������� ��������� � ������ help_instructions\n"
                   "��� ������� ��������� ��������� � � �������-������ � �������� � ��������� ������\n"
                   "\tex:  C:\\Users\\Desktop\\in.txt  C:\\Users\\Desktop\\out.txt\n"
                   "������� ���� ������ �������� �� ��������� ����� ����:\n"
                   "������ ������ ���� �������� ������ � 10-� ������ ���� ������, ���: \n"
                   "\t������ ������ - ���������� � 31\n"
                   "\t���� �������� - ���� ������(�������� ������� 5 �������� ������������ ���� ����������;\n"
                   "\t������ � 10-� ������ - ������������������ �������� � ������ 0 � 9;\n"
                   "\t����� 31 ������-����� ��������� ������+1;\n"
                   "\t���� ������ - ���� ������ �S� ��� �L�.\n"
                   "\tex:\n"
                   "\t31 T40S\n"
                   "\t32 H36S\n"
                   "\t33 C37S\n");
}

void about_instructions() {
    printf("-A n Add the number in storage location n into the accumulator\n"
                   "-S n Subtract the number in storage location n from the accumulator\n"
                   "-H n Copy the number in storage location n into the multiplier register\n"
                   "-T n Transfer the contents of the accumulator to storage location n and clear the\n"
                   "accumulator\n"
                   "-U n Transfer the contents of the accumulator to storage location n and do not\n"
                   "clear the accumulator\n"
                   "-C n Collate [logical and] the number in storage location n with the number in\n"
                   "the multiplier register and add the result into the accumulator\n"
                   "-R 2^n-2 Shift the number in the accumulator n places to the right\n"
                   "-L ^2n-2 Shift the number in the accumulator n places to the left\n"
                   "-E n If the sign of the accumulator is positive, jump to location n; otherwise\n"
                   "proceed serially\n"
                   "-G n If the sign of the accumulator is negative, jump to location n; otherwise\n"
                   "proceed serially\n"
                   "-O n Print the character represented by the most significant 5 bits of storage\n"
                   "location n\n"
                   "-X No operation\n"
                   "-Y Round the number in the accumulator to 34 bits\n"
                   "-Z Stop the machine and ring the warning bell");
}