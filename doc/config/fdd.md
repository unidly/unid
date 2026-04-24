# Unid Configuration Functional Design Document

The functional design bridges the needs expressed in the User Requirements
Specification and the detailed design.

The software contains many areas that must be configured for proper operation.
Among these are things like database configuration, network protocol
configuration, etc.

The user must be able to access and modify the configuration parameters in a
way that meets the needs of the user, and the efficient operation of the
software.

## Configuration

Configurations are stored on the server filesystem. The configuration file
is accessed during system startup, and the configuration variables are 
provided to the various classes that require these paramters

This project adopts the TOML v1.0.0 specification that defines the
structure and format of the configuration file. TOML was selected as the
best file specification for the following features:

* Minimal configuration file format
* Easy to read
* Native datatypes consistent with c++ datatypes
  * Key/Value Pairs (std::pair)
  * Arrays (std::vector)
  * Tables (std::unordered\_list)
  * Integers and floats
  * Booleans
  * Dates and times
* Widely supported
  * C++ libraries

## Directory structure

Configuration files are located in various directories of the filesystem,
as are other file types such as data, cache, executables, etc. This project
adopts the XDB Base Directory Specification that uses environment variables
to indicate the location of the various files.

With regard to configuration files, the $XDG_CONFIG_HOME contains the file
location relative to which the user-specific configuration files are stored.

In some instances, configuration files may be stored in locations depending
on the use, for example during development versus during production. The
$XDG_CONFIG_DIRS contains the various locations listed in the order of being
searched. For example, the search may load the configuration file loaded in
the developers $HOME directory may take precedence over the location of the
production directory.
