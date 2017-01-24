#include <stdio.h>
#include <stdlib.h>
#include "poker.h"
#include "arrays.h"
#include "uthash/uthash.h"
#include "time.h"
#include <sys/types.h>
#include <unistd.h>

// Poker hand evaluator
//
// Kevin L. Suffecool
// suffecool@bigfoot.com
//

// perform a binary search on a pre-sorted array
//
int findit( int key )
{
    int low = 0, high = 4887, mid;

    while ( low <= high )
    {
        mid = (high+low) >> 1;      // divide by two
        if ( key < products[mid] )
            high = mid - 1;
        else if ( key > products[mid] )
            low = mid + 1;
        else
            return( mid );
    }
    fprintf( stderr, "ERROR:  no match found; key = %d\n", key );
    return( -1 );
}

//
//   This routine initializes the deck.  A deck of cards is
//   simply an integer array of length 52 (no jokers).  This
//   array is populated with each card, using the following
//   scheme:
//
//   An integer is made up of four bytes.  The high-order
//   bytes are used to hold the rank bit pattern, whereas
//   the low-order bytes hold the suit/rank/prime value
//   of the card.
//
//   +--------+--------+--------+--------+
//   |xxxbbbbb|bbbbbbbb|cdhsrrrr|xxpppppp|
//   +--------+--------+--------+--------+
//
//   p = prime number of rank (deuce=2,trey=3,four=5,five=7,...,ace=41)
//   r = rank of card (deuce=0,trey=1,four=2,five=3,...,ace=12)
//   cdhs = suit of card
//   b = bit turned on depending on rank of card
//
void init_deck( int *deck )
{
    int i, j, n = 0, suit = 0x8000;

    for ( i = 0; i < 4; i++, suit >>= 1 )
        for ( j = 0; j < 13; j++, n++ )
            deck[n] = primes[j] | (j << 8) | suit | (1 << (16+j));
}


//  This routine will search a deck for a specific card
//  (specified by rank/suit), and return the INDEX giving
//  the position of the found card.  If it is not found,
//  then it returns -1
//
int find_card( int rank, int suit, int *deck )
{
	int i, c;

	for ( i = 0; i < 52; i++ )
	{
		c = deck[i];
		if ( (c & suit)  &&  (RANK(c) == rank) )
			return( i );
	}
	return( -1 );
}

void delete_card( int *deck, int card, size_t no_cards )
{
	int deleted = 0;
	for (int i = 0; i < no_cards-1; i++)
	{
		if (deck[i] == card)
		{
			deleted = 1;
		}
		deck[i] = deck[i+deleted];
	}
}

//
//  This routine takes a deck and randomly mixes up
//  the order of the cards.
//
void shuffle_deck( int *deck, size_t no_cards )
{
    int i, n, temp[no_cards];

    for ( i = 0; i < no_cards; i++ )
        temp[i] = deck[i];

    for ( i = 0; i < no_cards; i++ )
    {
        do {
			double rand_double = (double)rand() / (double)(RAND_MAX);
            n = (int)((no_cards - 0.0000001) * rand_double);
        } while ( temp[n] == 0 );
        deck[i] = temp[n];
        temp[n] = 0;
    }
}

void print_hand( int *hand, int n )
{
    int i, r;
    char suit;
    static char *rank = "23456789TJQKA";

    for ( i = 0; i < n; i++ )
    {
        r = (*hand >> 8) & 0xF;
        if ( *hand & 0x8000 )
            suit = 'c';
        else if ( *hand & 0x4000 )
            suit = 'd';
        else if ( *hand & 0x2000 )
            suit = 'h';
        else
            suit = 's';

        printf( "%c%c ", rank[r], suit );
        hand++;
    }
}


int hand_rank( short val )
{
    if (val > 6185) return(HIGH_CARD);        // 1277 high card
    if (val > 3325) return(ONE_PAIR);         // 2860 one pair
    if (val > 2467) return(TWO_PAIR);         //  858 two pair
    if (val > 1609) return(THREE_OF_A_KIND);  //  858 three-kind
    if (val > 1599) return(STRAIGHT);         //   10 straights
    if (val > 322)  return(FLUSH);            // 1277 flushes
    if (val > 166)  return(FULL_HOUSE);       //  156 full house
    if (val > 10)   return(FOUR_OF_A_KIND);   //  156 four-kind
	if (val > 4)	return(STRAIGHT_FLUSH);	  //   6 straight-flushes
    return(ROYAL_FLUSH);                   	  //   4 royal-flushes
}

