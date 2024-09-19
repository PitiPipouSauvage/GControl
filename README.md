# GControl project
## Synopsis
The GControl project is a botnet project built to test my abilities to build that kind of software.
Adding a client to the botnet is done as follows : 

### Installation
The `installer.c` compiled version is run on the target's computer, it downloads, using curl, the `client.c`, `updater.c`,
 and `runner.c` compiled versions. At that point the `installer` bin file executes the `runner` file, the `installer` 
files then stops and gets deleted by the `runner` file. The `runner` file then starts the `client` file. The `runner`
file is used to detect updates and when needed will start the updates.

### Updates
After the installation phase the `runner` file does nothing except query the server for updates. When an update is 
required the runner file sends an `Updating...` heartbeat to the server and shuts down the `client` file and starts
the `updater` file. The `updater` file cannot change the `runner`, it's mission is to update the client. It does so 
by downloading, still using curl, the newer version of the `client` binary and deleting the old one. When the updater
is finished the `runner` file gets notified and starts the `client` that will send a heartbeat immediately to the server
notifying that the client is up to date and running.

### Prompting instructions to the network <span id="prompt"></span>
To prompt requests to the network the admin can use the `admin.py` file which will connect to the server through a 
custom terminal.
- Basic commands
    - `help` will display all commands available.
    - `kill [ip]` will send a self-destruct instruction to either one ip or all if none is indicated.
    - `ddos [target]` will launch a full ddos attack on the target. It will stop any ongoing activity of the bots.
    - `mine <wallet> [ip]` will send a mining instruction. The miners will mine monero at the set address.
    - `status` will give you an overview of the network's availability.
    - `connect <ip>` will give you access to a specific bot terminal

## Network 
This section is to describe the networking protocol for communications between the C2 (Command and Control) server and 
the clients.

### Packet format
Each packet is a json encoded string which is decoded into an associative array when received by either the server or
the clients. 
For instance a heartbeat packet (see <a href=#heartbeats>Heartbeats</a>) will be as follows :
```json 
{
  "version" : "1.13",
  "activity" : "mining"
}
```
This will then be converted into 
```php
$packet = (
    "version" => "1.13",
    "activity" => "mining"
);
```
Server packets are structured as follows : 
```json 
{
  "request": "<request>"
  /* complementary information to complete said request */
}
```
Before sending a packet the server will check that the request is documented and is not an unknown request.

### Packet encryption
Every packet will be encrypted using RSA keys. Every client will get the public key which they will use to decrypt the
server's inputs and use to encrypt their own messages. The server will then decrypt with the private key and encrypt
with the private key.

### Client initiated connexion 
The connexions between the server and the client are initiated by the client to avoid firewalls issues. 
- #### Heartbeats <span id="heartbeats"></span>
  The client sends a ping called a heartbeat. A heartbeat is a socket connection which stays until the client is disconnected.
  These heartbeats are meant to signal the server that the client is alive. 
  Each heartbeat contains some information :
  - Client version
  - Activity (idle, mining, ddos)
  
  If the client version is not up to date the server will respond with an update request. On the client's system the 
  heartbeat is a separated thread as it is on the server.

  Each time the client changes state it cancels the heartbeat and sends a new one with the updated information.

All the following requests are made by the server using the heartbeat's socket to send it. On the client's machine this
is handled by creating a new thread per request. See more in the Request handling section.

- #### Change status
  The server sends a `change_status` request which can start a ddos attack or stop one or even start mining on the 
  client's machine.
- #### RTerminal
  The `rterminal` request is meant in case the admin wishes to access the cli of a specific client as specified in the 
  <a href=#prompt>Prompting instructions to the network section</a>.