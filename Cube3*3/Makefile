
APP_NAME  = Lego
APP_EXT   = 
#Ziel: $(EXEEXT) -> Ziel ist eine Ausfuehrbare Datei
#Ziel: .a        -> Ziel ist eine Static Library (Archiv) 
#Ziel: $(SOEXT)  -> Ziel ist eine Shared Library (Don't forget CFLAG=-fpic)

APP_SRC = main Arm_motor.c Dreh_motor.c Farbe_motor.c Farbe_sen.c Farbe_scan.c Side_dreh.c print_mode.c Top_Cross.c F2L.c OLL.c PLL.c Test.c Color_search.c \
	  ../lms2012/lms2012_auszug.c ../lms2012/d_lcd.c  ../lms2012/c_output.c
		   
#32-Bit ARM-Mode   --> Dateiendung *.c
#ASM-Mode          --> Dateiendung *.s

#Libarys
#Standard-C Library für printf() ... 
#SHLIBS   += -lc
#SHLIBS   += -lm
#SHLIBS   += -lpthread 
#SHLIBS   += -L$(LMSDIR)\lms2012\Linux_AM1808\sys\lib  -lc_ui -lc_memory -lc_com -lc_input -lc_output -lc_sound 
#LIBS     +=   $(LMSDIR)\lms2012\Linux_AM1808\source\validate.o

#APP_LD   = link.ld

#-----------------------------------------------------------------------------
# Globale Einstellungen

#PROJECT_TARGET = DEBUG / DEBUG_SPY / RELEASE
#DEBUG           -> Compilierung für max. Debugger Unterstützung (ohne Codeoptimierung)
#RELEASE         -> Compilierung für min. Debugger Unterstützung (mit Codeoptimierung)
PROJECT_TARGET = DEBUG
#PROJECT_TARGET = RELEASE

#Erzeugung der LST-Files Aktivieren
#LST_FILES = YES / NO
LST_FILES = YES

#Kommandozeilenausgabe ausschalten
$(VERBOSE).SILENT:

#-----------------------------------------------------------------------------
# tools


CC    := arm-none-linux-gnueabi-gcc
CPP   := arm-none-linux-gnueabi-g++
ASM   := arm-none-linux-gnueabi-as
LINK  := arm-none-linux-gnueabi-gcc
BIN   := arm-none-linux-gnueabi-objcopy
NM    := arm-none-linux-gnueabi-nm
RELF  := arm-none-linux-gnueabi-readelf
GDB   := arm-none-linux-gnueabi-gdb
RM    := rm -rf
MKDIR := mkdir 
TOUCH := touch
MV    := mv
SED   := sed
GREP  := grep

#-----------------------------------------------------------------------------
# Verzeichnisse

BLDDIR     = .
BINDIR     = bin
DEPDIR     = dep

#-----------------------------------------------------------------------------
# Abhaengigkeiten

ifeq ($(OS),Windows_NT)
#'mkdir' unter Windows mag kein Slash am Ende einer Pfadangabe, die mit dem Befehle $(dir) am Ende übrig bleibt
WINDIR = $(dir $(subst /,\\,$@))
else
WINDIR = $(dir $@)
endif

#-----------------------------------------------------------------------------
# Compiler-Flags
#-O0           Reduce compilation time and make debugging produce the expected results (default)
#-O -O1        The Compiler tries to reduce code size and execution time, without
#		       performing any optimizations that take a great deal of compilation tim
#-O2		   Optimize even more. GCC performs nearly all supported optimizations that
#              do not involve a space-speed tradeoff. As compared to '-O', this option
#              increases both compilation time and the performance of the generated code
#-O3           Optimize yet more. '-O3' turns on all optimizations specified by '-O2'
#              and also turns on the ....
#-Os		   Optimize for size. '-Os' enables all '-O2' optimizations that do not typically
#              increase code size. It also performs further optimizations designed to 
#              reduce code size
#-Ofast        Disregard strict standards compliance. '-Ofast' enables all '-O3'
#              optimizations. It also enables optimizations that are not valid for all
#              standard edit-compile-debug cycle, offering a reasonable level of optimization
#             while maintaining fast compilation and a good debugging experience.
#-g            Produce debugging information in the operating system’s native format (stabs,
#              COFF, XCOFF, or DWARF 2). GDB can work with this debugging information.
#-W            Ausschalten aller Warnings
#-Wall         Alle Warnings einschalten
#-fno-builtin  Einbettung von Funktionen deaktivieren
#-E            Ergebnis des Preprozessors anzeigen

