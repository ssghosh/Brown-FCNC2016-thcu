#!/bin/bash

date

echo "START"

combineTool.py -M T2W -i DataCard.txt -o workspace.root
combineTool.py -M Asymptotic -d workspace.root --there -n .limit -t -1
combineTool.py -M CollectLimits *.limit.* --use-dirs -o limits.json

echo "DONE"

date

