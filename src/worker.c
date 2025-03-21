#include <stdio.h>
#include <unistd.h>
#include "../include/worker.h"

void *do_work(void *arg) {
    int second = *(int *)arg;   // 传递时间

    printf("child thread is running...awake ater %d second.\n", second);
    sleep(second);              // pause 3 s

    printf("")
}
