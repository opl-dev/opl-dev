# BLDMAKE.PL
#
# Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
#


# all variables called *Path* are set up to end with a backslash
# all variables called *Path or *File are stored as absolute (file)paths within makmake
# all variables called UpPath* are stored as relative paths within makmake


use FindBin;		# for FindBin::Bin
use Getopt::Long;

my $PerlLibPath;    # fully qualified pathname of the directory containing our Perl modules

BEGIN {
# check user has a version of perl that will cope
	require 5.005_03;
# establish the path to the Perl libraries: currently the same directory as this script
	$PerlLibPath = $FindBin::Bin;	# X:/epoc32/tools
	$PerlLibPath =~ s/\//\\/g;	# X:\epoc32\tools
	$PerlLibPath .= "\\";
}

use lib $PerlLibPath;
use E32env;
use E32Plat;
use Modload;
use Output;
use Pathutl;
use E32Variant;

my %Options;
my %KeepGoing;

my $variantMacroHRHFile = Variant_GetMacroHRHFile();

# THE MAIN PROGRAM SECTION
##########################

{
	Load_SetModulePath($PerlLibPath);
	Plat_Init($PerlLibPath);

#	process the commmand-line
	unless (GetOptions(\%Options, 'v', "k|keepgoing")) {
		exit 1;
	}
	unless (@ARGV>=1) {
		&Usage();
	}
	my $Command=uc shift @ARGV;
	unless ($Command=~/^(BLDFILES|CLEAN|INF|PLAT)$/o) {
		&Usage();
	}
	my $CLPlat=uc shift @ARGV;

	unless ($CLPlat) {
		$CLPlat='ALL';
	}

	if ($Command eq 'INF') {
		&ShowBldInfSyntax();
		exit;
	}

	if ($Command eq 'PLAT') {
		my @PlatList = ($CLPlat);
		my $PlatName;
		if ($CLPlat eq "ALL") {
			@PlatList = &Plat_List();
			print(
				"Supported Platforms:\n",
				"  @PlatList\n\n"
			);
		}
		print(
			"Macros defined for BLD.INF preprocessing of MMPFILE sections:\n"
		);
		foreach $PlatName (@PlatList) {
			my %Plat;
			eval { &Plat_GetL($PlatName, \%Plat); };
			die $@ if $@;
			print(
				"\nPlatform $PlatName:\n",
				"  @{$Plat{MmpMacros}}\n"
			);
		}
		exit;
	}


#	check that the BLD.INF file exists
#	maybe BLDMAKE should allow a path to be specified leading to the BLD.INF file
	my $BldInfPath=&Path_WorkPath;
	unless (-e "${BldInfPath}BLD.INF") {
		&FatalError("Can't find \"${BldInfPath}BLD.INF\"");
	}

	if (!-d $E32env::Data{EPOCPath}){
		&FatalError("Directory \"$E32env::Data{EPOCPath}\" does not exist");
	}

#	decide the output directory
	my $OutDir=&Path_Chop($E32env::Data{BldPath}).$BldInfPath;

#	Work out the path for the IBY files
	my $RomDir=&Path_Chop($E32env::Data{RomPath}).$BldInfPath;

#	Work out the name for the BLD.INF module
	my @Dirs=&Path_Dirs($BldInfPath);
	my $Module='';
	foreach (@Dirs) {
		$Module.="$_.";
	}
	chop $Module;


	if ($Command eq 'CLEAN') {
		unlink "${BldInfPath}ABLD.BAT";
		$OutDir=~m-(.*)\\-o;
		if (-d $1) { # remove backslash for test because some old versions of perl can't cope
			opendir DIR, $1;
			my @Files=grep s/^([^\.].*)$/$OutDir$1/, readdir DIR;
			closedir DIR;
			unlink @Files;
		}
		exit;
	}

#	parse BLD.INF - to get the platforms and the export files
	eval { &Load_ModuleL('PREPFILE'); };
	&FatalError($@) if $@;

	my @RealPlats=();
	my @Exports=();
	my @TestExports=();
	if ($Options{v}) {
		print "Reading \"${BldInfPath}BLD.INF\" for platforms and exports\n";
	}
	&ParseBldInf(\@RealPlats, \@Exports, \@TestExports, $BldInfPath, 
		$E32env::Data{EPOCIncPath}, $E32env::Data{EPOCPath});
	
#	get any IDE platforms required into a new platforms list, and
#	Create a hash to contain the 'real' names of the platforms, i.e. WINS rather than VC6
	my @Plats=@RealPlats;
	my %Real;
	foreach (@RealPlats) { # hack to get VC6 batch files
		$Real{$_}=$_;
		my $AssocIDE;
		next unless $AssocIDE=&Plat_AssocIDE($_);
		push @Plats, $AssocIDE;
		$Real{$AssocIDE}=$Real{$_};
		
	}
	# a hack for VC7
	# rick push @Plats, "VC7";
	$Real{"VC7"}=$Real{"WINS"};
	
	if ($Options{v}) {
		print "Platforms: \"@Plats\"\n";
	}

#	check that the platform specified on the command-line is acceptable
#	and sort out a list of platforms to process
	my @DoRealPlats=@RealPlats;
	my @DoPlats=@Plats;
	unless ($CLPlat eq 'ALL') {
		unless (grep /^$CLPlat$/, @Plats) {
			&FatalError("Platform $CLPlat not supported by \"${BldInfPath}BLD.INF\"\n");
		}
		@DoPlats=($CLPlat);
		@DoRealPlats=$Real{$CLPlat};
	}
	undef $CLPlat;
			
#	sort out the export directories we might need to make
	my @ExportDirs=ExportDirs(\@Exports);
	my @TestExportDirs=ExportDirs(\@TestExports);

#	parse the BLD.INF file again for each platform supported by the project
#	storing the information in a big data structure
	my %AllPlatData;
	my %AllPlatTestData;
	my $Plat;
	foreach $Plat (@RealPlats) {
		if ($Options{v}) {
			print "Reading \"${BldInfPath}BLD.INF\" for $Plat\n";
		}
		my (@PlatData, @PlatTestData);
		&ParseBldInfPlat(\@PlatData, \@PlatTestData, $Plat, $BldInfPath);
		$AllPlatData{$Plat}=\@PlatData;
		$AllPlatTestData{$Plat}=\@PlatTestData;
	}
	undef $Plat;

	if ($Command eq 'BLDFILES') {

#		create the perl file, PLATFORM.PM, listing the platforms
		if ($Options{v}) {
			print "Creating \"${OutDir}PLATFORM.PM\"\n";
		}
		&CreatePlatformPm($OutDir, \@Plats, \@RealPlats, \%Real, \%AllPlatData, \%AllPlatTestData);

#		create the .BAT files required to call ABLD.PL
		if ($Options{v}) {
			print "Creating \"${BldInfPath}ABLD.BAT\"\n";
		}
		&CreatePerlBat($BldInfPath);

#		create the makefile for exporting files
		if ($Options{v}) {
			print "Creating \"${OutDir}EXPORT.MAKE\"\n";
		}
		&CreateExportMak("${OutDir}EXPORT.MAKE", \@Exports, \@ExportDirs);

#		create the makefile for exporting test files
		if ($Options{v}) {
			print "Creating \"${OutDir}EXPORTTEST.MAKE\"\n";
		}
		&CreateExportMak("${OutDir}EXPORTTEST.MAKE", \@TestExports, \@TestExportDirs);

#		create the platform meta-makefiles
		foreach (@DoPlats) {
			if ($Options{v}) {
				print "Creating \"$OutDir$_.MAKE\"\n";
			}
			&CreatePlatMak($OutDir, $E32env::Data{BldPath}, $AllPlatData{$Real{$_}}, $_, $Real{$_}, $RomDir, $Module, $BldInfPath);
		}
		foreach (@DoPlats) {
			if ($Options{v}) {
				print "Creating \"$OutDir${_}TEST.MAKE\"\n";
			}
			&CreatePlatMak($OutDir, $E32env::Data{BldPath}, $AllPlatTestData{$Real{$_}}, $_, $Real{$_}, $RomDir, $Module, $BldInfPath, 'TEST');
		}

#		create the platform test batch files
		foreach (@DoRealPlats) {
			if ($Options{v}) {
				print "Creating test batch files in \"$OutDir\" for $_\n";
			}
			&CreatePlatBatches($OutDir, $AllPlatTestData{$_}, $_);
		}

#		report any near-fatal errors
		if (scalar keys %KeepGoing) {
		    print STDERR
			    "\n${BldInfPath}BLD.INF WARNING(S):\n",
			    sort keys %KeepGoing
			    ;
		}

		exit;
	}
}


