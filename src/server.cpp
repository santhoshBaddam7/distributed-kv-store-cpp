#include "server.h"
#include "datastore.h"
#include "parser.h"
#include "replication.h"

#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

DataStore db;
int connectedClients = 0;

void handleClient(int client_socket)
{
    connectedClients++;

    char buffer[1024];
    bool isReplicaConnection = false;

    while(true)
    {
        memset(buffer,0,sizeof(buffer));

        int bytes = recv(client_socket,buffer,sizeof(buffer)-1,0);

        if(bytes <= 0)
            break;

        std::string input(buffer);

        input.erase(remove(input.begin(),input.end(),'\n'),input.end());
        input.erase(remove(input.begin(),input.end(),'\r'),input.end());

        if(input.empty())
            continue;

        auto tokens = splitCommand(input);

        std::string cmd = tokens[0];
        std::transform(cmd.begin(),cmd.end(),cmd.begin(),::toupper);

        std::string response;

        if(cmd == "REPLICA")
        {
            addReplica(client_socket);
            isReplicaConnection = true;
            response = "Replica registered\n";
        }

        else if(cmd == "PING")
        {
            response = "PONG\n";
        }

        else if(cmd == "SET")
        {
            db.set(tokens[1],tokens[2]);

            if(!isReplicaConnection)
                replicateCommand(input + "\n");

            response = "OK\n";
        }

        else if(cmd == "GET")
        {
            std::string val = db.get(tokens[1]);

            if(val == "NULL")
                response = "Key not found\n";
            else
                response = val + "\n";
        }

        else if(cmd == "DEL")
        {
            db.del(tokens[1]);

            if(!isReplicaConnection)
                replicateCommand(input + "\n");

            response = "DELETED\n";
        }

        else if(cmd == "EXPIRE")
        {
            db.expire(tokens[1],stoi(tokens[2]));

            if(!isReplicaConnection)
                replicateCommand(input + "\n");

            response = "OK\n";
        }

        else if(cmd == "INFO")
        {
            response =
            "MiniRedis\nKeys:" + std::to_string(db.size()) +
            "\nClients:" + std::to_string(connectedClients) + "\n";
        }

        else
        {
            response = "INVALID COMMAND\n";
        }

        send(client_socket,response.c_str(),response.size(),0);
    }

    connectedClients--;
    close(client_socket);
}

void startServer(int port)
{
    db.loadFromDisk();

    int server_fd = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd,(struct sockaddr*)&address,sizeof(address));
    listen(server_fd,5);

    std::cout<<"Server running on port "<<port<<std::endl;

    while(true)
    {
        int client_socket = accept(server_fd,nullptr,nullptr);

        std::thread clientThread(handleClient,client_socket);
        clientThread.detach();
    }
}