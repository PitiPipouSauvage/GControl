<?php

$pdo = new PDO('mysql:host=localhost;dbname=GControl', 'root', '');
$instruction = array();

class client_socket {
    private $client_socket;
    public function __construct($socket) {
        global $instruction;
        $this->client_socket = $socket;
        while (1) {
            if ($instruction != array()) {
                $this->request($instruction);
            }
        }
    }

    public function request($request) {
        $request = json_encode($request);
        socket_write($this->client_socket, $request, strlen($request));
    }
}

function client_socket_maintainer($socket) {
    global $pdo;
    global $instruction;
    $data = socket_read($socket, 1024);
    $decoded_data = json_decode($data);

    if (isset($decoded_data["instruction"])) {
        if ($decoded_data["id"] === "111A" && $decoded_data["password"] === "26hhkx7tjb") {
            $instruction = $decoded_data["instruction"];
            return NULL;
        }
    }
    $client_version = $decoded_data["version"];

    $sql_query = "SELECT client_version FROM versions WHERE last = true";
    $stmt = $pdo->prepare($sql_query);
    $stmt->execute();
    $version = $stmt->fetch(PDO::FETCH_ASSOC)["version"];
    if ($version != $client_version) {
        $packet = array(
            "request" => "update"
        );
        socket_write($socket, json_encode($packet), strlen(json_encode($packet)));
    }

    $client_socket = new client_socket($socket);
}

class clientHandler extends Thread {
    private $socket;
    public function __construct($socket) {
        $this->socket = $socket;
    }
    public function run() {
        client_socket_maintainer($this->socket);
        return NULL;
    }
}

while (True) {
    $socket = socket_create(AF_INET, SOCK_STREAM, 0);
    socket_bind($socket, "127.0.0.1", 12000);
    socket_listen($socket);
    $client_socket = socket_accept($socket);

    $client = new clientHandler($client_socket);
    $client->run();
}