################ END OF MAIN PROGRAM SECTION #################
#------------------------------------------------------------#
##############################################################


# SUBROUTINE SECTION
####################

sub Usage () {

	eval { &Load_ModuleL('E32TPVER'); };
	&FatalError($@) if $@;

	print
		"\n",
		"BLDMAKE - Project building Utility (Build ",&E32tpver,")\n",
		"\n",
		"BLDMAKE {options} [<command>] [<platform>]\n",
		"\n",
		"<command>: (case insensitive)\n",
		" BLDFILES - create build batch files\n",
		" CLEAN    - remove all files bldmake creates\n",
		" INF      - display basic BLD.INF syntax\n",
		" PLAT     - display platform macros\n",
		"\n",
		"<platform>: (case insensitive)\n",
		"  if not specified, defaults to \"ALL\"\n",
		"\n",
		"Options: (case insensitive)\n",
		" -v   ->  verbose mode\n",
		" -k   ->  keep going even if files are missing\n"
	;
	exit 1;
}

sub ShowBldInfSyntax () {

	print <<ENDHERE;

BLD.INF - Syntax

/* Use C++ comments if required */
// (Curly braces denote optional arguments)

PRJ_PLATFORMS
{DEFAULT} {-<platform> ...} {<list of platforms>}
// list platforms your project supports here if not default
// default = WINS ARMI ARM4 THUMB WINSCW

PRJ_EXPORTS
[<source path>\<source file>]	{<destination>}
// list each file exported from source on a separate line
// {<destination>} defaults to \\EPOC32\\Include\\<source file>

PRJ_TESTEXPORTS
[<source path>\<source file>]	{<destination>}
// list each file exported from source on a separate line
// {<destination>} defaults to BLD.INF dir

PRJ_MMPFILES
[<mmp path>\<mmp file>] {tidy} {ignore}
{MAKEFILE|NMAKEFILE} [<path>\<makefile>] {tidy} {ignore}

#if defined(<platform>)
// .MMP statements restricted to <platform>
#endif

PRJ_TESTMMPFILES
[<mmp path>\<mmp file>] {tidy} {ignore} {manual} {support}
{MAKEFILE|NMAKEFILE} [<path>\<makefile>] {tidy} {ignore} {manual} {support}

#if defined(<platform>)
// .MMP statements restricted to <platform>
#endif

ENDHERE

}

