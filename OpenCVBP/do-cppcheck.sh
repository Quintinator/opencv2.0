#!/bin/bash

find ./ \( -type f -name "*.cpp" -o -name "*.hpp" -o -name "*.c" -o -name "*.h" \) -not -path "" | /home/gunther/Downloads/cppcheck-2.13.0/cppcheck 	\
					--template="{file};{line};{severity};{id};{message}" \
					--language=c++	\
					--enable=all \
					--inconclusive	\
					--force --inline-suppr	\
					--std=c11 --std=c++17	\
					--suppress=missingIncludeSystem	\
					--file-list=-

