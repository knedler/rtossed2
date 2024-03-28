// user_syscalls.h
#ifndef USER_SYSCALLS_H
#define USER_SYSCALLS_H

#include <stdint.h>

int microsleep(uint32_t msec);
int _kill(int pid, int sig);

#endif // USER_SYSCALLS_H