sub WarnOrDie ($$) {
	my ($dieref, $message) = @_;
	if ($Options{k}) {
		$KeepGoing{$message} = 1;
	} else {
		push @{$dieref}, $message;
	}
}

sub ParseBldInf ($$$$$) {
	my ($PlatsRef, $ExportsRef, $TestExportsRef, $BldInfPath, $EPOCIncPath, $EPOCPath)=@_;

	my @Prj2D;
	eval { &Prepfile_ProcessL(\@Prj2D, "${BldInfPath}BLD.INF",$variantMacroHRHFile); };
	&FatalError($@) if $@;
	
	my @SupportedPlats=&Plat_List();

	my @DefaultPlats=('WINS', 'ARMI', 'ARM4', 'THUMB');

	my @Plats;
	my %RemovePlats;

	my $DefaultPlatsUsed=0;
	my %PlatformCheck;

	my %ExportCheck;
	my $Section=0;
	my @PrjFileDie;
	my $Line;
	my $CurFile="${BldInfPath}BLD.INF";
	LINE: foreach $Line (@Prj2D) {
		my $LineNum=shift @$Line;
		$_=shift @$Line;
		if ($LineNum eq '#') {
			$CurFile=$_;
			next LINE;
		}
		if (/^PRJ_(\w*)$/io) {
			$Section=uc $1;
			if ($Section=~/^(PLATFORMS|EXPORTS|TESTEXPORTS|MMPFILES|TESTMMPFILES)$/o) {
				if (@$Line) {
					push @PrjFileDie, "$CurFile($LineNum) : Can't specify anything on the same line as a section header\n";
				}
				next LINE;
			}
			push @PrjFileDie, "$CurFile($LineNum) : Unknown section header - $_\n";
			$Section=0;
			next LINE;
		}
		if ($Section eq 'PLATFORMS') {
#			platforms are gathered up into a big list that contains no duplicates.  "DEFAULT" is
#			expanded to the list of default platforms.  Platforms specified with a "-" prefix
#			are scheduled for removal from the list.  After processing platforms specified
#			with the "-" prefix are removed from the list.

			unshift @$Line, $_;
			my $Candidate;
			CANDLOOP: foreach $Candidate (@$Line) {
				$Candidate=uc $Candidate;
#				expand DEFAULT
				if ($Candidate eq 'DEFAULT') {
					$DefaultPlatsUsed=1;
					my $Default;
					foreach $Default (@DefaultPlats) {
						unless ($PlatformCheck{$Default}) {
							push @Plats, $Default;
							$PlatformCheck{$Default}="$CurFile: $LineNum";
						}
					}
					next CANDLOOP;
				}
#				check for removals
				if ($Candidate=~/^-(.*)$/o) {
					$Candidate=$1;
#					check default is specified
					unless ($DefaultPlatsUsed) {
						push @PrjFileDie, "$CurFile($LineNum) : \"DEFAULT\" must be specified before platform to be removed\n";
						next CANDLOOP;
					}
#					check platform is in the default list
					unless (grep /^$Candidate$/, @DefaultPlats) {
						push @PrjFileDie, "$CurFile($LineNum) : Platform $Candidate specified for removal is not in DEFAULT list\n";
						next CANDLOOP;
					}
					$RemovePlats{$Candidate}=1;
					next CANDLOOP;
				}
# 				cwtools hack: If tools platform is specified in bld.inf file then component is built for cwtools as well 
				if ($Candidate =~ /^tools/i)
						{
						push @Plats, 'CWTOOLS';
						}
#				check platform is supported

				unless (grep /^$Candidate$/, @SupportedPlats) {
					WarnOrDie(\@PrjFileDie, "$CurFile($LineNum) : Unsupported platform $Candidate specified\n");
					next CANDLOOP;
				}
#				check platform is not an IDE
				if ($Candidate=~/^VC/o) {
					push @PrjFileDie, "$CurFile($LineNum) : No need to specify platform $Candidate here\n";
					next CANDLOOP;
				}
#				add the platform
				unless ($PlatformCheck{$Candidate}) {
					push @Plats, $Candidate;
					$PlatformCheck{$Candidate}="$CurFile: $LineNum";
				}
			}
			next LINE;
		}
		if ($Section=~/^(EXPORTS|TESTEXPORTS)$/o) {

#			make path absolute - assume relative to group directory
			my $Source=&Path_MakeAbs($CurFile, $_);
			my $Releasable='';
			if (@$Line) {
#				get the destination file if it's specified
				$Releasable=shift @$Line;
			}
			if (@$Line) {
				push @PrjFileDie, "$CurFile($LineNum) : Too many arguments in exports section line\n";
				next LINE;
			}
			unless (&Path_Split('File', $Releasable)) {
#				use the source filename if no filename is specified in the destination
				$Releasable.=&Path_Split('File', $Source);
			}
			if ($Section eq 'EXPORTS') {
#				assume the destination is relative to $EPOCIncPath
				$Releasable=&Path_MakeEAbs($EPOCPath, $EPOCIncPath, $Releasable);
			}
			else {
				$Releasable=&Path_MakeEAbs($EPOCPath, $CurFile, $Releasable);
			}
#			sanity checks!
			if ($ExportCheck{uc $Releasable}) {
				push @PrjFileDie, "$CurFile($LineNum) : Duplicate export $Releasable (from line $ExportCheck{uc $Releasable})\n";
				next LINE;
			}
			$ExportCheck{uc $Releasable}="$CurFile: $LineNum";
			if (! -e $Source) {
				WarnOrDie(\@PrjFileDie, "$CurFile($LineNum) : Exported source file $Source not found\n");
			}
			else {
				if ($Section eq 'EXPORTS') {
					push @$ExportsRef, {
						'Source'=>$Source,
						'Releasable'=>$Releasable
					};
				}
				else {
					push @$TestExportsRef, {
						'Source'=>$Source,
						'Releasable'=>$Releasable
					};
				}
			}
			next LINE;
		}
	}
	if (@PrjFileDie) {
		print STDERR
			"\n${BldInfPath}BLD.INF FATAL ERROR(S):\n",
			@PrjFileDie
		;
		exit 1;
	}

#	set the list of platforms to the default if there aren't any platforms specified,
#	else add platforms to the global list unless they're scheduled for removal,
	unless (@Plats) {
		@$PlatsRef=@DefaultPlats;
	}
	else {
		my $Plat;
		foreach $Plat (@Plats) {
			unless ($RemovePlats{$Plat}) {
				push @$PlatsRef, $Plat;
			}
		}
	}
}

