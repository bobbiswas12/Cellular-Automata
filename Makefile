build:
	gcc stochastic-cellular-automata/main.c -o stochastic-cellular_automata -lraylib -lm -lpthread -ldl -lrt
	gcc we-ca/main.c -o elementry-ca -lraylib -lm -lpthread -ldl -lrt
