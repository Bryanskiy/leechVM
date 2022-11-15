.PHONY: all

all:
	$(MAKE) -C frontend
	g++ driver/driver.cc objFile/objFile.cc frontend/parser.tab.cc frontend/lex.yy.cc main.cc -o main.out