#include "../inc/client.hpp"

void Server::Quit(Client &client){

    std::vector<channel *>::iterator it = channels.begin();
   // Iterar sobre todos os canais e remover o cliente
    while (it != channels.end()) {
        channel* chan = *it;
        if (is_client_on_channel(client, *chan)) {
            std::string msg = ":" + client.get_nickname() + " QUIT :" + reasons(client.get_parsed_input().front(), 1);
            send_to_all_channel(msg, *chan);
            chan->remove_client(&client);

            // Se o canal ficar vazio após a remoção, deletar o canal
            if (chan->get_clients().empty()) {
                delete_channel(*chan);
                it = channels.erase(it);
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }

    // Remover o cliente da lista de clientes do servidor
    

    // (Opcional) Deletar o objeto cliente se o gerenciamento de memória for responsabilidade do servidor
    // delete &client;

}
