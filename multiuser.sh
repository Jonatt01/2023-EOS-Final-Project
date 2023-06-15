#!/bin/bash

tmux new-session -d

tmux split-window -v


tmux send-keys -t 0 "./user_client_register" Enter
tmux send-keys -t 1 "./user_register_Brain" Enter

tmux send-keys -t 0 "./user_client_duration" Enter
# tmux send-keys -t 0 "./user_client_mode" Enter
tmux send-keys -t 1 "./user_client_duration_Brain" Enter
# tmux send-keys -t 1 "./user_client_mode_Brain" Enter

# Switch focus to the first pane
tmux select-pane -t 0

# Attach to the session (this will keep the panes running)
tmux attach-session