sub ExportDirs ($) {
	my ($ExportsRef)=@_;

	my %ExportDirHash;
	foreach (@$ExportsRef) {
		my $dir=&Path_Split('Path',$$_{Releasable});
		$ExportDirHash{uc $dir}=$dir;
	}
	my @ExportDirs;
	foreach (keys %ExportDirHash) {
		push @ExportDirs, &Path_Chop($ExportDirHash{$_});
	}
	@ExportDirs;
}


sub ParseBldInfPlat ($$$$) {
	my ($DataRef, $TestDataRef, $Plat, $BldInfPath)=@_;

#	get the platform .MMP macros
	my %Plat;
	eval { &Plat_GetL($Plat,\%Plat); };
	&FatalError($@) if $@;

#	get the raw data from the BLD.INF file
	my @Prj2D;
	eval { &Prepfile_ProcessL(\@Prj2D, "${BldInfPath}BLD.INF", $variantMacroHRHFile, @{$Plat{MmpMacros}}); };
	&FatalError($@) if $@;
	
#	process the raw data
	my %Check;
	my $Section=0;
	my @PrjFileDie;
	my $Line;
	my $CurFile="${BldInfPath}BLD.INF";
	LINE: foreach $Line (@Prj2D) {

		my %Data;
		my %Temp;

		my $LineNum=shift @$Line;
		if ($LineNum eq '#') {
			$CurFile=shift @$Line;
			next LINE;
		}
		
#		upper-case all the data here
		foreach (@$Line) {
			$_=uc $_;
		}

		$_=shift @$Line;

#		check for section headers - don't test for the right ones here
#		because we do that in the first parse function

		if (/^PRJ_(\w*)$/o) {
			$Section=$1;
			next LINE;
		}

#		check for the sections we're interested in and get the .MMP file details
		if ($Section=~/^(MMPFILES|TESTMMPFILES)$/o) {

			$Data{Ext}='.MMP';

#			check for MAKEFILE statements for custom building
			if (/^MAKEFILE$/o) {
				$Data{Makefile}=2;  # hack - treat MAKEFILE=>NMAKEFILE   =1;
				$_=shift @$Line;
				$Data{Ext}=&Path_Split('Ext', $_);
			}
			if (/^NMAKEFILE$/o) {
				$Data{Makefile}=2;
				$_=shift @$Line;
				$Data{Ext}=&Path_Split('Ext', $_);
			}
			if (/^GNUMAKEFILE$/o) {
				$Data{Makefile}=1;
				$_=shift @$Line;
				$Data{Ext}=&Path_Split('Ext', $_);
			}
			
#			path considered relative to the current file
			$Data{Path}=&Path_Split('Path', &Path_MakeAbs($CurFile, $_));

#			this function doesn't care whether the .MMPs are listed with their extensions or not
			$Data{Base}=&Path_Split('Base', $_);

#			check the file isn't already specified
			if ($Check{$Data{Base}}) {
				push @PrjFileDie, "$CurFile($LineNum) : duplicate $Data{Base} (from line $Check{$Data{Base}})\n";
				next;
			}
			$Check{$Data{Base}}="$CurFile: $LineNum";

#			check the file exists
			unless (-e "$Data{Path}$Data{Base}$Data{Ext}") {
				WarnOrDie(\@PrjFileDie, "$CurFile($LineNum) : $Data{Path}$Data{Base}$Data{Ext} does not exist\n");
				next LINE;
			}

#			process the file's attributes
			if ($Section eq 'MMPFILES') {
				foreach (@$Line) {
					if (/^TIDY$/o) {
						$Data{Tidy}=1;
						next;
					}
					if (/^IGNORE$/o) {
						next LINE;
					}
					push @PrjFileDie, "$CurFile($LineNum) : Don't understand .MMP file argument \"$_\"\n";
				}
			}

#			process the test .MMP file's attributes
			elsif ($Section eq 'TESTMMPFILES') {
				foreach (@$Line) {
					if (/^TIDY$/o) {
						$Data{Tidy}=1;
						next;
					}
					if (/^IGNORE$/o) {
						next LINE;
					}
					if (/^MANUAL$/o) {
						$Data{Manual}=1;
						next;
					}
					if (/^SUPPORT$/o) {
						$Data{Support}=1;
						next;
					}
					push @PrjFileDie, "$CurFile($LineNum) : Don't understand test .MMP file argument \"$_\"\n";
				}
			}


#			store the data
			if ($Section eq 'MMPFILES') {
				push @$DataRef, \%Data;
				next LINE;
			}
			if ($Section eq 'TESTMMPFILES') {
				push @$TestDataRef, \%Data;
				next LINE;
			}
		}
	}
#	line loop end

#	exit if there are errors
	if (@PrjFileDie) {
		print STDERR
			"\n\"${BldInfPath}BLD.INF\" FATAL ERROR(S):\n",
			@PrjFileDie
		;
		exit 1;
	}
}


