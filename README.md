<<<<<<< HEAD
# TwoSocket Chat

This project was built to understand the fundamentals of networking in C, including sockets, binding, listening for connections, accepting clients and more
=======
# TwoSockeChat

It is a simple two-client TCP chat server written in C.
>>>>>>> a126ca3 (Chat improvement and error fixed)

## How It Works

The server listens for connections on:

- Address: `127.0.0.1`
- Port: `8080`
- Protocol: TCP
- Address family: IPv4

The server waits for exactly two clients to connect.

When one client sends a message, the server receives it and forwards it to the other client.

