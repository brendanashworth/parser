#ifndef _PARSER_H
#define _PARSER_H

#include <stdlib.h> // size_t

typedef struct http_parser_s http_parser_t;


typedef int (http_cb) (http_parser_t*);
typedef int (http_data_cb) (http_parser_t*, const char* buf, size_t len);
typedef int (http_header_cb) (http_parser_t*, const char* field, size_t len1,
        const char* value, size_t len2);


struct http_parser_s {
    unsigned int method;
    unsigned int http_major;
    unsigned int http_minor;

    /* CALLBACKS */
    http_cb*        on_headers_complete;
    http_data_cb*   on_url;
    http_data_cb*   on_body;
    http_header_cb* on_header;
};

// See https://tools.ietf.org/html/rfc7231#section-4
enum http_method {
    HTTP_GET     = 1,
    HTTP_HEAD    = 2,
    HTTP_POST    = 3,
    HTTP_PUT     = 4,
    HTTP_DELETE  = 5,
    HTTP_CONNECT = 6,
    HTTP_OPTIONS = 7,
    HTTP_TRACE   = 8,
};

/**
 * Executes the HTTP parser on a buffer.
 * @param  parser A null-initialized http_parser struct.
 * @param  buf    A writable buffer containing an HTTP request.
 * @param  len    The length of buf.
 * @return        Zero on success, non-zero on failure.
 */
int http_parser_execute(http_parser_t* parser, char* buf, size_t len);

/**
 * Initiate the HTTP parser by null-fill.
 * @param parser The HTTP parser.
 */
void http_parser_init(http_parser_t* parser);

#endif
