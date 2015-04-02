#ifndef _PARSER_H
#define _PARSER_H

#include <stdlib.h> // size_t

typedef struct http_parser_s http_parser_t;
typedef struct http_request_header_s http_request_header_t;


typedef int (http_data_cb) (http_parser_t*, const char *buf, size_t len);
typedef int (http_cb) (http_parser_t*);


struct http_request_header_s {
    char* name;
    char* value;
};

struct http_parser_s {
    char* http_request_method;
    char* http_request_path;
    int http_request_version_major;
    int http_request_version_minor;

    http_request_header_t** http_request_headers;

    /* PRIVATE */
    int priv_headers;

    /* CALLBACKS */
    http_cb* on_message_begin;
    http_cb* on_headers_complete;
    http_cb* on_message_complete;
    http_data_cb* on_url;
    http_data_cb* on_status;
    http_data_cb* on_header_field;
    http_data_cb* on_header_value;
    http_data_cb* on_body;
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
 * Frees and cleans up an HTTP parser.
 * @param parser The parser to clean up.
 */
void http_parser_cleanup(http_parser_t* parser);

#endif