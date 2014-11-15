CMD="ls "

MAX=10

echo executing $MAX flags, this may take a minute...

for i in `seq 1 $MAX`;
do
    CMD="$CMD -a "
done

eval $CMD

echo executing $MAX commands, this may take a minute...

CMD2="ls;"
for I in `seq 1 $MAX`
do
    CMD2="$CMD2 ls;"
done

eval $CMD2


