# TwoSockeChat

It is a simple two-client TCP chat server written in C.

## Features 

- Choose custom username

## How It Works

The server listens for connections on:

- Address: `127.0.0.1`
- Port: `8080`
- Protocol: TCP
- Address family: IPv4

The server waits for exactly two clients to connect.

When one client sends a message, the server receives it and forwards it to the other client.

