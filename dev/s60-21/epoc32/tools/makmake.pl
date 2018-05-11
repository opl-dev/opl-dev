# MAKMAKE.PL
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
use E32Variant;
use Genutl;
use Modload;
use Pathutl;
use Trgtype;

# THE MAIN PROGRAM SECTION
##########################

{
	Load_SetModulePath($PerlLibPath);
	Plat_Init($PerlLibPath);
}

my $MAKEFILE;
my $MMPFILE;
my %Options;
my %Plat;
my %TruePlat;
my %BldMacros;
my $PlatArg;
my @PlatOverrideList=();

{
	# process the command line
	unless (GetOptions(\%Options, 'd', 'mmp', 'plat=s', 'v')) {
		exit 1;
	}

	$Options{makemakefile}='1' unless ($Options{mmp} || $Options{plat});

	if ($Options{mmp} or $Options{plat}) {
		eval { &Load_ModuleL('MAKHELP'); };
		die $@ if $@;
	}

	if ($Options{mmp}) {
		&Help_Mmp;
		exit;
	}

	if ($Options{plat}) {
		eval { &Plat_GetL($Options{plat},\%Plat,\%BldMacros); };
		die $@ if $@;
		eval { &Load_ModuleL($Plat{MakeMod}); };
		die $@ if $@;
		&Help_Plat($Plat{Real},$Plat{CPU},\@{$Plat{MmpMacros}},\@{$Plat{Macros}});
		exit;
	}

#	show help & exit if necessary
	if (@ARGV!=2) {
		&Usage();
	}

	if ($Options{v}) {
		print "perl -S makmake.pl @ARGV\n";
		&Load_SetVerbose;
		&Path_SetVerbose;
		&Plat_SetVerbose;
	}

	# set up platform information
	# IDE platforms can be controlled by specifying the actual list of real platforms
	# e.g. cw_ide:plat1+plat2+plat3
	# 
	$PlatArg=uc pop @ARGV;
	if ($PlatArg=~/^(\S+):(\S+)$/) {
		$PlatArg=$1;
		@PlatOverrideList=split(/\+/,$2);
	}
	eval { &Plat_GetL($PlatArg,\%TruePlat,\%BldMacros); };
	die $@ if $@;
	if (scalar @PlatOverrideList) {
		$PlatArg=$PlatOverrideList[0];
	}

	$MMPFILE=uc pop @ARGV;
	die "ERROR: Can't specify MMP file on a different drive\n" if $MMPFILE=~/^\w:\\/o;
	if ($MMPFILE!~/.MMP$/o) {
		$MMPFILE.='.MMP';
	}
	$MMPFILE=&Path_AbsToWork($MMPFILE);

	eval { &Load_ModuleL('Mmp'); };
	die $@ if $@;
	if ($Options{v}) {
		&Mmp_SetVerbose;
	}
	if ($Options{d}) {
		die "ERROR: $E32env::Data{EPOCPath} does not exist\n" if (!-d $E32env::Data{EPOCPath});
	}

	
	#Hack for Visual Studio 6 support
	my $vc6edll = $ENV{EPOCROOT}."epoc32\\release\\wins\\udeb\\edll_vc6.lib";
	my $vc7edll = $ENV{EPOCROOT}."epoc32\\release\\wins\\udeb\\edll_vc7.lib";
	my $targetEdll = $ENV{EPOCROOT}."epoc32\\release\\wins\\udeb\\edll.lib";
	
	if (defined $ENV{MSDevDir}) {
		unless(-e $vc6edll) {
			die "File \"edll_vc6.lib\" not found!";
		}
		else {
			system("copy $vc6edll $targetEdll");
		}	
	}
	if (defined $ENV{VSCOMNTOOLS}) {
		unless(-e $vc7edll) {
			die "File \"edll_vc7.lib\" not found!";
		}
		else {
			system("copy $vc7edll $targetEdll");
		}	
	}
	#Hack ends

}
my %WarningLevel;
my $ABI;
my @AifStruct;
my $AllowDllData;
my $CompressTarget;
my $ASSPExports;
my @ASSPLibList;
my @BitMapStruct;
my $CallDllEntryPoints;
my @CompatibleABIs;
my $DataLinkAddress;
my @DebugLibList;
my %Def;
my %DocHash;
my $ExportUnfrozen;
my $FirstLib;
my $FixedProcess;
my %HeapSize;
my @LibList;
my $LinkAs;
my %MmpFlag;
my @PlatTxt2D;
my $ProcessPriority;
my @RamTargets;
my @ResourceStruct;
my @RomTargets;
my %SrcHash;
my $StackSize;
my @StatLibList;    
my @SysIncPaths;
my $Trg;
my %TrgType;
my @UidList;
my @UserIncPaths;
my $SrcDbg;
my %Path;
my $variantMacroHRHFile = Variant_GetMacroHRHFile();  # HRH file containing macros specific to a variant
my @variant_macros = Variant_GetMacroList(); # macros specific to a variant

