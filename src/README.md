---

Mini Redis-like Distributed Key-Value Store (C++)

A Redis-style distributed key-value database implemented in C++ using TCP sockets and multithreading.
The system supports concurrent clients, persistence, TTL-based expiration, logging, observability commands, and primary–replica replication.


---

Features

TCP socket based server

Concurrent client handling (thread-per-connection)

In-memory key-value datastore

Persistence to disk

TTL based key expiration

Background expiration worker

Logging for command activity

Observability commands

Primary → Replica replication



---

Architecture

Clients
   │
   ▼
TCP Server (Primary)
   │
   ├── Command Parser
   │
   ├── Key-Value Store (Hash Map)
   │
   ├── Persistence (disk)
   │
   ├── TTL Expiration Worker
   │
   └── Replication Module
            │
            ▼
        Replica Server


---

Supported Commands

Core Commands

PING
SET key value
GET key
DEL key

Database Management

EXISTS key
KEYS
FLUSH

Expiration

EXPIRE key seconds

Observability

INFO
CLIENTS

Persistence

SAVE
LOAD

Replication

REPLICA


---

Project Structure

redis-clone-cpp/
│
├── src/
│   ├── main.cpp
│   ├── server.cpp
│   ├── server.h
│   ├── datastore.cpp
│   ├── datastore.h
│   ├── parser.cpp
│   ├── parser.h
│   ├── replication.cpp
│   ├── replication.h
│   ├── logger.cpp
│   └── logger.h
│
├── database.txt
├── server.log
└── README.md


---

Build and Run

Compile

g++ -std=c++17 *.cpp -o redis_server -pthread

Run Primary Server

./redis_server

Default port:

6379

Run Replica Server

Modify port in main.cpp:

startServer(6380);

Then run again.


---

Replication Testing

1. Start primary server (6379)


2. Start replica server (6380)


3. Connect replica to primary



REPLICA

4. Send write command to primary



SET name ramu

5. Query replica



GET name

Output:

ramu


---

Example Session

PING
→ PONG

SET name Santhu
→ OK

GET name
→ Santhu

EXPIRE name 5
(wait)

GET name
→ Key not found


---

Technologies Used

C++

POSIX Sockets

Multithreading

HashMap based datastore

File persistence



---

Learning Outcomes

Distributed systems basics

TCP socket programming

Concurrent server design

Database internals

Replication mechanisms



---