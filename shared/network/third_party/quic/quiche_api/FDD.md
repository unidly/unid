#Google Quiche API

## Overview

The Google quic library, called [Quiche](CreateAndBindServerSocket), does not have a developed API for C++
applications. Building a generic QUIC API is worth the effort in order to
abstract the interface to the Google QUIC and also provide a bridge that can be consistently
update to match changes to Quiche.

### [RFC 9000 QUIC: A UDP-Based Multiplexed and Secure Transport](https://datatracker.ietf.org/doc/html/rfc9000)
## RFC 9000 Transport Parameters 18.2.

Many transport parameters listed here have integer values. Those transport parameters that are identified as integers use a variable-length integer encoding; see Section 16. Transport parameters have a default value of 0 if the transport parameter is absent, unless otherwise stated.

The following transport parameters are defined:

`original_destination_connection_id (0x00)`:
This parameter is the value of the Destination Connection ID field from the first Initial packet sent by the client; see Section 7.3. This transport parameter is only sent by a server.

`max_idle_timeout (0x01)`:
The maximum idle timeout is a value in milliseconds that is encoded as an integer; see (Section 10.1). Idle timeout is disabled when both endpoints omit this transport parameter or specify a value of 0.

`stateless_reset_token (0x02)`:
A stateless reset token is used in verifying a stateless reset; see Section 10.3. This parameter is a sequence of 16 bytes. This transport parameter MUST NOT be sent by a client but MAY be sent by a server. A server that does not send this transport parameter cannot use stateless reset (Section 10.3) for the connection ID negotiated during the handshake.

`max_udp_payload_size (0x03)`:
The maximum UDP payload size parameter is an integer value that limits the size of UDP payloads that the endpoint is willing to receive. UDP datagrams with payloads larger than this limit are not likely to be processed by the receiver.

The default for this parameter is the maximum permitted UDP payload of 65527. Values below 1200 are invalid.

This limit does act as an additional constraint on datagram size in the same way as the path MTU, but it is a property of the endpoint and not the path; see Section 14. It is expected that this is the space an endpoint dedicates to holding incoming packets.

`initial_max_data (0x04)`:
The initial maximum data parameter is an integer value that contains the initial value for the maximum amount of data that can be sent on the connection. This is equivalent to sending a MAX_DATA (Section 19.9) for the connection immediately after completing the handshake.

`initial_max_stream_data_bidi_local (0x05)`:
This parameter is an integer value specifying the initial flow control limit for locally initiated bidirectional streams. This limit applies to newly created bidirectional streams opened by the endpoint that sends the transport parameter. In client transport parameters, this applies to streams with an identifier with the least significant two bits set to 0x00; in server transport parameters, this applies to streams with the least significant two bits set to 0x01.

`initial_max_stream_data_bidi_remote (0x06)`:
This parameter is an integer value specifying the initial flow control limit for peer-initiated bidirectional streams. This limit applies to newly created bidirectional streams opened by the endpoint that receives the transport parameter. In client transport parameters, this applies to streams with an identifier with the least significant two bits set to 0x01; in server transport parameters, this applies to streams with the least significant two bits set to 0x00.

`initial_max_stream_data_uni (0x07)`:
This parameter is an integer value specifying the initial flow control limit for unidirectional streams. This limit applies to newly created unidirectional streams opened by the endpoint that receives the transport parameter. In client transport parameters, this applies to streams with an identifier with the least significant two bits set to 0x03; in server transport parameters, this applies to streams with the least significant two bits set to 0x02.

`initial_max_streams_bidi (0x08)`:
The initial maximum bidirectional streams parameter is an integer value that contains the initial maximum number of bidirectional streams the endpoint that receives this transport parameter is permitted to initiate. If this parameter is absent or zero, the peer cannot open bidirectional streams until a MAX_STREAMS frame is sent. Setting this parameter is equivalent to sending a MAX_STREAMS (Section 19.11) of the corresponding type with the same value.

`initial_max_streams_uni (0x09)`:
The initial maximum unidirectional streams parameter is an integer value that contains the initial maximum number of unidirectional streams the endpoint that receives this transport parameter is permitted to initiate. If this parameter is absent or zero, the peer cannot open unidirectional streams until a MAX_STREAMS frame is sent. Setting this parameter is equivalent to sending a MAX_STREAMS (Section 19.11) of the corresponding type with the same value.

