#include "src/parser.h"

#include <stdlib.h> // free
#include <string.h> // memset, strdup
#include <assert.h> // assert

http_parser_t parser;

// do not use http_parser_cleanup - will automatically be called on error
#define TEST_PARSER_ERROR(data)                                       \
    memset(&parser, '\0', sizeof(http_parser_t));                     \
    buf = strdup(data);                                               \
    assert(http_parser_execute(&parser, buf, strlen(buf)) != 0);      \
    free(buf)

int main() {
    char* buf;

    // initial request line
    TEST_PARSER_ERROR("GET");
    TEST_PARSER_ERROR("GET ");
    TEST_PARSER_ERROR("GET /");
    TEST_PARSER_ERROR("GET /path");
    TEST_PARSER_ERROR("GET /path ");
    TEST_PARSER_ERROR("GET /path H");
    TEST_PARSER_ERROR("GET /path HT");
    TEST_PARSER_ERROR("GET /path HTT");
    TEST_PARSER_ERROR("GET /path HTTP");
    TEST_PARSER_ERROR("GET /path HTTP/");
    TEST_PARSER_ERROR("GET /path HTTP/1");
    TEST_PARSER_ERROR("GET /path HTTP/1.");
    TEST_PARSER_ERROR("GET /path HTTP/1.1");
    TEST_PARSER_ERROR("GET /path HTTP/1.1\r");
    TEST_PARSER_ERROR("GET /path HTTP/1.1\r\n");
    TEST_PARSER_ERROR("GET /path HTTP/1.1\r\n\r");

    // with headers
    TEST_PARSER_ERROR(
        "GET / HTTP/1.1\r\n"
        "Content-Type"
    );
    TEST_PARSER_ERROR(
        "GET / HTTP/1.1\r\n"
        "Content-Type:"
    );
    TEST_PARSER_ERROR(
        "GET / HTTP/1.1\r\n"
        "Content-Type: "
    );
    TEST_PARSER_ERROR(
        "GET / HTTP/1.1\r\n"
        "Content-Type: text/plain"
    );
    TEST_PARSER_ERROR(
        "GET / HTTP/1.1\r\n"
        "Content-Type: text/plain\r"
    );
    TEST_PARSER_ERROR(
        "GET / HTTP/1.1\r\n"
        "Content-Type: text/plain\r\n"
        "Host: \r\n"
    );

    // these should actually succeed
    // TEST_PARSER_ERROR("GET /path HTTP/1.1\r\n\r\n");

    return 0;
}