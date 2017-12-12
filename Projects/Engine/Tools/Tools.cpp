
// Header
#include "Tools.h"

// Library includes

// Project includes

// Namespace declarations


namespace Tools {


void readline(FILE *f, char *string)
{
    do {
	    fgets(string, 255, f);
    } while ((string[0] == '/') ||  (string[0] == '\n'));

    return;
}

void readstr(FILE *f, char *string)
{
    do {
	    fgets(string, 255, f);
    } while (/*(string[0] == '/') ||*/ (string[0] == '\n'));

    return;
}


}
