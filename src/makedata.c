#include <stdlib.h>
#include <time.h>
#include "lib/bus.h"

int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));

	init(argc, argv);
	//unittest();
	execute();
}

