// modules
const cluster = require('cluster');
const numOfCPUs = require('os').cpus().length;
const http = require('http');
const url = require('url');
const net = require('net');
const process = require('process');
const hashmap = require('hashmap');
const crypto = require('crypto');

// global variables/constants
var port = 10101;
var host = "127.0.0.1";
var stdin = process.openStdin();
var blocked_sites = [];
var cache = new hashmap();

// setup a listener to standard input, to get the input entered
stdin.addListener("data", function(d) {
	// note:  d is an object, and when converted to a string it will
	// end with a linefeed.  so we (rather crudely) account for that
	// with toString() and then trim()

	console.log("you entered: [" + d.toString().trim() + "]");
    blocked_sites.push(d.toString().trim());
});

function log_listener() {
    console.log("Server is running on port: " + port + ", at localhost");
}

var server = http.createServer(requestHandler).listen(port, host, log_listener);
server.addListener('connect', httpsRequestHandler);

function isInArray(value, array) {
  return array.indexOf(value) > -1;
}

function requestHandler(client_req, client_res) {
	// check cache for url..
	var hash_key = crypto.createHash('sha1').update(client_req.url).digest('binary');
    var request_url = url.parse(client_req.url, true);
    var site_name = request_url.host.toString().trim();

    if(isInArray(site_name, blocked_sites)) {
        return client_res.end("<h1>This website has been blocked by the manager of the proxy server!</h1>");
    }

	if(cache.has(hash_key)) {
		// cache has the hashed url key...

		console.log('cached url: ' + client_req.url);
		var url_data = cache.get(hash_key);
		var chunked_data = JSON.stringify(url_data.data);
		var buffered_data = new Buffer(JSON.parse(chunked_data));
        url_data.header['content-length'] = buffered_data.length;
  		url_data.header['accept-encoding'] = client_req.headers['accept-encoding'];
    	console.log(url_data.header['accept-encoding'] + "...." + url_data.header['content-length'] + ", Data: " + chunked_data);
		cache.set(hash_key, url_data);
		client_res.write(url_data.data);
		client_res.writeHead(url_data.status, url_data.header);
		client_res.end();
        return;
	}	else	{
		// cache doesn't have the hashed url key...
		console.log('data not cached');
    	console.log('serve: ' + client_req.url);
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
			console.log("error: " + err);
		});

		// Proxy response/request hadler
		proxy.on('response', function(result) {
			result.on('data', function(data) {
				client_res.write(data, 'binary');
				data_to_cache.push(data);
            });
			result.on('end', function() {
				client_res.end();
                console.log("Data in cache: " + data_to_cache.toString());
				var cache_data = {"header":result.headers,"status":result.statusCode,"data":data_to_cache.toString()};
				cache.set(hash_key, cache_data);
				console.log('Data cached!!!!!!!!!');
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

function httpsRequestHandler(client_req, client_socket_conn, client_res) {
    console.log("serve " + client_req.url);
    var site_name = client_req.url.split(':')[0];
	console.log("host: " + site_name);

    if(isInArray(site_name, blocked_sites)) {
        return client_res.end("<h1>This website has been blocked by the manager of the proxy server!</h1>");
    }

    var server_socket_conn = new net.Socket();
    server_socket_conn.connect(client_req.url.split(':')[1], client_req.url.split(':')[0], function() {
        server_socket_conn.write(client_res);
        client_socket_conn.write("HTTP/"+client_req.httpVersion+" 200 Connection established\r\n\r\n");

    });

    server_socket_conn.on('end', function() {
        client_socket_conn.end();
    });
    client_socket_conn.on('data', function(data_chunk) {
        server_socket_conn.write(data_chunk);
    });
    server_socket_conn.on('data', function(data_chunk) {
        client_socket_conn.write(data_chunk);
    });
    client_socket_conn.on('end', function() {
        server_socket_conn.end();
    });
}