&SetVarsFromMmp($PlatArg);
die $@ if $@;

{
	# set up the makefile filepath - need to do this before loading the platform module
	# because UID source file will be added and set up in the makefile path under WINS
	if ($Options{d}) {
		$MAKEFILE=join ('', $Path{Bld}, &Path_Split('Base',$MMPFILE), $TruePlat{Ext});
	}
	else {
		$MAKEFILE=join "", &Path_WorkPath, &Path_Split('Base',$MMPFILE), $TruePlat{Ext};
	}
}

{

#	load the platform module
	eval { &Load_ModuleL($TruePlat{MakeMod}); };
	die $@ if $@;

	unless (defined &PMHelp_Mmp) {
#		check this function is defined - all modules must have it - if not perhaps the
#		platform module has not loaded is compiler module successfully via "use"
		die "ERROR: Module \"$Plat{MakeMod}\" not loaded successfully\n";
	}
}

{
	# allow the platform to bow out if it can't support some .MMP file specifications
	if (defined &PMCheckPlatformL) {
		eval { &PMCheckPlatformL(); };
		die $@ if $@;
	}
}

my @StdIncPaths=();

{
	# get the platform module to do it's mmpfile processing - WINS modules may set up an extra source file
	# for UIDs here depending upon the targettype
	&PMPlatProcessMmp(@PlatTxt2D) if defined &PMPlatProcessMmp;
}

{
	# if verbose mode set, output some info
	#--------------------------------------
	if ($Options{v}) {
		print  
			"Target: \"$Trg\"\n",
			"TargetType: \"$TrgType{Name}\"\n",
			"Libraries: \"@LibList\"\n",
			"Debug Libraries: \"@DebugLibList\"\n",
			"Static Libraries: \"@StatLibList\"\n",
			"Uids: \"@UidList\"\n",
			"BuildVariants: \"@{$Plat{Blds}}\"\n",
			"TargetMakeFile: \"$MAKEFILE\"\n",
			"UserIncludes: \"<Source Dir> @UserIncPaths\"\n",
			"SystemIncludes: \"@SysIncPaths\"\n"
		;
	}
}

my $CurAifRef;
my $CurBaseObj;
my $CurBld;
my $CurBitMapRef;
my @CurDepList;
my $CurDoc;
my $CurResrc;
my $CurResourceRef;
my $CurSrc;
my $CurSrcPath;
my $ResrcIsSys;