int hand_rank_3( short val )
{
    if (val > 6229) return(HIGH_CARD_THREE);        // 1277 high card
    if (val > 6020) return(ONE_PAIR_2);         // 2860 one pair
	if (val > 5800) return(ONE_PAIR_3);         // 2860 one pair
	if (val > 5580) return(ONE_PAIR_4);         // 2860 one pair
	if (val > 5360) return(ONE_PAIR_5);         // 2860 one pair
	if (val > 5140) return(ONE_PAIR_6);         // 2860 one pair
	if (val > 4920) return(ONE_PAIR_7);         // 2860 one pair
	if (val > 4700) return(ONE_PAIR_8);         // 2860 one pair
	if (val > 4480) return(ONE_PAIR_9);         // 2860 one pair
	if (val > 4260) return(ONE_PAIR_10);         // 2860 one pair
	if (val > 4040) return(ONE_PAIR_11);         // 2860 one pair
	if (val > 3820) return(ONE_PAIR_12);         // 2860 one pair
	if (val > 3600) return(ONE_PAIR_13);         // 2860 one pair
	if (val > 3380) return(ONE_PAIR_14);         // 2860 one pair
	if (val > 2401) return(THREE_OF_A_KIND_2);  //  858 three-kind
	if (val > 2335) return(THREE_OF_A_KIND_3);  //  858 three-kind
	if (val > 2269) return(THREE_OF_A_KIND_4);  //  858 three-kind
	if (val > 2203) return(THREE_OF_A_KIND_5);  //  858 three-kind
	if (val > 2137) return(THREE_OF_A_KIND_6);  //  858 three-kind
	if (val > 2071) return(THREE_OF_A_KIND_7);  //  858 three-kind
	if (val > 2005) return(THREE_OF_A_KIND_8);  //  858 three-kind
	if (val > 1939) return(THREE_OF_A_KIND_9);  //  858 three-kind
	if (val > 1873) return(THREE_OF_A_KIND_10);  //  858 three-kind
	if (val > 1807) return(THREE_OF_A_KIND_11);  //  858 three-kind
	if (val > 1741) return(THREE_OF_A_KIND_12);  //  858 three-kind
	if (val > 1675) return(THREE_OF_A_KIND_13);  //  858 three-kind
    return(THREE_OF_A_KIND_14);                  //  Three of a kind, aces
}

short eval_5cards( int c1, int c2, int c3, int c4, int c5 )
{
    int q;
    short s;

    q = (c1|c2|c3|c4|c5) >> 16;

    /* check for Flushes and StraightFlushes
    */
    if ( c1 & c2 & c3 & c4 & c5 & 0xF000 )
	return( flushes[q] );

    /* check for Straights and HighCard hands
    */
    s = unique5[q];
    if ( s )  return ( s );

    /* let's do it the hard way
    */
    q = (c1&0xFF) * (c2&0xFF) * (c3&0xFF) * (c4&0xFF) * (c5&0xFF);
    q = findit( q );

    return( values[q] );
}

unsigned find_fast(unsigned u)
{
    unsigned a, b, r;
    u += 0xe91aaa35;
    u ^= u >> 16;
    u += u << 8;
    u ^= u >> 4;
    b  = (u >> 8) & 0x1ff;
    a  = (u + (u << 2)) >> 19;
    r  = a ^ hash_adjust[b];
    return r;
}

int eval_5hand_fast(int c1, int c2, int c3, int c4, int c5)
{
    int q = (c1 | c2 | c3 | c4 | c5) >> 16;
    short s;
    if (c1 & c2 & c3 & c4 & c5 & 0xf000) return flushes[q]; // check for flushes and straight flushes
    if ((s = unique5[q]))                return s;          // check for straights and high card hands
    return hash_values[find_fast((c1 & 0xff) * (c2 & 0xff) * (c3 & 0xff) * (c4 & 0xff) * (c5 & 0xff))];
}

