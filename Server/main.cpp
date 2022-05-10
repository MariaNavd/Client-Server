#define SHARED_MEMORY_KEY ftok("/tmp", 822)
#define SERVER_TIMEOUT 20 // Default timeout value

#include "server.h"
#include "data.h"

int main(int argc, char* argv[])
{
	int timeout = SERVER_TIMEOUT;
	// Handling input arguments
	if (argc > 3)
	{
		cerr << "Too many arguments" << endl;
		return 1;
	}
	else if (argc != 1)
	{
		if (!strcmp(argv[1], "--timeout"))
		{
			if (argc == 2)
			{
				cerr << "Specify server timeout (sec)" << endl;
				return 1;
			}
			else
			{
				timeout = atoi(argv[2]);
				if (timeout <= 0)
				{
					cerr << "Time value assumed to be positive" << endl;
					return 1;
				}
			}
		}
		else
		{
			cerr << "Incorrect input" << endl;
			return 1;
		}
	}

    initData(); // Init data base
    Server* s = new Server(SHARED_MEMORY_KEY, timeout); // Start server
    delete s;
    return 0;
}
