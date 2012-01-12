# Microsoft Developer Studio Project File - Name="Menu" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Menu - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Menu.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Menu.mak" CFG="Menu - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Menu - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Menu - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Menu - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "Menu - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Menu - Win32 Release"
# Name "Menu - Win32 Debug"
# Begin Group "SessionManager"

# PROP Default_Filter ""
# Begin Group "include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SessionManager\include\AlgebraicObject.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\BaseProperties.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\ComputationManager.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\DatabaseManager.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\DCM.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\DIC.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\FEData.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\InformationCenter.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\Menu.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\MIC.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\ObjectFactory.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\ObjectSmith.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\OID.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\OutMessages.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\Property.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\RandomDefinitionsGenerator.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\ResourceManager.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\SessionManager.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\SMBaseOfDataSet.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\SMDataSet.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\SMDataSetComputationProblems.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\SMDescriptionOfDataSet.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\Supervisor.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\TheObjects.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\ViewContents.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\viewStructure.h
# End Source File
# End Group
# Begin Group "src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SessionManager\src\BaseProperties.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\ComputationManager.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\DatabaseManager.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\Dcm.c
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\Dic.c
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\FEData.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\hacks.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\Menu.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\Mic.c
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\ObjectFactory.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\ObjectSmith.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\OutMessages.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\Property.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\RandomDefinitionsGenerator.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\registerProperties.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\ResourceManager.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\SessionManager.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\SMBaseOfDataSet.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\SMDataSet.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\SMDataSetComputationProblems.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\SMDescriptionOfDataSet.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\Supervisor.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\TheObjects.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\ViewContents.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\viewStructure.C
# End Source File
# End Group
# Begin Group "test"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SessionManager\test\magnus.C
# End Source File
# End Group
# End Group
# Begin Group "Statistics"

# PROP Default_Filter ""
# Begin Group "include No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Statistics\include\DataSet.h
# End Source File
# Begin Source File

SOURCE=.\Statistics\include\OneVariableDataSet.h
# End Source File
# Begin Source File

SOURCE=.\Statistics\include\QuickSort.h
# End Source File
# Begin Source File

SOURCE=.\Statistics\include\statsFunctions.h
# End Source File
# End Group
# Begin Group "src No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Statistics\src\DataSet.C
# End Source File
# Begin Source File

SOURCE=.\Statistics\src\OneVariableDataSet.C
# End Source File
# Begin Source File

SOURCE=.\Statistics\src\QuickSort.C
# End Source File
# Begin Source File

SOURCE=.\Statistics\src\statsFunctions.C
# End Source File
# End Group
# End Group
# Begin Group "SMApps"

# PROP Default_Filter ""
# Begin Group "include No. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SMApps\include\DataProblems.h
# End Source File
# Begin Source File

SOURCE=.\SMApps\include\fastProblems.h
# End Source File
# Begin Source File

SOURCE=.\SMApps\include\OVDSAnalysisOfMeanProblem.h
# End Source File
# Begin Source File

SOURCE=.\SMApps\include\OVDSAnalysisOfVarianceProblem.h
# End Source File
# Begin Source File

SOURCE=.\SMApps\include\OVDSComparisionWithMeanProblem.h
# End Source File
# Begin Source File

SOURCE=.\SMApps\include\OVDSComparisionWithVarianceProblem.h
# End Source File
# Begin Source File

SOURCE=.\SMApps\include\OVDSComputationOfAnIntervalProblem.h
# End Source File
# Begin Source File

SOURCE=.\SMApps\include\OVDSFullDataAnalysisProblem.h
# End Source File
# Begin Source File

SOURCE=.\SMApps\include\OVDSFurtherAnalysisProblem.h
# End Source File
# Begin Source File

SOURCE=.\SMApps\include\OVDSGoodnessOfFitProblem.h
# End Source File
# Begin Source File

SOURCE=.\SMApps\include\OVDSGraphicalAnalysisProblem.h
# End Source File
# Begin Source File

SOURCE=.\SMApps\include\OVDSReportProblem.h
# End Source File
# End Group
# Begin Group "src No. 2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\SMApps\src\DataProblems.C
# End Source File
# Begin Source File

SOURCE=.\SMApps\src\fastProblems.C
# End Source File
# Begin Source File