CCINC   = -I$(BLDDIR) -I../lms2012

#ASMFLAGS  =  -mthumb-interwork 
#CCFLAGS   = -c -mthumb-interwork -mlong-calls -ffunction-sections  
#LINKFLAGS = -Wl,-Map,$(BINDIR)/$(APP_NAME).map,--cref,--gc-sections

#-----------------------------------------------------------------------------
# Versions Abhängigkeiten 

ifeq ($(PROJECT_TARGET),DEBUG)
CCFLAGS  += -Wall -fno-builtin -g -O 
else ifeq ($(PROJECT_TARGET),RELEASE)
CCFLAGS  += -Wall -DNDEBUG
CCFLAGS  += -O2  -g
#CCFLAGS += -Os
else
$(error Wrong Setting PROJECT_TARGET='$(PROJECT_TARGET)' (unknown))
endif


ifeq ($(LST_FILES),YES)
LST_FLAG += -Wa,-adhlns=$(basename $@).lst 
endif

DEP_FLAG  = -MT $@ -MD -MP -MF $(DEPDIR)/$*.Tpo -c

#-----------------------------------------------------------------------------
# Versions Abhängigkeiten

SRCS  =  $(addprefix $(BINDIR)/, $(notdir $(basename $(ASM_SRC)))) \
	 	 $(addprefix $(BINDIR)/, $(notdir $(basename $(APP_SRC)))) 

VPATH =  $(dir $(ASM_SRC)) $(dir $(APP_SRC))
		 
#-----------------------------------------------------------------------------
# Abhängigkeiten

.PHONY:

all: $(BINDIR)/.f $(DEPDIR)/.f $(BINDIR)/$(APP_NAME)$(APP_EXT)

$(BINDIR)/$(APP_NAME).a : $(SRCS:=.o) makefile
	@echo "-> Linking $(notdir $?) --> $(notdir $@)"
	$(AR) rcf  $@ $^
	@echo  "---> Created Static Library: "$@

$(BINDIR)/$(APP_NAME).so : $(SRCS:=.o) makefile
	@echo "-> Linking $(notdir $?) --> $(notdir $@)"
	$(CC) $(LINKFLAGS) -shared -Wl,-soname,$@ -o $@ $(SRCS:=.o) $(LIBS) $(SHLIBS)
	@echo "---> Created Shared Library: "$@

$(BINDIR)/$(APP_NAME): $(SRCS:=.o) makefile
	@echo "-> Linking $(notdir $?) --> $(notdir $@)"
	$(CC) $(LINKFLAGS) -o $@ $(SRCS:=.o) $(LIBS) $(SHLIBS) 
	@echo  "---> Created Executable: "$@
	scp $(BINDIR)/$(APP_NAME) root@192.168.0.101:/media/card
#	$(LINK) $(SHLIBS) -T $(BINDIR)/$(APP_NAME).ld $(LINKFLAGS) -o $@ 

		
-include $(addprefix ./$(DEPDIR)/,$(notdir $(SRCS:=.Po)))        

$(BINDIR)/%.o : $(BLDDIR)/%.c 
	@echo "-> ARM-Compiling $< to $@"
	$(CC) -marm  $(CCFLAGS) $(CCINC) $(LST_FLAG) $(DEP_FLAG) -o$@ $<
	$(MV) -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po

