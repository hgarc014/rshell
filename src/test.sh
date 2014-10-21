
COUNT="ls "
I=0
MAX=10
echo about to execute $MAX flags, this may take a minute...

for i in `seq 1 $MAX`;
do
    COUNT="$COUNT -a"
    I=$i
done

#while [ $I -lt $MAX ];do
#COUNT="$COUNT -a"
#let I=I+1
#done

COUNT="$COUNT ; "
echo $COUNT

echo $I
eval $COUNT

