#include "replication.h"
#include <vector>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

std::vector<int> replicas;

void addReplica(int socket)
{
    replicas.push_back(socket);
    std::cout << "Replica registered\n";
}

void replicateCommand(const std::string &cmd)
{
    for(int sock : replicas)
    {
        send(sock, cmd.c_str(), cmd.size(), 0);
        std::cout << "Replicated command: " << cmd << std::endl;
    }
}