$(BINDIR)/%.o : $(BLDDIR)/%.ct 
	@echo "-> Thumb-Compiling $< to $@"
	$(CC) -mthumb $(CCFLAGS) $(CCINC) $(LST_FLAG) $(DEP_FLAG) -x c -o$@ $<
	$(MV) -f $(DEPDIR)/$*.Tpo $(DEPDIR)/$*.Po
	
.PHONY : clean
clean: 
	@echo "Deleting $(BINDIR)- und $(DEPDIR)- Directorys"
	$(TOUCH) $@
	$(RM) $(BINDIR)
	$(RM) $(DEPDIR)

.PRECIOUS: %/.f
%/.f:
	@echo "-> MkDir  $(dir $@)"
	$(MKDIR) $(WINDIR)
	$(TOUCH) $@

#-------------------------------------------------------------------------------
# GCC: Dependency Generation
#-M          Instead of outputting the result of preprocessing, output a rule 
#            suitable for make describing the dependencies of the main source file. 
#            The preprocessor outputs one make rule containing the object file name 
#            for that source file, a colon, and the names of all the included 
#            files, including those coming from ‘-include’ or ‘-imacros’ command line options.
# -MT target Change the target of the rule emitted by dependency generation.  
#            By default CPP takes the name of the main input file, deletes 
#            any directory components and any file suffix such as ‘.c’, 
#            and appends the platform’s usual object suffix.
# -MF file   When used with ‘-M’ or ‘-MM’, specifies a file to write the dependencies to. 
#            If no ‘-MF’ switch is given the preprocessor sends the rules to the same 
#            place it would have sent preprocessed output.
#            When used with the driver options ‘-MD’ or ‘-MMD’, ‘-MF’ overrides the default
#            dependency output file.
# -MP        This option instructs CPP to add a phony target for each dependency 
#            other than the main file, causing each to depend on nothing. These 
#            dummy rules work around errors make gives if you remove header files 
#            without updating the ‘Makefile’ to match.
# -MD        is equivalent to ‘-M -MF file’, except that ‘-E’ is not implied.
#            The driver determines file based on whether an ‘-o’ option is given. 
#            If it is, the driver uses its argument but with a suffix of ‘.d’, 
#            otherwise it takes the name of the input file, removes any directory 
#            components and suffix, and applies a ‘.d’ suffix. 
#            If ‘-MD’ is used in conjunction with ‘-E’, any ‘-o’ switch is understood 
#            to specify the dependency output file (see [-MF], page 134), but if used
#            without ‘-E’, each ‘-o’ is understood to specify a target object file.
#            Since ‘-E’ is not implied, ‘-MD’ can be used to generate a dependency output
#            file as a side-effect of the compilation process.
# -MMD       Like ‘-MD’ except mention only user header files, not system header files.
# -H         anzeigen der genutzten Include-Pfade
# -mno-cygwin Nutzung der MINGW/INclude Files und Nutzung der OS-Spezifischen DLL's anstatt CYGWIN1.DLL
#-------------------------------------------------------------------------------
# GCC: Linker
#http://linux.web.cern.ch/linux/scientific4/docs/rhel-ld-en-4/win32.html

