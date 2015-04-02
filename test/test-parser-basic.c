#include "src/parser.h"

#include <stdlib.h> // free
#include <string.h> // strcmp, strlen, strdup
#include <assert.h> // assert

char data[] = {
    "GET /cookies HTTP/1.1\r\n"
    "Host: 127.0.0.1:8090\r\n"
    "Connection: keep-alive\r\n"
    "Cache-Control: max-age=0\r\n"
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
    "\r\n"
    "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.17 "
    "(KHTML, like Gecko) Chrome/24.0.1312.56 Safari/537.17\r\n"
    "Accept-Encoding: gzip,deflate,sdch\r\n"
    "Accept-Language: en-US,en;q=0.8\r\n"
    "Accept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.3\r\n"
    "Cookie: name=wookie\r\n"
    "\r\n"
};

int function_calls = 0;

int cb_url_path(http_parser_t* parser, const char* buf, size_t len) {
    function_calls++;

    assert(strcmp(buf, "/cookies") == 0);
    assert(strlen(buf) == len);

    return 0;
}

int cb_header(http_parser_t* parser, const char* field, size_t len1,
        const char* value, size_t len2) {
    function_calls++;

    assert(strlen(field) == len1);
    assert(strlen(value) == len2);

    return 0;
}

int main() {
    http_parser_t parser;
    http_parser_init(&parser);
    parser.on_url = &cb_url_path;
    parser.on_header = &cb_header;

    char* buf = strdup(data);

    int result = http_parser_execute(&parser, buf, strlen(buf));
    assert(result == 0);

    // asserts
    assert(strcmp(parser.method, "GET") == 0);

    assert(parser.http_major == 1);
    assert(parser.http_minor == 1);

    // 9 headers, plus other function calls
    assert(function_calls == 1 + 9);

    free(buf);

    return 0;
}