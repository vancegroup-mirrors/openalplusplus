MAKE= gmake

alpp:
	cd src && $(MAKE) alpp
tests:
	cd tests && $(MAKE) all
docs:   
	doxygen Doxyfile
clean:
	rm -f *.o core core.* *~
	cd src && $(MAKE) clean
	cd include && $(MAKE) clean
	cd tests && $(MAKE) clean
