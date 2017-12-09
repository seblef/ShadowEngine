
#include "stdafx.h"
#include "NavMap.h"
#include "AS_PathFinder.h"

AS_PathFinder::AS_PathFinder(int w, int h) : _width(w), _height(h), _onClosedList(10)
{
	_openList=new int[w*h+2];
	_openX=new int[w*h+2];
	_openY=new int[w*h+2];
	_FCost=new int[w*h+2];
	_HCost=new int[w*h+2];
	_cells=new AS_CellItem[w*h];
}

AS_PathFinder::~AS_PathFinder()
{
	delete[] _openList;
	delete[] _openX;
	delete[] _openY;
	delete[] _FCost;
	delete[] _HCost;
	delete[] _cells;
}

bool AS_PathFinder::findPath(int startX, int startY,
							 int targetX, int targetY,
							 NavPath& p, const NavMap& nm)
{
	int onOpenList=0, parentX=0, parentY=0,
	a=0, b=0, m=0, u=0, v=0, temp=0, corner=0, numberOfOpenListItems=0,
	addedGCost=0, tempGcost = 0, path = 0,
	tempx, pathX, pathY,
	newOpenListItemID=0;

//2.Quick Path Checks: Under the some circumstances no path needs to
//	be generated ...

//	If starting location and target are in the same location...
	if (startX == targetX && startY == targetY)
	{
		p.addCase(NavCase(startX,startY));
		return true;
	}

//	If target square is unwalkable, return that it's a nonexistent path.
	if(nm.getMap(targetX,targetY)==0)
		return false;

//3.Reset some variables that need to be cleared
	if(_onClosedList > 1000000) //reset whichList occasionally
	{
		for(int i=0;i<(_width+1)*(_height+1);++i)
			_cells[i].setWhich(0);

		_onClosedList = 10;	
	}

	_onClosedList+=2; //changing the values of onOpenList and onClosed list is faster than redimming whichList() array
	onOpenList=_onClosedList-1;

	getCell(startX,startY).setGCost(0); //reset starting square's G value to 0

//4.Add the starting location to the open list of squares to be checked.
	numberOfOpenListItems = 1;
	_openList[1]=1;//assign it as the top (and currently only) item in the open list, which is maintained as a binary heap (explained below)
	_openX[1]=startX;
	_openY[1]=startY;

//5.Do the following until a path is found or deemed nonexistent.
	do
	{

//6.If the open list is not empty, take the first cell off of the list.
//	This is the lowest F cost cell on the open list.
		if (numberOfOpenListItems != 0)
		{

//7. Pop the first item off the open list.
			parentX=_openX[_openList[1]];
			parentY=_openY[_openList[1]]; //record cell coordinates of the item
			getCell(parentX,parentY).setWhich(_onClosedList); //add the item to the closed list

//	Open List = Binary Heap: Delete this item from the open list, which
//  is maintained as a binary heap. For more information on binary heaps, see:
//	http://www.policyalmanac.org/games/binaryHeaps.htm
			--numberOfOpenListItems;//reduce number of open list items by 1	
		
//	Delete the top item in binary heap and reorder the heap, with the lowest F cost item rising to the top.
			_openList[1]=_openList[numberOfOpenListItems+1];//move the last item in the heap up to slot #1
			v=1;

//	Repeat the following until the new item in slot #1 sinks to its proper spot in the heap.
			bool end=false;
			do
			{
				u=v;		
				if(2*u+1 <= numberOfOpenListItems) //if both children exist
				{
	 				//Check if the F cost of the parent is greater than each child.
					//Select the lowest of the two children.
					if (_FCost[_openList[u]] >= _FCost[_openList[2*u]]) 
						v = 2*u;
					if(_FCost[_openList[v]] >= _FCost[_openList[2*u+1]]) 
						v = 2*u+1;		
				}
				else
				{
					if(2*u <= numberOfOpenListItems) //if only child #1 exists
					{
	 				//Check if the F cost of the parent is greater than child #1	
						if(_FCost[_openList[u]] >= _FCost[_openList[2*u]]) 
							v = 2*u;
					}
				}

				if(u != v) //if parent's F is > one of its children, swap them
				{
					temp = _openList[u];
					_openList[u] = _openList[v];
					_openList[v] = temp;			
				}
				else
					end=true;
		
			} while(!end);//reorder the binary heap


//7.Check the adjacent squares. (Its "children" -- these path children
//	are similar, conceptually, to the binary heap children mentioned
//	above, but don't confuse them. They are different. Path children
//	are portrayed in Demo 1 with grey pointers pointing toward
//	their parents.) Add these adjacent child squares to the open list
//	for later consideration if appropriate (see various if statements
//	below).
			for(b=parentY-1; b <= parentY+1; ++b)
			{
				for(a=parentX-1;a <= parentX+1;++a)
				{
				//	If not off the map (do this first to avoid array out-of-bounds errors)
					if(a!=-1 && b!=-1 && a!=_width && b!=_height)
					{
						AS_CellItem& c(getCell(a,b));
						AS_CellItem& pc(getCell(parentX,parentY));
					//	If not already on the closed list (items on the closed list have
					//	already been considered and can now be ignored).			
						if(c.getWhich() != _onClosedList)
						{
						//	If not a wall/obstacle square.
							if(nm.getMap(a,b))
							{
							//	Don't cut across corners
								corner=1;	
								if(a==parentX-1) 
								{
									if(b==parentY-1)
									{
										if(nm.getMap(parentX-1,parentY)==0 || nm.getMap(parentX,parentY-1)==0)
											corner=0;
									}
									else if(b==parentY+1)
									{
										if(nm.getMap(parentX,parentY+1)==0 || nm.getMap(parentX-1,parentY)==0)
											corner=0;
									}
								}
								else if(a==parentX+1)
								{
									if(b==parentY-1)
									{
										if(nm.getMap(parentX,parentY-1)==0 || nm.getMap(parentX+1,parentY)==0)
											corner=0;
									}
									else if(b==parentY+1)
									{
										if(nm.getMap(parentX+1,parentY)==0 || nm.getMap(parentX,parentY+1)==0)
											corner=0;
									}
								}	
							
								if(corner)
								{
								//	If not already on the open list, add it to the open list.			
									if(c.getWhich()!=onOpenList) 
									{	
										//Create a new open list item in the binary heap.
										++newOpenListItemID; //each new item has a unique ID #
										m=numberOfOpenListItems+1;
										_openList[m]=newOpenListItemID;//place the new open list item (actually, its ID#) at the bottom of the heap
										_openX[newOpenListItemID]=a;
										_openY[newOpenListItemID]=b;//record the x and y coordinates of the new item

										//Figure out its G cost
										if(abs(a-parentX)==1 && abs(b-parentY)==1)
											addedGCost=14;//cost of going to diagonal squares	
										else	
											addedGCost=10;//cost of going to non-diagonal squares

										c.setGCost(pc.getGCost() + addedGCost);

										//Figure out its H and F costs and parent
										_HCost[_openList[m]] = 10*(abs(a - targetX) + abs(b - targetY));
										_FCost[_openList[m]] = c.getGCost() + _HCost[_openList[m]];
										c.setParentX(parentX);
										c.setParentY(parentY);

										//Move the new open list item to the proper place in the binary heap.
										//Starting at the bottom, successively compare to parent items,
										//swapping as needed until the item finds its place in the heap
										//or bubbles all the way to the top (if it has the lowest F cost).
										while(m!=1) //While item hasn't bubbled to the top (m=1)	
										{
											//Check if child's F cost is < parent's F cost. If so, swap them.	
											if(_FCost[_openList[m]] <= _FCost[_openList[m/2]])
											{
												temp = _openList[m/2];
												_openList[m/2] = _openList[m];
												_openList[m] = temp;
												m = m/2;
											}
											else
												break;
										}
										++numberOfOpenListItems;//add one to the number of items in the heap

										//Change whichList to show that the new item is on the open list.
										c.setWhich(onOpenList);
									}
								//8.If adjacent cell is already on the open list, check to see if this 
								//	path to that cell from the starting location is a better one. 
								//	If so, change the parent of the cell and its G and F costs.	
									else //If whichList(a,b) = onOpenList
									{
										//Figure out the G cost of this possible new path
										if(abs(a-parentX)==1 && abs(b-parentY)==1)
											addedGCost=14;//cost of going to diagonal tiles	
										else	
											addedGCost=10;//cost of going to non-diagonal tiles

										tempGcost=pc.getGCost() + addedGCost;
		
										//If this path is shorter (G cost is lower) then change
										//the parent cell, G cost and F cost. 		
										if (tempGcost < c.getGCost()) //if G cost is less,
										{
											c.setParentX(parentX);
											c.setParentY(parentY);
											c.setGCost(tempGcost);

											//Because changing the G cost also changes the F cost, if
											//the item is on the open list we need to change the item's
											//recorded F cost and its position on the open list to make
											//sure that we maintain a properly ordered open list.
											for(int x=1; x<=numberOfOpenListItems;++x) //look for the item in the heap
											{
												if(_openX[_openList[x]]==a && _openY[_openList[x]] == b) //item found
												{
													_FCost[_openList[x]]=c.getGCost() + _HCost[_openList[x]];//change the F cost
				
													//See if changing the F score bubbles the item up from it's current location in the heap
													m=x;
													while(m!=1) //While item hasn't bubbled to the top (m=1)	
													{
														//Check if child is < parent. If so, swap them.	
														if(_FCost[_openList[m]] < _FCost[_openList[m/2]])
														{
															temp = _openList[m/2];
															_openList[m/2] = _openList[m];
															_openList[m] = temp;
															m = m/2;
														}
														else
															break;
													} 
													break; //exit for x = loop
												} //If openX(openList(x)) = a
											} //For x = 1 To numberOfOpenListItems
										}//If tempGcost < Gcost(a,b)

									}//else If whichList(a,b) = onOpenList	
								}//If not cutting a corner
							}//If not a wall/obstacle square.
						}//If not already on the closed list 
					}//If not off the map
				}//for (a = parentXval-1; a <= parentXval+1; a++){
			}//for (b = parentYval-1; b <= parentYval+1; b++){

		}//if (numberOfOpenListItems != 0)
//9.If open list is empty then there is no path.	
		else
			return false;


		if(getCell(targetX,targetY).getWhich()==onOpenList)
			break;

	} while(1);

//10.Save the path if it exists.

//c. Now copy the path information over to the databank. Since we are
//	working backwards from the target to the start location, we copy
//	the information to the data bank in reverse order. The result is
//	a properly ordered set of path data, from the first step to the
//	last.
	pathX = targetX ; pathY = targetY;
	p.addCase(NavCase(pathX,pathY));
	do
	{
//d.Look up the parent of the current cell.	
		tempx=getCell(pathX,pathY).getParentX();		
		pathY=getCell(pathX,pathY).getParentY();
		pathX=tempx;

		p.addCase(NavCase(pathX,pathY));

//e.If we have reached the starting square, exit the loop.	
	}
	while(pathX!=startX || pathY!=startY);	

	return true;
}
