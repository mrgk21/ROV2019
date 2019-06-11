# Description
This folder contains:
1. Server file:
	+ Establishes connection via terminal and transmits data in bytes
2. Client File:
	+ Receives data in bytes and parses them to string

### Errors/Issues - Server:
+ `PORT` > 1023 (linux permissive port for tcp)
+ It gives a `bad file descripter` error on incorrect server ending.
+ The server binding sometimes fails because Linux does not close the connection immediately, but waits for a timeout of approx 2 mins before assuming its closed. Solution is to manually close the connection using `tcpkill`.
+ Could not connect to a remote client, the code works for **local client only**. Possible port number issue (will review later).

### Errors/Issues - Client:
+ Add the server ip address `192.168.x.x` in `HOST` and port number in `PORT`
