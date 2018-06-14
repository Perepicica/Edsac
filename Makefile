edsac.exe : input.o running.o check_data.o output.o
	gcc -o edsac.exe input.o running.o check_data.o output.o

input.o : input.c
	gcc -std=c11 -pedantic -Wall -Wextra -c input.c

running.o : running.c
	gcc -std=c11 -pedantic -Wall -Wextra -c running.c
	
output.o : output.c
	gcc -std=c11 -pedantic -Wall -Wextra -c output.c

check_data.o : check_data.c
	gcc -std=c11 -pedantic -Wall -Wextra -c check_data.c
