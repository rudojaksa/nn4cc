#!/usr/bin/perl
# include "CONFIG.pl"
# include "help.pl"
# include "support.pl"
print STDERR "\n";

# --------------------------------------------------------------------------------- HELP
$HELP_MAIN=<<EOF;

NAME
    erplot - errors plot for $PACKAGE

USAGE
    erplot [OPTIONS] LOGDIRS/FILES...

DESCRIPTION
    Plot training/testing errors history from the $PACKAGE logs.

OPTIONS
        -h  This help.
#       -v  Verbose.
CC(-g 640x480)  Size output image to given geomettry.  The CC(-g max) is fullscreen.
   CC(-t[r12])  Plot tCC(r)ain/CC(t)estCC(1)/testCC(2) files (t is alias for 1).
    CC(-[COL])  Plot particular column (CC(-amae) plots AMAE errors from CC(amae) column).
            The CC(-y1..y12) can be used to address columns y1 to y12.

EOF

# -------------------------------------------------------------------------------- ARGVS
foreach $i (@ARGV) { if($i eq "-h") { printhelp($HELP_MAIN); exit 0; }}

# geometry
our $WIDTH = 1200;
our $HEIGHT = 720;
for($i=0; $i<$#ARGV; $i++) {
  next if $ARGV[$i] ne "-g";
  my $ok=0;
  if($ARGV[$i+1] =~ /^([0-9]*)x([0-9]*)$/) {
    $ok=1;
    $WIDTH=$1 if $1;
    $HEIGHT=$2 if $2; }
  if($ARGV[$i+1] eq "max") { $ok=1; $WIDTH="max"; $HEIGHT="max"; }
  next if not $ok;
  $ARGV[$i] = ""; $ARGV[$i+1] = ""; last; }

# train/test1/test2
foreach $i (@ARGV) {
  if($i =~ /^-t([12rt]+)$/) {
    my $s = $1;
    $XTRAIN=1 if $s=~/r/;
    $XTEST1=1 if $s=~/t/;
    $XTEST1=1 if $s=~/1/;
    $XTEST2=1 if $s=~/2/;
    $i=""; }}
$XTRAIN=1 and $XTEST1=1 and $XTEST2=1 if not defined $XTRAIN and not defined $XTEST1 and not defined $XTEST2;

# which files
our @AFILE;
for($i=0; $i<=$#ARGV; $i++) {
  next if $ARGV[$i] eq "";
  next if not -f $ARGV[$i];
  push @AFILE,$ARGV[$i];
  $ARGV[$i] = ""; }

# which logdirs
our @ADIR;
for($i=0;$i<=$#ARGV;$i++) {
  next if $ARGV[$i] eq "";
  next if not -d $ARGV[$i];
  push @ADIR,$ARGV[$i];
  $ARGV[$i] = ""; }

# which columns
our @ACOLS; # argv columns
for($i=0;$i<=$#ARGV;$i++) {
  if($ARGV[$i] =~ /^-(([a-z0-9_-]+)(\.\.([a-z0-9_-]+))?)$/ and not inar \@ACOLS,$1) {
    push @ACOLS,$1;
    $ARGV[$i] = ""; }}
push @ACOLS,"amae" if not scalar @ACOLS; # default

# ----------------------------------------------------------------------------- GEOMETRY

if($WIDTH eq "max" or $HEIGHT eq "max") {
  my $randr = cmd_silent "xrandr | grep current";
  if($randr =~ /current ([0-9]+) x ([0-9]+)/) {
    $WIDTH = $1 if $WIDTH eq "max";
    $HEIGHT = $2 if $HEIGHT eq "max"; }}
else {
  $WIDTH = 1600 if $WIDTH eq "max";
  $HEIGHT = 1200 if $HEIGHT eq "max"; }

printf STDERR "%10s: $WIDTH${CK_}x$CD_$HEIGHT\n","geometry";

# ---------------------------------------------------------------------------------- TMP

# main tmp directory
our $TMPDIR = "/tmp/erplot";
# sys "rmdir -v --ignore-fail-on-non-empty $TMPDIR/*";

# find the newest directory (TODO: file)
my $sx = $ADIR[0]; foreach(@ADIR) { $sx = $_ if newerdir $_,$sx; }
$sx =~ /\/([^\/]*)$/;
my $newestdir = $1;

# this plot tmp directory
our $TMP = "$TMPDIR/$newestdir";
sys "mkdir -p $TMP" if not -d $TMP;

# ----------------------------------------------------------------------- IDENTIFY FILES
our @TRAIN;	# training-set logs
our @TEST1;	# test-set logs
our @TEST2;	# test2 logs
our @OTHER;	# other logs

