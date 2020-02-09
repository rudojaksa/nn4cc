$ERPLOT=<<EOF;

set terminal pngcairo enhanced size __WIDTH__,__HEIGHT__ font 'DejaVuSansCondensed,12' rounded
set encoding utf8
set termoption dash

DIR   = '__DIR__'
IMAGE = DIR.'/erplot.png'

set output IMAGE
set logscale x
set logscale y
set tics front
set samples 7000
set border linewidth 1.1
set key samplen 5
set key reverse bottom left Left height 0.5

set title '__TITLE__'

set yrange [__YMIN__:__YMAX__]

set ytics add (1,2,3,4,5,6,7,8,9,10,100,1000)
set ytics add (0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9)
set ytics add (0.01,0.02,0.03,0.04,0.05,0.06,0.07,0.08,0.09)
set ytics add (0.001,0.002,0.003,0.004,0.005,0.006,0.007,0.008,0.009)

set xtics (1,10,100,"1k" 1000,"10k" 10000,"100k" 100000,"1M" 1000000,"10M" 10000000,"100M" 100000000,"1G" 1000000000,"10G" 10000000000.0)
set palette model RGB
set palette defined ( 0 "red", 1 "white" )
unset colorbox

set style line 1 lt 1 lw 3 lc rgb "gray80"
set style line 2 lt 1 lw 3 lc rgb "gray40"
set style line 3 lt 1 lw 3 lc rgb "#FF4C3B"
set style line 4 lt 1 lw 3 lc rgb "#7B9CFF"

set xrange [10:]
#set xtics add ('0' 10)

__PLOT__

system("feh ".IMAGE." &")

EOF

