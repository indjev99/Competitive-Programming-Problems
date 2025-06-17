SOLUTIONS:=hristo hristo2 hristo3 encho encho-no-pc encho-no-pcsz emo emo-no-pc emo-no-pcsz emo-slow emo-slow-no-pc emo-slower naive kirchoff wrong-kirchoff

all: $(SOLUTIONS:%=bin/%) $(SOLUTIONS:%=bin/%.asan) bin/check-test

bin/%: src/%.cpp
	@mkdir -p bin
	g++ -Wall -Wextra -std=gnu++17 -O2 $< -o $@
bin/%.asan: src/%.cpp
	@mkdir -p bin
	g++ -Wall -Wextra -std=gnu++17 -g -Og -fsanitize=address,undefined $< -o $@
nmst.pdf: statement/nmst.tex
	cd statement; latexmk
	cp -T statement/build/nmst.pdf nmst.pdf