# add logdirs
my @files;
foreach(@ADIR) {
  foreach my $f (split /\n/,`ls $_/*.log`) {
    next if $f =~ /journal\.log$/;
    next if $f =~ /gcc\.log$/;
    push @files,$f; }}

# add and categorize files
foreach my $f (@AFILE,@files) {
  if   ($f =~ /ertrain\.log$/) { push @TRAIN,$f if $XTRAIN; }
  elsif($f =~ /ertest2\.log$/) { push @TEST2,$f if $XTEST2; }
  elsif($f =~ /ertest1\.log$/) { push @TEST1,$f if $XTEST1; }
  elsif($f =~ /ertest\.log$/)  { push @TEST1,$f if $XTEST1; }
  else			       { push @OTHER,$f; }}

# aggregate files
push @FILE,@TEST1;
push @FILE,@TEST2;
push @FILE,@TRAIN;
push @FILE,@OTHER;

if(not scalar @FILE) {
  print STDERR "     ${CR_}error:$CD_ no input files\n";
  exit 1; }

# ----------------------------------------------------------------------------- JOURNALS
our %JRF; # journals file-names
our %JRN; # journals content

# identify journals
foreach my $f (@FILE) {
  my $dir = ".";
  $dir = $1 if $f =~ /^(.*)\/([^\/]*)$/;
  my $journal = "$dir/journal.log";
  next if not -f $journal;
  $JRF{$f} = $journal; }

# read journals
my %jrn;
foreach my $f (@FILE) {
  next if defined $jrn{$JRF{$f}};
  $jrn{$JRF{$f}} = `cat $JRF{$f}`; }

# parse journals
our %TOP; # topology from journal
our %GAM; # gamma from journal
foreach my $j (keys %jrn) {
  $TOP{$j} = $1 if $jrn{$j} =~ /\ntopology: (.*)\n/;
  $GAM{$j} = $1 if $jrn{$j} =~ /\nlearning rate: (.*)\n/; }

# add journals
foreach my $f (@FILE) {
  $JRN{$f} = "\n$jrn{$JRF{$f}}\n"; }

# verbose
foreach my $j (keys %jrn) {
  printf STDERR "%10s: $CK_$j $TOP{$j} g=$GAM{$j} $CD_\n","journal"; }

# --------------------------------------------------------------------------- FILE NAMES
our %TNM; # tmp filenames
our %XNM; # text names
our %JNM; # names from journal

foreach my $f (@FILE) {
  $f =~ /\/?([^\/]*)\/([^\/]*)$/;
  my $tdir = "$TMPDIR/$1"; sys "mkdir -p $tdir" if not -d $tdir;
  $TNM{$f} = "$TMPDIR/$1/$2";
  $XNM{$f} = "$1";
  $JNM{$f} = $XNM{$f};
  $JNM{$f} = $1 if $JRN{$f} =~ /\nname: (.*)\n/; }

print STDERR " directory: $CC_$_$CD_\n" foreach @ADIR;
print STDERR "train file: $CG_$_$CD_ $CK_($JNM{$_})$CD_\n" foreach @TRAIN;
print STDERR "test1 file: $CG_$_$CD_ $CK_($JNM{$_})$CD_\n" foreach @TEST1;
print STDERR "test2 file: $CG_$_$CD_ $CK_($JNM{$_})$CD_\n" foreach @TEST2;
print STDERR "other file: $CG_$_$CD_ $CK_($JNM{$_})$CD_\n" foreach @OTHER;

# -------------------------------------------------------------------- REQUESTED COLUMNS

# read headers
our %HDR;  # header
our %FST;  # 1st row
foreach my $f (@FILE) {
  my $s1 = `head -1 $f`;
  my $s2 = `head -2 $f | tail -1`;
  my $fst=$s2;
  if($s1 =~ /^\h*\#\h*(.*?)\h*$/) {
    my @h = split / +/,$1;
    $HDR{$f} = \@h; }
  else {
    $fst=$s1; }	# 1st line
  my @fst = split / +/,$fst;
  $FST{$f} = \@fst; }

# default column numbers if no header provided
foreach my $f (@FILE) {
  next if @{$HDR{$f}};
  my $n = scalar @{$FST{$f}};
  push @{$HDR{$f}},"pat" if $n>0;
  push @{$HDR{$f}},"amae" if $n>1;
  push @{$HDR{$f}},"aed" if $n>2;
  push @{$HDR{$f}},"amse" if $n>3;
  push @{$HDR{$f}},"armse" if $n>4; }

# reconstruct header string
our %HDRs; # header string
foreach my $f (@FILE) {
  my $s = "";
  $s .= "$_ " foreach @{$HDR{$f}};
  $HDRs{$f} = $s; }

