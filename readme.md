**This project has been created as part of the 42 curriculum by aferryat, amandour**

**description**:

- the irc project created to simulate communication between clients with other clients using irc server

- the irc server project contains commands as JOIN, MODE, TOPIC and PRIVMSG allow you to create or join a channel or set Mode to channel , set topic of channel and lastly send private message to user or a text message to a channel.

- to implement the server we need to create a socket which is fd represent end point allow send data or receive data between process or hosts.

- after creating “`socket`” fd, the socket file descriptor aren't ready yet it need to know which port file descriptor should listen to and which interface  so we use bind sys-call to put does informations.

- The ”`listen`"  syscall makes our server monitor a specific port, e.g., 1025. Any client can connect from any source port, but it must target our server’s listening port (1025). If a client tries to connect to a different port on our server, the connection will fail

- after that we used “`poll`” used to wait for events comes from clients, it would watch our clients and wait until one of the clients send data or disconnect in our case so then the poll write in revent the event that happened and then we check if revent  comes from server fd if comes from server fd  that mean a new client try to connect if it not and comes from one of the clients that mean that the client itself try to send data or disconnected

- then finally we start reading from client the messages that the client sent and then we use authentication so i make sure that client has correct password and unique identity, when the user successfully registered the user now allowed to send messages, create/channels channels

- and this the whole project about.

Implemented Commands :
    JOIN
    Join one or more channels.
    JOIN <channel>{,<channel>} [<key>{,<key>}]

    Creates channel if it doesn't exist
    Adds client to channel
    Sends topic and member list
    Broadcasts JOIN to all members

    INVITE
    Invite a user to an invite-only channel.
    INVITE <nickname> <channel>

    Requires operator privileges
    Adds user to invite list
    Notifies invited user

    KICK
    Remove a user from a channel.
    KICK <channel> <user> 

    Requires operator privileges
    Removes user from channel

    MODE
    View or change channel modes.
    MODE <channel> [<modestring> [<params>]]
    Channel Modes:

    i - Invite-only channel
    t - Topic restricted to operators
    k - Channel password/key
    o - Operator privileges
    l - User limit

    TOPIC
    View or change channel topic.
    TOPIC <channel> [:<new topic>]

    View current topic (no parameters)
    Change topic (respects +t mode)
    Broadcasts topic changes

    PRIVMSG
    Send messages to users or channels.
    PRIVMSG <target> :<message>

    Send to user (private message)
    Send to channel (broadcasts to all members)

Resources

Official Documentation
https://datatracker.ietf.org/doc/html/rfc2812 - IRC Client Protocol
https://datatracker.ietf.org/doc/html/rfc1459 - Original IRC Protocol
https://modern.ircdocs.horse/   - Modern IRC reference

Implementation Guides
https://en.wikipedia.org/wiki/List

*_of_*

Internet

*_Relay_*

Chat

*_commands : Command reference

Testing

Using netcat
bash nc localhost 6667

Using IRC Clients
LimeChat - macOS GUI client: http://limechat.net/mac/*

*youtube Resource*
    Internet Relay Chat : https://www.youtube.com/watch?v=6UbKenFipjo

*other resource*
    the socket, poll, sockopt, acept, etc... syscall comes from manual like (man7.org, die.net)