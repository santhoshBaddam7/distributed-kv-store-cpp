#ifndef REPLICATION_H
#define REPLICATION_H

#include <vector>
#include <string>

void addReplica(int socket);
void replicateCommand(const std::string &cmd);

#endif