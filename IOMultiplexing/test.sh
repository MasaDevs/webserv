#!/bin/bash

run_tests() {
	c++ Blocking.cpp -o Blocking
	c++ Non-Blocking.cpp -o Non-Blocking
	c++ select.cpp -o select

	echo "+++++++++++++++++++++++++++++++++++++++++"
	echo
	echo Blocking test
	echo
	./Blocking
	echo "+++++++++++++++++++++++++++++++++++++++++"
	echo
	echo Non-Blocking
	./Non-Blocking
	echo "+++++++++++++++++++++++++++++++++++++++++"
	echo
	echo select
	echo
	./select
}

run_tests



