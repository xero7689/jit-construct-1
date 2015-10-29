#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "util.h"

void interpret(const char *const input)
{
	// Initialize the tape with 30,000 zeroes.
	uint8_t tape[30000] = { 0 };

	// Set the pointer to point at the left most cell of the tape.
	uint8_t *ptr = tape;

        // Variable for repeat pattern
        int pc;
        int offset;

	char current_char;
	for (int i = 0; (current_char = input[i]) != '\0'; ++i) {
                //printf("CC: %c\n", current_char);
		switch (current_char) {
		case '>':
                        pc = 0;
                        offset = i;
                        while (input[offset++] == '>')
                            pc += 1;
                        ptr += pc;
                        i = offset - 2;
			break;
		case '<':
                        pc = 0;
                        offset = i;
                        while (input[offset++] == '<')
                            pc += 1;
                        ptr -= pc;
                        i = offset - 2;
			break;
		case '+':
                        pc = 0;
                        offset = i;
                        while (input[offset++] == '+')
                            pc += 1;
                        *ptr += pc;
                        i = offset - 2;
			break;
		case '-':
                        pc = 0;
                        offset = i;
                        while (input[offset++] == '-')
                            pc += 1;
                        *ptr -= pc;
                        i = offset - 2;
			break;
		case '.':
			putchar(*ptr);
			break;
		case ',':
			*ptr = getchar();
			break;
		case '[':
                        // Pattern [-]                 
                        offset = i;
                        if (input[offset+1] == '-' && input[offset+2] == ']') {
                            *ptr = 0;
                            i += 2;
                            break;
                        }

                        // Pattern [>+<-]
                        if (input[offset+1] == '>' &&\
                            input[offset+2] == '+' &&\
                            input[offset+3] == '<' &&\
                            input[offset+4] == '-' &&\
                            input[offset+5] == ']') {
                            *(ptr+1) += *ptr;
                            *ptr = 0;
                            i += 5;
                            break;
                        }
                        
                        // Default
			if (!(*ptr)) {
				int loop = 1;
				while (loop > 0) {
					current_char = input[++i];
					if (current_char == ']')
						--loop;
					else if (current_char == '[')
						++loop;
				}
			}
			break;
		case ']':
			if (*ptr) {
				int loop = 1;
				while (loop > 0) {
					current_char = input[--i];
					if (current_char == '[')
						--loop;
					else if (current_char == ']')
						++loop;
				}
			}
			break;
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2) err("Usage: interpreter <inputfile>");
	char *file_contents = read_file(argv[1]);
	if (file_contents == NULL) err("Couldn't open file");
	interpret(file_contents);
	free(file_contents);
}
