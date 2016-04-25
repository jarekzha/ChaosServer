#!/bin/bash

SEP=@_@	#自定义分隔符

#执行顺序
# 先通过awk把时间列提取出来
# 把时间和Log内容整合到一行传递给sort
# sort 排序
# 输出Log内容

awk -F "[][]" -v awk_seq="$SEP" '{printf("%s%s%s\n",$2, awk_seq, $0); }' $1 | \
			sort -k 1,2 | \
			awk -F "$SEP" '{print $2}'

