cmd = mpicxx -o
cores = 16
runcmd = mpirun --oversubscribe -v -np $(cores)


p1a = program_1a.cpp
p1b = program_1b.cpp
p1c = program_1c.cpp

p2a = program_2a.cpp
p2b = program_2b.cpp

p3a = program_3a.cpp

all: 1a 1b 1c 2a 2b 3a

1a: $(p1a)
	$(cmd) p1a $(p1a)
	
1b: $(p1b)
	$(cmd) p1b $(p1b)

1c: $(p1c)
	$(cmd) p1c $(p1c) -DHAVEMPI
	g++ -o p1cSerial $(p1c)

2a: $(p2a)
	$(cmd) p2a $(p2a)

2b: $(p2b)
	$(cmd) p2b $(p2b)

3a: $(p3a)
	echo "3a isnt done yet"
	
	
run1a:
	$(runcmd) p1a
	
run1b:
	$(runcmd) p1b

run1c:
	$(runcmd) p1c
	$(runcmd) p1cSerial

run2a:
	$(runcmd) p2a

run2b:
	$(runcmd) p2b
	

