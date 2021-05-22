# Project 5: Socket Programming

Starting from this project, we'll move from switches and routers to the end hosts -- the computers that utilize the network for communication with each other.
The standard way to write programs that communicate over the network is socket programming.
While originally developed for Unix computers programmed in C, the socket abstraction is general and not tied to any specific operating system or programming language. This allows programmers to use the socket mental model to write correct network programs in many contexts.

This project will give you experience with basic socket programming in C.  Specifically, you will write a pair of programs (a client and a server) that send and receive arbitrary messages over the Internet.
Your client and server should meet the following specifications. Be sure to read these before and after programming to make sure your implementation fulfills them:

##### Server specification
* The server program should listen on a socket, wait for a client to connect, receive a stream of bytes from the client, print those bytes directly to stdout, and then wait for the next client indefinitely.
* The server should take one command-line argument: the port number to listen on for client connections.
* The server should accept and process client communications in an infinite loop, allowing multiple clients to send messages to the same server. The server should only exit in response to an external signal (e.g. SIGINT from pressing `ctrl-c`).
* The server should maintain a 10 client connection queue and handle multiple client connection attempts sequentially. In real applications, a TCP server would fork a new process to handle each client connection concurrently, but that is not necessary for this assignment.
* The server should gracefully handle error values potentially returned by socket programming library functions (see specifics for each language below).

##### Client specification
* The client program should contact a server, read some bytes from stdin, send the bytes, and exit.
* The client should read and send the message *exactly* as it appears in stdin until reaching an EOF (end-of-file).  This implies that messages do not contain EOFs.
* The client should take two command-line arguments: the IP address of the server and the port number of the server.
* The client must be able to handle arbitrarily large messages by iteratively reading and sending chunks of the message, rather than reading the whole message into memory first.
* The client should handle partial sends (when a socket only transmits part of the data given in the last `send` call) by attempting to re-send the rest of the data until it has all been sent.
* The client should gracefully handle error values potentially returned by socket programming library functions.

##### Getting started

We have provided scaffolding code in `client-c.c` and `server-c.c`.
*You should read and understand this code before starting to program.*

You should program only in the locations of the provided files marked with `TODO` comments. There is one `TODO` section per client and one per server. You can add functions if you wish, but do not change file names, as they will be used for automated testing.

##### Implementation
The classic "Beej's Guide to Network Programming" is located here: https://beej.us/guide/bgnet/html/.  The [system call section](https://beej.us/guide/bgnet/html/#system-calls-or-bust) will be most relevant. The man pages are also useful for looking up individual functions (e.g.  `man socket`).
Note that while the examples in the guide are relevant, they are *not* complete solutions to this assignment.
It is your task to find out why.

You will need to add socket programming and I/O code in the locations marked `TODO`. The reference solutions have roughly 70  (well commented and spaced) lines of code in the `TODO` sections of each file. Your implementations may be shorter or longer.

You should build your solution by running `make` in the `553-hw5` directory. Your code *must* build using the provided Makefile in the Vagrant environment. The server should be run as `./server-c [port] > [output file]`. The client should be run as `./client-c [server IP] [server port] < [message file]`.

##### Testing

You should test your implementations by attempting to send messages from clients to servers. The server can be run in the background (append a `&` to the command) or in a separate SSH window. You should use `127.0.0.1` as the server IP and a high server port number between 10000 and 60000. You can kill a background server with the command `fg` to bring it to the foreground then `ctrl-c`.

The Bash script `test_client_server.sh` will test your implementation by attempting to send a message, "Testing 1234!\n":

`./test_client_server.sh [server port]`

If you get a permissions error, run `chmod 744 test_client_server.sh` to give the script execute privileges.

If successful, the test script will print "SUCCESS" if the message is sent and received correctly.
Otherwise, it will print a diff of the sent and received message.
Note that, for some messages, a difference may not be visible.
If that's the case, you might want to view the input and output with a Hex editor like `xxd` to see the byte-level representation.

###### Debugging hints
Here are some debugging tips. If you are still having trouble, ask a question on Piazza or see an instructor during office hours.

* Do *not* assume that the testing script is complete.  Your program should be able to handle *any* input.
* There are defined buffer size and queue length constants in the scaffolding code. You should use them. If they are not defined in a particular file, you don't need them.
* There are multiple ways to read and write from stdin/stdout. Any method is acceptable as long as it does not read an unbounded amount into memory at once and does not modify the message.
* If you are using buffered I/O to write to stdout, make sure to call `flush` or the end of a long message may not write.
* Remember to close the socket at the end of the client program.
* Make sure you are using `127.0.0.1` as the server IP argument to the client and the same server port for both client and server programs.
* If you get "address already in use" errors, make sure you don't already have a server running. Otherwise, restart your ssh session.
* If you are getting other connection errors, try a different port between 10000 and 60000.

##### Submission and grading
Submit the project by uploading your modified client and server files to Canvas.

We will grade your projects by running the `test_client_server.sh` script and additional tests with large messages, multiple simultaneous clients, etc. Double check the specifications above and perform your own tests before submitting.

This project adapted, with permission, from Nick Feamster [feamster -at- uchicago . edu].

