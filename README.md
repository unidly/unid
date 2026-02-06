# Unid - An ultra fast, failsafe distributed ID server

Unid performs a simple job in a highly reliable way for globally distributed
database and other applications. That job is providing identifiers that can
be used at the application level for guaranteed uniqueness.

## Identifieer

An Unid identifier is a globally unique integer of boundless size. Many
applications can live with a 16 bit, 32 bit or 64 bit identifier. Some
require 128 bit adentifiers and beyond. Unid supports a "big number" integer
type that can be constructed with the width required for the application.

## Purpose of Unid

Modern of-the-shelf databases support sequential identifiers that are
up to 64-bits in size. These may be used as primary keys allowing fast lookup
operations using indexes, and space-wise very efficient.

Going beyond 64-bits natively is supported by a 128-bit a UUID (GUID). A UUID is
generated in a random way, and using a UUID as a primary key is also common,
especially in large-scale databases. The advantage of a UUID is that it is
highly improbable (but not impossible) to have a redundant identifier create
depending on the size of a recordset in a database. 

Unid is designed to provide unique identfiers within a system that is of
global scale and "lives" not only for years, but generations. Common indexing
strategies of today are inadequate for this task.
