#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

void addItem(char writeChar[], int fp);
void removeItem(int fp);

int main (int argc, char *argv[]) {
      int fp;
      ssize_t bytes;

      printf("OPEN\n");
      fp = open("/dev/poly_list", O_RDWR);

      addItem("a", fp);		// Enqueue 'a'
      addItem("b", fp);		// Enqueue 'b'
      addItem("c", fp);		// Enqueue 'c'
      addItem("d", fp);		// Enqueue 'd'
      addItem("e", fp);		// Enqueue 'e'

      removeItem(fp);		// Dequeue 'a'
      removeItem(fp);		// Dequeue 'b'
      removeItem(fp);		// Dequeue 'c'
      removeItem(fp);		// Dequeue 'd'

      ioctl(fp, 1, (void *)NULL);	// Change to stack

      removeItem(fp);		// Dequeue 'e'

      ioctl(fp, 1, (void *)NULL);

      addItem("a", fp);		// Push 'a'
	  addItem("b", fp);		// Push 'b'
	  addItem("c", fp);		// Push 'c'
	  addItem("d", fp);		// Push 'd'

	  removeItem(fp);		// Pop 'd'
	  removeItem(fp);		// Pop 'c'
	  removeItem(fp);		// Pop 'b'
	  removeItem(fp);		// Pop 'a'

      close(fp);
      printf("CLOSE\n\n");
}

void addItem(char writeChar[], int fp)
{
	write(fp, writeChar, sizeof &(writeChar));
}

void removeItem(int fp)
{
	char readMessage[] = "c";
	int response = read(fp, readMessage, sizeof readMessage);

	if(response == 1)	// List not empty
		printf("Character popped is : %s\n", readMessage);

}
