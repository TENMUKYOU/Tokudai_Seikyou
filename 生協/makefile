PROJECTNAME   = AttendanceManagement
COMPILER      = gcc
SRC_EXTENSION = c
OBJ_EXTENSION = o

DLL = \
	felicalib.dll\
	SDL2.dll

INCLUDE_PATH = \
	-IC:/include

PRODUCTS = .

ObjRoot=.\_obj
rDir=$(shell dir /a:d)
Sources=$(foreach var,$(rDir),$(wildcard $(var)/*.$(SRC_EXTENSION)))
h=$(foreach var,$(rDir),$(wildcard $(var)/*.h))
header=$(h) $(hpp)
headerPath=$(addprefix -I,$(sort $(foreach var,$(header),$(dir $(var)))))
Objects=$(addprefix $(ObjRoot)/,$(Sources:.$(SRC_EXTENSION)=.o))


$(ObjRoot)/%.$(OBJ_EXTENSION): ./%.$(SRC_EXTENSION)
	@echo Updated : $<
	@$(COMPILER) -c $< $(headerPath) $(INCLUDE_PATH) -o $@

$(PRODUCTS)/$(PROJECTNAME): $(Objects)
	@echo Uniting...
	@$(COMPILER) $^ $(headerPath) $(INCLUDE_PATH) $(DLL) -o $@
	@echo Execute : .\$@.exe
	@echo (keep this current directory)
	@echo Done!

.PHONY: clean
clean:
	@del /q $(PRODUCTS)\$(PROJECTNAME).exe $(ObjRoot)\