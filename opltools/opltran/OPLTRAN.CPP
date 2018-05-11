// OPLTRAN.CPP
//
// WINC Command line OPL translator for Symbian OS
//
// Copyright (c) 1998-2002 Symbian Ltd. All rights reserved.

#include <f32file.h>
#include <fbs.h>
#include <opltran.h>
#include <e32cons.h>
#include <barsc.h>
#include <opltran.rsg>
#include "opltranerr.h" // mapping OPL error numbers.
#if !defined(__UIQ__)
#include <stdlib.h> // Windows exit
#endif
#include <bautils.h>

#include "winutil.h"
#include "opltrant.h"

// Version and copyright notices:
_LIT(KOpltranVersionDetails,"Opltran v3.0\n");

_LIT(KDefaultAppPath,"epoc32\\wins\\c\\System\\Apps\\.app");
_LIT(KOpltranResourceFile,"opltran.rsc");
_LIT(KEikResourceFile,"epoc32\\release\\wins\\udeb\\z\\system\\data\\eikcore.rsc");
_LIT(KEikResourceFileName,"eikcore.rsc");

// Error output format details
enum {EErrorFormatODE=0,EErrorFormatMSD};

// Resource file details
enum {EOplResource=0, EEikResource};

const TInt KOpltranResourceSignature=0x00000004; // Should really be in .HRH file.
const TInt KEikResourceSignature=0x00000004; // EEikResourceSignatureValue;

// Drives, paths and buffers
GLDEF_D TFileName InputPath;
GLDEF_D TFileName IncludePath;
GLDEF_D TFileName EpocRoot;
GLDEF_D TText8 CurrentWorkingDirectory8[KMaxFileName+1]; // Tmp buffer for Windows call. ("+1" is for zero terminator)
GLDEF_D RFs theFileServer;
GLDEF_D TFileName OutputPath;
GLDEF_D TConvertFlag ConvertFlag=ENoConvert;

LOCAL_C TFileName ExeDrive;
LOCAL_C TFileName CurrentWorkingDirectory;
LOCAL_C TBool	  RemoteUse=EFalse;

LOCAL_C void ShowError(const TDesC& aMsg,TInt aErr,const TDesC& aExtra=KNullDesC);

// #define _ERROR_FILE
#ifdef _ERROR_FILE
	_LIT(KErrNameLiteral,"OPLTRAN.err");
	const TPtrC KErrName(KErrNameLiteral);
	LOCAL_C TBool	ErrorFileOpen=EFalse;
	LOCAL_C RFile	ErrorFile;
	LOCAL_C TParse	ErrorFileDetails;
#endif

enum TPrintLoudness
	{
	EPrintError=0,
	EPrintNormal,
	EPrintVerbose
	};

#if !defined(__UIQ__)
LOCAL_C TInt	ExitValue=EXIT_SUCCESS; // ERRORLEVEL exit value
#endif
LOCAL_C TInt	ErrorFormatFlag=EErrorFormatODE; // Format of translator error msgs.
LOCAL_C TInt	PrintLevel=(TInt)EPrintNormal;
LOCAL_C TBool	SingleShot=EFalse;
LOCAL_D ROplTextSystem theTextSystem;

class CConsoleReporter : public CConsoleBase
	{
public:
	void Printf(TInt aLoud, TRefByValue<const TDesC> aFmt,...);
	void SetQuiet();	
	void SetVerbose();
private:
	void HeadedPrintf(TPtrC aBuf);
	};

void CConsoleReporter::SetQuiet()
	{
	PrintLevel=EPrintError; // only error msgs shown.
	}

void CConsoleReporter::SetVerbose()
	{
	PrintLevel=EPrintVerbose;
	}

// Fix bug [1043920] OPLTRAN.EXE panics on 9500 SDK
void CConsoleReporter::Printf(TInt aLoud, TRefByValue<const TDesC> aFmt,...)
	{
	TBool printIt=EFalse;
	if (aLoud==EPrintError) // always show errors
		printIt=ETrue;
	else if ((aLoud==EPrintNormal) && (PrintLevel!=EPrintError))
		printIt=ETrue;
	else if ((aLoud==EPrintVerbose) && (PrintLevel==EPrintVerbose))
		printIt=ETrue;
	if (printIt)
		{
		VA_LIST list;
		VA_START(list,aFmt);
		TBuf<0xff> buf(0xff);
		buf.FillZ(0xff);
		buf.Zero();
		buf.AppendFormatList(aFmt,list);
		HeadedPrintf(buf);
		}
	}

void CConsoleReporter::HeadedPrintf(TPtrC aBuf)
	{
	if (SingleShot==EFalse)
		{
		SingleShot=ETrue;
		CConsoleBase::Printf(KOpltranVersionDetails);
		}
	CConsoleBase::Printf(aBuf);
	}

