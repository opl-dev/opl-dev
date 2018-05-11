# RSG2OSG.PL
#
# Copyright (c) 2000-2002 Symbian Ltd. All rights reserved.
#
# Converts Symbian OS Resource Compiler 'RSG' output file to
# a format suitable for inclusion in OPL programs
#
# Version 1.00(010) - 1 January 2002

use Cwd;

sub print_usage
	{
	print <<USAGE_EOF;

Usage:
  rsg2osg srcfile

Convert Symbian OS Resource Compiler output file (.RSG) to an OPL INCLUDE
file (.OSG)

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
($name,$path,$suffix) = fileparse($sourcefile,"\.rsg");
$targetfile = $path . $name . ".tsg";

#--------------------------------------------------------
# Process the RSG, replacing the C++ text with OPL stuff
#--------------------------------------------------------
open INFILE,  "$sourcefile" or die "* Can't open $sourcefile";
open OUTFILE, ">$targetfile" or die "* Can't write to $targetfile";
while (<INFILE>){
	
	s/#define /CONST / ;
	s/  /&/ ;			# replace first double space with &
					# this makes the variable names long
					# ints for when they're included by OPL
	s/0x/=&/ ;

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