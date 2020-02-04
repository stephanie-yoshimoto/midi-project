#
# Change the HW variable to match the homework
#
NUM	= 3
TEST_DIR= test_data_files
CLEAN	= NoSec_* NoCut_* Grade_* Result_* $(TEST_DIR)/*

HW	= hw$(NUM)
CLASS	= /homes/cs240
PROJ	= $(CLASS)/public/homework/$(HW)

CC	= gcc
CFLAGS	= -g -Wall -Werror -std=c99 -I$(CLASS)/public/include
LDFLAGS	= -L$(CLASS)/public/lib -lmd -lm

all:: $(HW)_main
# Uncomment when test files are released
all:: $(HW)_test
#all:: git

$(HW)_main : $(HW)_main.o $(HW).o git Makefile
	$(CC) -o $(HW)_main $(HW)_main.o $(HW).o $(LDFLAGS)

$(HW)_test : $(PROJ)/$(HW)_test.o $(HW).o git Makefile
	@if [ ! -d $(TEST_DIR) ]; then mkdir $(TEST_DIR); fi
	$(CC) -o $(HW)_test $(PROJ)/$(HW)_test.o $(HW).o $(LDFLAGS)

$(HW).o : Makefile $(HW).h
$(HW)_main.o : Makefile $(HW).h

Makefile : $(PROJ)/Makefile
#	rm -f Makefile
# 	cp $(PROJ)/Makefile .
	echo  

$(HW).h : $(PROJ)/$(HW).h
# 	rm -f $(HW).h
	cp $(PROJ)/$(HW).h .
#	echo

Input :# $(PROJ)/Input
# 	rm -f $@
# 	cp $(PROJ)/Input .
	echo

$(HW)_main.c : $(PROJ)/$(HW)_main.c
# 	rm -f $@
	cp $(PROJ)/$(HW)_main.c .
	#echo

#
# Extra targets to do interesting things...
#
clean :
	rm -f core *.o $(HW)_main $(HW)_test $(CLEAN)

git:
		git checkout master >> .local.git.out || echo
		git add *.c >> .local.git.out || echo
		git add *.h >> .local.git.out || echo
		git commit -a -m "Commit homework $(HW)" >> .local.git.out || echo
		git push origin master

submit:
		git checkout master >> .local.git.out || echo
		git add *.c *.h  >> .local.git.out || echo
		git commit --allow-empty -a -m "Submission for homework $(HW)" >> .local.git.out || echo
		git push origin master

verify:
		~cs240/bin/sub-verify $(HW)