short eval_5hand( int *hand )
{
    int c1, c2, c3, c4, c5;

    c1 = *hand++;
    c2 = *hand++;
    c3 = *hand++;
    c4 = *hand++;
    c5 = *hand;

    return( eval_5hand_fast(c1,c2,c3,c4,c5) );
}

short eval_3hand( int *hand )
{
	int c1, c2, c3, c4, c5;
	
	c1 = *hand++;
	c2 = *hand++;
	c3 = *hand++;
	c4 = 98306;
	c5 = 164099;
	
	return ( three_hand_hash[eval_5hand_fast(c1,c2,c3,c4,c5)] );
}

short eval_board( int board[3][5] )
{
	int *fron = *board++;
	int *midd = *board++;
	int *back = *board++;
	
	int fron_rank = eval_3hand( fron );
	int midd_rank = eval_5hand( midd );
	int back_rank = eval_5hand( back );
	
	if (back_rank > midd_rank)
	{
		return 0;
	}
	if (midd_rank > fron_rank)
	{
		return 0;
	}
	
	int fron_val = hand_rank_3( fron_rank );
	int midd_val = hand_rank( midd_rank ); 
	int back_val = hand_rank( back_rank ) / 2;
	
	if (back_val == 1)
	{
		back_val = 0;
	}
	
    return( fron_val + midd_val + back_val );
}

// Define a struct to hold a hash table for used cards
struct cards_struct
{
	int id;            /* we'll use this field as the key */
	int used;          /* binary variable to indicate if card is used */
	UT_hash_handle hh; /* makes this structure hashable */
};

struct cards_struct *cards_hash = NULL;

// Add card to hash table
void add_card( int card_id ) 
{
	struct cards_struct *s;
	s = malloc(sizeof(struct cards_struct));
	s->id = card_id;
	s->used = 0;
    HASH_ADD_INT( cards_hash, id, s );
}

// Find the struct corresponding to a given card
struct cards_struct * find_hash_card (int card_id) 
{
    struct cards_struct *s;
    HASH_FIND_INT( cards_hash, &card_id, s );
    return s;
}

/*
	Initialize a hash table that checks if cards have already been used
	value 0: card not used
	value 1: card used
*/
void init_cards_hash( int *deck )
{	
	for (int i = 0; i < 52; i++)
	{
		add_card( deck[i] );
	}
}

// Clean deck by putting used card at the end
void clean_deck( int *deck , int used_card, size_t no_already_added)
{
	int moved = 0;
	
	// First clean the deck from cards already in use
	for ( int i = 0; i < 52-no_already_added-1; i++ )
	{
		if (used_card == deck[i])
		{
			moved = 1;
		}
		deck[i] = deck[i+moved];
	}
	deck[52-no_already_added-1] = used_card;
}

void update_card_hash( int *deck, int used_card, size_t no_already_added )
{
	struct cards_struct *s = find_hash_card( used_card );
	if (s->used == 0)
	{
		s->used = 1;
		clean_deck( deck, used_card, no_already_added );
	}
}

int draw_random_card( int *deck, size_t no_already_added )
{
	double rand_double = (double)rand() / (double)(RAND_MAX);
    int n = (int)((52 - no_already_added - 0.0000001) * rand_double);
	return deck[n];
}

int * fill_hand( int *hand )
{
	
	
}