sub FatalError (@) {

	print STDERR "BLDMAKE ERROR: @_\n";
	exit 1;
}

sub CreatePlatformPm ($$$$$$) {
	my ($BatchPath, $PlatsRef, $RealPlatsRef, $RealHRef, $AllPlatDataHRef, $AllPlatTestDataHRef)=@_;
# 	cwtools hack: excludes CWTOOLS from list of RealPlats
	my @RealPlats;
	foreach my $Plat (@$RealPlatsRef){
	unless ($Plat =~ /^cwtools/i){
			push @RealPlats, $Plat;
		}
	}

	&Output(
		"# Bldmake-generated perl file - PLATFORM.PM\n",
		"\n",
		"# use a perl integrity checker\n",
		"use strict;\n",
		"\n",
		"package Platform;\n",
		"\n",
		"use vars qw(\@Plats \@RealPlats %Programs %TestPrograms);\n",
		"\n",
		"\@Plats=qw(@$PlatsRef);\n",
		"\n",
		"\@RealPlats=qw(@RealPlats);\n",
		"\n",
		"%Programs=(\n"
	);
	my %All; # all programs for all platforms
	my $TmpStr;
	my $Plat;
	foreach $Plat (@$PlatsRef) {
		$TmpStr="	$Plat=>[";
		if (@{${$AllPlatDataHRef}{$$RealHRef{$Plat}}}) {
			my $ProgRef;
			foreach $ProgRef (@{${$AllPlatDataHRef}{$$RealHRef{$Plat}}}) {
				$TmpStr.="'$$ProgRef{Base}',";
				$All{$$ProgRef{Base}}=1;
			}
			chop $TmpStr;
		}
		&Output(
			"$TmpStr],\n"
		);
	}
	$TmpStr="	ALL=>[";
	if (keys %All) {
		my $Prog;
		foreach $Prog (keys %All) {
			$TmpStr.="'$Prog',";
		}
		chop $TmpStr;
	}
	&Output(
		"$TmpStr]\n",
		");\n",
		"\n",
		"%TestPrograms=(\n"
	);
	%All=();
	foreach $Plat (@$PlatsRef) {
		$TmpStr="	$Plat=>[";
		if (@{${$AllPlatTestDataHRef}{$$RealHRef{$Plat}}}) {
			my $ProgRef;
			foreach $ProgRef (@{${$AllPlatTestDataHRef}{$$RealHRef{$Plat}}}) {
				$TmpStr.="'$$ProgRef{Base}',";
				$All{$$ProgRef{Base}}=1;
			}
			chop $TmpStr;
		}
		&Output(
			"$TmpStr],\n"
		);
	}
	$TmpStr="	ALL=>[";
	if (keys %All) {
		my $Prog;
		foreach $Prog (keys %All) {
			$TmpStr.="'$Prog',";
		}
		chop $TmpStr;
	}
	&Output(
		"$TmpStr]\n",
		");\n",
		"\n",
		"1;\n"
	);

#	write the PLATFORM.PM file
	&WriteOutFileL($BatchPath."PLATFORM.PM");
}

sub CreatePerlBat ($) {
	my ($BldInfPath)=@_;

#	create ABLD.BAT, which will call ABLD.PL
#   NB. must quote $BldInfPath because it may contain spaces, but we know it definitely
#       ends with \ so we need to generate "\foo\bar\\" to avoid quoting the close double quote
	&Output(
		"\@ECHO OFF\n",
		"\n", 
		"REM Bldmake-generated batch file - ABLD.BAT\n",
		"REM ** DO NOT EDIT **", 
		"\n",
		"\n",
		"perl -S ABLD.PL \"${BldInfPath}\\\" %1 %2 %3 %4 %5 %6 %7 %8 %9\n",
		"if errorlevel==1 goto CheckPerl\n",
		"goto End\n",
		"\n",
		":CheckPerl\n",
		"perl -v >NUL\n",
		"if errorlevel==1 echo Is Perl, version 5.003_07 or later, installed?\n",
		"goto End\n",
		"\n",
		":End\n"
	);

#	check that the .BAT file does not already exist and is read-only
	if ((-e "${BldInfPath}ABLD.BAT")  && !(-w "${BldInfPath}ABLD.BAT")) {
		warn "BLDMAKE WARNING: read-only ABLD.BAT will be overwritten\n";
		chmod 0222, "${BldInfPath}ABLD.BAT";
	}

#	create the .BAT file in the group directory
	&WriteOutFileL($BldInfPath."ABLD.BAT",1);

}

