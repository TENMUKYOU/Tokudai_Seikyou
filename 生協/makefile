PROJECTNAME   = AttendanceManagement
COMPILER      = g++
SRC_EXTENSION = cpp
OBJ_EXTENSION = o
FLAGS         =

DLL = \
	SDL2.dll\
	felicalib.dll\
	SDL2_ttf.dll\
	C:\MinGW\bin\SDL2_image.dll

INCLUDE_PATH = \
	-IC:/include\

PRODUCTS = .

ObjRoot=.\\_obj
rDir=$(shell dir -ad)
Sources=$(foreach var,$(rDir),$(wildcard $(var)/*.$(SRC_EXTENSION)))
h=$(foreach var,$(rDir),$(wildcard $(var)/*.h))
header=$(h) $(hpp)
headerPath=$(addprefix -I,$(sort $(foreach var,$(header),$(dir $(var)))))
Objects=$(addprefix $(ObjRoot)/,$(Sources:.$(SRC_EXTENSION)=.o))


$(ObjRoot)/%.$(OBJ_EXTENSION): ./%.$(SRC_EXTENSION)
	@echo Updated : $<
	@$(COMPILER) -c $< $(FLAGS) $(headerPath) $(INCLUDE_PATH) -o $@ -g

$(PRODUCTS)/$(PROJECTNAME): $(Objects)
	@echo Uniting...
	@$(COMPILER) $^ $(FLAGS) $(headerPath) $(INCLUDE_PATH) $(DLL) -o $@ -g
	@echo Execute : .\$@.exe
	@echo (keep this current directory)
	@echo Done!

.PHONY: clean
clean:
	@del -q $(PRODUCTS)\$(PROJECTNAME).exe $(ObjRoot)\