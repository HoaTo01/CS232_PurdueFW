/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// prototypes
void clear(void);
void greet(void);
int **init(int d);
void draw(int **board, int d);
bool move(int tile, int **board, int d);
bool won(int **board, int d);

int main(int argc, string argv[])
{
    int d;
    int **board;
    int i;
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    board = init(d);
    if(board == NULL)
        return(0);
    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw(board,d);

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won(board,d))
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile, board,d))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    for (i = 0;i<d;i++)
    {
        free(board[i]);
    }
    free(board);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
int **init(int d)
{
    // TODO
    int index;
    int **board = (int **)malloc(sizeof(int *)*d);
    if (board == NULL)
        return(NULL);
    for(index = 0; index <d;index++)
    {
        *(board+index) = (int*)malloc(sizeof(int)*d);
        if(*(board+index) == NULL)
            return NULL;
    }
    int i, j = 0;
    int numToFill = d*d - 1;
    int temp;
    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
            *(*(board+i)+j) = numToFill;
            numToFill--;
        }
    }
    if ((d*d - 1)%2 == 1)
    {
        temp = (*(*(board+d-1)+d-2));
        (*(*(board+d-1)+d-2)) = (*(*(board+d-1)+d-3));
        (*(*(board+d-1)+d-3)) = temp;
    }
    return(board);
}

/**
 * Prints the board in its current state.
 */
void draw(int **board, int d)
{
    // TODO
    int i, j = 0;
    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
            if ( (*(*(board+i)+j)) != 0)
                printf("%3d", (*(*(board+i)+j)));
            else
                printf("%3c",'_');
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile, int **board, int d)
{
    // TODO
    int i, j = 0;
    int current_i, current_j = 0;
    int blank_i, blank_j = 0;

    if ((tile > 0)  && (tile < (d*d)))
    {
        for (i = 0; i < d; i++)
        {
            for (j = 0; j < d; j++)
            {
                if( (*(*(board+i)+j)) == tile)
                {
                    current_i = i;
                    current_j = j;

                }
                if( (*(*(board+i)+j)) == 0)
                {
                    blank_i = i;
                    blank_j = j;
                }
            }
        }
        if (((blank_i == current_i) && ((blank_j == current_j + 1) || (blank_j == current_j - 1))) || ((blank_j == current_j) && ((blank_i == current_i + 1) || (blank_i == current_i - 1))))
        {
            board[blank_i][blank_j] = board[current_i][current_j];
            board[current_i][current_j] = 0;
            return true;
        }

    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(int **board, int d)
{
    // TODO
    int i, j = 0;
    int numberToCheck = 1;
    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
            if (i == d - 1 && j == d - 1)
                return(true);
            if (board[i][j] != numberToCheck)
                return(false);
            numberToCheck++;
        }
    }
    return false;
}