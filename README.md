# Unid Server and Unid client

The unid_server is a daemon software that provides the unid id ranges to
unid_clients. A unid is a 64-bit unsigned integer that is used to provide a
unique primary key that is used in the database as a primary key for records
that are created. A unid id range is a token that is provided to a unid_client
that distributes the unids for the API that creates, retrieves, updates and
deletes database records.


