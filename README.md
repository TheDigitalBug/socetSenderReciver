# Project Name : socketSenderReciver

# Info:
This project shows how to send and receive data via TCP/IP using Socket.
socketSenderReciver is written on C project.
Client-Server architecture.

#
![servClient](/servClient.png?raw=true "servClient")

# Usage:
```
$> make make
```
run server:
```
$> ./server
SERVER is started
Waiting...
```
run client:
```
$> ./client 192.168.0.123 1238 MOV004.MOD
// ./client serverIP serverPORT fileToSend

```
