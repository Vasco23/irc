#include "../inc/client.hpp"

void Server::Quit(Client &client){
    // std::cout << client.get_fd() << " entrei QUIT\n";
   std::vector<channel *>::iterator it = channels.begin();
    while(it != channels.end()){
        if (is_client_on_channel(client, (**it))){
            std::string msg = ":" + client.get_nickname() + " QUIT the server";
		    send_to_all_channel(msg, (**it));
            if ((**it).get_num_of_ops() == 1){
                if (is_client_op(client, (**it))){
                    if ((**it).get_clients().size() == 1){
                        (**it).remove_client(&client);
                        delete *it;
                        channels.erase(it);
                        continue;
                    }
                    (**it).get_clients().front().second = true;
                }
            }
            (**it).remove_client(&client);
        }
        else{
            it++;
        }
    }
    std::vector<Client *>::iterator it2 = clients.begin();
    int i = 1;
    while(it2 != clients.end()){
        if (client.get_fd() == (*it2)->get_fd()){
		    update_poll_fds(client.get_fd());
            close((*it2)->get_fd());
            delete *it2;
            clients.erase(it2);
        }
        else{
            it2++;
            i++;
        }
    }
    // Remover o cliente da lista de clientes do servidor
    // (Opcional) Deletar o objeto cliente se o gerenciamento de memória for responsabilidade do servidor
    // delete &client;
}
