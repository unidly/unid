# UPD

## Functional Design

UDP is a high speed IP transport layer protocol that is connectionless and
prioritizes speed and efficiency over reliability. The Unid software uses very
simple data architecture consisting of Plan Old Data (POD). It does not
communicate arrays, variable length strings, or large amounts of data, so UDP
is an ideal mechanism for tranfering the data between client, server, and
backup nodes.
