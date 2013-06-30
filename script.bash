#!/bin/bash

motif=${1//(/.}
motif=${motif//)/.}

#echo "$motif"

if [ "$#" -eq 0 ]; then
	cat comptoirDuFeu.txt | sed -rn '/^[A-Z]/p'	
else
	cat comptoirDuFeu.txt | sed -rn "/^$motif/,/^[A-Z]/{//d ; s/^\s// ; s/\s+/ /g ; s/\. /#@#/ ; s/ ([0-9]+ GG)/#@#\1/ ; p}" | awk -F"#@" '{print $1,$3,$2}' | column -ts "#"
fi