`ack_delay_exponent (0x0a)`:
The acknowledgment delay exponent is an integer value indicating an exponent used to decode the ACK Delay field in the ACK frame (Section 19.3). If this value is absent, a default value of 3 is assumed (indicating a multiplier of 8). Values above 20 are invalid.

`max_ack_delay (0x0b)`:
The maximum acknowledgment delay is an integer value indicating the maximum amount of time in milliseconds by which the endpoint will delay sending acknowledgments. This value SHOULD include the receiver's expected delays in alarms firing. For example, if a receiver sets a timer for 5ms and alarms commonly fire up to 1ms late, then it should send a max_ack_delay of 6ms. If this value is absent, a default of 25 milliseconds is assumed. Values of 214 or greater are invalid.

`disable_active_migration (0x0c)`:
The disable active migration transport parameter is included if the endpoint does not support active connection migration (Section 9) on the address being used during the handshake. An endpoint that receives this transport parameter MUST NOT use a new local address when sending to the address that the peer used during the handshake. This transport parameter does not prohibit connection migration after a client has acted on a preferred_address transport parameter. This parameter is a zero-length value.

`preferred_address (0x0d)`:
The server's preferred address is used to effect a change in server address at the end of the handshake, as described in Section 9.6. This transport parameter is only sent by a server. Servers MAY choose to only send a preferred address of one address family by sending an all-zero address and port (0.0.0.0:0 or [::]:0) for the other family. IP addresses are encoded in network byte order.

The `preferred_address` transport parameter contains an address and port for both IPv4 and IPv6. The four-byte IPv4 Address field is followed by the associated two-byte IPv4 Port field. This is followed by a 16-byte IPv6 Address field and two-byte IPv6 Port field. After address and port pairs, a Connection ID Length field describes the length of the following Connection ID field. Finally, a 16-byte Stateless Reset Token field includes the stateless reset token associated with the connection ID. The format of this transport parameter is shown in Figure 22 below.

The Connection ID field and the Stateless Reset Token field contain an alternative connection ID that has a sequence number of 1; see Section 5.1.1. Having these values sent alongside the preferred address ensures that there will be at least one unused active connection ID when the client initiates migration to the preferred address.

The Connection ID and Stateless Reset Token fields of a preferred address are identical in syntax and semantics to the corresponding fields of a NEW_CONNECTION_ID frame (Section 19.15). A server that chooses a zero-length connection ID MUST NOT provide a preferred address. Similarly, a server MUST NOT include a zero-length connection ID in this transport parameter. A client MUST treat a violation of these requirements as a connection error of type TRANSPORT_PARAMETER_ERROR.

```C
Preferred Address {
  IPv4 Address(32), IPv4 Port(16), IPv6 Address(128), IPv6 Port(16),
      Connection ID Length(8), Connection ID(..), Stateless Reset Token(128),
}
```
Figure 22: Preferred Address Format

`active_connection_id_limit (0x0e)`:
This is an integer value specifying the maximum number of connection IDs from the peer that an endpoint is willing to store. This value includes the connection ID received during the handshake, that received in the preferred_address transport parameter, and those received in NEW_CONNECTION_ID frames. The value of the active_connection_id_limit parameter MUST be at least 2. An endpoint that receives a value less than 2 MUST close the connection with an error of type TRANSPORT_PARAMETER_ERROR. If this transport parameter is absent, a default of 2 is assumed. If an endpoint issues a zero-length connection ID, it will never send a NEW_CONNECTION_ID frame and therefore ignores the active_connection_id_limit value received from its peer.

`initial_source_connection_id (0x0f)`:
This is the value that the endpoint included in the Source Connection ID field of the first Initial packet it sends for the connection; see Section 7.3.

`retry_source_connection_id (0x10)`:
This is the value that the server included in the Source Connection ID field of a Retry packet; see Section 7.3. This transport parameter is only sent by a server.

