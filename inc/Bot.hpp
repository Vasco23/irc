#pragma once

#include "server.hpp"
#include "client.hpp"

class Server;
class channel;
class Client;

class Bot{
    private:
    public:
        bool verifyMessage(std::string message, Client &client, channel &channel, Server &server);
        // void kickUser(Client &client);
};


/*
    bot vai ter que ter uma estrutura de dados com as palavras proibidas de serem ditas
    bot vai ter que ter uma estrutura de dados com que ele vai responder (strings)
    bot vai quer que executar comandos (kick)

*/
