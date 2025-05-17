#!/usr/bin/env bats

COMPILER="${BATS_TEST_DIRNAME}/../compile"
TARGET_C="${BATS_TEST_DIRNAME}/../target.c"
PYTHON_DIR="${BATS_TEST_DIRNAME}/python/"
RETURN_VALUE_DIR="${BATS_TEST_DIRNAME}/return_value/"

#
# Figure out what the name of the llc executable is (assuming a limited number
# of options for the common environments in which we'll be building this code
# for this course).
#
WHICH_LLC_70=$(which llc-7.0 || true)
WHICH_LLC_13=$(which llc-13 || true)
if [ -n "$WHICH_LLC_70" ]; then
	LLC=llc-7.0
elif [ -n "$WHICH_LLC_13" ]; then
	LLC=llc-13
else
	LLC=llc
fi


#
# This function uses the compiler toolchain (i.e. the solution to this
# assignment) to generate an LLVM IR file (llfile, from argument $2) from the
# input python file (pyfile, from argument $1).  Then, it compiles the LLVM IR
# file into an object file (objfile, from argument $3) using llc.  Finally, it
# compiles the object file along with target.c to generate an executable
# (target_exe, from argument $4).
#
do_compilation() {
	local pyfile="$1"
	local llfile="$2"
	local objfile="$3"
	local target_exe="$4"

	"${COMPILER}" < "${pyfile}" > "${llfile}"
	"${LLC}" -filetype=obj -o="${objfile}" "${llfile}"
	gcc "${TARGET_C}" "${objfile}" -o "${target_exe}"
}


#
# This function cleans up the artifacts of compilation.
#
cleanup_compilation() {
	local llfile="$1"
	local objfile="$2"
	local target_exe="$3"

	rm -f "${llfile}" "${objfile}" "${target_exe}"
}


@test "LLVM IR representing correct computation generated for straightline_1" {
	filename=straightline_1
	pyfile="${PYTHON_DIR}/${filename}.py"
	return_value_file="${RETURN_VALUE_DIR}/${filename}"
	llfile="${BATS_TMPDIR}/${filename}.ll"
	objfile="${BATS_TMPDIR}/${filename}.o"
	target_exe="${BATS_TMPDIR}/target"

	#
	# Compile to a target executable and then run that executable and compare
	# its output to the expected output (stored in the file represented by
	# return_value_file).
	#
	do_compilation "$pyfile" "$llfile" "$objfile" "$target_exe"
	run "${target_exe}"
	expected=$(cat "$return_value_file")
	echo "output: $output"
	echo "expected: $expected"
	[ "$output" = "$expected" ]
	cleanup_compilation "$llfile" "$objfile" "$target_exe"
}



@test "LLVM IR representing correct computation generated for straightline_2" {
	filename=straightline_2
	pyfile="${PYTHON_DIR}/${filename}.py"
	return_value_file="${RETURN_VALUE_DIR}/${filename}"
	llfile="${BATS_TMPDIR}/${filename}.ll"
	objfile="${BATS_TMPDIR}/${filename}.o"
	target_exe="${BATS_TMPDIR}/target"

	#
	# Compile to a target executable and then run that executable and compare
	# its output to the expected output (stored in the file represented by
	# return_value_file).
	#
	do_compilation "$pyfile" "$llfile" "$objfile" "$target_exe"
	run "${target_exe}"
	expected=$(cat "$return_value_file")
	echo "output: $output"
	echo "expected: $expected"
	[ "$output" = "$expected" ]
	cleanup_compilation "$llfile" "$objfile" "$target_exe"
}



@test "LLVM IR representing correct computation generated for straightline_3" {
	filename=straightline_3
	pyfile="${PYTHON_DIR}/${filename}.py"
	return_value_file="${RETURN_VALUE_DIR}/${filename}"
	llfile="${BATS_TMPDIR}/${filename}.ll"
	objfile="${BATS_TMPDIR}/${filename}.o"
	target_exe="${BATS_TMPDIR}/target"

	#
	# Compile to a target executable and then run that executable and compare
	# its output to the expected output (stored in the file represented by
	# return_value_file).
	#
	do_compilation "$pyfile" "$llfile" "$objfile" "$target_exe"
	run "${target_exe}"
	expected=$(cat "$return_value_file")
	echo "output: $output"
	echo "expected: $expected"
	[ "$output" = "$expected" ]
	cleanup_compilation "$llfile" "$objfile" "$target_exe"
}



@test "LLVM IR representing correct computation generated for straightline_4" {
	filename=straightline_4
	pyfile="${PYTHON_DIR}/${filename}.py"
	return_value_file="${RETURN_VALUE_DIR}/${filename}"
	llfile="${BATS_TMPDIR}/${filename}.ll"
	objfile="${BATS_TMPDIR}/${filename}.o"
	target_exe="${BATS_TMPDIR}/target"

	#
	# Compile to a target executable and then run that executable and compare
	# its output to the expected output (stored in the file represented by
	# return_value_file).
	#
	do_compilation "$pyfile" "$llfile" "$objfile" "$target_exe"
	run "${target_exe}"
	expected=$(cat "$return_value_file")
	echo "output: $output"
	echo "expected: $expected"
	[ "$output" = "$expected" ]
	cleanup_compilation "$llfile" "$objfile" "$target_exe"
}



@test "LLVM IR representing correct computation generated for straightline_5" {
	filename=straightline_5
	pyfile="${PYTHON_DIR}/${filename}.py"
	return_value_file="${RETURN_VALUE_DIR}/${filename}"
	llfile="${BATS_TMPDIR}/${filename}.ll"
	objfile="${BATS_TMPDIR}/${filename}.o"
	target_exe="${BATS_TMPDIR}/target"

	#
	# Compile to a target executable and then run that executable and compare
	# its output to the expected output (stored in the file represented by
	# return_value_file).
	#
	do_compilation "$pyfile" "$llfile" "$objfile" "$target_exe"
	run "${target_exe}"
	expected=$(cat "$return_value_file")
	echo "output: $output"
	echo "expected: $expected"
	[ "$output" = "$expected" ]
	cleanup_compilation "$llfile" "$objfile" "$target_exe"
}
