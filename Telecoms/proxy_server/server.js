// modules
var http = require('http');
var url = require('url');

// global variables/constants
var port = 10101;
var host = "127.0.0.1";
function log_listener() {
    console.log("Server is running on port: " + port + ", at localhost");
}

var server = http.createServer(function(client_req, client_res) {
    console.log('serve: ' + client_req.url);

    var request_url = url.parse(client_req.url, true);

    var options = {
        host: request_url.host,
        port: port,
        path: request_url.path,
        method: client_req.headers['method']
    }

    // Proxy variable/requesting.....
    var proxy = http.request(options);

    // Proxy error handler
    proxy.end();
    proxy.on('error', function(err) {
        console.log("error: " + err);
    });

    // Proxy response/request hadler
    proxy.on('response', function(result) {
        result.on('data', function(chunk) {
            client_res.write(chunk, 'binary');
        });
        result.on('end', function() {
            client_res.end();
        });
        client_res.writeHead(result.statusCode, result.headers);
    });
    client_req.on('data', function(chunk) {
        proxy.write(chunk, 'binary');
    });
    client_req.on('end', function() {
        proxy.end();
    });
}).listen(port, host, log_listener);