# get columns-map for requested columns
our %REQ; # requested columns names (in array)
our %RNO; # requested columns numbers (in file-header order)
foreach my $f (@FILE) {
  my @h = ();
  foreach my $c (@ACOLS) {

    if($c =~ /^(.*)\.\.(.*)$/) {
      my $c1 = $1; my $i1;
      my $c2 = $2; my $i2;
      for(my $i=0; $i<=$#{$HDR{$f}}; $i++) {
	next if $c1 ne $HDR{$f}[$i];
	$i1 = $i; last; }
      for(my $i=$i1; $i<=$#{$HDR{$f}}; $i++) {
	next if $c2 ne $HDR{$f}[$i];
	$i2 = $i; last; }
      if(defined $i1 and defined $i2) {
	for(my $i=$i1; $i<=$i2; $i++) {
	  my $h = $HDR{$f}[$i];
	  push @h,$h;
	  $RNO{$f}{$h} = $i+1; }}}

    else {
      for(my $i=0; $i<=$#{$HDR{$f}}; $i++) {
	next if $c ne $HDR{$f}[$i];
	my $h = $HDR{$f}[$i];
	push @h,$h;
	$RNO{$f}{$h} = $i+1;
	last; }}}

  $REQ{$f} = \@h; }

# ------------------------------------------------------------------------ PRINT HEADERS

my @old;
foreach my $f (@FILE) {
  next if inar \@old,$HDRs{$f};
  push @old,$HDRs{$f};

  printf STDERR "%10s:","headers";

  # limit the number of columns
  my $n = $#{$HDR{$f}}; 
  my $end = "";
  if($n>12) { my $nn=$n+1; $end=" ... $HDR{$f}[$n]$CK_=$nn$CD_"; $n=12; }

  for(my $i=0; $i<=$n; $i++) {
    my $h = $HDR{$f}[$i];
    my $c1 = "";
       $c1 = $CC_ if inar $REQ{$f},$h;
       $c1 = $CK_ if $h eq "pat";
    printf STDERR " $c1$h$CK_=%d$CD_",$i+1; }

  print STDERR "$end\n"; }

# ---------------------------------------------------------------------- COLUMNS SUMMARY

# all columns to be displayed
our @COLS;
foreach my $f (@FILE) {
  foreach my $h (@{$REQ{$f}}) {
    push @COLS,$h if not inar \@COLS,$h; }}

printf STDERR "%10s:","to plot";
print STDERR " $CC_$_$CD_" foreach @COLS;
print STDERR "\n";

# -------------------------------------------------------------------- NICE COLUMN NAMES

our %CNM;
foreach my $h (@COLS) {
  my $nm = $h;
     $nm =~ tr/a-z/A-Z/; # TODO: manual map-table
  $CNM{$h} = $nm; }

# -------------------------------------------------- SORT LOGS & REMOVE BLOCK SEPARATORS

foreach my $f (@FILE) {
  next if newer $TNM{$f},$f;
  sys "sort -n $f | sed 's:  \\+: :g' | sed 's:^0 :0.1 :' > $TNM{$f}"; }

# ------------------------------------------------------------------------------ MIN MAX
# TODO: cached MIN-MAX
my @all; # all numbers from all columns

foreach my $f (@FILE) {
  my $clist = "";
     $clist .= "$RNO{$f}{$_}," foreach @{$REQ{$f}};
     $clist =~ s/,$//;
  my $cnums = cmd_silent "cat $TNM{$f} | grep -v '#' | cut -d ' ' -f $clist" if $clist ne "";
  push @all,split(/[ \t\n]/,$cnums); }

our $YMAX = $all[0];
our $YMIN = $all[0];
foreach my $n (@all) {
  $YMIN = $n if $n < $YMIN;
  $YMAX = $n if $n > $YMAX; }

printf STDERR "%10s: $YMIN$CK_-$CD_$YMAX\n","range";

# -------------------------------------------------------------------------------- TITLE

# columns
my @req;
foreach my $h (@ACOLS) {
  my $hh = $h;
  $hh = $CNM{$h} if defined $CNM{$h};
  if($h =~ /^(.*)\.\.(.*)$/) {
    my $h1 = $1; $h1 = $CNM{$h1} if defined $CNM{$h1};
    my $h2 = $2; $h2 = $CNM{$h2} if defined $CNM{$h2};
    $hh = "$h1..$h2"; }
  push @req,$hh if not inar \@req,$hh; }
# foreach my $f (@FILE) {
#   foreach my $h (@{$REQ{$f}}) {
#     push @req,$CNM{$h} if not inar \@req,$CNM{$h}; }}
my $req;
$req .= "$_, " foreach @req;
$req =~ s/, $//;

