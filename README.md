# Project Name : socketSenderReciver

# Info:
socketSenderReciver is written on C project.
Client-Server architecture.
I wrote this code to send file from Client to Server.
The Server should save the file locally. 

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
