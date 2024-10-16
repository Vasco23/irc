#include "../inc/client.hpp"

void Server::Quit(Client &client){
    std::vector<channel *>::iterator it = channels.begin();
    while(it != channels.end()){
        if (is_client_on_channel(client, (**it))){
            std::string msg = ":" + client.get_nickname() + " QUIT the server";
 		    send_to_all_channel(msg, (**it));
            if ((**it).get_num_of_ops() == 1){
                if (is_client_op(client, (**it))){
                    if ((**it).get_clients().size() == 1){
                        delete *it;
                        channels.erase(it);
                        continue;
                    }
                    (**it).get_clients().front().second = true;
                }
            }
            (**it).remove_client(&client);
        }
        it++;
    }
    std::vector<Client *>::iterator it2 = clients.begin();
    while(it2 != clients.end()){
        if (client.get_fd() == (*it2)->get_fd()){
 		    update_poll_fds(client.get_fd());
            close((*it2)->get_fd());
            delete *it2;
            clients.erase(it2);
            return;
        }
        it2++;
    }
}
