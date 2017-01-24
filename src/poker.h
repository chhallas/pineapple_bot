#define ROYAL_FLUSH 50
#define	STRAIGHT_FLUSH	30
#define	FOUR_OF_A_KIND	20
#define	FULL_HOUSE	12
#define	FLUSH		8
#define	STRAIGHT	4
#define	THREE_OF_A_KIND 2
#define	TWO_PAIR	0
#define	ONE_PAIR	0
#define	HIGH_CARD	0

#define	RANK(x)		((x >> 8) & 0xF)

#define HIGH_CARD_THREE 0
#define ONE_PAIR_2 0
#define ONE_PAIR_3 0
#define ONE_PAIR_4 0
#define ONE_PAIR_5 0
#define ONE_PAIR_6 1
#define ONE_PAIR_7 2
#define ONE_PAIR_8 3
#define ONE_PAIR_9 4
#define ONE_PAIR_10 5
#define ONE_PAIR_11 6
#define ONE_PAIR_12 7
#define ONE_PAIR_13 8
#define ONE_PAIR_14 9
#define THREE_OF_A_KIND_2 10
#define THREE_OF_A_KIND_3 11
#define THREE_OF_A_KIND_4 12
#define THREE_OF_A_KIND_5 13
#define THREE_OF_A_KIND_6 14
#define THREE_OF_A_KIND_7 15
#define THREE_OF_A_KIND_8 16
#define THREE_OF_A_KIND_9 17
#define THREE_OF_A_KIND_10 18
#define THREE_OF_A_KIND_11 19
#define THREE_OF_A_KIND_12 20
#define THREE_OF_A_KIND_13 21
#define THREE_OF_A_KIND_14 22

static char *value_str[] = {
	"",
	"Straight Flush",
	"Four of a Kind",
	"Full House",
	"Flush",
	"Straight",
	"Three of a Kind",
	"Two Pair",
	"One Pair",
	"High Card"
};

void init_deck( int *deck );
short eval_5hand( int *hand );
unsigned find_fast(unsigned u);
int eval_5hand_fast(int c1, int c2, int c3, int c4, int c5);
int hand_rank( short val );

#define CLUB	0x8000
#define DIAMOND 0x4000
#define HEART   0x2000
#define SPADE   0x1000

#define Deuce	0
#define Trey	1
#define Four	2
#define Five	3
#define Six	4
#define Seven	5
#define Eight	6
#define Nine	7
#define Ten	8
#define Jack	9
#define Queen	10
#define King	11
#define Ace	12