LOCAL_D CConsoleReporter* theConsole=NULL;
LOCAL_D COplModuleTranslator *theTranslator=NULL;
LOCAL_D CTextTranslator::TDebugFlag theDebugFlag=CTextTranslator::ERelease;

// Unicode resource file handling.
class RResourceFileU : public RResourceFile
	{
public:
	void ReadL(TDes16& aDes,TInt aResourceId) const;
	};

void RResourceFileU::ReadL(TDes16& aDes, TInt aResourceId) const
	{
	TPtr8 readBuffer((TText8* ) aDes.Ptr(),aDes.MaxSize());
	TRAPD(err,RResourceFile::ReadL(readBuffer,aResourceId));
	if (err!=KErrNone)
		User::Leave(err);
	if (readBuffer.Length()%2!=0)
		User::Leave(KErrCorrupt);
	aDes.SetLength((readBuffer.Length()+1)>>1);
	}

LOCAL_C TInt SetOutputPath(const TParse& aSourceFile)
// Build the OutputPath based on the optional command line token,
// filling in the missing bits from the source filename.
	{
	TParse SourceAllButExt;
	TFileName sourceName=aSourceFile.Name();
	SourceAllButExt.Set(aSourceFile.DriveAndPath(),&sourceName,NULL);
	// OutputPath construction.
	TParse OutputDrvAndPathParse, OutputParse;
	OutputDrvAndPathParse.Set(OutputPath,NULL,NULL);
	OutputParse.Set(OutputDrvAndPathParse.DriveAndPath(),&(SourceAllButExt.FullName()),NULL);
	OutputPath=OutputParse.FullName();
	return (ETrue);
	}

LOCAL_C void ForceTrailingSlash(TDes &aPath)
	{
	_LIT(KTrailingSlash,"\\");
	const TPtrC Slash=KTrailingSlash();
	if (!((aPath.Length())&&(aPath.Right(1)==Slash)))
		aPath.Append(Slash);
	}

/*
 *  Convert the relative path of a filename to an absolute path.
 *  e.g. ..\src\fred.opl in J:\TEST\GROUP becomes J:\TEST\src\fred.opl
 *  [A bit convoluted, because unlike DOS, F32 can't cope with 
 *  'j:\test\group\..\src\fred.opl' as a valid filename.]
 */
LOCAL_C void ConvertRelativePathToAbsolute(TDes& aFileName)
	{
	// Ensure argument is valid
	_LIT(KRelativePathHeader,"..\\");
	const TInt rlen=KRelativePathHeader().Length(); // Save my fingers.
	if (aFileName.Length()<rlen)
		return;

	// Filename should start with "..\"
	if (aFileName.Left(rlen)!=KRelativePathHeader())
		return;

	TParse cwd;
	cwd.Set(CurrentWorkingDirectory,NULL,NULL);
	while (aFileName.Left(rlen)==KRelativePathHeader())
		{
		aFileName.Delete(0,rlen);
		cwd.PopDir();
		}

	// There must be no more relative folders...
	if (aFileName.Find(KRelativePathHeader) != KErrNotFound)
		{
		_LIT(KParsingCommandLine,"parsing relative path");
		ShowError(KParsingCommandLine,KErrPathNotFound);
		}

	// Prepend the truncated working directory to the filename.
	aFileName.Insert(0,cwd.DriveAndPath());
	}


