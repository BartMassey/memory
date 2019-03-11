#!/bin/sh

# Profile memory access times under various scenarios
# Bart Massey

K=1024
M=`expr 1024 \* $K`
G=`expr 1024 \* $M`

for MODE in r w a
do
    for STRIDE in 1 p
    do
        cat <<EOF |
 8 K
 128 K
 1 M
 8 M
 128 M
 1 G
 2 G
 3 G
 4 G
 8 G
 12 G
 16 G
 20 G
 24 G
 32 G
 40 G
EOF
        while read COUNT UNIT
        do
            case $UNIT in
                K) U=$K ;;
                M) U=$M ;;
                G) U=$G ;;
                *)
                    echo "memory.sh: bad unit" >&2
                    exit 1
                    ;;
            esac
            SIZE=`expr $COUNT \* $U`

            RESULT=`./memory $MODE $STRIDE $SIZE 2>/dev/null`
            if [ $? -ne 0 ]
            then
                break
            fi
            echo "$RESULT"
            SLOW=`echo "$RESULT" | awk -F, '{print ($5 + 0.0 > 5.0);}'`
            if [ $SLOW = "1" ]
            then
                break
            fi
        done
    done
done
