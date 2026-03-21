# Unid Data Structures

## Unid Block

Sequence
Unid
Length

## Node Data

id
ip
port
role
status

## Heartbeat

node id
timestamp
sequence
status

## Write ahead Log

The write ahead log distribututes what to node wants to commit, but doen't
commit it unit a majority of the peers acknowledge reciept. The log consists
a structured series of integers that describe the desired transaction.

Activities that are logged:

1. Node failures

(server to server, client to server, server to client, client to
client)
2. Distribute a unid (client to client)
3. Distribute a unid block (server to server)
4. Distribute a unid block (server to client)
5. Election for a new leader (server to server)
6. Election to add a new server or client  node
7. Election to remove a server or client node

For example

Assign unid block to a client

logid:logtype:destid:sequence:unid:length :

Where:
logid is a number that uniquely identifies the log entry
destid is the destination id of the node that will receive the block
seq is the sequence number that the producer uses to prevent duplicate receipt
unid is the unid
length is the number of units in the block starting with unid

