
#ifndef Tools_Tools_h
#define Tools_Tools_h


// Library includes

// Project includes
#include "Files.h"
#include "Strings.h"

// Forward declaration

// Namespace declarations


#define BIT(x) (1<<(x))

#define aeDelete( p )	\
	if ( p ) {			\
		delete p;		\
		p = 0;			\
	}


namespace Tools {


void readline(FILE *f, char *string);
void readstr(FILE *f, char *string);


}


#endif
