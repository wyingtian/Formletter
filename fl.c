#include	<stdio.h>
#include	<stdlib.h>
#include 	<stdbool.h>
#include	<string.h>
#include	"fl.h"

/*
 *	formletter program version 1.0
 *
 *	usage: fl format < datafile
 *
 *	data comes from stdin, output goes to stdout
 */

static char	*myname ;		/* used by fatal() */

int main(int ac, char *av[])
{
	FILE *fpfmt, *data;
	
	myname = *av;

	/*
	 * 	check that there is at least one arg: the format file
	 */

	if ( ac == 1 )
		fatal("usage: fl format [datafile..]","");

	/* 
	 * This for loop below is to add change command line option function
	 * the -D, is to make the field seperator be default ';', 
	 * -dY is to make the field seperator using 'Y'
	 * -r# is to make the record seperator using '#'
	 * The default field_separator id ';', record_separator is'\n'
	*/
	int i = 1;
	char field_separator = ';';
	char record_separator = '\n';
	bool has_data = false;
	bool hasfmt = false;
	for (i = 1; i < ac; i++) {
		if (av[i][0] == '-') {
			if (av[i][1] == 'D') {
				field_separator = '\n';
			} else if (av[i][1] == 'd') {
				field_separator = av[i][2];
			} else if (av[i][1] == 'r') {
				record_separator = av[i][2];
			}
		} else {
			if (!hasfmt) {
				if (  (fpfmt = fopen( av[i] , "r")) == NULL )    
					fatal("Cannot open format file:", av[i]);
				hasfmt = true;
			} else {
				if (  (data = fopen( av[i] , "r")) == NULL )
					fatal("Cannot open data file:", av[i]);
				process(fpfmt, data, field_separator, record_separator);
				has_data = true;
				fclose(data);
			}
		}
	}
	/* if can't open file report usage to add format file*/
	if (!hasfmt) {
		fatal("usage: fl format [format_file..]","");
	}
	/* if noe record data, process from stdin*/
	if (!has_data) {
		process(fpfmt, stdin, field_separator, record_separator);
	}
	fclose(fpfmt);

	return 0;
}
/* the function to report to error message*/
void fatal(char *s1, char *s2)
/*
 *	fatal error handler
 *	purpose: print error message to stderr then exit
 *	input:   two strings that are printed 
 *	never returns
 */
{
	fprintf(stderr, "%s: %s%s\n",  myname, s1, s2 );
	exit(1);
}