LOCAL_C TBool ParseCommandLineL(TParse &aParse)
// Gets the command line from source. Returns ETrue if there is a possible
// OPL filename on the command line and EFalse otherwise.
// Sets aParse to the OPL filename if one is found.
//
// Drives&Paths			Location		Default		Used for locating:
// ExeDrive				EXE file		c:			RSC
// (oplfile drive)		OPL filename	ExeDrive	OPL
// OutputPath drive		-o arg			opl drive	OPO and debug
// IncludePath drive	-i arg			opl drive	OPH
	{
	TBool ret=EFalse;
	
	TBuf<0x100> cmd;
	RProcess().CommandLine(cmd);
	cmd.Trim();
	TLex iLex(cmd);

	TFileName OplName;

	FOREVER
		{
		TPtrC token = iLex.NextToken();
		if (token.Length()==0) // No more tokens.
			break;
		_LIT(KSwitchDelimiters,"-/");
		TBufC<2> switchDelimiters(KSwitchDelimiters);
		if (switchDelimiters.Find(token.Left(1))!=KErrNotFound)
			{
			TBool ValidArgument=EFalse;
			if (token.Length()>1)
				{
				HBufC* theSwitch;
				theSwitch = HBufC::NewL(token.Length()-1);
				*theSwitch = token.Mid(1);
				_LIT(KIncludePathSwitch,"i");
				_LIT(KOutputDirectorySwitch,"o");
				_LIT(KSymbolicDebugSwitch,"s");
				_LIT(KAlternativeErrorFormatSwitch,"e");
				_LIT(KConvertSwitch,"conv");
				_LIT(KQuietSwitch,"q");
				_LIT(KVerboseSwitch,"v");
				_LIT(KEpocRoot,"epocroot");
				if (theSwitch->Left(1).CompareF(KIncludePathSwitch)==0)
					{ // The Include path.
					if (theSwitch->Length()>1)
						{
						IncludePath=theSwitch->Mid(1);
						ForceTrailingSlash(IncludePath);
						ValidArgument=ETrue;
						}
					}
				else if (theSwitch->Left(1).CompareF(KOutputDirectorySwitch)==0)
					{ // Output directory.
					if (theSwitch->Length()>1)
						{
						OutputPath=theSwitch->Mid(1);
						ForceTrailingSlash(OutputPath);
						ValidArgument=ETrue;
						}
					}
				else if (theSwitch->Left(1).CompareF(KSymbolicDebugSwitch)==0)
					{ // Symbolic debug flag
					theDebugFlag=CTextTranslator::EDebug;
					ValidArgument=ETrue;
					}
				else if (theSwitch->CompareF(KAlternativeErrorFormatSwitch)==0)
					{ // alternate error format flag
					ErrorFormatFlag=EErrorFormatMSD;
					ValidArgument=ETrue;
					}
				else if (theSwitch->CompareF(KConvertSwitch)==0)
					{ // Convert flag
					ConvertFlag=EConvert;
					ValidArgument=ETrue;
					}
				else if (theSwitch->CompareF(KVerboseSwitch)==0)
					{
					theConsole->SetVerbose();
					ValidArgument=ETrue;
					}
				else if (theSwitch->CompareF(KQuietSwitch)==0)
					{ 
					theConsole->SetQuiet();
					ValidArgument=ETrue;
					}
// Workround for [1206638] Unable to build OPLTRAN with Nokia 7710 SDK
// If -epocroot<value> is given on command line, it overrides the value
// taken from the OPLTRAN.EXE filename. This allows OPLTRAN from one
// dev kit to be used with another. See \opl\opltools\group\release.txt
				else if (theSwitch->Left(8).CompareF(KEpocRoot)==0)
					{ 
					if (theSwitch->Length()>8)
						{
						EpocRoot=theSwitch->Mid(8);
						ForceTrailingSlash(EpocRoot);
						ValidArgument=ETrue;
						RemoteUse=ETrue;
						}
					}
				}

			if (!ValidArgument)
				{
				_LIT(KErrInvalidArgIgnored,"Invalid argument %S ignored.\n");
				theConsole->Printf(EPrintError,KErrInvalidArgIgnored,&token);
				}
			}
		else
			{ // the OPL program name.
			if (OplName.Length()==0)
				OplName=token;
			else
				{
				// Skip further program names.
				_LIT(KErrFileNamesIgnored,"Other filename %S ignored.\n");
				theConsole->Printf(EPrintError,KErrFileNamesIgnored,&token);
				}
			}
		}

	if (OplName.Length())
		{
		// Pluck the drive letter from the process filename.
		// Note that if the filename doesn't have a drive letter, it must be
		// running on the same drive as the SDK, so use the drive letter from
		// the current working directory.
		TFileName exe=RProcess().FileName();
		TParse ParseExe;
		ParseExe.Set(exe,NULL,NULL);

		ConvertToUnicodeL(CurrentWorkingDirectory,TPtrC8(CurrentWorkingDirectory8));
		ForceTrailingSlash(CurrentWorkingDirectory);

		if (ParseExe.Drive().Length())
			{
			ExeDrive=ParseExe.Drive();
			//
			_LIT(KVerboseExeDrive,"[ExeDrive=%S]\n");
			theConsole->Printf(EPrintVerbose,KVerboseExeDrive,&ExeDrive);
			}
		else
			{
			TParse ParseCWD;
			ParseCWD.Set(CurrentWorkingDirectory,NULL,NULL);
			ExeDrive=ParseCWD.Drive();
			//
			_LIT(KVerboseCwdSupplied,"[cwd supplied ExeDrive=%S]\n");
			theConsole->Printf(EPrintVerbose,KVerboseCwdSupplied,&ExeDrive);
			}

		// Convert relative pathname to absolute.
		ConvertRelativePathToAbsolute(OplName);

		// Now massage the command line arguments...
		if (ConvertFlag!=ENoConvert)
			{
			// However, lose the default .opl extension for convert option
			// This allows command-lines such as "opltran -conv *" to convert Toolbar to Toolbar.opl
			// (it may be nice to lose this altogether in a future version).
			aParse.Set(OplName,NULL,&CurrentWorkingDirectory);
			}
		else
			{
			_LIT(KAnyOpl,"*.opl");
			const TPtrC anyOpl(KAnyOpl);
			aParse.Set(OplName,&anyOpl,&CurrentWorkingDirectory);
			}

		InputPath=aParse.DriveAndPath();
		SetOutputPath(aParse);
		// IncludePath construction.
		TParse IncludePathParse;
		TFileName parsePath=aParse.DriveAndPath();
		IncludePathParse.Set(IncludePath,&parsePath,NULL);
		IncludePath=IncludePathParse.FullName();

		_LIT(KVerboseSource,"[source=%S]\n");
		_LIT(KVerboseInputPath,"[InputPath=%S]\n");
		_LIT(KVerboseIncludePath,"[IncludePath=%S]\n");
		_LIT(KVerboseCwd,"[cwd=%S]\n");
		_LIT(KVerboseOutputPath,"[OutputPath=%S]\n");
		_LIT(KVerboseER,"[EpocRoot=%S]\n");
		theConsole->Printf(EPrintVerbose,KVerboseSource,&aParse.FullName());
		theConsole->Printf(EPrintVerbose,KVerboseInputPath,&InputPath);
		theConsole->Printf(EPrintVerbose,KVerboseIncludePath,&IncludePath);
		theConsole->Printf(EPrintVerbose,KVerboseCwd,&CurrentWorkingDirectory);
		theConsole->Printf(EPrintVerbose,KVerboseOutputPath,&OutputPath);
		theConsole->Printf(EPrintVerbose,KVerboseER,&EpocRoot);

		ret = ETrue;
		}
	else
		{
		_LIT(KNoOPLFileName,"[No OPL filename recognised]\n");
		theConsole->Printf(EPrintVerbose,KNoOPLFileName);
		}
	return ret;
	}

