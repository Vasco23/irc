#include "../inc/Bot.hpp"

bool Bot::verifyMessage(std::string message, Client &client, channel &channel, Server &server) {
    std::ifstream file("src/server/forbiddenWords.txt");
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de palavras proibidas." << std::endl;
        return false;
    }

    while (std::getline(file, line)) {
        // if (client.getBadBehavior() > 2){
        //     // kickUser(client);
        // }

        if (message.find(line) != std::string::npos) {
            client.incrementBadBehavior();
            server.send_to_all_channel(":Bot PRIVMSG " + channel.get_name() + " " + client.get_nickname() + " has said a forbidden expression! If you keep with this behavior, you will be kicked!", channel);
            return true;
        }
    }
    file.close();
    return false;
}