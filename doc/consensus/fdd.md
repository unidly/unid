# Consensus

In a Unid system, a leader Unid Server provides other Unid Servers with
Unid Blocks for distribution. The Unid Servers a geographically located
and provide Unid Blocks to Unid Clients. The Unid Clients, serve the Unids
to co-located applications.

Unid uses consensus for the following situations:

* Detecting failure and electing a new leader
* Handling the addition or removal of nodes

## Distributed system design patterns

### References

* [Distributed system desing patters](https://medium.com/@nishantparmar/distributed-system-design-patterns-2d20908fecfc)

## Paxos

### References

* [Lamport Papers](https://lamport.azurewebsites.net/pubs/pubs.html#lamport-paxos)
* [The Part Time Parlaiment](https://lamport.azurewebsites.net/pubs/lamport-paxos.pdf)
* [Paxos Mde Simple](https://lamport.azurewebsites.net/pubs/paxos-simple.pdf)
* [The Paxos Algorithm](https://www.youtube.com/watch?v=d7nAGI_NZPk)
* [Paxos in Pictures: Lamport's Distributed Consensus Algorithm](https://www.youtube.com/watch?v=UUQ8xYWR4do)
* Megastore - Paxos used - Paper: Megastore: Providing Scalable, Highly
Available Storage for Interactive Services, Baker, J et al

## Raft

### References

* [Designing for Understandability: The Raft Consensus Algorithm](https://www.youtube.com/watch?v=vYp4LYbnnW8)

## Heartbeat

Node failure is an important indicator for triggering a conssenses algorithm.
A hearbeat is a signal to a node that another node is alive. Unid Servers send
a heartbeat to all other servers in the network so that any failure can be
handled.

### References

*