LOCAL_C TInt GetOplErrResource(const TInt aError)
	// Return the resource id with R_OTRN_UNKNOWN_ERROR if unknown
	{
	TInt resourceNo=R_OTRN_UNKNOWN_ERROR;
 	if (aError<0&&aError>-26)
		resourceNo=R_OTRN_UNKNOWN_ERROR-aError; // NB aError is negative here
	else if (aError<-31&&aError>=KOplMaxErr) // may be more errors added !
		resourceNo-=(aError+6); // -32 is mapped to -26 etc.
	return resourceNo;
	}

LOCAL_C TInt MapTranslatorError(const TInt aErr,TInt& aResId)
// Return the mapped error and set aResId to OPLTRAN's resource number or R_OTRN_UNKNOWN_ERROR
// If an OPL-specific error the high word has KOplErrorMask ORed in to avoid clashes with E32 errors
// If not mapped the passed error code is returned unchanged
	{
	const TInt KOpltErrBase=-70+EErrMissingQuote;	// tran errors in same order as for Opl1993 but start at 2
	TBool oplError=ETrue;
	TInt map;
	switch (aErr)
		{
	case EErrMissingQuote:
	case EErrBadStringLength:
	case EErrBadIdentifier:
	case EErrIdentifierTooLong:
	case EErrBadLogicalDevice:
	case EErrBadFieldName:
	case EErrBadNumber:
	case EErrBadSuffices:
	case EErrBadCharacter:
	case EErrFnArgument:
	case EErrTypeMismatch:
	case EErrMissingLabel:
	case EErrDuplicateName:
	case EErrBadDeclaration:
	case EErrBadArraySize:
	case EErrStructure:
	case EErrUnexpectedEof:
	case EErrSyntax:
	case EErrMismatchedBracket:
	case EErrBadFieldList:
	case EErrTooComplex:
	case EErrMissingComma:
	case EErrDataTooLarge:
	case EErrArgNotReferencedByValue:
	case EErrSubscript:
	case EErrInconsistentArgs:
		map=(KOpltErrBase-aErr);
		break;
	case EErrLineTooLong:
		map=KOplErrRecord;
		break;
	case EErrAssignToConstant:
		map=KOplErrBadAssignment;
		break;
	case EErrIncludeProcedure:
		map=KOplErrIncludeProcedure;
		break;
	case EErrTooManyOPXs:
		map=KOplErrTooManyOPXs;
		break;
	case EErrTooManyOpxFunctions:
		map=KOplErrTooManyOpxFunctions;
		break;
	case EErrUndefinedVariable:
		map=KOplErrUndefinedVariable;
		break;
	case EErrUndefinedProcedure:
		map=KOplErrUndefinedProcedure;
		break;
	case EErrIconMaskMissing:
		map=KOplErrIconMaskMissing;
		break;
	case EErrIncompatibleDeclaration:
		map=KOplErrIncompatibleDeclaration;
		break;
	default:
		// must be E32 error
		oplError=EFalse;
		map=aErr;
		// map=OplUtil::MapError(aErr);
		}
	if (oplError)
		{
		aResId=GetOplErrResource(TInt16(map));
		map=OPLERR(map);
		}
	else
		aResId=R_OTRN_UNKNOWN_ERROR;
	return map;
	}

LOCAL_C TInt GetTranslatorErrorResourceNumber(TInt aErr)
	{
	TInt resNo=0;
	MapTranslatorError(aErr,resNo);
	return resNo;
	}

