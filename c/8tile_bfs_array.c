// 8tile by BFS search (use array)
//
// How to Build :
//   gcc 8tile_bfs_array.c -o 8tile_bfs_array
// How to Execute :
//   ./8tile_bfs_array 123456708  or  ./8tile_bfs_array 513276480
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define _MAX_UBOUND 362880	// 9!=362880

const char c_strTargetTile[] = "123456780";

typedef struct tilenode
{
	char tile[10];
	
	long prev;
	long next;
} TileNode;

TileNode g_TileTrees[_MAX_UBOUND];
int g_cTileVisited[28000000];

long g_iLevelStart, g_iLevelEnd;

void  SetBit( int A[],  long k )
   {
      A[k/32] |= 1 << (k%32);  // Set the bit at the k-th position in A[i]
   }
   
void  ClearBit( int A[],  long k )                
   {
      A[k/32] &= ~(1 << (k%32));
   }
   
int TestBit( int A[],  long k )
   {
      return ( (A[k/32] & (1 << (k%32) )) != 0 ) ;     
   }

void SwapStringChar(char *strString, int pos1, int pos2)
{
	char cTmp;
	
	cTmp = strString[pos1];
	strString[pos1] = strString[pos2];
	strString[pos2] = cTmp;
}

int NextMove(int iFlag, char *strSourceTile, char *strNextTile)  // 0:Has Next Move, -1:No Next Move
{
	int iX_Pos;
	
	strcpy(strNextTile, strSourceTile);
	
	for(int i=0; i<9; i++)
		if( strNextTile[i] == '0' )
		{
			iX_Pos = i;
			break;
		}
		
	switch(iX_Pos)
	{
		case 0 :
			if( iFlag == 0 ) SwapStringChar(strNextTile, 0, 3);
			if( iFlag == 1 ) SwapStringChar(strNextTile, 0, 1);
			if( iFlag >= 2 ) return -1;
			break;
		case 1 :
			if( iFlag == 0 ) SwapStringChar(strNextTile, 1, 4);
			if( iFlag == 1 ) SwapStringChar(strNextTile, 1, 2);
			if( iFlag == 2 ) SwapStringChar(strNextTile, 1, 0);
			if( iFlag >= 3 ) return -1;
			break;
		case 2 :
			if( iFlag == 0 ) SwapStringChar(strNextTile, 2, 5);
			if( iFlag == 1 ) SwapStringChar(strNextTile, 2, 1);
			if( iFlag >= 2 ) return -1;
			break;
		case 3 :
			if( iFlag == 0 ) SwapStringChar(strNextTile, 3, 6);
			if( iFlag == 1 ) SwapStringChar(strNextTile, 3, 4);
			if( iFlag == 2 ) SwapStringChar(strNextTile, 3, 0);
			if( iFlag >= 3 ) return -1;
			break;
		case 4 :
			if( iFlag == 0 ) SwapStringChar(strNextTile, 4, 7);
			if( iFlag == 1 ) SwapStringChar(strNextTile, 4, 5);
			if( iFlag == 2 ) SwapStringChar(strNextTile, 4, 3);
			if( iFlag == 3 ) SwapStringChar(strNextTile, 4, 1);
			if( iFlag >= 4 ) return -1;
			break;
		case 5 :
			if( iFlag == 0 ) SwapStringChar(strNextTile, 5, 8);
			if( iFlag == 1 ) SwapStringChar(strNextTile, 5, 4);
			if( iFlag == 2 ) SwapStringChar(strNextTile, 5, 2);
			if( iFlag >= 3 ) return -1;
			break;
		case 6 :
			if( iFlag == 0 ) SwapStringChar(strNextTile, 6, 7);
			if( iFlag == 1 ) SwapStringChar(strNextTile, 6, 3);
			if( iFlag >= 2 ) return -1;
			break;
		case 7 :
			if( iFlag == 0 ) SwapStringChar(strNextTile, 7, 8);
			if( iFlag == 1 ) SwapStringChar(strNextTile, 7, 6);
			if( iFlag == 2 ) SwapStringChar(strNextTile, 7, 4);
			if( iFlag >= 3 ) return -1;
			break;
		case 8 :
			if( iFlag == 0 ) SwapStringChar(strNextTile, 8, 7);
			if( iFlag == 1 ) SwapStringChar(strNextTile, 8, 5);
			if( iFlag >= 2 ) return -1;
			break;
		default :
			return -1;
			break;
	}
	
	return 0;
}

