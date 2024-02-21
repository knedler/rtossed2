// sh.c
#include <stdio.h>
#include <string.h>
#include "sh.h"

#define BUF_SIZE 48
#define MAX_WORDS (BUF_SIZE / 2)

uint8_t get_line(char *buf);
uint8_t parse_string(char *string, uint8_t len, char words[MAX_WORDS][BUF_SIZE]);

uint8_t shell(void)
{
	while (1) {
		char input[BUF_SIZE] = {0};
		char words[MAX_WORDS][BUF_SIZE] = {0};
		uint8_t input_len = 0;
		uint8_t word_count = 0;
	
		// Make shell look like bash
		printf("\r$ ");

		// Get input line
		input_len = get_line(input);

		// Get words from input
		word_count = parse_string(input, input_len, words);

		// If echo
		if ((4 <= input_len) && (!strcmp(words[0], "echo"))) {
			printf("%s\n\r", input + 5);
		}

		printf("Words: %d\r\n", word_count);
	}

	return 0;
}

uint8_t get_line(char *buf)
{
	uint8_t len = 0;
	uint8_t c;

	do {
		// Get char
		c = getchar();

		switch ((char) c) {
		case '\n' : 
		case '\r' :
			// End of line
			buf[len] = '\0';
			printf("\r\n");
			return ++len;
		case '\b' :
			// Backspace
			if (0 < len) {
				len--;
				printf("\b \b");
			}
			break;
		default:
			// Regular char
			buf[len] = c;
			len++;
			printf("%c", c);
			break;
		}

	} while (len < (BUF_SIZE - 1));

	buf[len] = '\0';
	printf("\r\n");

	return ++len;	
}

uint8_t parse_string(char *input_string, uint8_t len, char words[MAX_WORDS][BUF_SIZE])
{
	char last_char = ' ';
	uint8_t word_count = 0;
	uint8_t word_len = 0;

	for (uint8_t i = 0; i < (len - 1); i++) {
		// If 2 or more ' '
		if ((input_string[i] == ' ') && (last_char == ' ')) {
			last_char = input_string[i];
			continue;
		}

		// If end of word
		if ((input_string[i] == ' ') && (last_char != ' ')) {
			words[word_count - 1][word_len] = '\0';
			last_char = input_string[i];
			continue;
		}

		// If new word
		if ((input_string[i] != ' ') && (last_char == ' ')) {
			word_len = 0;
			word_count++;
		}

		// Add chars to word
		words[word_count - 1][word_len] = input_string[i];
		word_len++;
		last_char = input_string[i];

		// If last word add null char
		if (i == (len - 2)) {
			words[word_count - 1][word_len] = '\0';
		}
	}

	return word_count;
}
