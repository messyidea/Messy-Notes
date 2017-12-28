默认 debian9 的 vim 会自动识别 mouse ，导致不能够直接通过 ssh 粘帖文本

The only way to cut vim from mouse awareness seems to be commenting out the configuration about the mouse.

In /usr/share/vim/vim80/defaults.vim I commented out the mouse specific detection as in:

" In many terminal emulators the mouse works just fine.  By enabling it you
" can position the cursor, Visually select and scroll with the mouse.
"if has('mouse')
"  set mouse=r
"endif
(in those vim configuration files, " is initiating a comment).

The change has allowed us to copy and paste again without any problems.

from: https://unix.stackexchange.com/questions/318824/vim-cutpaste-not-working-in-stretch-debian-9
