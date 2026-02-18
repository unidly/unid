# Network Failure

## Functional design

When sending a UDP datagram, the operating system returns error codes that
may indicate that the connection was refused, destination is unreachable, or
the destination host was unreachable. These errors are how the prgram knows
that the local networ, interface or routing has failed.

On the receiving side, detecting failure is more difficult because lack of
received packets doesn't automatically mean failure. The receiving system call
might return an error if a local network issue occurs (e.g., someone unplugs
the network cable!). But lacking that another mechanism can be implemented to
monitor the status of the connection. A heartbeat message using ICMP ping
can be used to indicate connectivity without straining the network resources.

### Unid server network failure

Server failure requires immediate reaction. The distributed servers elect
a replacement server, and broadcast the new server identification to all
servers, clients and backup clients in the system. It is assumed that the
operating clients have enough unids assigned to reduce or eliminate any
impact on the unid consummers.

### Unid client network failure

It is assumed that if there is a network failure that impacts the Unid Client,
then the API application will also suffer from the network down situation. In
a failsafe system, a backup system takes over from the failed server. The
backup server also has a unid client. The backup server unid client is always
synchronized with the man server unid client, and immediately takes over
should a network failure occur.

### Fundamental failsafe fechanisms

In a Unid based system, there are a number of distributed servers that provide
unids to clients in their particular regions. Each of these regional servers
has a fully synchronized backup server, ideally co-located, but on a different
network interface.

All of the servers have a real-time replicated dataset that includes the state
and dataset of each of the servers. All of the servers can take over the
responsibility of a failed server at any time. A consensus algorithm guides
which server takes over anothers responsibility during a failure.

Just as servers and clients can fail, they can also recover. The algorithm
must provide for these recoveries or addtion of new clients and servers in
a constrolled manner.

