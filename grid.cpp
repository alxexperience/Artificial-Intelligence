#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <math.h>
#include <ctime>
#include <cstdlib>
#include <stdio.h>
using namespace std;

//grid dimensions
const int WIDTH = 100;
const int HEIGHT = 100;

//2x2 grids
static int grid[WIDTH][HEIGHT];
static int directionGrid[WIDTH][HEIGHT];

//general map.
static int map[WIDTH][HEIGHT];

//Open and closed lists
static int oList[WIDTH][HEIGHT];
static int cList[WIDTH][HEIGHT];

//How you are moving along the grid.
//Starts directly right, then goes clockwise.
const int d = 8; //Directions
static int directions[WIDTH][HEIGHT];
static int dx[d] = {1, 1, 0, -1, -1, -1, 0, 1};
static int dy[d] = {0, 1, 1, 1, 0, -1, -1, -1};

class node
{	
	//the position currently
	int xPos;
	int yPos;

	//The values for A* to use.
	int cost;
	int heuristic;

	public:
		node(int x, int y, int gX, int hX)
		{
			xPos = x;
			yPos = y;
			cost = gX;
			heuristic = hX;
		}
		//Functions to retrieve information.
		int getXPos() const
		{
			return xPos;
		}
		
		int getYPos() const
		{
			return yPos;
		}

		int getCost() const
		{
			return cost;
		}

		int getHeuristic() const
		{
			return heuristic;
		}

		const int& estimate(const int& xF, const int& yF) const
		{
			static int xDist, yDist, distance;

			//Caclulates x and y distances.
			xDist = xF - xPos;
			yDist = yF - yPos;

			//Heuristic Estimate
			//Euclidian Distance
			distance = static_cast<int>(sqrt(xDist*xDist + yDist*yDist));

			return distance;

		}

		void updateHeuristic(const int& xFinal, const int& yFinal)
		{
			//multiple by 10?
			heuristic = cost + estimate(xFinal, yFinal);
		}

		void nextCost(const int& dir)
		{
			if(dir == 8)
			{
				if(dir % 2 == 0)
					cost += 10;
				else
					cost += 14;
			}
			else
				cost += 10;
		}

};

bool operator<(const node& a, const node& b)
{
	return a.getHeuristic() > b.getHeuristic();
}
//A* starts here
string pathFinding(const int& xFirst, const int& yFirst,const int& xLast,const int& yLast)
{
	//This is the open list!
	static priority_queue<node> hQ[2];
	static int index; //heuristic queue index.
	static node* n1;
	static node* n2;
	static int i, j, x, y, xdx, ydy;
	static char c;
	index = 0;

	//0 the node map.
	for(y=0;y<HEIGHT;y++)
	{
		for(x=0;x<WIDTH;x++)
		{
			oList[x][y] = 0;
			cList[x][y] = 0;
		}
	}

	//Making the starting node and placing it in open list.
	n1 = new node(xFirst, yFirst, 0, 0);
	n1->updateHeuristic(xLast, yLast);
	hQ[index].push(*n1);
	oList[x][y]=n1->getHeuristic();

	//The search
	while(!hQ[index].empty())
	{
		n1 = new node( hQ[index].top().getXPos(), hQ[index].top().getYPos(), hQ[index].top().getCost(), hQ[index].top().getHeuristic());

		x = n1->getXPos();
		y = n1->getYPos();
	

		//Remove the node from the Open list.
		hQ[index].pop();
		oList[x][y] = 0;

		//marking it on the closed list.
		cList[x][y] = 1;



		//End the search when we reach goal.
		if(x == xLast && y == yLast)
		{
			//Create the path between the nodes.
			//The optimal path.
			string path = "";

			//Create the path from the end to the beginning
			while(!(x == xFirst && y == yFirst))
			{
				j = directions[x][y];
				c='0' + (j + d/2)%d;
				path = c + path;
				x += dx[j];
				y += dy[j];
			}

			//Some garbage collection
			delete n1;

			while(!hQ[index].empty())
			{
				hQ[index].pop();
			}

			return path;
		}
	

		//Generate moves aka the child nodes in all possible directions.
		for(i=0;i<d;i++)
		{
			//Changes in x and y.
			xdx = x + dx[i];
			ydy = y + dy[i];

			if(!(xdx < 0 || xdx > WIDTH - 1 || ydy < 0 || ydy > HEIGHT - 1 || map[xdx][ydy] == 1 || cList[xdx][ydy] == 1))
			{
						n2 = new node(xdx, ydy, n1->getCost(), n1->getHeuristic());
						n2->nextCost(i);
						n2->updateHeuristic(xLast, yLast);

						//add top open list if not yet there.
						if(oList[xdx][ydy] == 0)
						{
							oList[xdx][ydy] = n2->getHeuristic();
							hQ[index].push(*n2);

							//keep track of its parent node direction.
							directions[xdx][ydy] = (i + d/2)%d;
						}
						else if(oList[xdx][ydy] > n2->getHeuristic())
						{
							oList[xdx][ydy] = n2->getHeuristic();

							//update parent direction
							directions[xdx][ydy] = (i + d/2)%d;

							//Replace node by emptying one hQ to another one.
							//node to be replaced will be ignored
							//and new node will be pushed in instead.
								//change comment
							while(!(hQ[index].top().getXPos() == xdx && hQ[index].top().getYPos() == ydy))
							{
								hQ[1 - index].push(hQ[index].top());
								hQ[index].pop();
							}

							index = 1 - index;
							hQ[index].push(*n2);
						}
						else
							delete n2;
					
				
			}
			
		}

		delete n1;

	
	}
	return ""; // no path found
}

