#!/bin/bash
export PATH=../build:../scripts:$PATH

if [ "$1" != "" ]; then
FILE=$1
else
FILE=query.txt  
fi

if ! xcompiler -q $FILE ; then exit 1 ; fi 

if [ "$2" == "x" ]; then exit 1 ; fi

tmux has-session -t dev
if [ $? != 0 ]
then
    tmux new-session -s dev -n "TEST" -d
    tmux split-window -v 
    tmux resize-pane -t dev:0.0 -y 5
    tmux split-window -h 
    tmux split-window -h
    tmux split-window -h
    tmux split-window -h

    tmux set -g pane-border-status top
    tmux set -g pane-border-format "#{pane_index}" 

    tmux resize-pane -t dev:0.1 -x 15
    tmux resize-pane -t dev:0.2 -x 15
    tmux resize-pane -t dev:0.3 -x 15
    tmux resize-pane -t dev:0.4 -x 15       
    tmux resize-pane -t dev:0.5 -x 15       

    tmux send-keys -t dev:0.0 'xretractor -v' C-m
    sleep 2
    tmux send-keys -t dev:0.1 'clear; xqry -s str1' C-m
    tmux send-keys -t dev:0.2 'clear; xqry -s str2' C-m
    tmux send-keys -t dev:0.3 'clear; xqry -s str3' C-m
    tmux send-keys -t dev:0.4 'clear; xqry -s str4' C-m
    tmux send-keys -t dev:0.5 'clear; xqry -s core1' C-m
fi
tmux attach -t dev
