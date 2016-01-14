#include <pthread.h>
#include <stdio.h>
#include PathFinderNew.h

/**class threadWatcher {
}*/

class Robot {
	private: 
		tuple<int, int> Location;
		char directionFacing;
		vector<tuple<int,int>> stacksVisited;
		vector<tuple<int,int>> stacksToBeVisited;
		vector<tuple<int,int>> blocksCollected;
		vector<tuple<int,int>> blocksDiscarded;
		vector<tuple<int,int>> locationsVisited; 
		//each time you revisit a position, increase its obstacle prob. value with
		//diminishing returns
		//implement timeout with threads
	
	public:
		//internal
		void updateGrid() {
		
		}
		
		void updatePosition() {
		
		}
		
		void updateDirectionFacing() {
		
		}
		
		void updateCount() {
		
		}
		
		tuple<int, int> getLocation() {
			return Location;
		}
		
		char getDirectionFacing() {
			return directionFacing;
		}
		
		void interrupt() {
			
		}
		
		//sending
		bool changeDirectionFacing () {
		
		}
		
		bool move() {
		
		}
		
		bool randomMovement() {
		
		}
		
		//receiving
		bool getInfoFromCam() {
		
		}
		
		bool getInfoFromIR() {
		
		}
		
}

int main() {
	//makefile?
}

/**
//example code
void *inc_x(void *x_void_ptr) {

	int *x_ptr = (int *)x_void_ptr;
	while(++(*x_ptr) < 100);
	printf("x increment finished\n");
	return NULL;

}

int main() {

int x = 0, y = 0;
	printf("x: %d, y: %d\n", x, y);
	pthread_t inc_x_thread;
	if(pthread_create(&inc_x_thread, NULL, inc_x, &x)) {
		fprintf(stderr, "Error creating thread\n");
		return 1;
	}

	while(++y < 100);
	printf("y increment finished\n");
	if(pthread_join(inc_x_thread, NULL)) {
		fprintf(stderr, "Error joining thread\n");
		return 2;
	}

printf("x: %d, y: %d\n", x, y);
return 0;

}*/