LOCAL_C void GetResourceFileNameL(TDes& aFileName)
	{
	TFileName exe=RProcess().FileName();
	TParse ParseExe;
	ParseExe.Set(exe,NULL,NULL);

	aFileName=ParseExe.DriveAndPath();
	ForceTrailingSlash(aFileName);
	_LIT(KDataPath,"z\\system\\data\\");
	aFileName.Append(KDataPath);
	}

/*
 *  Find the right filename for the Eikcore.rsc resource file.
 *  When running 'remotely' i.e. not on the current drive, 
 *  the filename will be .EXE pathname + z\system\data\ + filename.
 *  e.g. for d:\rtools\opltran\opltran.exe, it will be
 *	 "d:\rtools\opltran\z\system\data\eikcore.rsc"
 *  and for 'local' use, e.g. on Z drive, it will be 
 *   "z:\epoc32\release\wins\udeb\z\system\data\eikcore.rsc"
 *  This is because only that one file is copied from the WINS folder
 *  into the new 'winc\udeb' folder, when creating a 'remote' version 
 *  of OPLTRAN.EXE.
 */
LOCAL_C void GetEikResourceFileNameL(TDes& aFileName)
	{
	if (!RemoteUse)
		{
		aFileName=EpocRoot;
		aFileName.Append(KEikResourceFile);
		}
	else
		{
		// Otherwise, it's going to the remote one, which has been copied under WINC.
		GetResourceFileNameL(aFileName);
		aFileName.Append(KEikResourceFileName);
		}
	}

LOCAL_C void ReadResourceFileL(TDes& aDes, const TInt& aResourceId,const TInt &aFileType)
	{
	TFileName fileName;
	
	TInt signature;
	if(aFileType==EOplResource)
		{
		GetResourceFileNameL(fileName);
		fileName.Append(KOpltranResourceFile);
		signature=KOpltranResourceSignature;
		}
		else
		{
		GetEikResourceFileNameL(fileName);
		signature=KEikResourceSignature;
		}
	BaflUtils::NearestLanguageFile(theFileServer,fileName);
	TParse resourceFile;
	resourceFile.Set(fileName,&ExeDrive,NULL);

	RResourceFileU rsc;
	TRAPD(r,rsc.OpenL(theFileServer,(resourceFile.FullName())));
	if (r!=KErrNone)
		{
		_LIT(KUnableToOpenResource,"\nUnable to open resource file %S\n");
		theConsole->Printf(EPrintError,KUnableToOpenResource,&(resourceFile.FullName()));
		}
		else
		{
		// !! Need cleanup protection around here?
		TRAPD(ss,rsc.ConfirmSignatureL(signature));
		if (ss!=KErrNone)
			; 
		else
			{
			TRAPD(rr,rsc.ReadL(aDes,aResourceId));
			if (rr!=KErrNone)
				{
				// Can't use ShowError() here.
				_LIT(KErrorReadingResource,"Error %d reading resource file %S\n");
				theConsole->Printf(EPrintError,KErrorReadingResource,rr,&(resourceFile.FullName()));
				}
			}
		}	
	rsc.Close();
	}

LOCAL_C TInt SystemResourceFileOffset()
	{
	TFileName rscFileName;
	GetEikResourceFileNameL(rscFileName);
	BaflUtils::NearestLanguageFile(theFileServer,rscFileName);
	TInt offset=0;
	RResourceFile rsc;
	TRAPD(r,rsc.OpenL(theFileServer,rscFileName));
	if (r==KErrNone)
		{
		TRAPD(ss,rsc.ConfirmSignatureL(KEikResourceSignature));
		if (ss==KErrNone)
			{
			offset=rsc.Offset();
			}
		}
	rsc.Close();
	return(offset);
	}

LOCAL_C void GetTranslateErrorText(TDes& aDes,TInt aError)
	{
	if (aError<0)
		{
		if (aError>KErrNotFound || aError<KErrDirFull)
			aError=KErrUnknown;
		TRAPD(r,ReadResourceFileL(aDes,SystemResourceFileOffset()+1-aError,EEikResource));
		if(r!=KErrNone)
			{
			_LIT(KErrorReadingEikonResource,"Error %d reading Eikon resource file\n");
			theConsole->Printf(EPrintError,KErrorReadingEikonResource,r);
			}
		}
	else
		{ // must be a translator internal error
		TRAPD(rr,ReadResourceFileL(aDes,GetTranslatorErrorResourceNumber(aError),EOplResource));
		if (rr!=KErrNone)
			{
			_LIT(KErrorReadingOPLTResource,"Error %d reading OPLT resource file\n");
			theConsole->Printf(EPrintError,KErrorReadingOPLTResource,rr);
			}
		}
	}

LOCAL_C void ShowError(const TDesC& aMsg,TInt aErr,const TDesC& aExtra/*=KNullDesC*/)
// Reports errors in opltran.exe, *not* translation errors in OPL source.
	{
	TBuf<128> ErrorDes;
	GetTranslateErrorText(ErrorDes,aErr);
	_LIT(KErrorString,"Error %S %S: %S\n");
	theConsole->Printf(EPrintError,KErrorString,&aMsg,&aExtra,&ErrorDes);
	}

