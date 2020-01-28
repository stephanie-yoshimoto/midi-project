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
