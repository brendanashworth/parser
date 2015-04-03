#include "src/parser.h"
#include "bench/bench.c"

#include <string.h> // memset, strdup

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

int main() {

    int len = strlen(data);

    BENCHMARK(simple, 10)

        char* buf = strdup(data);

        http_parser_t parser;
        http_parser_init(&parser);

        http_parser_execute(&parser, buf, len);

        free(buf);

    END_BENCHMARK(simple)
    BENCHMARK_SUMMARY(simple);

    return 0;
}