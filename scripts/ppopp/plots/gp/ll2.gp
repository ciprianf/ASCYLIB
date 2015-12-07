load 'gp/style.gp'
set macros
NOYTICS = "set format y ''; unset ylabel"
YTICS = "set ylabel 'Throughput (Mops/s)' offset 2"
PSIZE = "set size 0.5, 0.6"

set key horiz maxrows 1

set output "eps/ll_thr2.eps"

set terminal postscript color "Helvetica" 24 eps enhanced
set rmargin 0
set lmargin 3
set tmargin 3
set bmargin 2.5

title_offset   = -0.5
top_row_y      = 0.44
bottom_row_y   = 0.0
graphs_x_offs  = 0.1
plot_size_x    = 2.615
plot_size_y    = 1.11

DIV              =    1e6
FIRST            =    2
OFFSET           =    3
column_select(i) = column(FIRST + (i*OFFSET)) / (DIV);

LINE0 = '"lazy"'
LINE0lf = '"harris"'
LINE1 = '"mcs-gl-opt"'
LINE2 = '"optik-gl"'
LINE3 = '"optik"'
LINE4 = '"optik-cache"'

PLOT0 = '"Large\n{/*0.8(8192 elements, 1% updates)}"'
PLOT1 = '"Medium\n{/*0.8(4096 elements, 10% updates)}"'
PLOT2 = '"Small\n{/*0.8(2048 elements, 20% updates)}"'
PLOT3 = '"Large skewed\n{/*0.8(512 elements, 50% updates)}"'
PLOT4 = '"Small skewed\n{/*0.8(128 elements, 100% updates)}"'

# font "Helvetica Bold"
set label 1 "Opteron" at screen 0.018, screen 0.18 rotate by 90 font ',30' textcolor rgb "red"
set label 2 "Xeon"    at screen 0.018, screen 0.66 rotate by 90 font ',30' textcolor rgb "red"


# ##########################################################################################
# XEON #####################################################################################
# ##########################################################################################


FILE0 = '"data/lpdxeon2680.ll.i8192.u40.w0.dat"'
FILE1 = '"data/lpdxeon2680.ll.i1024.u40.w0.dat"'
FILE2 = '"data/lpdxeon2680.ll.i64.u40.w0.dat"'
FILE3 = '"data/lpdxeon2680.ll.i8192.u40.w2.dat"'
FILE4 = '"data/lpdxeon2680.ll.i64.u40.w2.dat"'

unset xlabel
set xrange [0:61]
set xtics ( 1, 10, 20, 30, 40, 50, 60 ) offset 0,0.4
unset key


set size plot_size_x, plot_size_y
set multiplot layout 5, 2

set size 0.5, 0.6
set origin 0.0 + graphs_x_offs, top_row_y
set title @PLOT0 offset 0.2,title_offset font ",28"
set ylabel 'Throughput (Mops/s)' offset 2,0.5
set ytics 0.5
plot \
     @FILE0 using 1:(column_select(0)) title @LINE0lf ls 6 with linespoints, \
     "" using 1:(column_select(1)) title @LINE0 ls 1 with linespoints, \
     "" using 1:(column_select(2)) title @LINE1 ls 2 with linespoints, \
     "" using 1:(column_select(3)) title @LINE2 ls 3 with linespoints, \
     "" using 1:(column_select(4)) title @LINE3 ls 4 with linespoints, \
     "" using 1:(column_select(5)) title @LINE4 ls 8 with linespoints, \
     "" using 1:(column_select(6)) title @LINE4 ls 5 with linespoints

set origin 0.5 + graphs_x_offs, top_row_y
@PSIZE
set lmargin 4
@YTICS
set ylabel ""
unset ylabel
set title @PLOT1
set ytics 1
plot \
     @FILE1 using 1:(column_select(0)) title @LINE0lf ls 6 with linespoints, \
     "" using 1:(column_select(1)) title @LINE0 ls 1 with linespoints, \
     "" using 1:(column_select(2)) title @LINE1 ls 2 with linespoints, \
     "" using 1:(column_select(3)) title @LINE2 ls 3 with linespoints, \
     "" using 1:(column_select(4)) title @LINE3 ls 4 with linespoints, \
     "" using 1:(column_select(5)) title @LINE4 ls 8 with linespoints, \
     "" using 1:(column_select(6)) title @LINE4 ls 5 with linespoints

