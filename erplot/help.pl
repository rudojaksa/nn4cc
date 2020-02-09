# ------------------------------------------------------------------------------------ HELP

sub printhelp {
  my $help = $_[0];

  # commented-out
  $help =~ s/(\n\#.*)*\n/\n/g;

  if($SUBVERSION ne "none") {
    $SUB = $SUBVERSION;
    $BLT = " $CK_(built $MKDATE on $MKHOST)$CD_"; }

  $help .= "VERSION\n";
  $help .= "    $PACKAGE-$VERSION$SUB $COPYLEFT$BLT\n\n";

  # CC(text)
  my %REX; my $i=0; $REX{$i++} = $3 while $help =~ s/([^A-Z0-9])(C[CWD])\((.+?)\)/${1}__c$2${i}__/;

  # TODO: use push array to avoid being overwritten later
  $help =~ s/(\n[ ]*)(-[a-zA-Z0-9]+(\[?[ =][A-Z]{2,}(x[A-Z]{2,})?\]?)?)([ \t])/$1$CC_$2$CD_$5/g;

  $help =~ s/\[([+-])?([A-Z]+)\]/\[$1$CC_$2$CD_\]/g;
  $help =~ s/(\n|[ \t])(([A-Z_\/-]+[ ]?){4,})/$1$CC_$2$CD_/g;

  # CC(text)
  $help =~ s/__cCC([0-9]+)__/$CC_$REX{$1}$CD_/g;
  $help =~ s/__cCW([0-9]+)__/$CW_$REX{$1}$CD_/g;
  $help =~ s/__cCD([0-9]+)__/$CD_$REX{$1}$CD_/g;

  print $help; }

# -----------------------------------------------------------------------------------------