#ifdef _ERROR_FILE
LOCAL_C void SetErrorFileName(const TDesC& aName,const TFileName& aPath)
	{
	ErrorFileDetails.Set(aName,&aPath,NULL);
	}

LOCAL_C void KillErrorFile()
	{
	TInt r=theFileServer.Delete(ErrorFileDetails.FullName());
	if ((r!=KErrNone) && (r!=KErrNotFound))
		{
		_LIT(KDeletingErrFile,"deleting err file");
		ShowError(KDeletingErrFile,r,ErrorFileDetails.FullName());
		}
	}	

LOCAL_C TInt OpenErrorFile()
	{
	TUint Mode=EFileShareExclusive|EFileStreamText|EFileWrite;
	TInt r=ErrorFile.Replace(theFileServer,ErrorFileDetails.FullName(),Mode);
	
	if (r!=KErrNone)
		{
		_LIT(KCreatingErrFile,"creating err file");
		ShowError(KCreatingErrFile,r,ErrorFileDetails.FullName());
		return r;
		}

	ErrorFileOpen=ETrue;
	return r;
	}

LOCAL_C TInt LogError(const TDesC& aText)
// Attempt to write the error text to the error file.
	{
	if (ErrorFileOpen==EFalse)
		{
		TInt r=OpenErrorFile();
		if (r!=KErrNone)
			return r;
		}
		
	TInt r=ErrorFile.Write(aText);
	if (r!=KErrNone)
		{
		_LIT(KWritingErrFile,"writing err file");
		ShowError(KWritingErrFile,r,ErrorFileDetails.FullName());
		return r;
		}

	r=ErrorFile.Flush();
	if (r!=KErrNone)
		{
		_LIT(KFlushingErrFile,"flushing err file");
		ShowError(KFlushingErrFile,r,ErrorFileDetails.FullName());
		return r;
		}
	return r;
	}

LOCAL_C void TransError(TRefByValue<const TDesC> aFmt,...)
// Errors detected by translator. They are written to error file.
	{
	VA_LIST list;
	VA_START(list,aFmt);
	aFileBuf.Zero();
	aFileBuf.AppendFormatList(aFmt,list);
	TInt r=LogError(aFileBuf);
	if (r==KErrNone)
		{
		// Give the user the bad news.
		_LIT(KTranslationFailed,"Translation failed.\n");
		theConsole->Write(KTranslationFailed);
		}
	else
		theConsole->PError(aFileBuf); // Can't write to file, so stick it on the console.
	}
#endif

LOCAL_C void ConvertFile(const TDesC& aSource)
	{
	_LIT(KConverting,"Converting %S\n");
	theConsole->Printf(EPrintNormal,KConverting,&aSource);

	MTextSource* textSrc;
	TFileName fileName=aSource;
	TInt err=theTextSystem.OpenSource(fileName,textSrc);
	if (err!=KErrNone)
		{
		_LIT(KInConversion,"in conversion");
		ShowError(KInConversion,err);
		return;
		}

	TRAPD(r,((CFileSource*)textSrc)->ConvertL(theFileServer,aSource));

	if (r!=KErrNone)
		{
		_LIT(KConvertError,"convert error");
		ShowError(KConvertError,r);
		}
	}