SOURCE=.\SMApps\src\menuDefns.C
# End Source File
# Begin Source File

SOURCE=.\SMApps\src\OVDSAnalysisOfMeanProblem.C
# End Source File
# Begin Source File

SOURCE=.\SMApps\src\OVDSAnalysisOfVarianceProblem.C
# End Source File
# Begin Source File

SOURCE=.\SMApps\src\OVDSComparisionWithMeanProblem.C
# End Source File
# Begin Source File

SOURCE=.\SMApps\src\OVDSComparisionWithVarianceProblem.C
# End Source File
# Begin Source File

SOURCE=.\SMApps\src\OVDSComputationOfAnIntervalProblem.C
# End Source File
# Begin Source File

SOURCE=.\SMApps\src\OVDSFullDataAnalysisProblem.C
# End Source File
# Begin Source File

SOURCE=.\SMApps\src\OVDSFurtherAnalysisProblem.C
# End Source File
# Begin Source File

SOURCE=.\SMApps\src\OVDSGoodnessOfFitProblem.C
# End Source File
# Begin Source File

SOURCE=.\SMApps\src\OVDSGraphicalAnalysisProblem.C
# End Source File
# Begin Source File

SOURCE=.\SMApps\src\OVDSReportProblem.C
# End Source File
# End Group
# End Group
# Begin Group "General"

# PROP Default_Filter ""
# Begin Group "include No. 3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\general\include\AdaptSimpson.h
# End Source File
# Begin Source File

SOURCE=.\general\include\Associations.h
# End Source File
# Begin Source File

SOURCE=.\general\include\BlackBox.h
# End Source File
# Begin Source File

SOURCE=.\general\include\BTree.h
# End Source File
# Begin Source File

SOURCE=.\general\include\Cell.h
# End Source File
# Begin Source File

SOURCE=.\general\include\Chars.h
# End Source File
# Begin Source File

SOURCE=.\general\include\conversions.h
# End Source File
# Begin Source File

SOURCE=.\general\include\DArray.h
# End Source File
# Begin Source File

SOURCE=.\general\include\DCell.h
# End Source File
# Begin Source File

SOURCE=.\general\include\DList.h
# End Source File
# Begin Source File

SOURCE=.\general\include\File.h
# End Source File
# Begin Source File

SOURCE=.\general\include\GCD.h
# End Source File
# Begin Source File

SOURCE=.\general\include\Int2.h
# End Source File
# Begin Source File

SOURCE=.\general\include\IStreamPoll.h
# End Source File
# Begin Source File

SOURCE=.\general\include\KripkeStructure.h
# End Source File
# Begin Source File

SOURCE=.\general\include\KState.h
# End Source File
# Begin Source File

SOURCE=.\general\include\List.h
# End Source File
# Begin Source File

SOURCE=.\general\include\LogWatcher.h
# End Source File
# Begin Source File

SOURCE=.\general\include\MagnusHome.h
# End Source File
# Begin Source File

SOURCE=.\general\include\Pair.h
# End Source File
# Begin Source File

SOURCE=.\general\include\Proba.h
# End Source File
# Begin Source File

SOURCE=.\general\include\QuickAssociations.h
# End Source File
# Begin Source File

SOURCE=.\general\include\Random.h
# End Source File
# Begin Source File

SOURCE=.\general\include\RandomNumbers.h
# End Source File
# Begin Source File

SOURCE=.\general\include\Regex.h
# End Source File
# Begin Source File

SOURCE=.\general\include\Set.h
# End Source File
# Begin Source File

SOURCE=.\general\include\Stack.h
# End Source File
# Begin Source File

SOURCE=.\general\include\String.h
# End Source File
# Begin Source File

SOURCE=.\general\include\Timer.h
# End Source File
# Begin Source File

SOURCE=.\general\include\Type.h
# End Source File
# Begin Source File

SOURCE=.\general\include\Vector.h
# End Source File
# Begin Source File

SOURCE=.\general\include\WordParser.h
# End Source File
# End Group
# Begin Group "src No. 3"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\general\src\AdaptSimpson.C
# End Source File
# Begin Source File

SOURCE=.\general\src\Array.C
# End Source File
# Begin Source File

SOURCE=.\general\src\Associations.C
# End Source File
# Begin Source File

SOURCE=.\general\src\BlackBox.C
# End Source File
# Begin Source File

