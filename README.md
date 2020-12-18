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
- `e` - jump to word end, jump to next word end
- `I` - switch to INSERT MODE and start edit at line start
- `a` - switch to INSERT MODE and start edit after cursor
- `A` - switch to INSERT MODE and start edit at line end
- `C` - switch to INSERT MODE and delete line part after cursor
- `s` - switch to INSERT MODE and delete char at cursor
- `S` - switch to INSERT MODE and delete line from the first non-whitespace character
- `o` - switch to INSERT MODE and create new empty line below current
- `O` - switch to INSERT MODE and create new empty line above current
- `x` - remove character at cursor
- `h`, `j`, `k`, `l` - cursor movement
- `Y` - copy line
- `yy` - copy line
- `p` - paste line after active line
- `P` - paste line before active line
- `dd` - cut active line or selection
- `D` - cut from cursor till the end of the line
- `B` - cut from cursor till the start of the line

### TODO for NORMAL MODE

- `cmd + p` - paste at cursor
- `zz` - scroll editor buffer so that active line is in the middle
- `v` - visual selection
- cut/copy & paste selection (extend `d`/`y` & `p`)

## TODO

- command line
- open/create file in buffer
- save files
- search text in file
- replace text in file
- syntax highlight