If present, transport parameters that set initial per-stream flow control limits (initial_max_stream_data_bidi_local, initial_max_stream_data_bidi_remote, and initial_max_stream_data_uni) are equivalent to sending a MAX_STREAM_DATA frame (Section 19.10) on every stream of the corresponding type immediately after opening. If the transport parameter is absent, streams of that type start with a flow control limit of 0.

A client MUST NOT include any server-only transport parameter: original_destination_connection_id, preferred_address, retry_source_connection_id, or stateless_reset_token. A server MUST treat receipt of any of these transport parameters as a connection error of type TRANSPORT_PARAMETER_ERROR.


## [RFC 9001 Using TLS to Secure QUIC](https://datatracker.ietf.org/doc/html/rfc901)

## [RFC 9002 QUIC Loss Detection and Congestion Control](https://datatracker.ietf.org/doc/html/rfc9002)

## [RFC 8999 Version-Independent Properties of QUIC](https://datatracker.ietf.org/doc/html/rfc8999)

## [qlog: Structured Logging for Network Protocols](https://datatracker.ietf.org/doc/draft-ietf-quic-qlog-main-schema/)


## Other APIs

The first step is to review the existing APIs in order to develope an interface model that will be
generalized for our development.

There are three QUIC developer APIs that will be analyzed:

1. Cloudflair QUICHE
1. MsQuic
1. Tquic

### [Cloudflair QUICHE](https://github.com/cloudflare/quiche)

This is a completely separate implementatin for Google's Quiche. It is written in Rust and is used
internally by Cloudflair. A library version has been released, and is available on GitHub. It has
a published Rust and C/C++ API.

### [MsQuic](https://github.com/microsoft/msquic)

MsQuic was designed and developed by Microsoft. MsQuic is a Microsoft implementation of the IETF QUIC
protocol. It is cross-platform, written in C and designed to be a general purpose QUIC library. MsQuic
also has C++ API wrapper classes and exposes interop layers for both Rust and C#.

