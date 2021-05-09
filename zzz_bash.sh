#!/bin/bash
rm ir.txt
for ldmodel in {1..6}
do
	for strength in {2..8}
	do
#		for widthmode in {0..3}
#		do
#			for preeqmode in {1..4}
#			do
#				../run/run.sh $ldmodel $strength $widthmode $preeqmode
#				echo 'ldmodel'$ldmodel'strength'$strength'widthmode'$widthmode'preeqmode'$preeqmode''
#			done			
#		done
		./zzz.sh l"$ldmodel"s"$strength"w1p2/rp048115.L00.116 l"$ldmodel"s"$strength"w1p2/rp048115.L01.116
	done
done