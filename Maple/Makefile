.PHONY: all linux windows clean

SRCS = syntax/lexer.c syntax/parser.c syntax/ast.c \
       execution/interpreter.c execution/compiler.c execution/vm.c execution/value.c \
       core/builtin.c core/config_parser.c core/error.c core/runtime.c core/coreilb.c core/shell.c \
       core/util/main.c core/util/logger.c core/util/platform.c core/util/getline.c

linux:
	@gcc -Wall -g -Isyntax -Iexecution -Icore -Icore/util -o maple $(SRCS)

windows:
	@x86_64-w64-mingw32-gcc -Wall -g -Isyntax -Iexecution -Icore -Icore/util -o maple.exe $(SRCS)

all: linux windows

clean:
	@rm -f maple maple.exe