{

	# LOOPING SECTION
	#----------------
# Load the output module
	eval { &Load_ModuleL('OUTPUT'); };
	die $@ if $@;

	&PMStartBldList($Plat{MakeCmd}) if defined &PMStartBldList;
	my $LoopBld;
	foreach $LoopBld (@{$Plat{Blds}}) {
		$CurBld=$LoopBld;
		&PMBld if defined &PMBld;
	}
	undef $CurBld;
	undef $LoopBld;
	&PMEndBldList if defined &PMEndBldList;


	# Load the Dependency Generator
	eval { &Load_ModuleL('MAKDEPS'); };
	die $@ if $@;
	eval { &Deps_InitL($E32env::Data{EPOCIncPath},@StdIncPaths); };
	die $@ if $@;
	if ($Options{v}) {
		&Deps_SetVerbose;
	}
	if ($Plat{UsrHdrsOnly}) {
		&Deps_SetUserHdrsOnly;
	}
	&Deps_SetUserIncPaths(@UserIncPaths);
	&Deps_SetSysIncPaths(@SysIncPaths);
	&Deps_SetPlatMacros(@{$Plat{Macros}});


#	Start source list - bitmaps, resources, .AIF files, documents, sources.

	&PMStartSrcList if defined &PMStartSrcList;

#	start bitmaps

	if ($Options{v}) {
		print "Starting bitmaps\n";
	}
	my $LoopBitMapRef;
	foreach $LoopBitMapRef (@BitMapStruct) {
		$CurBitMapRef=$LoopBitMapRef;
		if ($Options{v}) {
			print "BitMap: \"$$CurBitMapRef{Trg}\"\n";
		}
		&PMBitMapBld if defined &PMBitMapBld;
	}
	undef $CurBitMapRef;
	undef $LoopBitMapRef;

#	end bitmaps

#	start resources

	if ($Options{v}) {
		print "Starting resources\n";
	}
	my $LoopResourceRef;
	foreach $LoopResourceRef (@ResourceStruct) {
		$CurResourceRef=$LoopResourceRef;
		if ($Options{v}) {
			print "Resource: \"$$CurResourceRef{Trg}\"\n";
		}
		eval { @CurDepList=&Deps_GenDependsL($$CurResourceRef{Source}, ("LANGUAGE_$$CurResourceRef{Lang}")); };
		die $@ if $@;
		&PMResrcBld if defined &PMResrcBld;
		undef @CurDepList;
	}
	undef $CurResourceRef;
	undef $LoopResourceRef;

#	end resources

#	start aifs

	if ($Options{v}) {
		print "Starting aifs\n";
	}

# Add tools-relative include path to sys includes, to allow for shared include\aiftool.rh
	use FindBin;
	$FindBin::Bin =~ /:(.*)\//;
	my $extraIncPath = $1;
	$extraIncPath =~ s/\//\\/g;
	my @SavedSysIncPaths = @SysIncPaths;
	push @SysIncPaths, "$extraIncPath\\INCLUDE";
	&Deps_SetSysIncPaths(@SysIncPaths);

	my $LoopAifRef;
	foreach $LoopAifRef (@AifStruct) {
		$CurAifRef=$LoopAifRef;
		if ($Options{v}) {
			print "Aif: \"$$CurAifRef{Trg}\"\n";
		}
		eval { @CurDepList=&Deps_GenDependsL("$$CurAifRef{Source}"); };
		die $@ if $@;
		&PMAifBld if defined &PMAifBld;
		undef @CurDepList;
	}
	undef $CurAifRef;
	undef $LoopAifRef;

	@SysIncPaths = @SavedSysIncPaths;
	&Deps_SetSysIncPaths(@SysIncPaths);

#	end aifs

#	start sources

	if ($Options{v}) {
		print "Starting sources\n";
	}
	my $LoopSrcPath;
	foreach $LoopSrcPath (sort keys %SrcHash) {
		$CurSrcPath=$LoopSrcPath;
		if ($Options{v}) {
			print "Sourcepath: \"$CurSrcPath\"\n";
		}
		my $LoopSrc;
		foreach $LoopSrc (sort @{$SrcHash{$CurSrcPath}}) {
			$CurSrc=$LoopSrc;
			if ($Options{v}) {
				print "Source: \"$CurSrc\"\n";
			}
			&PMStartSrc if defined &PMStartSrc;

#			strict depend alt 1 start - call different module function if strict depend flag specified
			if (((not $MmpFlag{StrictDepend}) || (not defined &PMSrcBldDepend)) && defined &PMSrcDepend) {
				eval { @CurDepList=&Deps_GenDependsL($CurSrcPath.$CurSrc); };
				die $@ if $@;
				&PMSrcDepend if defined &PMSrcDepend;
				undef @CurDepList;
			}
#			strict depend alt 1 end

			my $LoopBld;
			foreach $LoopBld (@{$Plat{Blds}}) {
				$CurBld=$LoopBld;
				&PMStartSrcBld if defined &PMStartSrcBld;

#				strict depend alt 2 start - call the module function that deals with dependencies generated for each build variant
				if ($MmpFlag{StrictDepend} && defined &PMSrcBldDepend) {
					eval { @CurDepList=Deps_GenDependsL($CurSrcPath.$CurSrc,@{$BldMacros{$CurBld}}); };
					die $@ if $@;
					&PMSrcBldDepend if defined &PMSrcBldDepend;
					undef @CurDepList;
				}
#				strict depend alt 2 end

				&PMEndSrcBld if defined &PMEndSrcBld;
			}
			undef $CurBld;
			undef $LoopBld;
			&PMEndSrc if defined &PMEndSrc;
		}
		undef $CurSrc;
		undef $LoopSrc;
	}
	undef $CurSrcPath;
	undef $LoopSrcPath;

#	end sources

#	start documents

	if ($Options{v}) {
		print "Starting documents\n";
	}
	foreach $LoopSrcPath (sort keys %DocHash) {
		$CurSrcPath=$LoopSrcPath;
		if ($Options{v}) {
			print "Sourcepath: \"$CurSrcPath\"\n";
		}
		my $LoopDoc;
		foreach $LoopDoc (sort @{$DocHash{$CurSrcPath}}) {
			$CurDoc=$LoopDoc;
			if ($Options{v}) {
				print "Document: \"$CurDoc\"\n";
			}
			&PMDoc if defined &PMDoc;
		}
		undef $CurDoc;
		undef $LoopDoc;
	}
	undef $CurSrcPath;
	undef $LoopSrcPath;

#	end documents

#	rombuild

	my %SpecialRomFileTypes=(
		KEXT=>'extension[MAGIC]',
		LDD=>'device[MAGIC]',
		PDD=>'device[MAGIC]',
		VAR=>'variant[MAGIC]'
	);
	unless ($TruePlat{Ext} =~ /\.DSP|\.xml|\.VCPROJ/i) { # hack to avoid rombuild target for IDE makefiles
		&Output("ROMFILE:\n");
		unless ($Plat{OS} ne 'EPOC32' or $TrgType{Basic} eq 'LIB') {
			my $ref;
			foreach $ref (@RomTargets) {
				my $RomFileType='file';
				if ($$ref{FileType}) {	# handle EKERN.EXE and EFILE.EXE with new ROMFILETYPE keyword instead
					$RomFileType=$$ref{FileType}; # or just do this bit as a custom build makefile
				}
				elsif ($CallDllEntryPoints) {
					$RomFileType='dll';
				}
				elsif ($SpecialRomFileTypes{$TrgType{Name}}) {
					$RomFileType=$SpecialRomFileTypes{$TrgType{Name}};
				}
#				$RomFileType='primary[MAGIC]' if $Trg eq 'EKERN.EXE';
#				$RomFileType='secondary[MAGIC]' if $Trg eq 'EFILE.EXE';
				my $RomPath="System\\Libs\\";
				if ($$ref{Path}) {
					$RomPath=$$ref{Path};
				}
				elsif ($TrgType{Path}) {
					$RomPath=$TrgType{Path};
					$RomPath=~s-Z\\(.*)-$1-o;
				}
				elsif ($TrgType{Name} eq 'EXE') {
					$RomPath="Test\\";
				}
				my $RomFile=$LinkAs;
				if ($$ref{File}) {
					$RomFile=$$ref{File};
				}
				my $RomDecorations='';
				if ($DataLinkAddress) {
					$RomDecorations="reloc=$DataLinkAddress";
				}
				elsif ($FixedProcess) {
					$RomDecorations.='fixed';
				}
				my $IbyTextFrom="$RomFileType=$E32env::Data{RelPath}$Plat{Real}\\##BUILD##\\$Trg";
				my $IbyTextTo="$RomPath$RomFile";
				my $Spaces= 60>length($IbyTextFrom) ? 60-length($IbyTextFrom) : 1; 
#				&Output("\t\@echo \"", $IbyTextFrom, ' 'x$Spaces, "$IbyTextTo $RomDecorations\"\n");
				&Output("\t\@echo ", $IbyTextFrom, ' 'x$Spaces, "$IbyTextTo $RomDecorations\n");
			}
			foreach $ref (@RamTargets) {
				my $RomFileType='data';
				my $RomPath="Img\\";
				if ($$ref{Path}) {
					$RomPath=$$ref{Path};
				}
				my $RomFile=$Trg;
				if ($$ref{File}) {
					$RomFile=$$ref{File};
				}
				my $RomDecorations='attrib=r';
				my $IbyTextFrom="$RomFileType=$E32env::Data{RelPath}$Plat{Real}\\##BUILD##\\$Trg";
				my $IbyTextTo="$RomPath$RomFile";
				my $Spaces= 60>length($IbyTextFrom) ? 60-length($IbyTextFrom) : 1; 
#				&Output("\t\@echo \"", $IbyTextFrom, ' 'x$Spaces, "$IbyTextTo $RomDecorations\"\n");
				&Output("\t\@echo ", $IbyTextFrom, ' 'x$Spaces, "$IbyTextTo $RomDecorations\n");
			}
		}
	}
#	end rombuild

	&PMEndSrcList if defined &PMEndSrcList;
}