SOURCE=.\general\src\BTree.C
# End Source File
# Begin Source File

SOURCE=.\general\src\Cell.C
# End Source File
# Begin Source File

SOURCE=.\general\src\Chars.C
# End Source File
# Begin Source File

SOURCE=.\general\src\DArray.C
# End Source File
# Begin Source File

SOURCE=.\general\src\DList.C
# End Source File
# Begin Source File

SOURCE=.\general\src\Gcd.c
# End Source File
# Begin Source File

SOURCE=.\general\src\GenericObject.C
# End Source File
# Begin Source File

SOURCE=.\general\src\KripkeStructure.C
# End Source File
# Begin Source File

SOURCE=.\general\src\KState.C
# End Source File
# Begin Source File

SOURCE=.\general\src\List.C
# End Source File
# Begin Source File

SOURCE=.\general\src\LogWatcher.C
# End Source File
# Begin Source File

SOURCE=.\general\src\MagnusHome.C
# End Source File
# Begin Source File

SOURCE=.\general\src\Proba.C
# End Source File
# Begin Source File

SOURCE=.\general\src\Random.C
# End Source File
# Begin Source File

SOURCE=.\general\src\RandomNumbers.C
# End Source File
# Begin Source File

SOURCE=.\general\src\Set.C
# End Source File
# Begin Source File

SOURCE=.\general\src\Stack.C
# End Source File
# Begin Source File

SOURCE=.\general\src\Trichotomy.C
# End Source File
# Begin Source File

SOURCE=.\general\src\Type.C
# End Source File
# Begin Source File

SOURCE=.\general\src\Vector.C
# End Source File
# Begin Source File

SOURCE=.\general\src\WordParser.C
# End Source File
# End Group
# End Group
# Begin Group "global"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\global\BaseObjectOf.h
# End Source File
# Begin Source File

SOURCE=.\global\config.h
# End Source File
# Begin Source File

SOURCE=.\global\DerivedObjectOf.h
# End Source File
# Begin Source File

SOURCE=.\global\error.h
# End Source File
# Begin Source File

SOURCE=.\global\ExtendedIPC.h
# End Source File
# Begin Source File

SOURCE=.\global\GenericObject.h
# End Source File
# Begin Source File

SOURCE=.\global\global.h
# End Source File
# Begin Source File

SOURCE=.\global\IPC.h
# End Source File
# Begin Source File

SOURCE=.\global\Object.h
# End Source File
# Begin Source File

SOURCE=.\global\ObjectOf.h
# End Source File
# Begin Source File

SOURCE=.\global\PureRep.h
# End Source File
# Begin Source File

SOURCE=.\global\RefCounter.h
# End Source File
# Begin Source File

SOURCE=.\global\Trichotomy.h
# End Source File
# End Group
# Begin Group "front_end"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\front_end\databaseBrowser.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\databaseHtmlConverter.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\databaseMain.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\databaseMessages.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\databaseParser.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\databaseUtility.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\dataSetDialogs.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\dataSetProblems.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\dialog.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\dialogs.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\editing.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\fileDialogs.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\files.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\general.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\global.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\greeting.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\help.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\helpStrings.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\magnus
# End Source File
# Begin Source File

SOURCE=..\front_end\magnus.in
# End Source File
# Begin Source File

SOURCE=..\front_end\messages.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\nodeGraph.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\options.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\problemsDialogs.tcl
# End Source File
# Begin Source File

SOURCE="..\front_end\tcltk-fixes.tcl"
# End Source File
# Begin Source File

SOURCE=..\front_end\testing.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\tmp.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\updateMenus.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\viewParameters.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\views.tcl
# End Source File
# Begin Source File

SOURCE=..\front_end\workspace.tcl
# End Source File
# End Group
# Begin Source File

SOURCE=.\global\Afx.h
# End Source File
# Begin Source File

SOURCE=.\Statistics\src\Distributions.C
# End Source File
# Begin Source File

SOURCE=.\Statistics\include\Distributions.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\SMDataSetComputation.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\SMDataSetComputation.h
# End Source File
# Begin Source File

SOURCE=.\SessionManager\src\SMObject.C
# End Source File
# Begin Source File

SOURCE=.\SessionManager\include\SMObject.h
# End Source File
# End Target
# End Project