sub CreateExportMak ($$$) {
	my ($Makefile, $ExportsRef, $ExpDirsRef)=@_;

#	create EXPORT.MAKE

	my $erasedefn = "\@erase";
	$erasedefn = "\@erase 2>>nul" if ($ENV{OS} eq "Windows_NT");
	&Output(
		"ERASE = $erasedefn\n",
		"\n",
		"\n",
		"EXPORT : EXPORTDIRS"
	);
	my $ref;
	if (@$ExportsRef) {
		foreach $ref (@$ExportsRef) {
			my $name=&Path_Quote($$ref{Releasable});
			&Output(
				" \\\n",
				"\t$name"
			);
		}
	}
	else {
		&Output(
			" \n",
			"\t\@echo Nothing to do\n"
		);
	}
	&Output(
		"\n",
		"\n",
		"\n",
		"EXPORTDIRS :"
	);
	my $dir;
	foreach $dir (@$ExpDirsRef) {
		$dir=&Path_Quote($dir);
		&Output(
			" $dir"
		);
	}
	&Output(
		"\n",
		"\n"
	);
	foreach $dir (@$ExpDirsRef) {
		$dir=&Path_Quote($dir);
		&Output(
			"$dir :\n",
			    "\t\@perl -S emkdir.pl \"\$\@\"\n",
			"\n"
		);
	}
	&Output(
		"\n",
		"\n"
	);
	foreach $ref (@$ExportsRef) {
		my $dst=&Path_Quote($$ref{Releasable});
		my $src=&Path_Quote($$ref{Source});
		&Output(
			"$dst : $src\n",
			    "\tcopy \"\$?\" \"\$\@\"\n",
			"\n"
		);
	}
	&Output(
		"\n",
		"\n"
	);
	if (@$ExportsRef) {
		&Output(
			"CLEANEXPORT :\n"
		);
		foreach $ref (@$ExportsRef) {
			&Output(
				"\t-\$(ERASE) \"$$ref{Releasable}\"\n"
			);
		}
		&Output(
			"\n",
			"WHAT :\n"
		);
		foreach $ref (@$ExportsRef) {
			&Output(
				"\t\@echo \"$$ref{Releasable}\"\n"
			);
		}
	}
	else {
		&Output(
			"CLEANEXPORT :\n",
			"\t\@echo Nothing to do\n",
			"WHAT :\n",
			"\t\@rem do nothing\n"
		);
	}
	
#	write EXPORT.MAKE
	&WriteOutFileL($Makefile);
}