{

	# open the makefile and write all the text it requires to it if makmake has so far been successful
	#-------------------------------------------------------------------------------------------------
	eval { &Path_MakePathL($MAKEFILE); };
	die $@ if $@;
	if ($Options{v}) {
		print "Creating: \"$MAKEFILE\"\n";
	}
	open MAKEFILE,">$MAKEFILE" or die "ERROR: Can't open or create file \"$MAKEFILE\"\n";
	print MAKEFILE &OutText or die "ERROR: Can't write output to file \"$MAKEFILE\"\n";
	close MAKEFILE or die "ERROR: Can't close file \"$MAKEFILE\"\n";
	if ($Options{v}) {
		print "Successful MakeFile Creation\n";
	}
}

	
################ END OF MAIN PROGRAM SECTION #################
#------------------------------------------------------------#
##############################################################





# SUBROUTINE SECTION
####################

sub FatalError (@) {

	print STDERR "MAKMAKE ERROR: @_\n";
	exit 1;
}

sub Usage () {

		eval { &Load_ModuleL('MAKHELP'); };
		die $@ if $@; 
		eval { &Help_Invocation; };
		die $@ if $@;
		exit;
}

sub SetVarsFromMmp ($) {

	my ($platname)=@_;

	# MMP FILE PROCESSING - filter the mmp file content through the GCC preprecessor
	#-------------------------------------------------------------------------------
	eval { &Plat_GetL($platname,\%Plat,\%BldMacros); };
	return $@ if $@;
	&Mmp_Reset;
	eval { &Mmp_ProcessL($E32env::Data{EPOCPath}, $MMPFILE, \%Plat); };
	return $@ if $@;

	%WarningLevel=&Mmp_WarningLevel;
	$ABI=&Mmp_ABI;
	@AifStruct=@{&Mmp_AifStruct};
	$AllowDllData=&Mmp_AllowDllData;
	$CompressTarget=&Mmp_CompressTarget;
	$ASSPExports=&Mmp_ASSPExports;
	@ASSPLibList=&Mmp_ASSPLibList;
	@BitMapStruct=@{&Mmp_BitMapStruct};
	$CallDllEntryPoints=&Mmp_CallDllEntryPoints;
	@CompatibleABIs=&Mmp_CompatibleABIs;
	$DataLinkAddress=&Mmp_DataLinkAddress;
	@DebugLibList=@{&Mmp_DebugLibList};
	%Def=%{&Mmp_Def};
	%DocHash=%{&Mmp_DocHash};
	$ExportUnfrozen=&Mmp_ExportUnfrozen;
	$FirstLib=&Mmp_FirstLib;
	$FixedProcess=&Mmp_FixedProcess;
	%HeapSize=%{&Mmp_HeapSize};
	@LibList=@{&Mmp_LibList};
	$LinkAs=&Mmp_LinkAs;
	%MmpFlag=%{&Mmp_MmpFlag};
	@PlatTxt2D=&Mmp_PlatTxt2D;
	$ProcessPriority=&Mmp_ProcessPriority;
	@RamTargets=&Mmp_RamTargets;
	@ResourceStruct=@{&Mmp_ResourceStruct};
	@RomTargets=&Mmp_RomTargets;
	%SrcHash=%{&Mmp_SrcHash};
	$StackSize=&Mmp_StackSize;
	@StatLibList=&Mmp_StatLibList;    
	@SysIncPaths=&Mmp_SysIncPaths;
	$Trg=&Mmp_Trg;
	%TrgType=%{&Mmp_TrgType};
	@UidList=&Mmp_UidList;
	@UserIncPaths=&Mmp_UserIncPaths;
	$SrcDbg=&Mmp_SrcDbg;

#	finish defining any macros

	if ($Plat{CPU} eq 'MARM') {
#		apply the ABI source define - note that it is difficult to define a corresponding
#		.MMP define since we can't be sure what the ABI is until we've processed the .MMP file,
#		though we could apply it for generic MARM builds only
		push @{$Plat{Macros}}, "__MARM_${ABI}__";
	}

	if ($TrgType{Basic}=~/^(DLL|EXE)$/o) { # this macro may soon be removed
		push @{$Plat{Macros}},'__'.$TrgType{Basic}.'__';
	}

#	add the macros defined in the .mmp file
	push @{$Plat{Macros}}, &Mmp_Macros;

# set up a hash containing the start paths for various things
	undef %Path;

#	set up ASSP link path - this is the path where the target looks for ASSP-specific import libraries
	$Path{ASSPLink}="$E32env::Data{LinkPath}$Plat{ASSP}\\";

#	set up build path
	$Path{Bld}=join('', &Path_Chop($E32env::Data{BldPath}), &Path_Split('Path',$MMPFILE), &Path_Split('Base',$MMPFILE), "\\$Plat{Real}\\");

#	set up lib path - this is the path where the target puts it's import library
	$Path{Lib}="$E32env::Data{LinkPath}";
	unless ($ASSPExports) {
		$Path{Lib}.="$ABI\\";
	}
	else {
		$Path{Lib}.="$Plat{ASSP}\\";
	}

#	set up link path - this is the place where the target looks for ordinary libraries
	$Path{Link}="$E32env::Data{LinkPath}$ABI\\";

#	set up stat link path - this is where the target looks for static libraries
	$Path{StatLink}="$E32env::Data{LinkPath}";
	unless ($Plat{OS} eq 'WINS') {	# WINC and WINS versions of EEXE are different
		$Path{StatLink}.="$ABI\\"; # ARM static libraries are currently always ASSP-independent
	}
	else {
		$Path{StatLink}.="$Plat{ASSP}\\"; # WINC static libraries are currently always ASSP-specific
	}

#	set up release path
	unless ($TrgType{Basic} eq 'LIB') {
		$Path{Rel}="$E32env::Data{RelPath}$Plat{Real}\\";
	}
	else {
		$Path{Rel}=$Path{StatLink}; # static libraries can't #define the __SINGLE__ macro
	}
}


