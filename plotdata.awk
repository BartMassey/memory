#!/usr/bin/env awk -f
BEGIN {
    FS=","
}
{
    mode = $1
    stride = $2
    size = $4
    time = $6
    if (mode != curmode || stride != curstride) {
        if (started)
            printf "\n\n"
        printf "\"%s,%s\"\n", mode, stride
        curmode = mode
        curstride = stride
        started = 1
    }
    print size, time
}