double monte_carlo_sim( int board[3][5], int *deck, size_t back_len, size_t midd_len, size_t fron_len, size_t n_sim, size_t no_already_added, int actions[27][3], int first )
{	
	double exp_value = 0.0;
	
	// double diff;
	// clock_t start;
	// clock_t end;
	
	if (back_len + midd_len + fron_len == 13)
	{
		// start = clock();
		int *back = *board++;
		int *midd = *board++;
		int *fron = *board++;
		double royalty = eval_5hand( midd ) + eval_5hand( back ); // + eval_3hand( fron ) - needs to be implemented
		// end = clock();
		// diff = (double)(end - start) / CLOCKS_PER_SEC;
		// printf("Royalty time: %f s \n", diff );
		return royalty;
	}
	
	int new_cards[3];
	int temp;
	int max;
	
	for (int i = 0; i < n_sim; i++)
	{
		temp = 0;
		max = 0;

		for (int j = 0; j < 3; j++)
		{
			new_cards[j] = draw_random_card( deck, no_already_added + j);
			update_card_hash( deck, new_cards[j], no_already_added + j );
		}
		
		for (int j = 0; j < 27; j++)
		{
			int row_len;
			int action;
			int fron_len_temp = fron_len;
			int midd_len_temp = midd_len;
			int back_len_temp = back_len;

			for (int k = 0; k < 3; k++)
			{
				action = actions[j][k];
				if (action == 0)
				{
					fron_len_temp++;
					row_len = fron_len_temp;
				}
				else if (action == 1)
				{
					midd_len_temp++;
					row_len = midd_len_temp;
				}
				else if (action == 2)
				{
					back_len_temp++;
					row_len = back_len_temp;
				}
				board[actions[j][k]][row_len] = new_cards[k];
			}
			
			if (fron_len_temp <= 3 && midd_len_temp <= 5 && back_len_temp <= 5)
			{
				temp = monte_carlo_sim( board, deck, back_len_temp, midd_len_temp, fron_len_temp, n_sim, no_already_added + 3, actions, first + 1 );
				if (temp > max)
				{
					max = temp;
				}
			}
		}
		exp_value += max;
	}
	return exp_value / n_sim;
}

void prepare_deck( int *deck, int *used_cards, size_t no_used_cards )
{
	int cards_left = 52;
	
	for (int i = 0; i < no_used_cards; i++)
	{
		delete_card( deck, used_cards[i], cards_left );
		cards_left--;
	}
}

void set_seed()
{
	void srand();
	srand( getpid() );
}

int enumerate( int board[3][5], int *deck, size_t fron_len, size_t midd_len, size_t back_len, size_t no_cards_left, size_t no_perms, int perms[no_perms][no_cards_left] )
{
	int index, perm_value, max_value;
	max_value = 0;
	
	for ( int i = 0; i < no_perms; i++ )
	{ 
		index = 0;
		
		for (int k = 0; k < 3 - fron_len; k++)
		{
			board[0][2-k] = deck[perms[i][index]];
			index++;
		}
		for (int k = 0; k < 5 - midd_len; k++)
		{
			board[1][4-k] = deck[perms[i][index]];
			index++;
		}
		for (int k = 0; k < 5 - back_len; k++)
		{
			board[2][4-k] = deck[perms[i][index]];
			index++;
		}
		
		perm_value = eval_board( board );
		if (perm_value >= max_value)
		{
			max_value = perm_value;
		}
	}
	return max_value;
}

double fast_monte_carlo_sim( int board[3][5], int *deck, size_t fron_len, size_t midd_len, size_t back_len, size_t n_sim, size_t cards_left )
{	
	double exp_value = 0.0;
	int index;
	size_t to_draw = 13 - fron_len - midd_len - back_len;
	int rands[to_draw];
	double rand_double;
	int n;
	
	for (int i = 0; i < n_sim; i++)
	{
		index = 0;
		
		for ( int i = 0; i < to_draw; i++ )
		{
			rand_double = (double)rand() / (double)(RAND_MAX);
			n = (int)((cards_left - 0.0000001) * rand_double);
			rands[i] = n;
		}
		
		for (int k = 0; k < 3 - fron_len; k++)
		{
			board[0][2-k] = deck[rands[index]];
			index++;
		}
		for (int k = 0; k < 5 - midd_len; k++)
		{
			board[1][4-k] = deck[rands[index]];
			index++;
		}
		for (int k = 0; k < 5 - back_len; k++)
		{
			board[2][4-k] = deck[rands[index]];
			index++;
		}
		
		exp_value += eval_board( board );
	}
	
	return exp_value / n_sim;
}

// This is a non-optimized method of determining the
// best five-card hand possible out of seven cards.
// I am working on a faster algorithm.
//
short eval_7hand( int *hand )
{
    int i, j, q, best = 9999, subhand[5];

	for ( i = 0; i < 21; i++ )
	{
		for ( j = 0; j < 5; j++ )
			subhand[j] = hand[ perm7[i][j] ];
		q = eval_5hand( subhand );
		if ( q < best )
			best = q;
	}
	return( best );
}