#-------------------------------------------------------------------------------
# mno-cygwin -- Building Mingw executables using Cygwin
#http://www.delorie.com/howto/cygwin/mno-cygwin-howto.html
#
#-------------------------------------------------------------------------------
# Make: Automatic Variables
# MAKEFLAGS make automatically passes down variable values that were defined on the command line, 
#           by putting them in the MAKEFLAGS variable. See Options/Recursion.
# -xxx      Ignore Error
# @xxx      Supress Echo
# $@        The file name of the target of the rule. If the target is an archive member, 
#           then `$@' is the name of the archive file. In a pattern rule that has multiple 
#           targets, `$@' is the name of whichever target caused the rule's commands to be run. 
# $%        The target member name, when the target is an archive member. 
#           For example, if the target is foo.a(bar.o) then '$%' is bar.o and '$@' is foo.a. 
#           '$%' is empty when the target is not an archive member. 
# $<        The name of the first prerequisite. If the target got its commands from an implicit rule, 
#           this will be the first prerequisite added by the implicit rule (see Implicit Rules). 
# $?        The names of all the prerequisites that are newer than the target, with spaces between them
#           For prerequisites which are archive members, only the member named is used (see Archives). 
# $^        The names of all the prerequisites, with spaces between them. For prerequisites which are 
#           archive members, only the member named is used. A target has only one prerequisite 
#           on each other file it depends on, no matter how many times each file is listed as a 
#           prerequisite. So if you list a prerequisite more than once for a target, the value 
#           of '$^' contains just one copy of the name. This list does not contain any of the 
#           order-only prerequisites; for those see the `$|' variable, below. 
# $+        This is like '$^', but prerequisites listed more than once are duplicated in the 
#           order they were listed in the makefile. This is primarily useful for use in linking 
#           commands where it is meaningful to repeat library file names in a particular order. 
# $|        The names of all the order-only prerequisites, with spaces between them. 
# $*        The stem with which an implicit rule matches. If the target is dir/a.foo.b and the 
#           target pattern is a.%.b then the stem is dir/foo. The stem is useful for constructing 
#           names of related files. In a static pattern rule, the stem is part of the file name
#           that matched the '%' in the target pattern. 
#           In an explicit rule, there is no stem; so `$*' cannot be determined in that way. 
#           Instead, if the target name ends with a recognized suffix, '$*' is set to the target
#           name minus the suffix. For example, if the target name is `foo.c', then `$*' is set 
#           to `foo', since `.c' is a suffix. GNU make does this bizarre thing only for compatibility 
#           with other implementations of make. You should generally avoid using `$*' except in 
#           implicit rules or static pattern rules. 
#           If the target name in an explicit rule does not end with a recognized suffix, '$*' is 
#           set to the empty string for that rule. 
# $?        is useful even in explicit rules when you wish to operate on only the prerequisites that
#           have changed. For example, suppose that an archive named lib is supposed to contain copies 
#           of several object files. This rule copies just the changed object files into the archive: 
#             lib: foo.o bar.o lose.o win.o
#                 ar r lib $?
#           Of the variables listed above, four have values that are single file names, and three have 
#           values that are lists of file names. These seven have variants that get just the file's 
#           directory name or just the file name within the directory. The variant variables' names 
#           are formed by appending `D' or `F', respectively. These variants are semi-obsolete 
#           in GNU make since the functions dir and notdir can be used to get a similar effect. 
#           Note, however, that the `D' variants all omit the trailing slash which always appears 
#           in the output of the dir function. Here is a table of the variants: 
# $(@D)     The directory part of the file name of the target, with the trailing slash removed. If the 
#           value of `$@' is dir/foo.o then `$(@D)' is dir. This value is . if `$@' does not contain a slash. 
# $(@F)     The file-within-directory part of the file name of the target. If the
#           value of '$@' is dir/foo.o then '$(@F)' is foo.o. '$(@F)' is equivalent to `$(notdir $@)'. 
# $(*D) $(*F) The directory part and the file-within-directory part of the stem; dir and foo in this example. 
# $(%D) $(%F) The directory part and the file-within-directory part of the target archive member name. 
#             This makes sense only for archive member targets of the form archive(member) and is useful 
#             only when member may contain a directory name.
# $(<D) $(<F) The directory part and the file-within-directory part of the first prerequisite. 
# $(^D) $(^F) Lists of the directory parts and the file-within-directory parts of all prerequisites. 
# $(+D) $(+F) Lists of the directory parts and the file-within-directory parts of all prerequisites, 
#             including multiple instances of duplicated prerequisites. 
# $(?D) $(?F) Lists of the directory parts and the file-within-directory parts of all prerequisites that
#             are newer than the target.
#-------------------------------------------------------------------------------
# Make: Functions for File Names (http://www.gnu.org/software/make/manual/html_node/File-Name-Functions.html)
# Several of the built-in expansion functions relate specifically to taking apart file names or lists of file names. 
#
# Each of the following functions performs a specific transformation on a file name. The argument of the function 
# is regarded as a series of file names, separated by whitespace. (Leading and trailing whitespace is ignored.) 
# Each file name in the series is transformed in the same way and the results are concatenated with single spaces between them. 
#
# $(dir names...)
# Extracts the directory-part of each file name in names. The directory-part of the file name is everything up through 
# (and including) the last slash in it. If the file name contains no slash, the directory part is the string ‘./’. 
# For example,  $(dir src/foo.c hacks)     --> produces the result ‘src/ ./’
# 
# $(notdir names...)
# Extracts all but the directory-part of each file name in names. If the file name contains no slash, it is left unchanged. 
# Otherwise, everything through the last slash is removed from it. 

