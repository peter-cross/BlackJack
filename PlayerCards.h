#ifndef PLAYER_CARDS
#define  PLAYER_CARDS

#include <string>
using namespace std;


class PlayerCards
{
public:
	PlayerCards();
	
	void clearCards();
	void getFirstCards( int* deck, int &deckIdx );
	void getNextCard( int* deck, int &deckIdx );
	
	int playerCount();
	int playerNextIdx();
	bool blackJack();
	
	void displayCard( int cardIdx );
	void displayLastCard();
	void displayAllCards();
	void countGoneCards();
	
	
	static string blackJack( int cnt );
	
	static int cardsCount[13],
		   	   score;
	
	static const string hearts[13],
	   		  			diamonds[13],
	   		  			clubs[13],
	   		  			spades[13];

	static const string ranks[13];

	static const int COUNT[13];
	
private:
	int playerCount( int cardIdx );
	int countCard( int deckIdx );
	int adjustCount( int plCount );
	void countGoneCards( int cardIdx );
	
	int cards[10];
	int nextIdx;	
};

#endif