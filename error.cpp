#include"scanner.h"

void Error::printOut() {
	printf("%s", errorImformation);
}

Error ErrorFactory::createError(const char* errorImformation,int type) {
	char* Char = (char*)malloc(100*sizeof(char));
	switch (type) {
	case 1:
		sprintf(Char, "\nerror in %d line,need Token %s\n", scanner->getLineNo(), errorImformation); break;
	}
	errorImformation = Char;
	return Error(errorImformation);
}