LOCAL_C void TranslateFile(const TDesC& aSource)
	{
	if (ConvertFlag!=ENoConvert)
		{
		ConvertFile(aSource);
		return;
		}
	_LIT(KTranslating,"Translating %S\n");
	theConsole->Printf(EPrintNormal,KTranslating,&aSource);
	TParse output;
	_LIT(KOpo,".opo");
	TInt r=theFileServer.Parse(KOpo,OutputPath,output);

	if (r!=KErrNone)
		{
		_LIT(KParsingOutputFileName,"parsing output file name");
		ShowError(KParsingOutputFileName,r,output.FullName());
		}
	else
		{
		TSourceTranslateError anErr;
		TRAP(r,theTranslator->StartTranslateL(theTextSystem,aSource,output.FullName(),theDebugFlag,anErr));
		if (r!=KErrNone)
			{
			_LIT(KInStartTranslate,"in StartTranslate");
			ShowError(KInStartTranslate,r);
			}
		else
			{
			do
				{
				r=theTranslator->TranslateChunk();
				} while (r==KErrNone);
			if (r!=KErrGeneral || anErr.Error()!=EErrSuccess)
				{
#if !defined(__UIQ__)
				ExitValue=EXIT_FAILURE;
#endif
				TBuf<128> ErrorDes;
				GetTranslateErrorText(ErrorDes,anErr.Error());

				MTextSource* textSrc;
				TFileName fileName=anErr.Source();
				if (theTextSystem.OpenSource(fileName,textSrc)!=KErrNone)
					{
					_LIT(KTranslatingFailed,"\n%S : Translating failed with Error %d, at byte offset %d\n");
					_LIT(KErrorLocationFailed,"Failed to open %S to locate error\n");
					theConsole->Printf(EPrintError,KTranslatingFailed,&anErr.Source(),anErr.Error(),anErr.Position());
					theConsole->Printf(EPrintError,KErrorLocationFailed,&anErr.Source());
					}
				else
					{
					TInt line=0;
					TInt offset=0;
					TRAP(r,((CFileSource*)textSrc)->MapPosToLineAndOffsetL(theFileServer,anErr.Source(),
						anErr.Position(),line,offset));

					if (r!=KErrNone)
						{
						_LIT(KMappingTranslatorError,"mapping translator error");
						ShowError(KMappingTranslatorError,r);
						}
					else
						{
						if (ErrorFormatFlag==EErrorFormatODE)
							{
							// ODE format "Error LOC::C:\TEMP\TEST.OPL 5: Duplicate Name"
							_LIT(KODEFormatError,"Error LOC::%S %d: %S\n");
							theConsole->Printf(EPrintError,KODEFormatError,&anErr.Source(),line,&ErrorDes);
							}
						else
							{
							// MSDEV etc format. "C:\TEMP\TEST.OPL(5) : error 19 : column 18 : Syntax error"
							_LIT(KMSDevFormatError,"%S(%d) : error %d : column %d : %S\n");
							theConsole->Printf(EPrintError,KMSDevFormatError,&anErr.Source(),line,anErr.Error(),offset,&ErrorDes);
							}
						}
					}
				}
			else
				{
				_LIT(KTranslatedOK,"%S translated OK\n");
				theConsole->Printf(EPrintNormal,KTranslatedOK,&aSource);
				}
			}		
		}
	}

LOCAL_D void Translate(TParse& aSource)
// Checks for wild-cards and does the necessary
	{
	if (!aSource.IsWild())
		TranslateFile(aSource.FullName());
	else
		{
		RDir dir;
		// Note that opening "*." will not supply un-extensioned files (like Toolbar) 
		// - a bug in Open?)
		TInt r=dir.Open(theFileServer,aSource.FullName(),KEntryAttNormal);

		if (r!=KErrNone)
			{
			_LIT(OpendingDir,"opening directory");
			ShowError(OpendingDir,r);
			}
		else
			{
			TEntry entry;
			TInt TryOne=EFalse;
			while (dir.Read(entry)==KErrNone)
				{
				TryOne=ETrue;
				// Lose any .* wildcard extensions
				theFileServer.Parse(entry.iName,aSource.DriveAndPath(),aSource);

				// Set output path to match.
				SetOutputPath(aSource);
				TranslateFile(aSource.FullName());
				}
			if (TryOne==EFalse)
				{
				_LIT(LocatingSource,"locating source");
				ShowError(LocatingSource,KErrNotFound,aSource.FullName());
				}
			}
		}
	}

class MAppCallBack : public MTranAppCB
	{
public:
	void OplTargetIsAppL(TFileName& aName);
	};

void MAppCallBack::OplTargetIsAppL(TFileName& aName) // the call back.
	{
	// The generated app filename will be
	// EPOCROOT plus KDefaultAppPath (epoc32\wins\c\System\Apps\.app)
	
	TFileName defaultAppPath(EpocRoot);
	defaultAppPath.Append(KDefaultAppPath);

	TParse appParse;
	User::LeaveIfError(appParse.SetNoWild(defaultAppPath,&aName,NULL));
	User::LeaveIfError(appParse.AddDir(appParse.Name()));
	aName=appParse.FullName();

	TFileName parseName=appParse.Name();
	TParse AllButExt;
	AllButExt.Set(appParse.DriveAndPath(),&parseName,NULL);
	
	_LIT(KVerboseOutputting,"[Outputting to %S]\n");
	theConsole->Printf(EPrintVerbose,KVerboseOutputting,&(AllButExt.FullName()));
	}

_LIT(KOldRoot,"epoc32\\");

// _LIT(KTest,"K:\\Crystal6.0\\test\\sdk\\epoc32\\ver1\\epoc32\\release\\winc\\...");

TPtrC FindEpocRootL(const TDesC& aExeName)
	{
	TInt next=0;
	TInt index=0;
	TInt remainder=0;
	TInt step=0;
	TPtrC search(aExeName);
//	search.Set(KTest);
	while ((next=search.FindF(KOldRoot))!=KErrNotFound)
		{
		step=next+KOldRoot().Length();
		index+=step;
		remainder=search.Length()-step;
		if (!(remainder>0))
			break;
		search.Set(search.Right(remainder));
		}
	if (!index) // not found
		User::Leave(KErrBadName);
	return(aExeName.Left(index-KOldRoot().Length()));
	}