[MsQuic API Functions](https://microsoft.github.io/msquic/msquicdocs/docs/api/ConnectionStart.html)

```c
typedef struct QUIC_API_TABLE {
  QUIC_SET_CONTEXT_FN SetContext;
  QUIC_GET_CONTEXT_FN GetContext;
  QUIC_SET_CALLBACK_HANDLER_FN SetCallbackHandler;

  QUIC_SET_PARAM_FN SetParam;
  QUIC_GET_PARAM_FN GetParam;

  QUIC_REGISTRATION_OPEN_FN RegistrationOpen;
  QUIC_REGISTRATION_CLOSE_FN RegistrationClose;
  QUIC_REGISTRATION_SHUTDOWN_FN RegistrationShutdown;

  QUIC_CONFIGURATION_OPEN_FN ConfigurationOpen;
  QUIC_CONFIGURATION_CLOSE_FN ConfigurationClose;
  QUIC_CONFIGURATION_LOAD_CREDENTIAL_FN
  ConfigurationLoadCredential;

  QUIC_LISTENER_OPEN_FN ListenerOpen;
  QUIC_LISTENER_CLOSE_FN ListenerClose;
  QUIC_LISTENER_START_FN ListenerStart;
  QUIC_LISTENER_STOP_FN ListenerStop;

  QUIC_CONNECTION_OPEN_FN ConnectionOpen;
  QUIC_CONNECTION_CLOSE_FN ConnectionClose;
  QUIC_CONNECTION_SHUTDOWN_FN ConnectionShutdown;
  QUIC_CONNECTION_START_FN ConnectionStart;
  QUIC_CONNECTION_SET_CONFIGURATION_FN
  ConnectionSetConfiguration;
  QUIC_CONNECTION_SEND_RESUMPTION_FN ConnectionSendResumptionTicket;

  QUIC_STREAM_OPEN_FN StreamOpen;
  QUIC_STREAM_CLOSE_FN StreamClose;
  QUIC_STREAM_START_FN StreamStart;
  QUIC_STREAM_SHUTDOWN_FN StreamShutdown;
  QUIC_STREAM_SEND_FN StreamSend;
  QUIC_STREAM_RECEIVE_COMPLETE_FN StreamReceiveComplete;
  QUIC_STREAM_RECEIVE_SET_ENABLED_FN StreamReceiveSetEnabled;

  QUIC_DATAGRAM_SEND_FN DatagramSend;

} QUIC_API_TABLE;
```

### [Tquic](https://github.com/Tencent/tquic)

TQUIC is a high-performance, lightweight, and cross-platform library for the IETF QUIC protocol. Like
Cloudflair's Quiche, it is written in Rust and has a C/C++ API. Tquic was developed by Tencent, a Chinese
company. It is one of the highest grossing multimedia companies in the world based on revenue.
It is also the world's largest company in the video game industry based on its equity investments.

[Tquic API Functions](https://tquic.net/docs/api_reference/c_quic)


#### Configuration
##### QUIC
```
struct quic_config_t *quic_config_new(void);
void quic_config_free(struct quic_config_t *config);

void quic_config_set_max_idle_timeout(struct quic_config_t *config, uint64_t v);
void quic_config_set_max_udp_payload_size(struct quic_config_t *config, uint16_t v);
void quic_config_set_initial_max_data(struct quic_config_t *config, uint64_t v);
void quic_config_set_initial_max_stream_data_bidi_local(struct quic_config_t *config, uint64_t v);
void quic_config_set_initial_max_stream_data_bidi_remote(struct quic_config_t *config, uint64_t v);
void quic_config_set_initial_max_stream_data_uni(struct quic_config_t *config, uint64_t v);
void quic_config_set_initial_max_streams_bidi(struct quic_config_t *config, uint64_t v);
void quic_config_set_initial_max_streams_uni(struct quic_config_t *config, uint64_t v);
void quic_config_set_ack_delay_exponent(struct quic_config_t *config, uint64_t v);
void quic_config_set_max_ack_delay(struct quic_config_t *config, uint64_t v);
void quic_config_set_congestion_control_algorithm(struct quic_config_t *config, enum quic_congestion_control_algorithm v);
void quic_config_set_initial_congestion_window(struct quic_config_t *config, uint64_t v);
void quic_config_set_slow_start_thresh(struct quic_config_t *config, uint64_t v);
void quic_config_set_bbr_probe_rtt_duration(struct quic_config_t *config, uint64_t v);
void quic_config_enable_bbr_probe_rtt_based_on_bdp(struct quic_config_t *config, bool v);
void quic_config_set_bbr_probe_rtt_cwnd_gain(struct quic_config_t *config, double v);
void quic_config_set_bbr_rtprop_filter_len(struct quic_config_t *config, uint64_t v);
void quic_config_set_bbr_probe_bw_cwnd_gain(struct quic_config_t *config, double v);
void quic_config_set_copa_slow_start_delta(struct quic_config_t *config, double v);
void quic_config_set_copa_steady_delta(struct quic_config_t *config, double v);
void quic_config_enable_copa_use_standing_rtt(struct quic_config_t *config, bool v);
void quic_config_set_initial_rtt(struct quic_config_t *config, uint64_t v);
void quic_config_set_pto_linear_factor(struct quic_config_t *config, uint64_t v);
void quic_config_set_max_pto(struct quic_config_t *config, uint64_t v);
void quic_config_set_active_connection_id_limit(struct quic_config_t *config, uint64_t v);
void quic_config_enable_multipath(struct quic_config_t *config, bool enabled);
void quic_config_set_multipath_algorithm(struct quic_config_t *config, enum MultipathAlgorithm v);
void quic_config_set_max_connection_window(struct quic_config_t *config, uint64_t v);
void quic_config_set_max_stream_window(struct quic_config_t *config, uint64_t v);
void quic_config_set_max_concurrent_conns(struct quic_config_t *config, uint32_t v);
int quic_config_set_reset_token_key(struct quic_config_t *config, const uint8_t *token_key, size_t token_key_len);
void quic_config_set_address_token_lifetime(struct quic_config_t *config, uint64_t seconds);
int quic_config_set_address_token_key(struct quic_config_t *config, const uint8_t *token_keys, size_t token_keys_len);
void quic_config_enable_retry(struct quic_config_t *config, bool enabled);
void quic_config_set_cid_len(struct quic_config_t *config, uint8_t v);
void quic_config_set_send_batch_size(struct quic_config_t *config, uint16_t v);
void quic_config_set_zerortt_buffer_size(struct quic_config_t *config, uint16_t v);
void quic_config_set_max_undecryptable_packets(struct quic_config_t *config, uint16_t v);
void quic_config_enable_encryption(struct quic_config_t *config, bool v);
void quic_config_set_tls_config(struct quic_config_t *config, struct quic_tls_config_t *tls_config);
void quic_config_set_tls_selector(struct quic_config_t *config, const struct quic_tls_config_select_methods_t *methods, quic_tls_config_select_context_t context);
```

##### TLS
```
struct quic_tls_config_t *quic_tls_config_new(void);
struct quic_tls_config_t *quic_tls_config_new_with_ssl_ctx(SSL_CTX *ssl_ctx);
void quic_tls_config_free(struct quic_tls_config_t *tls_config);

int quic_tls_config_set_certificate_file(struct quic_tls_config_t *tls_config, const char *cert_file);
int quic_tls_config_set_private_key_file(struct quic_tls_config_t *tls_config, const char *key_file);
int quic_tls_config_set_ca_certs(struct quic_tls_config_t *tls_config, const char *ca_path);
void quic_tls_config_set_verify(struct quic_tls_config_t *tls_config, bool verify);
void quic_tls_config_set_early_data_enabled(struct quic_tls_config_t *tls_config, bool enable);
int quic_tls_config_set_application_protos(struct quic_tls_config_t *tls_config, const char *const *protos, intptr_t proto_num);
int quic_tls_config_set_ticket_key(struct quic_tls_config_t *tls_config, const uint8_t *ticket_key, size_t ticket_key_len);
```
#### Endpoint
##### Creation/Destruction
```
struct quic_endpoint_t *quic_endpoint_new(struct quic_config_t *config, bool is_server, const struct quic_transport_methods_t *handler_methods, quic_transport_context_t handler_ctx, const struct quic_packet_send_methods_t *sender_methods, quic_packet_send_context_t sender_ctx);
void quic_endpoint_free(struct quic_endpoint_t *endpoint);
```

##### Receiving/Sending
```
int quic_endpoint_recv(struct quic_endpoint_t *endpoint,uint8_t *buf, size_t buf_len, const struct quic_packet_info_t *info);

typedef struct quic_packet_send_methods_t {
  int (*on_packets_send)(void *psctx, struct quic_packet_out_spec_t *pkts, unsigned int count);
} quic_packet_send_methods_t;
```

##### Timeout events
````
uint64_t quic_endpoint_timeout(const struct quic_endpoint_t *endpoint);
void quic_endpoint_on_timeout(struct quic_endpoint_t *endpoint);
````

##### Internal events
```
int quic_endpoint_process_connections(struct quic_endpoint_t *endpoint);
```

##### Transport Callbacks
```
typedef struct quic_transport_methods_t {
  void (*on_conn_created)(void *tctx,  struct quic_conn_t *conn);
  /* ... */
} quic_transport_methods_t;

typedef struct quic_transport_methods_t {
  void (*on_conn_established)(void *tctx, struct quic_conn_t *conn);
  /* ... */
} quic_transport_methods_t;

typedef struct quic_transport_methods_t {
  void (*on_conn_closed)(void *tctx, struct quic_conn_t *conn);
  /* ... */
} quic_transport_methods_t;


typedef struct quic_transport_methods_t {
  void (*on_stream_created)(void *tctx, struct quic_conn_t *conn, uint64_t stream_id);
  /* ... */
} quic_transport_methods_t;

typedef struct quic_transport_methods_t {
  void (*on_stream_readable)(void *tctx, struct quic_conn_t *conn, uint64_t stream_id);
  /* ... */
} quic_transport_methods_t;

typedef struct quic_transport_methods_t {
  void (*on_stream_writable)(void *tctx, struct quic_conn_t *conn, uint64_t stream_id);
  /* ... */
} quic_transport_methods_t;

typedef struct quic_transport_methods_t {
  void (*on_conn_closed)(void *tctx, struct quic_conn_t *conn);
  /* ... */
} quic_transport_methods_t;

typedef struct quic_transport_methods_t {
  void (*on_new_token)(void *tctx, struct quic_conn_t *conn, const uint8_t *token, size_t token_len);
  /* ... */
} quic_transport_methods_t;
```

##### Miscellaneous
```
void quic_endpoint_set_cid_generator(struct quic_endpoint_t *endpoint, const struct ConnectionIdGeneratorMethods *cid_gen_methods, ConnectionIdGeneratorContext cid_gen_ctx);
bool quic_endpoint_exist_connection(struct quic_endpoint_t *endpoint, const uint8_t *cid, size_t cid_len);
struct quic_conn_t *quic_endpoint_get_connection(struct quic_endpoint_t *endpoint, uint64_t index);
void quic_endpoint_close(struct quic_endpoint_t *endpoint, bool force);
```

#### Connection
##### Creation/Closing
```
int quic_endpoint_connect(struct quic_endpoint_t *endpoint,
                          const struct sockaddr *local,
                          socklen_t local_len,
                          const struct sockaddr *remote,
                          socklen_t remote_len,
                          const char *server_name,
                          const uint8_t *session,
                          size_t session_len,
                          const uint8_t *token,
                          size_t token_len,
                          uint64_t *index);

int quic_conn_close(struct quic_conn_t *conn,
                    bool app,
                    uint64_t err,
                    const uint8_t *reason,
                    size_t reason_len);
```

##### Context
```
void quic_conn_set_context(struct quic_conn_t *conn,
                           void *data);

void *quic_conn_context(struct quic_conn_t *conn);
```

##### Logging and tracing
```
void quic_conn_set_keylog(struct quic_conn_t *conn, void (*cb)(const uint8_t *data,
                                                               size_t data_len,
                                                               void *argp), void *argp);

void quic_conn_set_keylog_fd(struct quic_conn_t *conn,
                             int fd);

void quic_conn_set_qlog(struct quic_conn_t *conn,
                        void (*cb)(const uint8_t *data, size_t data_len, void *argp),
                        void *argp,
                        const char *title,
                        const char *desc);

void quic_conn_set_qlog_fd(struct quic_conn_t *conn,
                           int fd,
                           const char *title,
                           const char *desc);

void quic_conn_trace_id(struct quic_conn_t *conn,
                        const uint8_t **out,
                        size_t *out_len);
```

##### Miscellaneous
```
uint64_t quic_conn_index(struct quic_conn_t *conn);
bool quic_conn_is_server(struct quic_conn_t *conn);
bool quic_conn_is_established(struct quic_conn_t *conn);
bool quic_conn_is_resumed(struct quic_conn_t *conn);
bool quic_conn_is_in_early_data(struct quic_conn_t *conn);
bool quic_conn_is_multipath(struct quic_conn_t *conn);
void quic_conn_application_proto(struct quic_conn_t *conn,
                                 const uint8_t **out,
                                 size_t *out_len);
void quic_conn_server_name(struct quic_conn_t *conn,
                           const uint8_t **out,
                           size_t *out_len);
void quic_conn_session(struct quic_conn_t *conn,
                       const uint8_t **out,
                       size_t *out_len);
int quic_conn_early_data_reason(struct quic_conn_t *conn,
                                const uint8_t **out,
                                size_t *out_len);
bool quic_conn_is_closing(struct quic_conn_t *conn);
bool quic_conn_is_draining(struct quic_conn_t *conn);
bool quic_conn_is_closed(struct quic_conn_t *conn);
bool quic_conn_is_handshake_timeout(struct quic_conn_t *conn);
bool quic_conn_is_idle_timeout(struct quic_conn_t *conn);
bool quic_conn_is_reset(struct quic_conn_t *conn);
bool quic_conn_peer_error(struct quic_conn_t *conn,
                          bool *is_app,
                          uint64_t *error_code,
                          const uint8_t **reason,
                          size_t *reason_len);
bool quic_conn_local_error(struct quic_conn_t *conn,
                           bool *is_app,
                           uint64_t *error_code,
                           const uint8_t **reason,
                           size_t *reason_len);
const struct quic_conn_stats_t *quic_conn_stats(struct quic_conn_t *conn);
```

#### Stream
##### Creation/Closing
##### Reading/Writing
##### Priorities
##### Events
##### Context
##### Miscellaneous

#### Path
##### Creation
##### Miscellaneous

#### Miscellaneous
##### Types
##### Functions
