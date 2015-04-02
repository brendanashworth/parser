# http parser

This is a server-oriented HTTP request parser. The API is modeled after [Joyent's http-parser](https://github.com/joyent/http-parser), with modifications (for speed, request-only, and other decisions).

## Features
- Callback-based API (`on_header`, `on_url`, `on_body`, with notifications like `on_headers_complete`)
- No copies or allocations (con: edits memory passed)

## API
Documentation pending, especially as it is still in heavy, breaking development.

## Comparison
Using the HTTP request given by [fast-http](https://github.com/fukamachi/fast-http), we aim to outperform the following HTTP C parsers:

- [joyent/http-parser](https://github.com/joyent/http-parser)
- [h2o/picohttpparser](https://github.com/h2o/picohttpparser)
- [taf2/libebb](https://github.com/taf2/libebb)
- [gnu/libmicrohttpd](http://www.gnu.org/software/libmicrohttpd/)

Right now (though it is without most safety checking), on a 2013 Macbook Pro, I'm getting about 2,000,000-1,900,000 requests parsed per second.