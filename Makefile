all:
	for impl in functions/*; do \
		name=`basename $$impl .h`; \
                echo $$name;\
		fname="\"`readlink -f .`/bin/$$name.rec\""; \
		gcc -I. -DFNAME=$$fname  -DFN="\"$$impl\"" -g -O1 record.c -lm -ldl -shared -fPIC -o bin/record_$$name.so; \
		gcc -I. -DFNAME=$$fname -DFN="\"$$impl\"" -g replay.c -lm -o bin/replay_$$name; \
		gcc -I. -DFNAME=$$fname  -DFN="\"$$impl\"" -g bench.c -lm -o bin/bench_$$name; \
		gcc -I. -DFNAME=$$fname  -DFN="\"$$impl\"" -g show.c -o bin/show_$$name; \
		gcc -I. -DFNAME=$$fname  -DFN="\"$$impl\"" -g summary.c -o bin/summary_$$name; \
	done