sub CreatePlatMak ($$$$$$$$$) {
	my ($BatchPath, $E32MakePath, $DataRef, $Plat, $RealPlat, $RomDir, $Module, $BldInfPath, $Test)=@_;

	unless ($Test) {
		$Test='';
	}
	else {
		$Test='TEST';
	}

	my $OutRomFile="$RomDir$RealPlat$Test.IBY";
	my $GCCDir="gcc\\bin";
	
	my $erasedefn = "\@erase";
	$erasedefn = "\@erase 2>>nul" if ($ENV{OS} eq "Windows_NT");
	&Output(
		"# set Path for custom builds using GCC - must set Path and PATH\n",
		'Path:=',&main::Path_Drive,$E32env::Data{EPOCPath},$GCCDir,";\$(Path)\n",
		"PATH:=\$(Path)\n",
		"\n",
		"# prevent MAKEFLAGS variable from upsetting calls to NMAKE\n",
		"unexport MAKEFLAGS\n",
		"\n",
		"ERASE = $erasedefn\n",
		"\n",
		"\n"
	);
	my $Command;
	my $Ref;
	foreach $Command (qw(CLEAN CLEANMAKEFILE FINAL FREEZE LIBRARY MAKEFILE RESOURCE SAVESPACE TARGET LISTING WHATMAKEFILE)) {
		&Output(
			"$Command :"
		);
		if (@$DataRef) {
			foreach $Ref (@$DataRef) {
				&Output(
					" $Command$$Ref{Base}"
				);
			}
		}
		else {
			&Output(
				"\n",
				"\t\@echo Nothing to do\n"
			);
		}
		&Output(
			"\n",
			"\n"
		);
	}
	&Output(
		"WHAT :"
	);
	my $whatcount=0;
	foreach $Ref (@$DataRef) {
		unless ($$Ref{Tidy}) {
			$whatcount++;
			&Output(
				" WHAT$$Ref{Base}"
			);
		}
	}
	if ($whatcount==0) {
		&Output(
			"\n",
			"\t\@rem do nothing\n" 
		);
	}
	&Output(
		"\n",
		"\n",
		"TIDY :"
	);
	my $Tidy='';
	foreach $Ref (@$DataRef) {
		if ($$Ref{Tidy}) {
			$Tidy.=" TIDY$$Ref{Base}";
		}
	}
	if ($Tidy) {
		&Output(
			"$Tidy\n"
		);
	}
	else {
		&Output(
			"\n",
			"\t\@echo Nothing to do\n"
		);
	}
	&Output(
		"\n",
		"\n"
	);
#	hack for non-EPOC platforms
	if ($RealPlat=~/^(WINS|WINSCW|WINC|TOOLS)$/o) {
		&Output(
			"ROMFILE :\n"
		);
	}
	else {
		&Output(
			'ROMFILE : STARTROMFILE'
		);
		foreach $Ref (@$DataRef) {
			&Output(
				" ROMFILE$$Ref{Base}"
			);
		}
		&Output(
			"\n",
			"\n",
			"STARTROMFILE :\n",
			    "\t\@perl -S emkdir.pl \"", &Path_Chop($RomDir), "\"\n",
			    "\t\@echo // $OutRomFile > $OutRomFile\n",
			    "\t\@echo // >> $OutRomFile\n"
		);
		if ($Test) {
			my ($Auto, $Manual);
			foreach $Ref (@$DataRef) {
				++$Auto unless ($$Ref{Manual} or $$Ref{Support});
				++$Manual if ($$Ref{Manual});
			}
			if ($Auto) {
				my $IbyTextFrom="data=$BatchPath$Plat.AUTO.BAT";
				my $IbyTextTo="Test\\$Module.$Plat.AUTO.BAT";
				my $Spaces= 60>length($IbyTextFrom) ? 60-length($IbyTextFrom) : 1; 
				&Output("\t\@echo ", $IbyTextFrom, ' 'x$Spaces, $IbyTextTo, ">> $OutRomFile\n");
			}
			if ($Manual) {
				my $IbyTextFrom="data=$BatchPath$Plat.MANUAL.BAT";
				my $IbyTextTo="Test\\$Module.$Plat.MANUAL.BAT";
				my $Spaces= 60>length($IbyTextFrom) ? 60-length($IbyTextFrom) : 1; 
				&Output("\t\@echo ", $IbyTextFrom, ' 'x$Spaces, $IbyTextTo, ">> $OutRomFile\n");
			}
		}
	}
	&Output(
		"\n",
		"\n"
	);
	my $CallNmake='nmake -nologo -x - $(VERBOSE) $(KEEPGOING)';
	my $CallGNUmake='$(MAKE) $(VERBOSE) $(KEEPGOING)';

	my %PlatHash;
	&Plat_GetL($RealPlat, \%PlatHash);
	my $CallMake=$CallNmake;
	if ($PlatHash{MakeCmd} eq "make") {
		$CallMake="$CallGNUmake -r";
	}
	&Plat_GetL($Plat, \%PlatHash);

	foreach $Ref (@$DataRef) {

#		standard commands
		unless ($$Ref{Makefile}) {
			my $MakefilePath=join('', &Path_Chop($E32MakePath), $$Ref{Path}, $$Ref{Base}, "\\", $RealPlat, "\\");
			my $RealMakefile="-f \"$MakefilePath$$Ref{Base}.$RealPlat\"";
			my $MakefileBase="$MakefilePath$$Ref{Base}";		
			&Output(
				"MAKEFILE$$Ref{Base}_FILES= \\\n",
					"\t\"$MakefileBase$PlatHash{Ext}\"",
			);
#			hacks for WINS/WINSCW/WINC and VC6
			if ($Plat =~ /^VC6/) {
				&Output(
					" \\\n\t\"$MakefileBase.DSW\"",
					" \\\n\t\"$MakefileBase.SUP.MAKE\""
				);
			}
			
			if ($Plat =~ /^VC7/) {
				&Output(
					" \\\n\t\"$MakefileBase.DSW\"",
					" \\\n\t\"$MakefileBase.SUP.MAKE\""
				);
			}
			
			if ($Plat eq 'CW_IDE') {
				&Output(
					" \\\n\t\"$MakefileBase.pref\""		# BUG: actually uses $BaseTrg, not mmp file name
				);
			}

			&Output(
				"\n",
				"\n",
				"MAKEFILE$$Ref{Base} :\n",
				    "\tperl -S makmake.pl -D $$Ref{Path}$$Ref{Base} $Plat\n",
				"\n",
				"CLEANMAKEFILE$$Ref{Base} :\n",
				    "\t-\$(ERASE) \$(MAKEFILE$$Ref{Base}_FILES)\n",
				"\n",
				"WHATMAKEFILE$$Ref{Base} :\n",
				    "\t\@echo \$(MAKEFILE$$Ref{Base}_FILES)\n",
				"\n",
				"TARGET$$Ref{Base} :\n",
				    "\t$CallMake $RealMakefile \$(CFG)\n",
				"\n",
				"SAVESPACE$$Ref{Base} :\n",
				    "\t$CallMake $RealMakefile \$(CFG) CLEANBUILD\$(CFG)\n",
				"\n",
				"LISTING$$Ref{Base} :\n",
				    "\t$CallMake $RealMakefile MAKEWORK\$(CFG) LISTING\$(CFG)\$(SOURCE)\n",
				"\n",
				"FINAL$$Ref{Base} :\n",
				    "\t\@rem do nothing\n",
				"\n",
			);
			foreach $Command (qw(CLEAN RESOURCE)) {
				&Output(
					"$Command$$Ref{Base} :\n",
					    "\t$CallMake $RealMakefile $Command\$(CFG)\n",
					"\n"
				);
			}
			foreach $Command (qw(FREEZE LIBRARY)) {
				&Output(
					"$Command$$Ref{Base} :\n",
					    "\t$CallMake $RealMakefile $Command\n",
					"\n"
				);
			}
			unless ($$Ref{Tidy}) {
				&Output(
					"WHAT$$Ref{Base} :\n",
					    "\t\@$CallMake -s $RealMakefile WHAT\$(CFG)\n",
					"\n"
				);
			}
			else {
				&Output(
					"TIDY$$Ref{Base} :\n",
					    "\t$CallMake $RealMakefile CLEANRELEASE CLEANLIBRARY\n",
					"\n"
				);
			}
			&Output(
				"ROMFILE$$Ref{Base} :\n",
				    "\t\@$CallMake $RealMakefile ROMFILE >> $OutRomFile\n",
				"\n",
				"\n"
			);
		}

#		calls to custom makefiles
		else {
			my $ChopRefPath=&Path_Chop($$Ref{Path});
			my $ChopBldInfPath=&Path_Chop($BldInfPath);
			my $MakefileCall;
			if ($$Ref{Makefile}==2) {
				$MakefileCall="cd $ChopRefPath;$CallNmake";
			} else {
				$MakefileCall="$CallGNUmake -C $ChopRefPath";
			}
			$MakefileCall.=" -f \"$$Ref{Base}$$Ref{Ext}\" TO_ROOT=";
			$MakefileCall.=&Path_Chop(&Path_UpToRoot($$Ref{Path}));
			&Output(
# should change to MAKEFILE
				"MAKEFILE$$Ref{Base} :\n",
				    "\t$MakefileCall PLATFORM=$Plat MAKMAKE\n",
				"\n",
# should call in custom makefiles maybe
				"CLEANMAKEFILE$$Ref{Base} :\n",
				"#	$MakefileCall PLATFORM=$Plat CLEANMAKEFILE\n",
				"\n",
				"WHATMAKEFILE$$Ref{Base} :\n",
				"#	\@$MakefileCall -s PLATFORM=$Plat WHATMAKEFILE\n",
				"\n",
# should change to TARGET
				"TARGET$$Ref{Base} :\n",
				    "\t$MakefileCall PLATFORM=$RealPlat CFG=\$(CFG) BLD\n",
				"\n",
# should ignore this target and just call the TARGET target instead?
				"SAVESPACE$$Ref{Base} :\n",
				    "\t$MakefileCall PLATFORM=$RealPlat CFG=\$(CFG) SAVESPACE\n",
				"\n",
				"LISTING$$Ref{Base} :\n",
				"\n",
				"\n",
# should change to LIBRARY
				"LIBRARY$$Ref{Base} :\n",
				    "\t$MakefileCall PLATFORM=$RealPlat LIB\n",
				"\n",
				"FREEZE$$Ref{Base} :\n",
				    "\t$MakefileCall PLATFORM=$RealPlat FREEZE\n",
				"\n",
			);
			foreach $Command (qw(CLEAN RESOURCE FINAL)) {
				&Output(
					"$Command$$Ref{Base} :\n",
					    "\t$MakefileCall PLATFORM=$RealPlat CFG=\$(CFG) $Command\n",
					"\n"
				);
			}
			unless ($$Ref{Tidy}) {
# should change to WHAT
				&Output(
					"WHAT$$Ref{Base} :\n",
					    "\t\@$MakefileCall -s PLATFORM=$RealPlat CFG=\$(CFG) RELEASABLES\n",
					"\n"
				);
			}
			else {
				&Output(
					"TIDY$$Ref{Base} :\n",
					    "\t$MakefileCall PLATFORM=$RealPlat TIDY\n",
# should change to CLEANLIBRARY
					    "\t$MakefileCall CLEANLIB\n",
					"\n"
				);
			}
			&Output(
				"ROMFILE$$Ref{Base} :\n",
				    "\t\@$MakefileCall PLATFORM=$RealPlat ROMFILE >> $OutRomFile\n",
				"\n",
				"\n"
			);
		}

	}
	
	&WriteOutFileL("$BatchPath$Plat$Test.MAKE");
}