set origin 1.0 + graphs_x_offs, top_row_y
@PSIZE
set ytics auto
@YTICS
set ylabel ""
unset ylabel
set title @PLOT2
# set ytics 1
plot \
     @FILE2 using 1:(column_select(0)) title @LINE0lf ls 6 with linespoints, \
     "" using 1:(column_select(1)) title @LINE0 ls 1 with linespoints, \
     "" using 1:(column_select(2)) title @LINE1 ls 2 with linespoints, \
     "" using 1:(column_select(3)) title @LINE2 ls 3 with linespoints, \
     "" using 1:(column_select(4)) title @LINE3 ls 4 with linespoints, \
     "" using 1:(column_select(5)) title @LINE4 ls 8 with linespoints, \
     "" using 1:(column_select(6)) title @LINE4 ls 5 with linespoints

set origin 1.5 + graphs_x_offs, top_row_y
@PSIZE
set title @PLOT3
@YTICS
set ylabel ""
unset ylabel
# set ytics 5
plot \
     @FILE3 using 1:(column_select(0)) title @LINE0lf ls 6 with linespoints, \
     "" using 1:(column_select(1)) title @LINE0 ls 1 with linespoints, \
     "" using 1:(column_select(2)) title @LINE1 ls 2 with linespoints, \
     "" using 1:(column_select(3)) title @LINE2 ls 3 with linespoints, \
     "" using 1:(column_select(4)) title @LINE3 ls 4 with linespoints, \
     "" using 1:(column_select(5)) title @LINE4 ls 8 with linespoints, \
     "" using 1:(column_select(6)) title @LINE4 ls 5 with linespoints

set origin 2.0 + graphs_x_offs, top_row_y
@PSIZE
set title @PLOT4
@YTICS
set ylabel ""
unset ylabel
set ytics 8
plot \
     @FILE4 using 1:(column_select(0)) title @LINE0lf ls 6 with linespoints, \
     "" using 1:(column_select(1)) title @LINE0 ls 1 with linespoints, \
     "" using 1:(column_select(2)) title @LINE1 ls 2 with linespoints, \
     "" using 1:(column_select(3)) title @LINE2 ls 3 with linespoints, \
     "" using 1:(column_select(4)) title @LINE3 ls 4 with linespoints, \
     "" using 1:(column_select(5)) title @LINE4 ls 8 with linespoints, \
     "" using 1:(column_select(6)) title @LINE4 ls 5 with linespoints



# ##########################################################################################
# OPTERON ##################################################################################
# ##########################################################################################

FILE0 = '"data/lpd48core.ll.i8192.u1.dat"'
FILE1 = '"data/lpd48core.ll.i4096.u10.dat"'
FILE2 = '"data/lpd48core.ll.i2048.u20.dat"'
FILE3 = '"data/lpd48core.ll.i512.u50.dat"'
FILE4 = '"data/lpd48core.ll.i128.u100.dat"'

set xlabel "# Threads" offset 0, 0.75 font ",28"
set xrange [0:65]
set xtics ( 1, 12, 24, 36, 48, 56, 64 ) offset 0,0.4

unset title

set lmargin 3
@PSIZE
set origin 0.0 + graphs_x_offs, bottom_row_y
# set title @PLOT0 offset 0.2,title_offset
set ylabel 'Throughput (Mops/s)' offset 2,-0.5
set ytics 0.5
plot \
     @FILE0 using 1:(column_select(0)) title @LINE0 ls 6 with linespoints, \
     @FILE0 using 1:(column_select(0)) title @LINE0 ls 1 with linespoints, \
     "" using 1:(column_select(1)) title @LINE1 ls 2 with linespoints, \
     "" using 1:(column_select(2)) title @LINE2 ls 3 with linespoints, \
     "" using 1:(column_select(3)) title @LINE3 ls 4 with linespoints, \
     "" using 1:(column_select(4)) title @LINE4 ls 8 with linespoints

