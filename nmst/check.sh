#!/bin/bash
set -e

testdir=tests

if [[ "$#" != 0 ]] && [[ -d "$1" ]]; then
	testdir=$1
	shift 1
fi

declare -a tests
mapfile -d '' tests < <(find "$testdir" -name '*.in' -print0)

sols=( "$@" )
if [[ "$#"  == 0 ]]; then
	sols=( ./bin/hristo ./bin/kirchoff ./bin/hristo2 ./bin/hristo3 ./bin/encho ./bin/emo ./bin/emo-slow ./bin/naive )
	sols+=( "${sols[@]/%/.asan}" )
else
	sols=( "$@" )
fi

tmp=tmp.out.$$
for sol in "${sols[@]}"; do
	echo "Running $sol"
	t_max=0
	fail=( )

	for t in "${tests[@]}"; do
		read n m < "$t"
		if [[ "$m" -gt 30 ]] && [[ "$sol" == *naive* ]]; then
			continue
		fi
		rm -f "$tmp"
		t_start=$(date +%s%3N)
		"$sol" < "$t" > "$tmp" || {
			echo "Test $t failed in $sol" >&2 
			fail+=( "$t" )
		}
		t_end=$(date +%s%3N)
		want=${t%.in}.sol
		if [[ ! -f "$want" ]]; then
			cp -T "$tmp" "$want"
		fi
		diff -q "$tmp" "$want" || {
			echo "Test $t error compared to $sol" >&2 
			fail+=( "$t" )
		}
		rm -f "$tmp"
		t=$((t_end-t_start))
		if [[ "$t" -gt "$t_max" ]]; then
			t_max=$t
		fi
	done
	echo "Max runtime: $t_max ms"
	if [[ "$sol" == */wrong-* ]] && [[ "${#fail[@]}" == 0 ]]; then
		echo "Expected failure!" >&2
	fi
done
