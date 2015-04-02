#include "src/parser.h"

#include <string.h> // strcmp, strlen
#include <assert.h> // assert

char data[] = {
    "POST /path HTTP/1.1\r\n"
    "Content-Length: 18\r\n"
    "\r\n"
    "hello world :) ftw"
};

int called = 0;

int cb_body(http_parser_t* parser, const char* body, size_t len) {
    assert(strcmp(body, "hello world :) ftw") == 0);
    assert(strlen(body) == len);

    called++;
    return 0;
}

int main() {
    http_parser_t parser;
    http_parser_init(&parser);
    parser.on_body = &cb_body;

    int result = http_parser_execute(&parser, data, strlen(data));
    assert(result == 0);

    assert(called == 1);

    return 0;
}