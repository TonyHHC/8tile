// 8tile by BFS search (use array)
//
// How to Build :
//   gcc -I /usr/include/SDL2 8tile_bfs_array_sdl.c -o 8tile_bfs_array_sdl -lSDL2 -lSDL2main
// How to Execute :
//   ./8tile_bfs_array_sdl 123456708  or  ./8tile_bfs_array_sdl 513276480
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL.h>

#define _MAX_UBOUND 362880	// 9!=362880

const char c_strTargetTile[] = "123456780";

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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

//The window we'll be rendering to
SDL_Window* window = NULL;
//The surface contained by the window
SDL_Surface* screenSurface = NULL;
// PIC 1-8
SDL_Surface *pic[9];
// Rect
SDL_Rect dest[9];

int SDL_Initial()
{
	//Initialize SDL 
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return 0;
	}
	
	//Create window
	window = SDL_CreateWindow( "8tile",//視窗標題
						SDL_WINDOWPOS_UNDEFINED,//視窗位置設定
						SDL_WINDOWPOS_UNDEFINED, 
						SCREEN_WIDTH, //視窗的寬度
						SCREEN_HEIGHT,//視窗的高度
						SDL_WINDOW_SHOWN  //顯示視窗
						);
	if( window == NULL )
	{
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}

	//Get window surface
	screenSurface = SDL_GetWindowSurface( window );

	//Fill the surface black
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00 ) );
	
	//Update the surface
	SDL_UpdateWindowSurface( window );

	//Wait 1 seconds
	SDL_Delay( 1000 );
	
	////////////////////////////////
	pic[1] = SDL_LoadBMP( "1.bmp" );
	pic[2] = SDL_LoadBMP( "2.bmp" );
	pic[3] = SDL_LoadBMP( "3.bmp" );
	pic[4] = SDL_LoadBMP( "4.bmp" );
	pic[5] = SDL_LoadBMP( "5.bmp" );
	pic[6] = SDL_LoadBMP( "6.bmp" );
	pic[7] = SDL_LoadBMP( "7.bmp" );
	pic[8] = SDL_LoadBMP( "8.bmp" );
	
	////////////////////////////////
	for(int i=1,m=0 ; i<=7 ; i+=3,m+=55)
	{
		for(int j=1; j<=3; j++)
			dest[i+j-1].x = 160+(55*(j-1));
		dest[i].y = dest[i+1].y = dest[i+2].y = 120+m;
		
		dest[i].w = dest[i+1].w = dest[i+2].w = 55;
		dest[i].h = dest[i+1].h = dest[i+2].h = 55;
	}
	
	return 0;
}

void SDL_End()
{
	//Destroy window
	SDL_DestroyWindow( window );

	//Quit SDL subsystems
	SDL_Quit();
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
					
					if( SDL_Initial() == 0 )
					{
						i = 0;
						do
						{
							SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00 ) );
							
							for( int x=1 ; x<=9 ; x++)
							{
								int n = (int)(g_TileTrees[i].tile[x-1])-(int)('0');
								SDL_BlitSurface( pic[n], NULL, screenSurface, &dest[x] );
							}
							
							//Update the surface
							SDL_UpdateWindowSurface( window );

							//Wait 1 seconds
							SDL_Delay( 1000 );
							
							i = g_TileTrees[i].next;
						} while(i>=0);
						
						
						
						SDL_End();
					}
					
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





