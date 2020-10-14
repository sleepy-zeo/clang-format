#include "StateManager.h"

#include <iostream>

StateManager::StateManager() {
    mainLoop();
}

StateManager::~StateManager() {
}

void StateManager::mainLoop() {

    State nextState = getState();

    while (nextState != State::SHUTDOWN) {
        switch (nextState) {
            case State::INIT:
                /// nextState = handleInit();
                break;
            case State::AUTHORIZING:
                /// nextState = handleAuthorizing();
                break;
            default:
                /// process other states...
                break;
        }
    }

    /// handleShutdown();
}

State StateManager::getState() {
    return m_state;
}

bool StateManager::setState(State newState) {
    if (newState == m_state) {
        std::cout << "alreadyInNewState" << std::endl;
        return true;
    }

    // 判断从oldState转换到newState是否是allowed
    bool allowed = false;
    switch (newState) {
        case State::INIT:
            allowed = false;
            break;
        case State::AUTHORIZING:
            allowed = (State::INIT == m_state || State::WAITING_TO_RETRY_CONNECTING == m_state);
            break;
        default:
            // other states...
            break;
    }

    if (!allowed) {
        std::cout << "stateChangeNotAllowed, oldState: " << m_state << ", newState: " << newState << std::endl;
        return false;
    }

    switch (newState) {
        case State::INIT:
        case State::AUTHORIZING:
        case State::CONNECTING:
        case State::WAITING_TO_RETRY_CONNECTING:
        case State::POST_CONNECTING:
        case State::CONNECTED:
            break;
        case State::SERVER_SIDE_DISCONNECT:
        case State::DISCONNECTING:
        case State::SHUTDOWN:
            break;
    }

    m_state = newState;

    // 唤醒一些正在等待state状态的函数
    // m_wakeEvent.notify_all();

    return true;
}
