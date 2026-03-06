# Unit testing with Google Test

This directory contains the unit tests for the Unid software.

## Suite and Test naming conventions

Because of Google Test implementation, it is best to not use underscores
in the suite and test names. This conflicts with our coding standards, but
is necessary to avoid problems.

Developers are required to use capitalized camel case for suite and test names.
For example:

TEST(NetworkUdpServerTests, SimpleAssertion) { ASSERT_EQ(1, 1); }

