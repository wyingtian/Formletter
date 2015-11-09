#include	<stdio.h>
#include	<stdlib.h>
#include 	<stdbool.h>
#include	<string.h>
#include	"fl.h"
/*
 *	Get record function for formletter program version 1.0
 *	usage: called by fl.c
 *  Author: ying
 */

char *parse_name (char *input);
char *parse_val (char *input);
char *read_one_field (FILE *fp, char field_separator, 
	char record_separator, bool *is_end_line, bool *is_end_file);
bool is_empty (char *input);
bool has_equal_sign(char * input);
bool non_blank_name (char *input);

/* Get one record, read in one filed at a time
 * skip the empty line. Line with empty field will be processed */
int get_record(symtab_t *tp, FILE *fp, char field_separator, char record_separator) {
	bool is_end_line = false;
	bool is_end_file = false;
	int res = 0;
	bool has_content = false;
	int num_read_field = 0;
	bool is_empty_field = true;
	/* skip the empty line */
	while ((!has_content) && (!is_end_file)) {
		while (!is_end_line) {
			/* read in one field */
			char *field = read_one_field(fp, field_separator,
				record_separator, &is_end_line, &is_end_file);
			num_read_field++;
			if (is_empty(field)) {
				is_empty_field = true;
				continue;
			} else {
				is_empty_field = false;
			}
			/* check validation */
			if (!has_equal_sign(field)) {
				char msg[255]; 
				sprintf(msg,"field \"%s\" is invalid",field);
				fatal("no equal sign:", msg);
			}
			if (!non_blank_name(field)) {
				char msg[255]; 
				sprintf(msg,"field \"%s\" is invalid",field);
				fatal("field name is blank:", msg);
			}
			/* parse to the name  */
			char *name = parse_name(field);
			/* parse to the value */
			char *value = parse_val(field);
			/* save to the table  */
			res = insert(tp, name, value);
			if (res == NO) {
				fatal("Fail to insert to table", field);
			}
			/* free the readin */
			free(name);
			free(value);
			free(field);
		}
		/* check whether it's a blank line */
		if (num_read_field == 1 && is_empty_field) {
			has_content = false;
		} else {
			has_content = true;
		}
		is_end_line = false;
	}

	if (is_end_file) {
		return NO;
	} else {
		return YES;
	}
}


/* read one field, content  between two field_separators */
char* read_one_field (FILE *fp, char field_separator,
 char record_separator, bool *is_end_line, bool *is_end_file) {

	int buf_len = MAXFLD + MAXVAL  + 3;
	char* read_content = (char *) malloc (sizeof(char) * buf_len);
	/* read untile the field seperator */
	int read_char;
	int id = 0;
	while ((read_char = fgetc(fp)) != EOF && read_char != record_separator
	 && read_char != field_separator) {
		read_content[id] = (char)read_char;
		id++;
		if (id >= buf_len) {
			buf_len *= 2;
		    read_content = (char *) realloc(read_content, buf_len);
		}
	}
	if (read_char == record_separator || read_char == EOF) {
		*(is_end_line) = true;
	} else {
		*(is_end_line) = false;
	}
	if (read_char == EOF) {
		*(is_end_file) = true;
	} else {
		*(is_end_file) = false;
	}
	read_content[id] = 0;
	return read_content;
}

/* check if it is empty */
bool is_empty (char *input) {
	int i = 0;
	if (strlen(input) == 0) return true;
	for (i = 0; i < strlen(input); i++) {
		if (input[i] != ' ' && input[i] != '\t') return false; 
	}
	return true;
}

/* check validation */
bool is_valid (char *input) {
	int i = 0;
	bool is_space = true;
	int input_len = strlen(input);
	for (i = 0; i < input_len; i++) {
		if (input[i] == '=') {
			if (is_space) {
				return false;
			} else {
				return true;
			}
		} else if (input[i] != ' ' && input[i] != '\t') {
			is_space = false;
		}
	}
	return false;
}

/* check if the filed name is empty*/
bool non_blank_name (char *input) {
	int i = 0;
	bool is_space = true;
	int input_len = strlen(input);
	for (i = 0; i < input_len; i++) {
		if (input[i] == '=') {
			if (is_space) {
				return false;
			} else {
				return true;
			}
		} else if (input[i] != ' ' && input[i] != '\t') {
			is_space = false;
		}
	}
	return false;
}
/* check if there are equal sign in a field*/
bool has_equal_sign(char * input){
	int i = 0;
	int input_len = strlen(input);
	for (i = 0; i < input_len; i++) {
		if (input[i] == '=') 
			return true;
	}
	return false;
}


/* get the value field, */
char *parse_val (char *input) {
	char *res = (char *) malloc (sizeof(char) * (MAXVAL + 1));
	memset(res, 0, sizeof(char) * (MAXVAL + 1));
	res[0] = 0;
	int i = 0;
	int end = strlen(input) - 1;
	bool is_copy = false;
	int copy_id = 0;
	bool after_equal = false;
	for (i = 0; i <= end; i++) {
		if (input[i] == '=' && (!after_equal)) {
			after_equal = true;
			continue;
		}
		if (after_equal && !is_copy) {
			if (input[i] != ' ' && input[i] != '\t') {
				is_copy = true;
				res[copy_id] = input[i];
				copy_id++;
			}
		} else if (is_copy) {
			res[copy_id] = input[i];
			copy_id++;
		}
	}
	res[copy_id] = 0;
	return res;
}

/* paser the name */
char *parse_name (char *input) {
	char *res = (char *) malloc(sizeof(char) * (MAXFLD + 1));
	memset(res, 0, sizeof(char) * (MAXFLD + 1));
	int i = 0;
	int equal_id = 0;
	int start = 0;
	int end = strlen(input) - 1;
	bool found = false;
	/* find the non-space start */
	for (i = 0; i < strlen(input); i++) {
		if (input[i] != ' ' && input[i] != '\t' && !found) {
			start = i;
			found = true;
		} else if (input[i] == '=') {
			equal_id = i;
			break;
		}
	}
	/* find the non-space tail */
	for (i = equal_id - 1; i > start; i--) {
		if (input[i] != ' ' && input[i] != '\t') {
			end = i;
			break;
		}
	}
	/* copy the results */
	for (i = start; i <= end; i++) {
		res[i - start] = input[i];
	}
	res[end + 1] = 0;

	return res;
}