# experiments
my @fil;
foreach my $f (@FILE) {
  push @fil,$JNM{$f} if not inar \@fil,$JNM{$f}; }
my $fil;
$fil .= "$_, " foreach @fil;
$fil =~ s/, $//;

# topologies
my @top;
foreach my $f (@FILE) {
  my $t = $TOP{$JRF{$f}};
  push @top,$t if not inar \@top,$t; }
my $top;
$top .= "$_, " foreach @top;
$top =~ s/, $//;

# parameters
my @gam;
foreach my $f (@FILE) {
  my $g = $GAM{$JRF{$f}};
  push @gam,$g if not inar \@gam,$g; }
my $gam;
$gam .= "$_, " foreach @gam;
$gam =~ s/, $//;
if   (scalar(@gam) == 0) { $gam = ""; }
elsif(scalar(@gam) == 1) { $gam = "γ=$gam"; }
else			 { $gam = "γ: $gam"; }

# allmost full title string
our $TITLE = "$req for $fil $top $gam";

# which-set prefix
my $tpre;
if   (not $XTEST1 and not $XTEST2) { $tpre = "training-set"; }
elsif(not $XTRAIN and not $XTEST1) { $tpre = "test2-set"; }
elsif(not $XTRAIN and not $XTEST2) { $tpre = "test-set"; }
elsif(not $XTRAIN)		   { $tpre = "test-sets"; }
$TITLE = "$tpre $TITLE" if defined $tpre;

printf STDERR "%10s: $TITLE\n","title";

# ----------------------------------------------------------------- PLOT LINES GENERATOR

sub plox {
  my $files = $_[0];

  my $main = 0; # whether this is a main (color) block
  $main = 1 if $files == \@TRAIN and not $XTEST1 and not $XTEST2;
  $main = 1 if $files == \@TEST2 and not $XTEST1;
  $main = 1 if $files == \@TEST1;

  my $ls = 4; # linestyle
  $ls = 1 if $files == \@TRAIN and not $main;
  $ls = 2 if $files == \@TEST2 and not $main;

  my $ncols = scalar @COLS;
  my $line = "";
  foreach my $h (@COLS) {
    foreach my $f (@{$files}) {

      # column
      my $c = $RNO{$f}{$h};
      next if not defined $c;

      # dashed
      my $dt = "";
      $dt = " dt '-'" if $ncols>1 and $h ne $COLS[0];

      # title name
      my $tn = $JNM{$f};

      # variable name
      my $tv = $CNM{$h};

      # complete title
      my $t = "";
      $t = "$tn $tv" if $main;
      $t = "$tn $tv" if $files == \@OTHER and $h eq $COLS[0];

      $line .= "     '$TNM{$f}' u 1:$c w l ls $ls$dt t '$t', \\\n"; }

    # highlight the last line
    $line =~ s/ ls 4 (.*?)$/ ls 3 $1/; }

  $line =~ s/''/'train'/ if $files == \@TRAIN and not $main;
  $line =~ s/''/'test2'/ if $files == \@TEST2 and not $main;
  return $line; }

# ----------------------------------------------------------------------------- WRITE GP
# include "erplot.gp.pl"

$ERPLOT_OLD = `cat $TMP/erplot.gp` if -f "$TMP/erplot.gp";

$ERPLOT =~ s/\n\h*\#.*//g;
$ERPLOT =~ s/\n\n\n+/\n\n/g;

$ERPLOT =~ s/__DIR__/$TMP/g;
$ERPLOT =~ s/__TITLE__/$TITLE/g;
$ERPLOT =~ s/__WIDTH__/$WIDTH/g;
$ERPLOT =~ s/__HEIGHT__/$HEIGHT/g;

$ymax = $YMAX * 1.1;
$ymin = $YMIN * 0.9;

$ERPLOT =~ s/__YMIN__/$ymin/g;
$ERPLOT =~ s/__YMAX__/$ymax/g;

$plot0 = plox \@TRAIN if $XTRAIN;
$plot1 = plox \@TEST1 if $XTEST1;
$plot2 = plox \@TEST2 if $XTEST2;
$plot3 = plox \@OTHER;

$plot = $plot0.$plot2.$plot1.$plot3;
$plot =~ s/^    /plot/;
$plot =~ s/, \\\n$//;

$ERPLOT =~ s/__PLOT__/$plot/;

writefile "$TMP/erplot.gp",$ERPLOT if $ERPLOT ne $ERPLOT_OLD;

# --------------------------------------------------------------------------------- PLOT

sys "LC_ALL='en_US.UTF-8' gnuplot $TMP/erplot.gp","gnuplot $TMP/erplot.gp";
print STDERR "\n";

# --------------------------------------------------------------------------------------
