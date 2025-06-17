@default_files = ('nmst.tex');
$out_dir = 'build';
$pdf_mode = 1;
$pdflatex = "lualatex -synctex=1 --interaction=nonstopmode -file-line-error %O %S";
