// modules
const http = require('http');
const marky = require('marky');
const url = require('url');
const net = require('net');
const hashmap = require('hashmap');
const crypto = require('crypto');

// global variables/constants
var port = 10101;
var host = "127.0.0.1";
var stdin = process.openStdin();
var blocked_sites = [];
var cache = new hashmap();

// Listener for the console, so that the desired websites to be
// blocked can be gotten.
stdin.addListener("data", function(d) {
    console.log("______ URL blocked: [" + d.toString().trim() + "] ______");
    blocked_sites.push(d.toString().trim());
});

// Method to print out the date and time of initialisation of the server, as well
// as the Host and Port that it's listening on.
function log_listener() {
    console.log("____________ " + (new Date()) + " <--> proxy server is running on port: " + port + ", at " + host + " ____________");
}

// Server initialisation, as well as attachment of listeners/handler.
var server = http.createServer(requestHandler).listen(port, host, log_listener);
server.addListener('connect', httpsRequestHandler);

// HTTP handler
function requestHandler(client_req, client_res) {
    // initalise timer.
    marky.mark("HTTP Request");
    // get hash key of URL.
    var hash_key = crypto.createHash('sha1').update(client_req.url).digest('binary');
    var request_url = url.parse(client_req.url, true);
    var site_name = request_url.host.toString().trim();

    // Checking if URL is blocked
    if(isInArray(site_name, blocked_sites)) {
        console.log("______ blocked site : '" + site_name + "' was attempted to be accessed.");
        client_res.write("HTTP/" + client_req.httpVersion + " 403 Forbidden\r\n\r\n");
        client_res.end();
    }   else    {
        // If URL is not blocked, then check the cache for it.
        if(cache.has(hash_key)) {
            // Write the data corressponding to the cached URL to the
            // client_res variable which is sent to back to the client.
            var url_data = cache.get(hash_key);
            var chunked_data = JSON.stringify(url_data.data);
            client_res.writeHead(url_data.status, url_data.header);
            var buffered_data = new Buffer(JSON.parse(chunked_data));
            cache.set(hash_key, url_data);
            client_res.write(buffered_data);
            client_res.end();

            // Stop the timer, and print to the management console the time
            // taken for the request to be fullfilled.
            marky.stop("HTTP Request");
            var times = marky.getEntries();
            var time = times[0].duration;
            marky.clear();
            console.log('cache --> ' + request_url.host.toString().trim() + ' //\\\\ elapsed time --> ' + time + 'ms');
            return;
        }	else	{
            // cache doesn't have the hashed url key...
            // url to cache once finish
            var data_to_cache = [];

            // must be transported accross port 80 for http request....
            var options = {
                host: request_url.host,
                port: 80,
                path: request_url.path,
                method: client_req.headers['method']
            }

            // Proxy variable/requesting.....
            var proxy = http.request(options);

            // Proxy error handler
            proxy.end();
            proxy.on('error', function(err) {
                console.log("*________ error: " + err + " ________*");
            });

            // Proxy response/request hadler
            proxy.on('response', function(result) {
                result.on('data', function(data) {
                    client_res.write(data, 'binary');
                    data_to_cache.push(data);
                });
                result.on('end', function() {
                    client_res.end();
                    var cache_data = {"header":result.headers,"status":result.statusCode,"data":data_to_cache.toString()};
                    cache.set(hash_key, cache_data);

                    marky.stop("HTTP Request");
                    var times = marky.getEntries();
                    var time = times[0].duration;
                    marky.clear();
                    console.log('serve --> ' + request_url.host.toString().trim() + ' //\\\\ elapsed time --> ' + time + 'ms');
                });
                client_res.writeHead(result.statusCode, result.headers);
            });
            client_req.on('data', function(data) {
                proxy.write(data, 'binary');
            });
            client_req.on('end', function() {
                proxy.end();
            });
        }
    }
}

function httpsRequestHandler(client_req, client_socket_conn, client_res) {
    var site_name = client_req.url.split(':')[0];
    var hash_key = crypto.createHash('sha1').update(site_name).digest('binary');

    if(isInArray(site_name, blocked_sites)) {
        console.log("______ blocked site : '" + site_name + "' was attempted to be accessed.");
        client_socket_conn.write("HTTP/" + client_req.httpVersion + " 403 Forbidden\r\n\r\n");
        client_socket_conn.end();
        return;
    } else  {
        if(cache.has(hash_key)) {
            // cache has the hashed url key...

            console.log('cache_S --> ' + client_req.url.split(':')[0]);
            var url_data = cache.get(hash_key);
            client_socket_conn.write(url_data.data);
            client_socket_conn.end();
            return;
        } else {
            var data_to_cache = [];
            console.log("serve_S --> " + site_name);

            var server_socket_conn = net.connect(client_req.url.split(':')[1], client_req.url.split(':')[0], () => {
                server_socket_conn.write(client_res);
                client_socket_conn.write("HTTP/" + client_req.httpVersion + " 200 Connection established\r\n\r\n");
            });

            server_socket_conn.on('data', function(message) {
                data_to_cache.push(message);
                client_socket_conn.write(message);
            });
            server_socket_conn.on('end', function() {
                client_socket_conn.end();
                var cache_data = {"header":null,"status":null,"data":data_to_cache.toString()};
                cache.set(hash_key, cache_data);
            });
            server_socket_conn.on('error', function() {
                //console.log("*________ error : proxy socket _________*");
                client_socket_conn.write("HTTP/" + client_req.httpVersion + " 500 Connection error\r\n\r\n");
                client_socket_conn.end();
            });
            client_socket_conn.on('data', function(message) {
                server_socket_conn.write(message);
            });
            client_socket_conn.on('end', function() {
                server_socket_conn.end();
            });
            client_socket_conn.on('error', function() {
                //console.log("*_________ error : client/brower socket _________*");
                server_socket_conn.end();
            });
        }
    }
}

// Utility functions
function isInArray(value, array) {
    return array.indexOf(value) > -1;
}
