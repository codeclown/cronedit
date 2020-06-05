GXX ?= g++
OUT ?= cronedit
libs = lib/parser.cpp lib/exec.cpp lib/filesystem.cpp lib/state.cpp lib/render.cpp lib/terminal.cpp lib/stringify.cpp lib/args.cpp

build:
	mkdir -p bin && $(GXX) $(libs) lib/cronedit.cpp -o bin/$(OUT)

test:
	mkdir -p bin && $(GXX) $(libs) lib/args_test.cpp -o bin/args_test && ./bin/args_test
	mkdir -p bin && $(GXX) $(libs) lib/filesystem_test.cpp -o bin/filesystem_test && ./bin/filesystem_test
	mkdir -p bin && $(GXX) $(libs) lib/parser_test.cpp -o bin/parser_test && ./bin/parser_test
	mkdir -p bin && $(GXX) $(libs) lib/state_test.cpp -o bin/state_test && ./bin/state_test
	mkdir -p bin && $(GXX) $(libs) lib/stringify_test.cpp -o bin/stringify_test && ./bin/stringify_test
	mkdir -p bin && $(GXX) $(libs) lib/render_test.cpp -o bin/render_test && ./bin/render_test
