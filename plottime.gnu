set term postscript enhanced eps color solid
#set term  png enhanced
#set term postscript eps enhanced
set key reverse Left top outside spacing 2.7 at screen 0.85,0.98
#set size 0.7,1.0
 
set lmargin 10
set rmargin 5
set tmargin 16
 
set bmargin 5
#set size square
 
set style line 1 linecolor rgbcolor "#FF0000" # 
set style line 2 linecolor rgbcolor "#BF0040" #
set style line 3 linecolor rgbcolor "#7F0080" # 
set style line 4 linecolor rgbcolor "#4F00C0" # 
set style line 5 linecolor rgbcolor "#0000FF" # 
set style line 6 linecolor rgbcolor "#007788" # 
set style line 7 linecolor rgbcolor "#00FF00" #
set style line 11 linecolor rgbcolor "#0000FF" #
set style line 12 linecolor rgbcolor "#0040BF" #
set style line 13 linecolor rgbcolor "#00807F" #
set style line 14 linecolor rgbcolor "#00BF00" #
set style line 15 linecolor rgbcolor "#F08090" #
set style line 16 linecolor rgbcolor "#9080F0" #
set style line 17 linecolor rgbcolor "#F08090" #
set style line 9 linecolor rgbcolor "#909090" #black
set linestyle 10 lt 0 linecolor rgbcolor "#707070" #
 
 
set ytics nomirror
unset y2tic
 
lastx=0.0
lasty=0.0
differ(x,y) = (dy=y-lasty, dx=x-lastx, lasty=y, lastx=x,dy/dx)
 
###################################################
set output "timeplot.eps"
set datafile separator ","
set xlabel "{/Times=30 Time {/Times-Italic t } [s]}"
set ylabel "{/Times=30 Position []}"

set zeroaxis

fileA = "data/testdata.csv"

plot \
fileA using 1:3 w l title "{/Times=22 testdata }",\

