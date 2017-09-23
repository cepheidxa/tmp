#!/usr/bin/env bash

#set -x

source_files=$@

INDENT_PARAM=" -l120"  #set max line lenght 120
INDENT_PARAM="$INDENT_PARAM -ts8  -ut" #set tab size to 8 spaces
INDENT_PARAM="$INDENT_PARAM -bad" #Force blank lines after the declarations
INDENT_PARAM="$INDENT_PARAM -bap" #Force blank lines after procedures bodies
INDENT_PARAM="$INDENT_PARAM -bbb" #Force blank lines before block comments
INDENT_PARAM="$INDENT_PARAM -bbo" #prefer to break long lines before boolean operators
INDENT_PARAM="$INDENT_PARAM -nbc" #Do not force newline after comma in declaration
INDENT_PARAM="$INDENT_PARAM -blf" #put braces on line following function definition line
INDENT_PARAM="$INDENT_PARAM -bli8" #Indent braces n spaces
INDENT_PARAM="$INDENT_PARAM -br" #Put braces on line with if, etc
INDENT_PARAM="$INDENT_PARAM -brs" #Put braces on struct declaration line
INDENT_PARAM="$INDENT_PARAM -c33" #Put comments to the right of code in column n
INDENT_PARAM="$INDENT_PARAM -cd33" #Put comments to the right of the declarations in column n
INDENT_PARAM="$INDENT_PARAM -cp33" #Put comments to the right of #else and #endif statements in  column n
INDENT_PARAM="$INDENT_PARAM -cbi8" #Indent braces after a case label N spaces
INDENT_PARAM="$INDENT_PARAM -cd1" #Put comments to the right of declarations in column n
INDENT_PARAM="$INDENT_PARAM -cdw" #Cuddle while of do {} while; and preceding }
INDENT_PARAM="$INDENT_PARAM -ce" #Cuddle else and preceding ‘}’
INDENT_PARAM="$INDENT_PARAM -ci8" #Continuation indent of n spaces
INDENT_PARAM="$INDENT_PARAM -cli0" #Case label indent of n spaces
INDENT_PARAM="$INDENT_PARAM -lc120" #Set maximum line length for comment formatting to n
INDENT_PARAM="$INDENT_PARAM -nlps" #Remove space between ‘#’ and preprocessor directive
INDENT_PARAM="$INDENT_PARAM -nbfda" #Don’t put each argument in a function  declaration  on  a  separate line
INDENT_PARAM="$INDENT_PARAM -ncs" #Do not put a space after cast operators
INDENT_PARAM="$INDENT_PARAM -npcs" #Do not put space after the function in function calls
INDENT_PARAM="$INDENT_PARAM -nprs" #Do not put a space after every ( and before every )
INDENT_PARAM="$INDENT_PARAM -npsl" #Put the type of a procedure on the same line as its name
INDENT_PARAM="$INDENT_PARAM -sc" #Put the ‘*’ character at the left of comments
INDENT_PARAM="$INDENT_PARAM -i8" #Set offset for labels to column n
INDENT_PARAM="$INDENT_PARAM -il0" #Set offset for labels to column n
INDENT_PARAM="$INDENT_PARAM -saf" #Put a space after each for
INDENT_PARAM="$INDENT_PARAM -sai" #Put a space after each if
INDENT_PARAM="$INDENT_PARAM -saw" #Put a space after each while
INDENT_PARAM="$INDENT_PARAM -sbi8" #Indent braces of a struct, union or enum N spaces
INDENT_PARAM="$INDENT_PARAM -sc" #Put the ‘*’ character at the left of comments
INDENT_PARAM="$INDENT_PARAM -sob" #Swallow optional blank lines
INDENT_PARAM="$INDENT_PARAM -ut" #Use tabs. This is the default
INDENT_PARAM="$INDENT_PARAM -hnl" #Prefer to break long lines at the position of newlines in the input
INDENT_PARAM="$INDENT_PARAM -nfca" #Do not format any comments

function zte_indent()
{
	local source_file=$1
	if [ ! -f $source_file ]; then
		echo "$source_file is not exit"
		exit
	fi

	#dos end line characters
	sed -i 's/\r//g' $source_file

	#end line white space
	sed -i 's/[[:space:]]*$//' $source_file

	#c99 comment to c89 comment, it is processed by indent
	#sed -i 's/\/\/\([\/[:space:]]*\)\(.*\)/\/\* \2 \*\//' $source_file

	indent  $INDENT_PARAM $source_file

	#end line white space
	sed -i 's/[[:space:]]*$//' $source_file
}

for file in $source_files
do
	zte_indent $file
done
