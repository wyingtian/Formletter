#include	<stdio.h>
#include	<stdlib.h>
#include 	<stdbool.h>
#include	<string.h>
#include	"fl.h"

/*
 *	merge format data and record data function for formletter program version 1.0
 *	usage: called by fl.c
 *  Author: ying
 */

char* handle_name_field (symtab_t *tp, FILE *fmt);

/* the function to merge format data and record data */
void mailmerge(symtab_t *tp, FILE *fmt) {
	fseek(fmt, 0L, SEEK_SET);
	int read_char;
	while ((read_char = fgetc(fmt)) != EOF) {
		if (read_char == '%') {
			char *val = handle_name_field(tp, fmt);
			if (val != NULL) {
				fprintf(stdout, "%s", val); 
				free(val);
			}
		} else {
			fprintf(stdout, "%c", (char)read_char);
		}
	}
}
/* 
 * handle the name filed
 * note: if the next char read is '%', then just return %, since
 * it's %% case. Else read everything, and convert to
 * comtent and return the converted value
 */
char* handle_name_field (symtab_t *tp, FILE *fmt) {

	int read_char;
	char nameBuffer[MAXFLD + 1];
	memset(nameBuffer, 0, MAXFLD);
	int char_id = 0;
	while ((read_char = fgetc(fmt)) != EOF && read_char != '%') {
		nameBuffer[char_id] = (char)read_char;
		char_id++;
	}
	if(char_id > MAXFLD)
	fatal("field name is unterminated or exceed MAX length","");
	nameBuffer[char_id] = 0;
	if (char_id == 0) {
		char *val = (char*) malloc (sizeof(char) * 2);
		val[1] = 0;
		val[0] = '%';
		return val;
	}
	if (nameBuffer[0] == '!') {
		/* call linux cmd */
		table_export(tp);
		fflush(stdout);
		system(&nameBuffer[1]);
	}
	char *find_val = lookup(tp, nameBuffer);
	if (find_val == NULL) {
		return NULL;
	} else {
		char *val =	(char *) malloc(sizeof(char) * (MAXVAL + 1));
		strcpy(val, lookup(tp, nameBuffer));
		return val;
	}
}