int main()
{
	srand(time(NULL));

	//create the empty map.
	for(int y=0; y<HEIGHT; y++)
	{
		for(int x=0; x < WIDTH; x++)
			map[x][y] = 0;
	}

	//Fill the map with a '+' pattern.
		//change
	for(int x =WIDTH/8; x < WIDTH*7/8;x++)
		map[x][HEIGHT/2] = 1;
	for( int y=HEIGHT/8; y < HEIGHT*7/8; y++)
		map[WIDTH/2][y] = 1;

	//Random start and finish positions.
	int xA, yA, xB, yB;

	switch(rand()%8)
	{
		case 0: xA=0;yA=0;xB=WIDTH-1;yB=HEIGHT-1;
			break;
		case 1: xA=0;yA=HEIGHT-1;xB=WIDTH-1;yB=0;
			break;
		case 2: xA=WIDTH/2-1;yA=HEIGHT/2-1;xB=WIDTH/2+1;yB=HEIGHT/2+1;
			break;
		case 3: xA=WIDTH/2-1;yA=HEIGHT/2+1;xB=WIDTH/2+1;yB=HEIGHT/2-1;
			break;
		case 4: xA=WIDTH/2-1;yA=0;xB=WIDTH/2+1;yB=HEIGHT-1;
			break;
		case 5: xA=WIDTH/2+1;yA=HEIGHT-1;xB=WIDTH/2-1;yB=0;
			break;
		case 6: xA=0;yA=HEIGHT/2-1;xB=WIDTH-1;yB=HEIGHT/2+1;
			break;
		case 7: xA=WIDTH-1;yA=HEIGHT/2+1;xB=0;yB=HEIGHT/2-1;
			break;
	}

	cout<<"The Map size (x, y) is: " << WIDTH << ", " << HEIGHT << endl;
	cout << "The start: " << xA << ", " << yA << endl;
	cout << "The finish: " << xB << ", " << yB << endl;

	//Retrieve the path.
	clock_t start = clock();

	string route = pathFinding(xA, yA, xB, yB);
	if(route == "")
		cout << "An empty route has been created" << endl;

	clock_t end = clock();
	double timeElapsed = double (end - start);

	cout << "The time to calculate the route in milliseconds was: " << timeElapsed << endl;
	cout << "route" << endl;
	cout << route << endl;

	//Follow the route on the map and display it
	if(route.length() > 0)
	{
		cout << "in route " << endl;
		int j;
		char c;
		int x = xA;
		int y = yA;
		map[x][y] = 2;

		for(int i = 0;i < route.length(); i++)
		{
			c=route.at(i);
			j=atoi(&c);
			x = x + dx[j];
			y = y + dy[j];
			map[x][y] = 3;
		}

		map[x][y] = 4;

		for(int y=0; y<HEIGHT;y++)
		{
			for(int x=0; x<WIDTH;x++)
			{
				if(map[x][y] == 0)
					cout << "."; 
				else if(map[x][y] == 1)
					cout << "|";//obstacle
				else if(map[x][y] == 2)
					cout << "S"; //start
				else if(map[x][y] == 3)
					cout << "P"; //path
				else if(map[x][y] == 4)
					cout << "G"; //goal
			}

			cout << endl;
		}
	}
	getchar(); //waiting for an enter keypress // change
	return(0);
}
