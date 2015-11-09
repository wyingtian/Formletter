#include	"ws13.h"
#include <stdio.h>
/*
 *	some constants
 */

#define	MAXFLD	40
#define	MAXVAL	120

/*
 *	function declarlations
 */

/* the function to get a record */
int	get_record(symtab_t *, FILE *, char field_separator, char record_separator);
/* the function to merge format data and record data */
void	mailmerge( symtab_t *, FILE *);
/* the function to process format data and record data */
void	process(FILE*, FILE*, char field_separator, char record_separator);
/* the function to report to error message*/
void	fatal(char *, char *);
