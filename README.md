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

### Prompting instructions to the network
To prompt requests to the network the admin can use the `admin.py` file which will connect to the server through a 
custom terminal.
- Basic commands
    - `help` will display all commands available.
    - `kill [ip]` will send a self-destruct instruction to either one ip or all if none is indicated.
    - `ddos [target]` will launch a full ddos attack on the target. It will stop any ongoing activity of the bots.
    - `mine <wallet> [ip]` will send a mining instruction. The miners will mine monero at the set address.
  