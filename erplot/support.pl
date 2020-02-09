
$CR_="\033[31m"; # color red
$CG_="\033[32m"; # color green
$CM_="\033[35m"; # color magenta
$CC_="\033[36m"; # color cyan
$CW_="\033[37m"; # color white
$CK_="\033[90m"; # color black
$CD_="\033[0m";  # color default

# inar(\@a,$s) - check whether the string is in the array
sub inar {
  my $a=$_[0]; # array ref
  my $s=$_[1]; # string
  foreach(@{$a}) { return 1 if $_ eq $s; }
  return 0; }

# return mtime of file (the last modification time)
sub getmtime {
  my $file=$_[0];
  my $t=0;
  if(-e $file) {
    $t=(stat($file))[9]; }
  return $t; }
sub getctime {
  my $file=$_[0];
  my $t=0;
  if(-e $file) {
    $t=(stat($file))[10]; }
  return $t; }

# check if file f1 is newer then f2
sub newer {
  my $f1=$_[0]; return 0 if not -f $f1; # always none < f2 => 0 
  my $f2=$_[1]; return 1 if not -f $f2; # always f1 > none => 1
  my $t1=getmtime($_[0]);
  my $t2=getmtime($_[1]);
  return 1 if $t1>$t2;
  return 0; }

# check if directory d1 is newer then d2
sub newerdir {
  my $d1=$_[0]; return 0 if not -d $d1; # always none < d2 => 0 
  my $d2=$_[1]; return 1 if not -d $d2; # always d1 > none => 1
  my $t1=getmtime($_[0]);
  my $t2=getmtime($_[1]);
  return 1 if $t1>$t2;
  return 0; }

# just run command
sub sys {
  my $cmd = $_[0];
  my $cmds = $cmd; $cmds = $_[1] if defined $_[1];
  print "$CM_$cmds$CD_\n";
  system $cmd; }
sub sys_silent {
  my $cmd = $_[0];
  system $cmd; }

# run command and return its stdout output
sub cmd {
  my $cmd = $_[0];
  print "$CM_$cmd$CD_\n";
  my $out = `$cmd`;
  return $out; }
sub cmd_silent {
  my $cmd = $_[0];
  my $out = `$cmd`;
  return $out; }

# write file from string
sub writefile {
  my $file = $_[0];
  my $s = $_[1];
  print "${CM_}write $file$CD_\n";
  open(O,">$file") or die "Can't create file $file ($!).";
  print O $s;
  close(O); }

