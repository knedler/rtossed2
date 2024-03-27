// kernel_syscalls.h
#ifndef KERNEL_SYSCALLS_H
#define KERNEL_SYSCALLS_H

#include <sys/types.h>

int sys_millisleep(uint32_t req);
int sys_kill(pid_t pid, int sig);

#endif				// KERNEL_SYSCALLS_H
