#include "mazeblaze2.h"
/*Details we require at each node :-
1) possible_neighbours
2) explored_neighbours
3) distance from each neighbour 
4) 

Details of global variables :-
1) current_dir 

The direction array here is [ W , NW , N , NE , E , SE , S , SW ] 
*/

struct node 
{
    int possible_neighbours [8] ;
    int explored_neighbours [8] ;
    int explored_distances [8] ;
} ;

int current_dir ;

void get_possible_neighbours() ;
int choose_dir() ;

int main ()
{
    struct node object_node[50] ;   //object_node is a object of struct node 
}

int choose_dir()    //returns the direction the bot should take at that node
{
    /*instead of using left follow , we go for west follow , which implie that our bot will always choose west first , if possible and unexplored . then it will go for NW , then N and so on */
    int i = 0 ;
    for (i = 0 ; i < 8 ; i++)
    {
        if (explored_neighbour[i] != possible_neighbour[i] )    //[0] is left , so west follow is given priority 
        {
            break ;
        }
    }
    return i+1 ; //returns i+1 since i goes form 0 - 7 , while our directions are from 1-8 
}

void get_possible_neighbours() 
{
    
}