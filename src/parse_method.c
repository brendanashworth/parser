#include "parser.h"

#define PEEK(ptr) **ptr

#define TAKE_CHAR(ptr, chr)                                                   \
    if (**ptr != chr) goto error;                                             \
    ++(*ptr)

// private, inlined method to parse method (GET, POST, etc) from request
// returns 0 on error
unsigned int http__parse_method(char** buf) {
	// break is not used because it isn't necessary (we return)
    switch (PEEK(buf)) {
    case 'G': // GET
        TAKE_CHAR(buf, 'G');
        TAKE_CHAR(buf, 'E');
        TAKE_CHAR(buf, 'T');

        return HTTP_GET;
    case 'P': // POST or PUT
        TAKE_CHAR(buf, 'P');

        switch (PEEK(buf)) {
        case 'O':
            TAKE_CHAR(buf, 'O');
            TAKE_CHAR(buf, 'S');
            TAKE_CHAR(buf, 'T');

            return HTTP_POST;
        case 'U':
            TAKE_CHAR(buf, 'U');
            TAKE_CHAR(buf, 'T');

            return HTTP_POST;
        }
    case 'H': // HEAD
    	TAKE_CHAR(buf, 'H');
    	TAKE_CHAR(buf, 'E');
    	TAKE_CHAR(buf, 'A');
    	TAKE_CHAR(buf, 'D');

    	return HTTP_HEAD;
    case 'D': // DELETE
    	TAKE_CHAR(buf, 'D');
    	TAKE_CHAR(buf, 'E');
    	TAKE_CHAR(buf, 'L');
    	TAKE_CHAR(buf, 'E');
    	TAKE_CHAR(buf, 'T');
    	TAKE_CHAR(buf, 'E');

    	return HTTP_DELETE;
    case 'C': // CONNECT
    	TAKE_CHAR(buf, 'C');
    	TAKE_CHAR(buf, 'O');
    	TAKE_CHAR(buf, 'N');
    	TAKE_CHAR(buf, 'N');
    	TAKE_CHAR(buf, 'E');
    	TAKE_CHAR(buf, 'C');
    	TAKE_CHAR(buf, 'T');

    	return HTTP_CONNECT;
    case 'O': // OPTIONS
    	TAKE_CHAR(buf, 'O');
    	TAKE_CHAR(buf, 'P');
    	TAKE_CHAR(buf, 'T');
    	TAKE_CHAR(buf, 'I');
    	TAKE_CHAR(buf, 'O');
    	TAKE_CHAR(buf, 'N');
    	TAKE_CHAR(buf, 'S');

    	return HTTP_OPTIONS;
    case 'T': // TRACE
    	TAKE_CHAR(buf, 'T');
    	TAKE_CHAR(buf, 'R');
    	TAKE_CHAR(buf, 'A');
    	TAKE_CHAR(buf, 'C');
    	TAKE_CHAR(buf, 'E');

    	return HTTP_TRACE;
    }

error:
	return 0;
}

#undef PEEK
#undef TAKE_CHAR
