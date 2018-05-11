# NewOPXVersion.pl
#
# Copyright (c) 2000-2002 Symbian Ltd. All rights reserved.
#
# Re-writes all OPX PKG files with a new version number
#
# Version 1.00(005) - 20 January 2002

use Cwd;

sub print_usage
	{
	print <<USAGE_EOF;

NewOPXVersion V1.00 (Build 005, 20 January 2002)
Copyright (c) 2000-2002 Symbian Ltd.

Usage:
  NewOPXVersion srcfile [old_build new_build] | [old_minor old_build new_minor new_build] | [old_major old_minor old_build new_major new_minor new_build]

Re-writes OPX PKG files with a new version number. Writing is done up to the
TYPE= argument in the PKG file, allowing greater combinations of version
information to be written. Example usage:

  -Build number only:	 NewOPXVersion MyOPX.pkg 114 115
  -Build & minor number: NewOPXVersion MyOPX.pkg "20 114" "30 115"
  -Full version:	 NewOPXVersion MyOPX.pkg "0 20 114" "1 00 115"

Quote marks as used above are optional.

USAGE_EOF
	}

#-------------------------------------------------------
# Process commandline arguments
#-------------------------------------------------------
my $sourcefile="";
my $targetfile="";
my $oldversion="";
my $newversion="";
my $show_debug=""; # Set this to "On" for on, anything else for off

if (@ARGV<3)
	{
	print_usage();
	exit 1;
	}

$sourcefile=$ARGV[0];
$oldversion=$ARGV[1];
if ($ARGV[4]!="")					# we're in at least build & minor mode
	{
	if ($ARGV[6]!="")				# we're in full version mode
		{
		$oldversion .= ", " . $ARGV[2] . ", " . $ARGV[3];
		$newversion = $ARGV[4] . ", " . $ARGV[5] . ", " . $ARGV[6];
		}
	else
		{
		$oldversion .= ", " . $ARGV[2];
		$newversion = $ARGV[3] . ", " . $ARGV[4];
		}
	}
else
	{
	$newversion=$ARGV[2];
	}

if ($sourcefile eq "" || $oldversion eq "" || $newversion eq "")
	{
	print_usage();
	exit 1;
	}

use File::Basename;
($name,$path,$suffix) = fileparse($sourcefile,"\.pkg");
$targetfile = $path . $name . ".pkg2";

$oldversion .= ", TYPE";
$newversion .= ", TYPE";

if ($show_debug eq "On")
	{
	print <<DEBUG_EOF;
Source File:	$sourcefile
Old Version:	$oldversion
New Version:	$newversion

DEBUG_EOF
	}

#------------------------------------------------------------
# Process the PKG, replacing the old version with the new one
#------------------------------------------------------------
open INFILE,  "$sourcefile" or die "* Can't open $sourcefile";
open OUTFILE, ">$targetfile" or die "* Can't write to $targetfile";
while (<INFILE>){
	
	s/$oldversion/$newversion/ ;

	print OUTFILE "$_";
	
	}
close INFILE;
close OUTFILE;

#-------------------------------------------
# Now replace the old file with the new copy
#-------------------------------------------
use File::Copy;
move("$targetfile","$sourcefile");

exit 0;