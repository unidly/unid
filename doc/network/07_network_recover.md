# Unid Network Recover

## Functional design

Everything will fail over time. Some failures occur because of simple human
error. Others result from natural disasters that may damage network lines,
or cause an electrical failure rendering a data center inoperable. The main
requirement of Unid servers and clients is to adapt to failures in a way that
maintains the serving of unids without fail.

Handling failure involves using redundancy (both local and regional),
replicating data that provides continuity for swapping a functional server
or client for one that fails. Shared algorithms assure that in the event of
a failure, clients are immediately notified of where to send rquests for
continuity of service.
