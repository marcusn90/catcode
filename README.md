# CATCODE

- 2 mods - NORMAL & INSERT, like vim.
- max 2 buffers in vertical split.

## INSERT MODE

`ESC` - switch to NORMAL MODE

## NORMAL MODE

- `i` - switch to INSERT MODE and start edit at cursor
- `\` - toggle split screen
- `1` - make buffer 1 active
- `2` - make buffer 2 active
- `(opt + ->)` - jump to next word start
- `(opt + <-)` - jump to word start, jump to prev word start
- `e` - jump to word end, jump to next word end
- `I` - switch to INSERT MODE and start edit at line start
- `a` - switch to INSERT MODE and start edit after cursor
- `A` - switch to INSERT MODE and start edit at line end

### TODO for NORMAL MODE

- `C` - switch to INSERT MODE and delete line part after cursor
- `S` - switch to INSERT MODE and delete line from the first non-whitespace character
- `o` - switch to INSERT MODE and create new empty line below current
- `O` - switch to INSERT MODE and create new empty line above current
- `x` - remove character at cursor
- `h`, `j`, `k`, `l` - cursor movement
- `dd` - delete line under cursor
- `yy` - copy line
- `p` - paste at cursor or to the next line if line was copied
- `P` - paste before cursor or to the prev line if line was copied

## TODO

- command line
- open/create file in buffer
- save files
- search text in file
- replace text in file
- syntax highlight