sub CreateExtraFile ($$) { # takes abs path for source and text
# allows modules to create extrafiles
	my ($FILE,$Text)=@_;
	if ($Options{makemakefile}) {	# only create if making the makefile
		if ($Options{v}) {
			print "Creating \"$FILE\"\n";
		}
		eval { &Path_MakePathL($FILE); };
		die $@ if $@;
		open FILE, ">$FILE" or die "WARNING: Can't open or create \"$FILE\"\n";
		print  FILE $Text or die "WARNING: Can't write text to \"$FILE\"\n";
		close FILE or die "WARNING: Can't close \"$FILE\"\n";
	}
}

sub ABI () {
	$ABI;
}
sub AddSrc ($$) { # needs abs path for source
# allows modules to add a source file to the project and have it created if necessary
	my ($SRCFILE,$Text)=@_;
	my $SrcPath=&Path_Split('Path',$SRCFILE);
	if ((not -e $SRCFILE) || (-M $SRCFILE > -M $MMPFILE)) {
		# only create the file if it's older than the .MMP file
		CreateExtraFile($SRCFILE,$Text);
	}
	push @{$SrcHash{$SrcPath}}, &Path_Split('File',$SRCFILE);
}
sub AddPlatMacros (@) {
# allows modules to add extra macros to the platform macro list
	push @{$Plat{Macros}},@_;
}
sub AifRef () {
	$CurAifRef;
}
sub AifStructRef () {
	\@AifStruct;
}
sub AllowDllData () {
	$AllowDllData;
}
sub CompressTarget () {
	$CompressTarget;
}
sub ASSPLibList () {
	@ASSPLibList;
}
sub ASSPLinkPath () {
#	this is the path where the target looks for ASSP-specific import libraries
	my $Path=$Path{ASSPLink};
	if ($CurBld) {
#regression hack
		if ($Plat{OS} eq 'EPOC32') {
			$Path.="UREL\\";
		}
		else {
			$Path.="UDEB\\";
		}
#regression hack end
	}
	$Path;
}
sub BaseMak () {
	&Path_Split('Base',$MAKEFILE);
}
sub BaseResrc () {
	&Path_Split('Base',$CurResrc);
}
sub BaseResrcList () {
	my @ResrcList=&ResrcList;
	my $Path;
	foreach $Path (@ResrcList) {
		$Path=&Path_Split('Base',$Path);
	}
	@ResrcList;
}
sub BaseSrc () {
	&Path_Split('Base',$CurSrc);
}
sub BaseSrcList () {
	my @SrcList=&SrcList;
	my $Path;
	foreach $Path (@SrcList) {
		$Path=&Path_Split('Base',$Path);
	}
	@SrcList;
}
sub BaseSysResrcList () {
	my @SysResrcList=&SysResrcList;
	my $Path;
	foreach $Path (@SysResrcList) {
		$Path=&Path_Split('Base',$Path);
	}
	@SysResrcList;
}
sub BaseTrg () {
	&Path_Split('Base',$Trg);
}
sub BitMapRef () {
	$CurBitMapRef;
}
sub BitMapStructRef () {
	\@BitMapStruct;
}
sub Bld () {
	$CurBld;
}
sub BldList () {
	@{$Plat{Blds}};
}
sub BldPath () {
	my $Path=$Path{"Bld"};
	if ($CurBld) {
		$Path.="$CurBld\\";
	}
	$Path;
}
sub CallDllEntryPoints () {
	$CallDllEntryPoints;
}
sub CompatibleABIs () {
	@CompatibleABIs;
}
sub DataLinkAddress () {
	$DataLinkAddress;
}
sub DataPath () {
	$E32env::Data{DataPath};
}
sub DebugLibList () {
	@DebugLibList;
}
sub DefFile () {
	"$Def{Path}$Def{Base}$Def{Ext}";
}
sub DefFileType () {
	$Plat{DefFile};
}
sub DepList () {
	sort @CurDepList;
}
sub Doc () {
	$CurDoc;
}
sub DocList () {
	if ($CurSrcPath) {
		return sort @{$DocHash{$CurSrcPath}};
	}
	my @DocList;
	my $Key;
	foreach $Key (keys %DocHash) {
		push @DocList,@{$DocHash{$Key}};
	}
	sort @DocList;
}
sub EPOCPath () {
	$E32env::Data{EPOCPath};
}
sub EPOCDataPath () {
	$E32env::Data{EPOCDataPath};
}
sub EPOCIncPath () {
	$E32env::Data{EPOCIncPath};
}
sub EPOCRelPath () {
	$E32env::Data{RelPath};
}
sub EPOCToolsPath () {
	$E32env::Data{EPOCToolsPath};
}
sub Exports () {
	@{$TrgType{Exports}{$Plat{"DefFile"}}};
}
sub ExportUnfrozen () {
	$ExportUnfrozen;
}
sub FirstLib () {
	$FirstLib;
}
sub FixedProcess () {
	$FixedProcess;
}
sub BasicTrgType () {
	$TrgType{Basic};
}
sub HeapSize () {
	%HeapSize;
}
sub LibList () {
	@LibList;
}
sub LibPath () {
#	this is the path where the target puts it's import library
	my $Path=$Path{Lib};
	if ($CurBld) {
#regression hack
		if ($Plat{OS} eq 'EPOC32') {
			$Path.="UREL\\";
		}
		else {
			$Path.="UDEB\\";
		}
#regression hack end
	}
	$Path;
}
sub LinkAs () {
	$LinkAs;
}
sub LinkPath () {
#	this is the place where the target looks for CPU-specific libraries
	my $Path=$Path{Link};
	if ($CurBld) {
#regression hack
		if ($Plat{OS} eq 'EPOC32') {
			$Path.="UREL\\";
		}
		else {
			$Path.="UDEB\\";
		}
#regression hack end
	}
	$Path;
}

