##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=simpledemo
ConfigurationName      :=Release
IntermediateDirectory  :=./bin
OutDir                 := $(IntermediateDirectory)
WorkspacePath          := "C:\Development\SVN\April Engine\aprilparticle"
ProjectPath            := "C:\Development\SVN\April Engine\aprilparticle\demos"
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Domagoj
Date                   :=11/10/10
CodeLitePath           :="C:\Program Files (x86)\CodeLite"
LinkerName             :=g++
ArchiveTool            :=ar rcus
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
CompilerName           :=g++
C_CompilerName         :=gcc
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
MakeDirCommand         :=makedir
CmpOptions             :=  $(Preprocessors)
LinkOptions            :=  -O2
IncludePath            :=  "$(IncludeSwitch)../../gtypes/include" "$(IncludeSwitch)../../hltypes/include" "$(IncludeSwitch)../../april/include" "$(IncludeSwitch)../include" "$(IncludeSwitch)../../april/aprilutil/include" "$(IncludeSwitch)." 
RcIncludePath          :=
Libs                   :=$(LibrarySwitch)gtypes $(LibrarySwitch)hltypes $(LibrarySwitch)aprilparticle $(LibrarySwitch)april $(LibrarySwitch)DevIL $(LibrarySwitch)aprilutil 
LibPath                := "$(LibraryPathSwitch)." "$(LibraryPathSwitch)../bin" "$(LibraryPathSwitch)../../gtypes/bin" "$(LibraryPathSwitch)../../hltypes/bin" "$(LibraryPathSwitch)../../aprilparticle/bin" "$(LibraryPathSwitch)../../april/bin" "$(LibraryPathSwitch)../../gtypes/include" "$(LibraryPathSwitch)../../hltypes/include" "$(LibraryPathSwitch)../../april/include" "$(LibraryPathSwitch)../include" 


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files (x86)\CodeLite
UNIT_TEST_PP_SRC_DIR:=C:\UnitTest++-1.3
Objects=$(IntermediateDirectory)/main$(ObjectSuffix) 

##
## Main Build Targets 
##
all: $(OutputFile)

$(OutputFile): makeDirStep $(Objects)
	@$(MakeDirCommand) $(@D)
	$(LinkerName) $(OutputSwitch)$(OutputFile) $(Objects) $(LibPath) $(Libs) $(LinkOptions)
	@echo Executing Post Build commands ...
	copy ..\bin\*.dll bin
	copy ..\..\april\bin\*.dll bin
	copy ..\..\gtypes\bin\*dll bin
	copy ..\..\hltypes\bin\*dll bin
	@echo Done

makeDirStep:
	@$(MakeDirCommand) "./bin"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main$(DependSuffix)
	$(CompilerName) $(SourceSwitch) "C:/Development/SVN/April Engine/aprilparticle/demos/main.cpp" $(CmpOptions) $(ObjectSwitch)$(IntermediateDirectory)/main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main$(DependSuffix): main.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) -MT$(IntermediateDirectory)/main$(ObjectSuffix) -MF$(IntermediateDirectory)/main$(DependSuffix) -MM "C:/Development/SVN/April Engine/aprilparticle/demos/main.cpp"

$(IntermediateDirectory)/main$(PreprocessSuffix): main.cpp
	@$(CompilerName) $(CmpOptions) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main$(PreprocessSuffix) "C:/Development/SVN/April Engine/aprilparticle/demos/main.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/main$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) $(OutputFile).exe