set origin 0.5 + graphs_x_offs, bottom_row_y
@PSIZE
set lmargin 4
@YTICS
set ylabel ""
unset ylabel
# set title @PLOT1
set ytics 1
plot \
     @FILE1 using 1:(column_select(0)) title @LINE0 ls 6 with linespoints, \
     @FILE1 using 1:(column_select(0)) title @LINE0 ls 1 with linespoints, \
     "" using 1:(column_select(1)) title @LINE1 ls 2 with linespoints, \
     "" using 1:(column_select(2)) title @LINE2 ls 3 with linespoints, \
     "" using 1:(column_select(3)) title @LINE3 ls 4 with linespoints, \
     "" using 1:(column_select(4)) title @LINE4 ls 8 with linespoints


set origin 1.0 + graphs_x_offs, bottom_row_y
@PSIZE
set ytics auto
@YTICS
set ylabel ""
unset ylabel
# set title @PLOT2
set ytics 2
plot \
     @FILE2 using 1:(column_select(0)) title @LINE0 ls 6 with linespoints, \
     @FILE2 using 1:(column_select(0)) title @LINE0 ls 1 with linespoints, \
     "" using 1:(column_select(1)) title @LINE1 ls 2 with linespoints, \
     "" using 1:(column_select(2)) title @LINE2 ls 3 with linespoints, \
     "" using 1:(column_select(3)) title @LINE3 ls 4 with linespoints, \
     "" using 1:(column_select(4)) title @LINE4 ls 8 with linespoints


set origin 1.5 + graphs_x_offs, bottom_row_y
@PSIZE
# set title @PLOT3
@YTICS
set ylabel ""
unset ylabel
set ytics 3
plot \
     @FILE3 using 1:(column_select(0)) title @LINE0 ls 6 with linespoints, \
     @FILE3 using 1:(column_select(0)) title @LINE0 ls 1 with linespoints, \
     "" using 1:(column_select(1)) title @LINE1 ls 2 with linespoints, \
     "" using 1:(column_select(2)) title @LINE2 ls 3 with linespoints, \
     "" using 1:(column_select(3)) title @LINE3 ls 4 with linespoints, \
     "" using 1:(column_select(4)) title @LINE4 ls 8 with linespoints


set origin 2.0 + graphs_x_offs, bottom_row_y
@PSIZE
# set title @PLOT4
@YTICS
set ylabel ""
unset ylabel
set ytics 2
plot \
     @FILE4 using 1:(column_select(0)) title @LINE0 ls 6 with linespoints, \
     @FILE4 using 1:(column_select(0)) title @LINE0 ls 1 with linespoints, \
     "" using 1:(column_select(1)) title @LINE1 ls 2 with linespoints, \
     "" using 1:(column_select(2)) title @LINE2 ls 3 with linespoints, \
     "" using 1:(column_select(3)) title @LINE3 ls 4 with linespoints, \
     "" using 1:(column_select(4)) title @LINE4 ls 8 with linespoints


unset origin
unset border
unset tics
unset xlabel
unset label
unset arrow
unset title
unset object

#Now set the size of this plot to something BIG
set size plot_size_x, plot_size_y #however big you need it
set origin 0.0, 1.1

#example key settings
# set key box 
#set key horizontal reverse samplen 1 width -4 maxrows 1 maxcols 12 
#set key at screen 0.5,screen 0.25 center top
set key font ",28"
set key spacing 1.5
set key horiz
set key at screen 1.3, screen 1.108 center top

#We need to set an explicit xrange.  Anything will work really.
set xrange [-1:1]
@NOYTICS
set yrange [-1:1]
plot \
     NaN title @LINE0lf ls 6 with linespoints, \
     NaN title @LINE0 ls 1 with linespoints, \
     NaN title @LINE1 ls 2 with linespoints, \
     NaN title @LINE2 ls 3 with linespoints, \
     NaN title @LINE3 ls 4 with linespoints, \
     NaN title @LINE4 ls 8 with linespoints

#</null>
unset multiplot  #<--- Necessary for some terminals, but not postscript I don't thin
