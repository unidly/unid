# Unid Configuration User Requirments

Configuration parameters provide the user, in this case the system engineer,
with the available to modify the run-time operation of Unid software. The user
modifies the operation by changing the values of configuration parameters that
the software uses.

Configuration parameters are maintained in the filesystem. The format of the
configuration file should be standardized and well documented. The file format
must be parsable by open source software, as well as designed to be readible
and comprehensive to cover the technical parameters that are used by the
systme.

## Summary of User Requirements

* Configurations via config file
* Minimal command line configuration, mostly for startup
* File format must be human readable
* File structure must follow a well-known specification
* Specification must cover needed configuration data types
