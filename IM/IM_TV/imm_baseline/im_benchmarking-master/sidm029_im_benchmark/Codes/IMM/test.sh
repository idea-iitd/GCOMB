var=NETHEPT/IC/
op=${var#*/}
echo $op
echo ${op%%/*}
echo ${var##*/}
echo ${var%/*}
echo ${var%%/*}
