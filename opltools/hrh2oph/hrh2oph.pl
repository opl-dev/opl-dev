# HRH2OPH.PL
#
# Copyright (c) 2000-2004 Symbian Software Ltd. All rights reserved.
#
# Converts Symbian OS Help Compiler '.HRH' output files to
# a format suitable for inclusion in OPL programs ('.OPH')
#
# Version 1.00(004) - 18 June 2004

use Cwd;

sub print_usage
	{
	print <<USAGE_EOF;

Usage:
  hrh2oph srcfile

Convert Symbian OS Help Compiler output file (.HRH) to an OPL INCLUDE
file (.OPH)

USAGE_EOF
	}

#-------------------------------------------------------
# Process commandline arguments
#-------------------------------------------------------
my $sourcefile="";
my $targetfile="";
my $opltran_spec="opltran -conv ";
my $errors = 0;

while (@ARGV)
	{
	my $arg = shift @ARGV;
	if ($arg =~ /^-/)
		{
		print "Unknown arg: $arg\n";
		$errors++;
		next;
		}
	$sourcefile=$arg;
	}

if ($errors || $sourcefile eq "")
	{
	print_usage();
	exit 1;
	}

use File::Basename;
($name,$path,$suffix) = fileparse($sourcefile,"\.hrh");
$targetfile = $path . $name . ".tph";

#--------------------------------------------------------
# Process the HRH, replacing the C++ text with OPL stuff
#--------------------------------------------------------
open INFILE,  "$sourcefile" or die "* Can't open $sourcefile";
open OUTFILE, ">$targetfile" or die "* Can't write to $targetfile";

my $find1 = "_LIT\\(K";
my $replace1 = "CONST K"; 
my $find2 = ",\"";
my $replace2 = "\$=\"";
my $find3 = "\"\\)\;";
my $replace3 = "\"";

while (<INFILE>){
	
	s/$find1/$replace1/;	# replace the _LIT defines with CONSTs
	s/$find2/$replace2/;	# replace the start of _LIT defines with OPL string assignment syntax
	s/$find3/$replace3/;	# replace the end of _LIT defines with closure of OPL string assignment
	s!// !rem !;		# replace "// etc." comments with "rem etc."
	s!//!rem !;		# replace "//etc." comments with "rem etc.
	s!\#!rem \#!;		# comment out lines beginning # (i.e. C++ pre-processor lines)

	print OUTFILE "$_";
	
	}
close INFILE;
close OUTFILE;

#-------------------------------------------------------
# Run OPLTRAN to convert the file
#-------------------------------------------------------
$opltran_spec .= "$targetfile -e -q";

system($opltran_spec);
if ($? != 0)
	{
	print "* OPLTRAN failed\n";
	exit 1;
	}

exit 0;