# Google Quiche API

## Overview

The Google quic library, called [Quiche](CreateAndBindServerSocket), does not have a developed API for C++
applications. Building a generic QUIC API is worth the effort in order to
abstract the interface to the Google QUIC and also provide a bridge that can be consistently
update to match changes to Quiche.

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

    QUIC_SET_CONTEXT_FN                 SetContext;
    QUIC_GET_CONTEXT_FN                 GetContext;
    QUIC_SET_CALLBACK_HANDLER_FN        SetCallbackHandler;

    QUIC_SET_PARAM_FN                   SetParam;
    QUIC_GET_PARAM_FN                   GetParam;

    QUIC_REGISTRATION_OPEN_FN           RegistrationOpen;
    QUIC_REGISTRATION_CLOSE_FN          RegistrationClose;
    QUIC_REGISTRATION_SHUTDOWN_FN       RegistrationShutdown;

    QUIC_CONFIGURATION_OPEN_FN          ConfigurationOpen;
    QUIC_CONFIGURATION_CLOSE_FN         ConfigurationClose;
    QUIC_CONFIGURATION_LOAD_CREDENTIAL_FN
                                        ConfigurationLoadCredential;

    QUIC_LISTENER_OPEN_FN               ListenerOpen;
    QUIC_LISTENER_CLOSE_FN              ListenerClose;
    QUIC_LISTENER_START_FN              ListenerStart;
    QUIC_LISTENER_STOP_FN               ListenerStop;

    QUIC_CONNECTION_OPEN_FN             ConnectionOpen;
    QUIC_CONNECTION_CLOSE_FN            ConnectionClose;
    QUIC_CONNECTION_SHUTDOWN_FN         ConnectionShutdown;
    QUIC_CONNECTION_START_FN            ConnectionStart;
    QUIC_CONNECTION_SET_CONFIGURATION_FN
                                        ConnectionSetConfiguration;
    QUIC_CONNECTION_SEND_RESUMPTION_FN  ConnectionSendResumptionTicket;

    QUIC_STREAM_OPEN_FN                 StreamOpen;
    QUIC_STREAM_CLOSE_FN                StreamClose;
    QUIC_STREAM_START_FN                StreamStart;
    QUIC_STREAM_SHUTDOWN_FN             StreamShutdown;
    QUIC_STREAM_SEND_FN                 StreamSend;
    QUIC_STREAM_RECEIVE_COMPLETE_FN     StreamReceiveComplete;
    QUIC_STREAM_RECEIVE_SET_ENABLED_FN  StreamReceiveSetEnabled;

    QUIC_DATAGRAM_SEND_FN               DatagramSend;

} QUIC_API_TABLE;
```

### [Tquic](https://github.com/Tencent/tquic)

TQUIC is a high-performance, lightweight, and cross-platform library for the IETF QUIC protocol. Like
Cloudflair, it is written in Rust and has a C/C++ API. Tquic was developed by Tencent, a Chinese
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

#### Endpoint
##### Creation/Destruction
##### Receiving/Sending
##### Timeout events
##### Internal events
##### Transport Callbacks
##### Miscellaneous

#### Connection
##### Creation/Closing
##### Context
##### Logging and tracion
##### Miscellaneous

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
