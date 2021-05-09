#!/bin/bash

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
		mkdir l"$ldmodel"s"$strength"w1p2
		cd l"$ldmodel"s"$strength"w1p2
		/Users/chipxipo/talys/gnew/cd1/run/run.sh $ldmodel $strength 1 2
		cd ../
	done
done