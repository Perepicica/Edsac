#include "running.h"

int from_instr_to_dec(int *line, int which) {
    int d = 1;
    int m;
    if (which & 1) {
        m = line[16];
        for (int i = 15; i >= 1; --i) {
            d *= 2;
            if (line[i]) m += d;
        }
        if (line[0]) m *= -1;
    } else {
        m = line[34];
        for (int i = 33; i >= 19; --i) {
            d *= 2;
            if (line[i]) m += d;
        }
        if (line[18])m *= -1;
    }
    return m;
}

void print_output(FILE *out, struct Command *command, int **binary_command, int *acc, int end_prog, int *multiplier) {

    //во всей функции печать проверяется на условие окончания программы
    // если true всё печатается в выходной файл else в консоль (debug)
    if (end_prog) printf("The program is completed");
    if (out == NULL) exit(110);
    if (end_prog) fprintf(out, "\n*** THE PROGRAM IS COMPLETED ***\n\nACCUMULATOR:\n");
    else printf("\nAccumulator:\n");

    for (int i = 0; i < 71; ++i) {
        if ((i == 17 || i == 35) && end_prog) fprintf(out, " ");
        if ((i == 17 || i == 35) && !end_prog) printf(" ");
        if (end_prog) fprintf(out, "%d", acc[i]); else printf("%d", acc[i]);
    }

    if (end_prog) fprintf(out, "\nMultiplier:\n");
    else printf("\nMultiplier:\n");
    for (int i = 0; i < 35; ++i) {
        if ((i == 17) && end_prog) fprintf(out, " "); //для удобства
        if ((i == 17) && !end_prog) printf(" ");
        if (end_prog) fprintf(out, "%d", multiplier[i]); else printf("%d", multiplier[i]);
    }

    if (end_prog) fprintf(out, "\nLongTank:\n"); else printf("\nLongTank:\n");
    int num;
    if (command[0].address & 1) num = command[0].address;
    else num = command[0].address - 1;

    for (int j = (command[0].address - 31) / 2 - 1; j >= 0; --j) {

        if (end_prog) fprintf(out, "%d-%d ", num, num - 1);
        else printf("%d-%d ", num, num - 1);

        for (int i = 0; i < 35; ++i) {

            if (end_prog) fprintf(out, "%d", binary_command[j][i]);
            else printf("%d", binary_command[j][i]);

            if (i == 16 || i == 17) {  //для удобства
                if (end_prog)fprintf(out, " "); else printf(" ");
            }
        }
        if (end_prog)
            fprintf(out, "  %d %d", from_instr_to_dec(binary_command[j], 1),
                    from_instr_to_dec(binary_command[j], 0));
        else
            printf("  %d %d", from_instr_to_dec(binary_command[j], 1),
                   from_instr_to_dec(binary_command[j], 0));
        if (end_prog)fprintf(out, "\n"); else printf("\n");
        num -= 2;
    }

    if (end_prog)
        fclose(out);
}




