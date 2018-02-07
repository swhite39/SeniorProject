" Additional vim directives added to

" 1. discourage use of arrow keys

" 2. ensure that tabbing is enabled when editing Makefiles

" 3. add sane indenting in C and C++ files

"inoremap  <Up>     <NOP>

"inoremap  <Down>   <NOP>

"inoremap  <Left>   <NOP>

"inoremap  <Right>  <NOP>

"noremap   <Up>     <NOP>

"noremap   <Down>   <NOP>

"noremap   <Left>   <NOP>

"noremap   <Right>  <NOP>
"
ino " ""<left>

ino ' ''<left>

inoremap { {<CR>}<Esc>ko

set number

set autoindent

set smartindent

set tabstop=4

set showmatch

filetype plugin indent on

autocmd FileType make set noexpandtab|set autoindent

autocmd FileType c set tabstop=2|set shiftwidth=2|set expandtab|set autoindent

autocmd FileType cpp set tabstop=2|set shiftwidth=2|set expandtab|set autoindent

syntax on
