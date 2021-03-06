#include "parser.h"

#include "parse_method.c" // http__parse_method

#include <stdlib.h> // size_t
#include <string.h> // strnlen, memset

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
    parser->method = http__parse_method(&buf);
    if (parser->method == 0)
        goto error;

    TAKE_CHAR(buf, ' ');

    // second: request path
    char* path = buf;
    TAKE_UNTIL(buf, ' ');

    if (parser->on_url)
        parser->on_url(parser, path, strnlen(path, len));

    // third: HTTP/
    TAKE_CHAR(buf, 'H');
    TAKE_CHAR(buf, 'T');
    TAKE_CHAR(buf, 'T');
    TAKE_CHAR(buf, 'P');
    TAKE_CHAR(buf, '/');

    // major version
    parser->http_major = PEEK(buf) - '0';
    TAKE_ANY(buf);
    TAKE_CHAR(buf, '.');

    // minor version
    parser->http_minor = PEEK(buf) - '0';
    TAKE_ANY(buf);
    TAKE_CHAR(buf, '\r');
    TAKE_CHAR(buf, '\n');

    // parse in headers
    while (PEEK(buf) != '\r') {
        char* field = buf;

        TAKE_UNTIL(buf, ':');
        TAKE_CHAR(buf, ' ');

        char* value = buf;

        TAKE_UNTIL(buf, '\r');
        TAKE_CHAR(buf, '\n');

        if (parser->on_header)
            parser->on_header(parser, field, strnlen(field, len),
                    value, strnlen(value, len));
    }

    // send end of headers
    if (parser->on_headers_complete)
        parser->on_headers_complete(parser);

    // ends with \r\n, end of request
    TAKE_CHAR(buf, '\r');
    TAKE_CHAR(buf, '\n');

    // body of request
    if (PEEK(buf) != '\0') {
        char* body = buf;

        // go until we hit \0 (which is falsy)
        while (*buf)
            ++buf;

        TAKE_CHAR(buf, '\0');

        if (parser->on_body)
            parser->on_body(parser, body, strnlen(body, len));
    }

    return 0;

error:

    return 1;
}

void http_parser_init(http_parser_t* parser) {
    memset(parser, '\0', sizeof(http_parser_t));
}

#undef TAKE_ANY
#undef TAKE_CHAR
#undef TAKE_UNTIL
#undef PEEK
