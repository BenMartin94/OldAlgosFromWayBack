#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	int rank = 0;
	int nproc = 1;
	
	std::cout << "Hello from processor " << rank << " of " << nproc << std::endl;

	return 0;
}
