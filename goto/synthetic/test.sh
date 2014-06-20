nogoto_result=`taskset -c 0 ./nogoto 2>/dev/null`
nogoto_time=`echo $nogoto_result | cut -d' ' -f 3`

goto_result=`taskset -c 0 ./goto 2>/dev/null`
goto_time=`echo $goto_result | cut -d' ' -f 3`

ratio=`python -c "print $nogoto_time/$goto_time"`
echo "ratio = $ratio"
