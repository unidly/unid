# Handshake Protocols

## Functionl design

Communication between Unid nodes follows a simple protocol to assure that the
delivery of the data was accomplished.

Each datagram carries a sequence number that is incremented each time a
datagram is recieved without error. Should a datagram become corrupted, or
lost in the ether during transmission, the sender, after waiting a period
of time, retransmits the datagram WITHOUT incrementing the sequence number.

If the receiver received a valid datagram, it responds with an acknowlegement
to the sender. If the sender receives the acknowledgement, then it increments
the sequence number for that node to be used in the next communication and
commits the transaction..

If the receiver receives a valid datagram, with the same sequence number of
previous datagram, it assumes that the acknowledgement was lost, and resends
the ACK.

This simple protocol assures that the data was recieved and that the data was
correct.

## Detailed design

### Acknowledgement


