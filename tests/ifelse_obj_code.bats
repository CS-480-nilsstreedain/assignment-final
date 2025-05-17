#!/usr/bin/env bats

COMPILER="${BATS_TEST_DIRNAME}/../compile"
TARGET_C="${BATS_TEST_DIRNAME}/../target.c"
PYTHON_DIR="${BATS_TEST_DIRNAME}/python/"
RETURN_VALUE_DIR="${BATS_TEST_DIRNAME}/return_value/"


#
# This function uses the compiler toolchain (i.e. the solution to this
# assignment) to generate an object code file (objfile, from argument $2) from
# the input python file (pyfile, from argument $1).  Then, it compiles that
# object file along with target.c to generate an executable (target_exe, from
# argument $3).
#
do_compilation() {
	local pyfile="$1"
	local objfile="$2"
	local target_exe="$3"

	"${COMPILER}" "${objfile}" < "${pyfile}" > /dev/null
	gcc "${TARGET_C}" "${objfile}" -o "${target_exe}"
}


#
# This function cleans up the artifacts of compilation.
#
cleanup_compilation() {
	local objfile="$1"
	local target_exe="$2"

	rm -f "${objfile}" "${target_exe}"
}


@test "Object code representing correct computation generated for ifelse_1" {
	filename=ifelse_1
	pyfile="${PYTHON_DIR}/${filename}.py"
	return_value_file="${RETURN_VALUE_DIR}/${filename}"
	objfile="${BATS_TMPDIR}/${filename}.o"
	target_exe="${BATS_TMPDIR}/target"

	#
	# Compile to a target executable and then run that executable and compare
	# its output to the expected output (stored in the file represented by
	# return_value_file).
	#
	do_compilation "$pyfile" "$objfile" "$target_exe"
	run "${target_exe}"
	expected=$(cat "$return_value_file")
	echo "output: $output"
	echo "expected: $expected"
	[ "$output" = "$expected" ]
	cleanup_compilation "$objfile" "$target_exe"
}



@test "Object code representing correct computation generated for ifelse_2" {
	filename=ifelse_2
	pyfile="${PYTHON_DIR}/${filename}.py"
	return_value_file="${RETURN_VALUE_DIR}/${filename}"
	objfile="${BATS_TMPDIR}/${filename}.o"
	target_exe="${BATS_TMPDIR}/target"

	#
	# Compile to a target executable and then run that executable and compare
	# its output to the expected output (stored in the file represented by
	# return_value_file).
	#
	do_compilation "$pyfile" "$objfile" "$target_exe"
	run "${target_exe}"
	expected=$(cat "$return_value_file")
	echo "output: $output"
	echo "expected: $expected"
	[ "$output" = "$expected" ]
	cleanup_compilation "$objfile" "$target_exe"
}



@test "Object code representing correct computation generated for ifelse_3" {
	filename=ifelse_3
	pyfile="${PYTHON_DIR}/${filename}.py"
	return_value_file="${RETURN_VALUE_DIR}/${filename}"
	objfile="${BATS_TMPDIR}/${filename}.o"
	target_exe="${BATS_TMPDIR}/target"

	#
	# Compile to a target executable and then run that executable and compare
	# its output to the expected output (stored in the file represented by
	# return_value_file).
	#
	do_compilation "$pyfile" "$objfile" "$target_exe"
	run "${target_exe}"
	expected=$(cat "$return_value_file")
	echo "output: $output"
	echo "expected: $expected"
	[ "$output" = "$expected" ]
	cleanup_compilation "$objfile" "$target_exe"
}



@test "Object code representing correct computation generated for ifelse_4" {
	filename=ifelse_4
	pyfile="${PYTHON_DIR}/${filename}.py"
	return_value_file="${RETURN_VALUE_DIR}/${filename}"
	objfile="${BATS_TMPDIR}/${filename}.o"
	target_exe="${BATS_TMPDIR}/target"

	#
	# Compile to a target executable and then run that executable and compare
	# its output to the expected output (stored in the file represented by
	# return_value_file).
	#
	do_compilation "$pyfile" "$objfile" "$target_exe"
	run "${target_exe}"
	expected=$(cat "$return_value_file")
	echo "output: $output"
	echo "expected: $expected"
	[ "$output" = "$expected" ]
	cleanup_compilation "$objfile" "$target_exe"
}



@test "Object code representing correct computation generated for ifelse_5" {
	filename=ifelse_5
	pyfile="${PYTHON_DIR}/${filename}.py"
	return_value_file="${RETURN_VALUE_DIR}/${filename}"
	objfile="${BATS_TMPDIR}/${filename}.o"
	target_exe="${BATS_TMPDIR}/target"

	#
	# Compile to a target executable and then run that executable and compare
	# its output to the expected output (stored in the file represented by
	# return_value_file).
	#
	do_compilation "$pyfile" "$objfile" "$target_exe"
	run "${target_exe}"
	expected=$(cat "$return_value_file")
	echo "output: $output"
	echo "expected: $expected"
	[ "$output" = "$expected" ]
	cleanup_compilation "$objfile" "$target_exe"
}
