#include	<stdio.h>
#include	"fl.h"
#include	"ws13.h"


/**
 *	process(fmt, data, field_separator,char record_separator)
 *
 *	Purpose: read from datafile, format and output selected records
 *	Input:   fmt 		- input stream from format file
 *		 data		- stream from datafile
 *	Output:  copied fmt to stdout with insertions
 *	Errors:  not reported, functions call fatal() and die
 *	
 **/
void process(FILE *fmt, FILE *data, char field_separator,
 char record_separator)
{
	symtab_t *tab;

	if ( (tab = new_table()) == NULL )
		fatal("Cannot create storage object","");

	while ( get_record(tab, data, field_separator,
	 record_separator) != NO ) /* while more data	*/
	{
		mailmerge( tab, fmt );		/* merge with format	*/
		clear_table(tab);
	}

	free_table(tab);			/* no memory leaks!	*/
}