sub CreatePlatBatches ($$$) {
	my ($OutDir, $DataRef, $Plat)=@_;

#	create the test batch files
#	this function won't work properly if the target basename is different from the .MMP basename
#	so perhaps it should call makmake on the .mmp file to check

	my $AutoText;
	my $ManualText;

	my $Ref;
	foreach $Ref (@$DataRef) {
		if ($$Ref{Manual}) {
			$ManualText.="$$Ref{Base}\n";
			next;
		}
		if ($$Ref{Support}) {
			next;
		}
		else {
			$AutoText.="$$Ref{Base}\n";
		}
	}

	if ($AutoText) {
		&Output($AutoText);
		&WriteOutFileL("$OutDir$Plat.AUTO.BAT");
	}

	if ($ManualText) {
		&Output($ManualText);
		&WriteOutFileL("$OutDir$Plat.MANUAL.BAT");
	}
}

sub WriteOutFileL ($$) { # takes batch file and boolean read-only flag
	my ($BATFILE, $ReadOnly)=@_;


	eval { &Path_MakePathL($BATFILE); };
	&FatalError($@) if $@;

	open BATFILE,">$BATFILE" or &FatalError("Can't open or create Batch File \"$BATFILE\"");
	print BATFILE &OutText or &FatalError("Can't write output to Batch File \"$BATFILE\"");
	close BATFILE or &FatalError("Can't close Batch File \"$BATFILE\"");
}

