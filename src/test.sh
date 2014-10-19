
COUNT="ls "
I=0
MAX=10000
echo about to execute $MAX flags, this may take a minute...

while [ $I -lt $MAX ];do
COUNT="$COUNT -a"
let I=I+1
done
echo $I
eval $COUNT

