#ifndef MAIN_SM_STATEMANAGER_H_
#define MAIN_SM_STATEMANAGER_H_

#include "State.h"

class StateManager {

public:
    explicit StateManager();
    ~StateManager();

    void mainLoop();

    bool setState(State state);

private:
    State getState();

private:
    State m_state;
};

#endif  // MAIN_SM_STATEMANAGER_H_