void DoE32MainL()
	{

	TParse src;

	_LIT(KConsoleName,"OPLTRAN\n");
	// Console::NewL() leaves with 'Not supported' on UIQ WINSCW.
	TRAPD(r, theConsole=REINTERPRET_CAST(CConsoleReporter*,Console::NewL(KConsoleName,TSize(KDefaultConsWidth,KDefaultConsHeight))));
#if !defined(__UIQ__)
	if (r!=KErrNone)
		User::Beep(440,5);
	else
#endif
		{
		// Do this after the console is created - need somewhere sensible  
		// for the output to go when debugging.
		TRAP(r,EpocRoot=FindEpocRootL(RProcess().FileName()));
		if (r!=KErrNone)
			{
			// EPOCROOT not found in process path, assume it's "\"
			_LIT(KSlash,"\\");
			EpocRoot=KSlash;
			}
		TRAP(r,_WingetcwdL());
		if (r!=KErrNone)
			{
			_LIT(KGetCWD,"using _getcwd()");
			ShowError(KGetCWD,r);
			}

		r=theFileServer.Connect();
		if (r!=KErrNone)
			{
			_LIT(KFileServer,"connecting to FileServer");
			ShowError(KFileServer,r);
			}

		r=theTextSystem.Connect();
		if (r!=KErrNone)
			{
			_LIT(KTextSystem,"connecting to TextSystem");
			ShowError(KTextSystem,r);
			}
		FbsStartup(); // icon embedding during AIF construction.
		RFbsSession::Connect();
		TRAP(r,theTranslator=COplModuleTranslator::NewL())
		if (r!=KErrNone)
			{
			_LIT(KAllocTranslator,"allocating the translator");
			ShowError(KAllocTranslator,r);
			}
		else
			{
			TRAP(r,theTranslator->SetTarget(EOplTargetER1));
			if (r!=KErrNone)
				{
				_LIT(KSettingTarget,"setting translator target");
				ShowError(KSettingTarget,r);
				}
			else
				{
				MAppCallBack myAppCB;
				TRAP(r,theTranslator->SetAppCB(&myAppCB));
				if (r!=KErrNone)
					{
					_LIT(KSettingCallback,"setting app callback");
					ShowError(KSettingCallback,r);
					}
				else if (ParseCommandLineL(src)) // Passed something plausible on the command line 
					{
#ifdef _ERROR_FILE
					SetErrorFileName(KErrName,InputPath);
					KillErrorFile();
#endif
					Translate(src);
#ifdef _ERROR_FILE
					if (ErrorFileOpen)
						ErrorFile.Close();
#endif
					}
				else
					{

					_LIT(KUsageLine1,"Usage: OPLTRAN infile [-ipath] [-opath] [-s] [-e] [-q] [-v]\n");
					_LIT(KUsageLine2,"      infile may include wild cards e.g. *.opl\n");
					_LIT(KUsageLine3,"      -ipath specifies path for INCLUDE files\n");
					_LIT(KUsageLine4,"      -opath sets the output directory for OPO files\n");
					_LIT(KUsageLine5,"      -s generates symbolic debugging info\n");
					_LIT(KUsageLine6,"      -e use alternate error format\n");
					_LIT(KUsageLine7,"      -q quiet (only report errors)\n");
					_LIT(KUsageLineA,"      -epocrootpath passes EPOCROOT to the tool\n");
					_LIT(KUsageLine8,"    Convertor functions (no translation)\n");
					_LIT(KUsageLine9,"      -conv converts ASCII files to Unicode (and vice versa)\n");
					_LIT(KUsageDebugLine1,"      -v verbose (overrides -q)\n");
					//
					theConsole->Printf(EPrintNormal,KUsageLine1);
					theConsole->Printf(EPrintNormal,KUsageLine2);
					theConsole->Printf(EPrintNormal,KUsageLine3);
					theConsole->Printf(EPrintNormal,KUsageLine4);
					theConsole->Printf(EPrintNormal,KUsageLine5);
					theConsole->Printf(EPrintNormal,KUsageLine6);
					theConsole->Printf(EPrintNormal,KUsageLine7);
					theConsole->Printf(EPrintNormal,KUsageLineA);
#ifdef _DEBUG
					theConsole->Printf(EPrintNormal,KUsageDebugLine1);
#endif
					theConsole->Printf(EPrintNormal,KUsageLine8);
					theConsole->Printf(EPrintNormal,KUsageLine9);
					}
				}
			}
		delete theTranslator;
		}

	RFbsSession::Disconnect();
	theTextSystem.Close();
	theFileServer.Close(); 

	delete theConsole;
	}

GLDEF_C TInt E32Main()
	{
	CTrapCleanup *trapCleanup=CTrapCleanup::New();
	TRAPD(err,DoE32MainL());
	delete trapCleanup;
#if !defined(__UIQ__)
	if (err)
		ExitValue=EXIT_FAILURE;
 	exit(ExitValue);
#endif
	// no need to return a value as execution never reaches this line.
	return err;
	}

