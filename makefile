all: question1 question2 question3 question4 question5

question1: question1.c
	gcc -Wall -Wextra -std=gnu99 -pthread question1.c -o question1

question2: question2.c
	gcc -Wall -Wextra -std=gnu99 -pthread question2.c -o question2

question3: question3.c
	gcc -Wall -Wextra -std=gnu99 -pthread question3.c -o question3

question4: question4.c
	gcc -Wall -Wextra -std=gnu99 -pthread question4.c -o question4

question5: question5.c
	gcc -Wall -Wextra -std=gnu99 -pthread question5.c -o question5

