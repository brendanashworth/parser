#include "parser.h"

#include <stdlib.h> // size_t, malloc
#include <string.h> // strnlen

#define TAKE_ANY(ptr) ++ptr

// ensure next character in ptr == chr, then bump and continue
#define TAKE_CHAR(ptr, chr)                                                   \
    if (*ptr != chr) goto error;                                              \
    ++ptr

// bumps the ptr until we get a chr (call on \0), then we null and continue
#define TAKE_UNTIL(ptr, chr)                                                  \
    while (*ptr != chr && *ptr != '\0')                                       \
        ++ptr;                                                                \
    if (*ptr == '\0') goto error;                                             \
    *ptr = '\0'; ++ptr

// give the char at the tip of the pointer
#define PEEK(ptr) *ptr

int http_parser_execute(http_parser_t* parser, char* buf, size_t len) {
    // first: request method
    parser->http_request_method = buf;

    TAKE_UNTIL(buf, ' ');

    // second: request path
    parser->http_request_path = buf;
    
    TAKE_UNTIL(buf, ' ');

    /*if (parser->on_url)
        parser->on_url(parser, path, strnlen(path, len));*/
    // on_url

    // third: HTTP/
    TAKE_CHAR(buf, 'H');
    TAKE_CHAR(buf, 'T');
    TAKE_CHAR(buf, 'T');
    TAKE_CHAR(buf, 'P');
    TAKE_CHAR(buf, '/');

    // major version
    parser->http_request_version_major = (PEEK(buf) == '1') ? 1 : PEEK(buf) - '0';
    TAKE_ANY(buf);
    TAKE_CHAR(buf, '.');

    // minor version
    parser->http_request_version_minor = (PEEK(buf) == '1') ? 1 : PEEK(buf) - '0';
    TAKE_ANY(buf);
    TAKE_CHAR(buf, '\r');
    TAKE_CHAR(buf, '\n');

    int headers = 0;
    while (1) {
        // ends with \r\n, end of headers
        if (PEEK(buf) == '\r') {
            TAKE_CHAR(buf, '\r');
            TAKE_CHAR(buf, '\n');
            break;
        }

        ++headers; // parse another header

        // allocate space for a new header
        http_request_header_t* header = malloc(sizeof *header);

        // reallocate with space for another header
        parser->http_request_headers = realloc(parser->http_request_headers,
                headers * sizeof(http_request_header_t*));

        // append new header location to list of headers in parser
        *(parser->http_request_headers + headers - 1) = header;

        header->name = buf;
        TAKE_UNTIL(buf, ':');
        TAKE_CHAR(buf, ' ');
        header->value = buf;

        TAKE_UNTIL(buf, '\r');
        TAKE_CHAR(buf, '\n');
    }

    parser->priv_headers = headers;

    return 0;

    // continue onto call if we have completed the entire packet
call:
    // this is called when the parser reaches \0; it will call the callbacks
    // provided

error:
    // free everything; no point in keeping it
    http_parser_cleanup(parser);

    return 1;
}

void http_parser_cleanup(http_parser_t* parser) {
    int headers = parser->priv_headers;

    for (; headers != 0; --headers)
        free( *(parser->http_request_headers + headers - 1) );

    free(parser->http_request_headers);
    parser->http_request_headers = NULL;
}

#undef TAKE_ANY
#undef TAKE_CHAR
#undef TAKE_UNTIL
#undef PEEK