sub MacroList ($) {
	if ($_[0]) {
	return @{$BldMacros{$_[0]}};
	}
	return @{$Plat{Macros}} unless $CurBld;
	(@{$Plat{Macros}},@{$BldMacros{$CurBld}});
}

# returns an array of Variant specific Macros
sub VariantMacroList($)
{
    return @variant_macros;
}

# returns the file containing Variant specific Macros
sub VariantFile($)
{
    return $variantMacroHRHFile;
}

sub MakeFilePath () {
	&Path_Split('Path',$MAKEFILE);
}
sub MmpFile () {
	$MMPFILE;
}
sub PerlLibPath () {
	$PerlLibPath;
}
sub Plat () {
	$Plat{Real};
}
sub PlatABI () {
	$Plat{"ABI"};
}
sub PlatCompiler () {
	$Plat{"Compiler"};
}
sub PlatName () {
	$Plat{Name};
}
sub PlatOS () {
	$Plat{OS};
}
sub PlatOverrideList () {
	@PlatOverrideList;
}
sub ProcessPriority () {
	$ProcessPriority;
}
sub RelPath () {
	my $Path=$Path{Rel};
	if ($CurBld) {
		$Path.="$CurBld\\";
	}
	$Path;
}
sub ResourceRef () {
	$CurResourceRef;
}
sub ResourceStructRef () {
	\@ResourceStruct;
}
sub SetCurBld($) {
	$CurBld=$_[0];		# used by ide_cw.pm when handling additional platforms
}
sub	SetStdIncPaths (@) {
# allows module to set standard include paths
	@StdIncPaths=();
	my $Path;
	foreach $Path (@_) {
		$Path=uc $Path;
		$Path=~s-^(.*[^\\])$-$1\\-o;
		push @StdIncPaths, $Path;	# only place drive letters may appear, up to modules to handle
	}
}
sub Src () {
	$CurSrc;
}
sub SrcHash () {
	\%SrcHash;		# hash of SOURCEPATH => (filename1, filename2, ...)
}
sub SrcList () {
	if ($CurSrcPath) {
		return sort @{$SrcHash{$CurSrcPath}};
	}
	my @SrcList;
	my $Key;
	foreach $Key (keys %SrcHash) {
		push @SrcList,@{$SrcHash{$Key}};
	}
	sort @SrcList;
}
sub StackSize () {
	$StackSize;
}
sub StatLibList () {
	@StatLibList;
}
sub StatLinkPath () {
	my $Path=$Path{StatLink};
	if ($CurBld) {
		$Path.="$CurBld\\";
	}
	$Path;
}
sub SrcPath () {
	$CurSrcPath;
}
sub SysIncPaths () {
	@SysIncPaths;
}
sub Trg () {
	$Trg;
}
sub TrgPath () {
	$TrgType{Path};
}
sub TrgType () {
	$TrgType{Name};
}
sub UidList () {
	@UidList;
}
sub UserIncPaths () {
	@UserIncPaths;
}
sub SrcDbg () {
	$SrcDbg;
}
sub CompilerOption
{
	my $CompOption=$WarningLevel{$_[0]};
	$CompOption="" if (!defined($CompOption)); 
	$CompOption;
}
