#ifndef MAIN_SM_STATE_H_
#define MAIN_SM_STATE_H_

#include <cstdint>
#include <iostream>

enum class State {
    /// Initial state, not doing anything.
    INIT,
    /// Waiting for authorization to complete.
    AUTHORIZING,
    /// Making a connection to AVS.
    CONNECTING,
    /// Waiting for a timeout before retrying to connect to AVS.
    WAITING_TO_RETRY_CONNECTING,
    /// Performing operations that require a connection, but which must be done before the connection
    /// becomes widely available.
    POST_CONNECTING,
    /// Connectsed to AVS and available for general use.
    CONNECTED,
    /// Handling the server disconnecting.
    SERVER_SIDE_DISCONNECT,
    /// Tearing down the connection.  Possibly waiting for some streams to complete.
    DISCONNECTING,
    /// The connection is completely shut down.
    SHUTDOWN,

};

std::ostream& operator<<(std::ostream& stream, State state) {
    switch (state) {
        case State::INIT:
            return stream << "INIT";
        case State::AUTHORIZING:
            return stream << "AUTHORIZING";
        case State::CONNECTING:
            return stream << "CONNECTING";
        case State::WAITING_TO_RETRY_CONNECTING:
            return stream << "WAITING_TO_RETRY_CONNECTING";
        case State::POST_CONNECTING:
            return stream << "POST_CONNECTING";
        case State::CONNECTED:
            return stream << "CONNECTED";
        case State::SERVER_SIDE_DISCONNECT:
            return stream << "SERVER_SIDE_DISCONNECT";
        case State::DISCONNECTING:
            return stream << "DISCONNECTING";
        case State::SHUTDOWN:
            return stream << "SHUTDOWN";
    }
    return stream << "";
}

#endif  // MAIN_SM_STATE_H_
