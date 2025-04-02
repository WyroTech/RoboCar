//
// Created by Andreas Wyrobek on 20.11.24.
//

#ifndef SYSTEM_H
#define SYSTEM_H


class System {
public:
    void init();

    void print_system_info();

private:
    static constexpr int SERIAL_CHANNEL = 115200;
};


#endif //SYSTEM_H
