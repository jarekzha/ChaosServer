#!/bin/sh 

awk -F '[<>]' '/player game data log/{   \
print $2, $4, $6, $8, $10,		\
$12, $14, $16, $18, $20,		#宠物resID\
$22, $24, $26, $28, $30,		#对应宠物释放次数\
$32, $34, $36, $38, $40,		#技能resID\
$42, $44, $46, $48, $50,		#对应技能释放次数\
$52, $54, $56, $58, $60;		#角色属性\
}' ../../src/xGame/log/xGameServer_analysis.log > gamelog.txt