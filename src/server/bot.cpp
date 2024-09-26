#include "../inc/Bot.hpp"

void Bot::kickUser(Client &client, channel &channel, Server &server){
    if (server.is_client_on_channel(client, channel)){
        server.send_to_all_channel(":Bot KICK " + channel.get_name() + " " + client.get_nickname() + " :You have been kicked by the bot!", channel);
        channel.remove_client(&client);
    }
}

bool Bot::verifyMessage(std::string message, Client &client, channel &channel, Server &server) {
    std::ifstream file("src/server/forbiddenWords.txt");
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de palavras proibidas." << std::endl;
        return false;
    }

    while (std::getline(file, line)) {
        if (client.getBadBehavior() > 2 &&  !server.is_client_op(client, channel)) {
            std::cout << "Kicking user" << std::endl;
            kickUser(client, channel, server);
            return true;
        }
        if (message.find(line) != std::string::npos) {
            client.incrementBadBehavior();
            server.send_to_all_channel(":Bot PRIVMSG " + channel.get_name() + " " + client.get_nickname() + " has said a forbidden expression! If you keep with this behavior, you will be kicked!", channel);
            return true;
        }
    }
    file.close();
    return false;
}