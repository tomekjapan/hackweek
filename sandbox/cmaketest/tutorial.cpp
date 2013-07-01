#include <stdio.h>
#include <stdlib.h>

#include "sandbox/MathFunctions/MathFunctions.h"

#include "TutorialConfig.h"

int main (int argc, char* argv[])
{
	if ( argc < 2 )
	{
		fprintf(
			stdout,
			"%s Version %d.%d\n",
			argv[0],
			Tutorial_VERSION_MAJOR,
			Tutorial_VERSION_MINOR
		);

		fprintf( stdout, "Usage: %s number\n", argv[0] );
		return 1;
	}

	const double inputValue = atof( argv[1] );
	const double outputValue = mysqrt( inputValue );

	fprintf( stdout, "Square root of %g is %g\n", inputValue, outputValue );
	return 0;
}
