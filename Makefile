# Makefile for EECS 280 Machine Learning Project

CXX ?= g++
CXXFLAGS = --std=c++11 -Wall -Werror -pedantic -g -fsanitize=address -fsanitize=undefined


all: test

test: BinarySearchTree_compile_check.exe \
		BinarySearchTree_tests.exe \
		BinarySearchTree_public_test.exe \
		Map_compile_check.exe Map_public_test.exe main.exe

	./BinarySearchTree_tests.exe
	./BinarySearchTree_public_test.exe

	./Map_public_test.exe

	./main.exe train_small.csv test_small.csv --debug > test_small_debug.out.txt
	diff -q test_small_debug.out.txt test_small_debug.out.correct

	./main.exe train_small.csv test_small.csv > test_small.out.txt
	diff -q test_small.out.txt test_small.out.correct

	./main.exe w16_projects_exam.csv sp16_projects_exam.csv > projects_exam.out.txt
	diff -q projects_exam.out.txt projects_exam.out.correct

	./main.exe w14-f15_instructor_student.csv w16_instructor_student.csv > instructor_student.out.txt
	diff -q instructor_student.out.txt instructor_student.out.correct

main.exe: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $@

BinarySearchTree_tests.exe: BinarySearchTree_tests.cpp BinarySearchTree.h
	$(CXX) $(CXXFLAGS) $< -o $@

%_public_test.exe: %_public_test.cpp %.h
	$(CXX) $(CXXFLAGS) $< -o $@

%_compile_check.exe: %_compile_check.cpp %.h
	$(CXX) $(CXXFLAGS) $< -o $@

# disable built-in rules
.SUFFIXES:

# these targets do not create any files
.PHONY: clean
clean :
	rm -vrf *.o *.exe *.gch *.dSYM *.stackdump *.out.txt

# Run style check tools
CPD ?= /usr/um/pmd-6.0.1/bin/run.sh cpd
OCLINT ?= /usr/um/oclint-0.13/bin/oclint
FILES := BinarySearchTree.h BinarySearchTree_tests.cpp Map.h main.cpp
style :
	$(OCLINT) \
    -no-analytics \
    -rule=LongLine \
    -rule=HighNcssMethod \
    -rule=DeepNestedBlock \
    -rule=TooManyParameters \
    -rc=LONG_LINE=90 \
    -rc=NCSS_METHOD=40 \
    -rc=NESTED_BLOCK_DEPTH=4 \
    -rc=TOO_MANY_PARAMETERS=4 \
    -max-priority-1 0 \
    -max-priority-2 0 \
    -max-priority-3 0 \
    $(FILES) \
    -- -xc++ --std=c++11
	$(CPD) \
    --minimum-tokens 100 \
    --language cpp \
    --failOnViolation true \
    --files $(FILES)
	@echo "########################################"
	@echo "EECS 280 style checks PASS"