int CheckVisited(char *strTile)		// 0 : visited, -1 : no
{
	long iPos = strtoul(strTile, NULL, 10);
	
	if( !TestBit(g_cTileVisited, iPos) )
	{
		SetBit(g_cTileVisited, iPos);
		return -1;
	}
	
	return 0;
}

int main(int argc, char *argv[])
{
	// check input is valid
	{
		if( strlen(argv[1]) != 9 )
		{
			printf("Input tile error !!\n");
			return -1;
		}
		
		for( int i=0; i<9; i++ )
		{
			if( argv[1][i]<'0'&& argv[1][i]>'8' )
			{
				printf("Input tile error !!\n");
				return -1;
			}
		}
	}
	
	// Print message
	{
		printf("**********************************************\n");
		printf("Initial tile : %s\n", argv[1]);
		printf("Target tile  : %s\n", c_strTargetTile);
	
		printf("\nStart search...\n");
	}
	
	// Initial
	{
		memset(g_cTileVisited, 0, sizeof(g_cTileVisited));
	}
	
	// Insert root node
	{
		g_iLevelStart = g_iLevelEnd = 0;
		
		strcpy( g_TileTrees[0].tile, argv[1] );
		g_TileTrees[0].prev = -1;
		g_TileTrees[0].next = -1;
	}
	
	// Start build BFS tree and search
	{
		long iNodes = 1;
		while(1)
		{
			// Check found target or not in current level
			for( long iIndex=g_iLevelStart ; iIndex<=g_iLevelEnd ; iIndex++ )
			{
				if( strcmp(g_TileTrees[iIndex].tile, c_strTargetTile)== 0 )	// Found
				{
					// reverse to root
					long i = iIndex;
					while(1)
					{
						g_TileTrees[g_TileTrees[i].prev].next = i;
						i = g_TileTrees[i].prev;
						if( i<0 )
							break;
					}
					
					// Print result
					printf("\nFound solution after search %ld nodes.\n", iNodes);
					printf("Here are the solutions :\n");
					
					long iMoves = -1;
					i = 0;
					
					do
					{
						iMoves++;
						printf("%s\n", g_TileTrees[i].tile);
						i = g_TileTrees[i].next;
					} while(i>=0);
					
					printf("\nTotal moves : %ld\n\n", iMoves);
					
					return 0;
				}
			}
			
			// Find next level
			char strTile[10];
			long iNewNodeIndex = g_iLevelEnd;
			long g_iLevelEndTmp = g_iLevelEnd;
			int  bHasNewLevel = -1;
			for( long iIndex=g_iLevelStart ; iIndex<=g_iLevelEndTmp ; iIndex++ )
			{
				for( int i=0; i<5; i++ )
				{
					if( NextMove(i, g_TileTrees[iIndex].tile, strTile) == 0 )
					{
						if( CheckVisited(strTile) == -1 ) // new node
						{
							bHasNewLevel = 1;
							iNewNodeIndex++;
							strcpy(g_TileTrees[iNewNodeIndex].tile, strTile);
							g_TileTrees[iNewNodeIndex].prev = iIndex;
							g_TileTrees[iNewNodeIndex].next = -1;
							
							g_iLevelEnd = iNewNodeIndex;
							
							if( (++iNodes%500) == 0 )
							{
								printf(">");
								fflush(stdout);
							}
						}
					}
					else
					{
						break;
					}
				}
			}
			
			if( bHasNewLevel == 1 )
			{
				g_iLevelStart = g_iLevelEndTmp+1;
			}
			else
			{
				printf("\nSorry, cannot find the solution path after search %ld nodes.\n\n", iNodes);
				return -1;
			}
			
			//
		}
	}
	
	return 0;
	
}