# A file name that ends with a slash becomes an empty string. This is unfortunate, because it means that the result does not 
# always have the same number of whitespace-separated file names as the argument had; but we do not see any other valid alternative. 
# For example,  $(notdir src/foo.c hacks)   --> produces the result ‘foo.c hacks’.
#
# $(suffix names...)
# Extracts the suffix of each file name in names. If the file name contains a period, the suffix is everything starting with 
# the last period. Otherwise, the suffix is the empty string. This frequently means that the result will be empty when names 
# is not, and if names contains multiple file names, the result may contain fewer file names. 
# For example,  $(suffix src/foo.c src-1.0/bar.c hacks)  --> produces the result ‘.c .c’.
#
# $(basename names...)
# Extracts all but the suffix of each file name in names. If the file name contains a period, the basename is everything starting 
# up to (and not including) the last period. Periods in the directory part are ignored. If there is no period, the basename is the 
# entire file name. 
# For example,  $(basename src/foo.c src-1.0/bar hacks)  --> produces the result ‘src/foo src-1.0/bar hacks’.
# 
# $(addsuffix suffix,names...)
# The argument names is regarded as a series of names, separated by whitespace; suffix is used as a unit. The value of suffix is 
# appended to the end of each individual name and the resulting larger names are concatenated with single spaces between them. 
# For example, $(addsuffix .c,foo bar)   --> produces the result ‘foo.c bar.c’.
# 
# $(addprefix prefix,names...)
# The argument names is regarded as a series of names, separated by whitespace; prefix is used as a unit. The value of prefix is 
# prepended to the front of each individual name and the resulting larger names are concatenated with single spaces between them. 
# For example, $(addprefix src/,foo bar)  --> produces the result ‘src/foo src/bar’. 
#
# $(join list1,list2)
# Concatenates the two arguments word by word: the two first words (one from each argument) concatenated form the first word 
# of the result, the two second words form the second word of the result, and so on. So the nth word of the result comes from 
# the nth word of each argument. If one argument has more words that the other, the extra words are copied unchanged into the result. 
# For example, ‘$(join a b,.c .o)’  --> produces ‘a.c b.o’. 
#
# $(wildcard pattern)
# The argument pattern is a file name pattern, typically containing wildcard characters (as in shell file name patterns). 
# The result of wildcard is a space-separated list of the names of existing files that match the pattern. See Using Wildcard 
# Characters in File Names. 
#
# $(realpath names...)
# For each file name in names return the canonical absolute name. A canonical name does not contain any . or .. components, 
# nor any repeated path separators (/) or symlinks. In case of a failure the empty string is returned. Consult the realpath(3) 
# documentation for a list of possible failure causes. 
#
# $(abspath names...)
# For each file name in names return an absolute name that does not contain any . or .. components, nor any repeated path 
# separators (/). Note that, in contrast to realpath function, abspath does not resolve symlinks and does not require the 
# file names to refer to an existing file or directory. Use the wildcard function to test for existence.
