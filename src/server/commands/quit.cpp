#include "../inc/client.hpp"

void Server::Quit(Client &client){
    std::cout << client.get_fd() << " entrei QUIT\n";
    std::vector<Client *>::iterator it2 = clients.begin();
    int i = 0;
    while(it2 != clients.end()){
        if (client.get_fd() == (*it2)->get_fd()){
            clients.erase(it2);
            close((*it2)->get_fd());
		    update_poll_fds(i);
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
