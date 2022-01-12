#include "PlayerCards.h"

#include <iostream>
#include <ctime>
#include <iomanip>
#include <algorithm>
using namespace std;


bool displayPlayerCards( PlayerCards* players, PlayerCards dealer, bool open );
string winners( PlayerCards* players, PlayerCards dealer );
void displayGoneCards();
int getPlayerNumber();
string blackJack( int cnt );
void shuffleDeck( int* arr, int deckSize, int decks );
string getInput( string promptMsg );

		   
int playersWins = 0,
	dealerWins = 0;
	   	   
const int DECK_SIZE = 52,
		  PLAYERS = 3,
		  DECKS = 6,
		  DEALER_MIN = 17;

int main()
{
	cout << " ============================================================================================================== \n";
	cout << " ===============================================   Black Jack   =============================================== \n";
	cout << " ============================================================================================================== \n";
	
	int* deck = new int[ DECK_SIZE * DECKS ];
	int deckIdx = 1;
	
	PlayerCards* players = new PlayerCards[PLAYERS];
	PlayerCards dealer;
		
	int dck = 0; 
	bool enoughCards = true;	
	bool moreCards = true;
		
	srand( time(NULL) );
	
	shuffleDeck( deck, DECK_SIZE, DECKS );
		
	do
	{
		for ( int i = 0; i < PLAYERS; i++ )
		{
			players[i].clearCards();
			players[i].getFirstCards( deck, deckIdx );
		}
			
		dealer.clearCards();
		dealer.getFirstCards( deck, deckIdx );
		
		displayGoneCards();
		
		bool blkJack = displayPlayerCards( players, dealer, false );
		
		if ( blkJack )
			cout << endl;
		else
			do 
			{
				moreCards = true;
						
				int plNumber = getPlayerNumber();
				
				if ( plNumber != 0 )
				{
					int k = plNumber - 1;
					
					cout << " Player " << plNumber << ":+";
					
					players[k].getNextCard( deck, deckIdx );
					players[k].displayLastCard();
					
					int plCount = players[k].playerCount();
						
					for ( int j = 0; j < 6; j++ )
						cout << "      ";
		
					blkJack = blkJack || players[k].blackJack();
					
					cout << "   Count " << plCount << " " << PlayerCards::blackJack( plCount );
				}
				else
					moreCards = false;
			}
			while ( moreCards  &&  !blkJack );
			
		
		int dealerCount = dealer.playerCount();
		
		if ( dealerCount < DEALER_MIN )
			cout << "\n Dealer  :+";
		
		while ( dealerCount < DEALER_MIN  &&  deckIdx < DECK_SIZE*DECKS-1 )
		{
			dealer.getNextCard( deck, deckIdx );
			dealer.displayLastCard();
			
			dealerCount = dealer.playerCount();
		}
		
		int dealerIdx = dealer.playerNextIdx();
		
		if ( dealerIdx > 2 )  
		{
			for ( int j = 0; j < 9 - dealerIdx; j++ )
				cout << "      ";
			
			cout << "   Count " << dealerCount << " " << PlayerCards::blackJack( dealerCount );
		}	
		
		cout << "\n -------------------------------------------------------------------------------------------------------------- ";
		displayPlayerCards( players, dealer, true );
		
		cout << "\n Winner  : " << winners( players, dealer );
		cout << "\n ============================================================================================================== ";
		
		enoughCards = deckIdx < DECK_SIZE*DECKS - 2 * (PLAYERS + 1) - 4;
		
		if ( !enoughCards )
		{
			dck++;
			
			deckIdx = 1;
			enoughCards = true;
			
			shuffleDeck( deck, DECK_SIZE, DECKS );
		
			for ( int i = 0; i < 13; i++ )
				PlayerCards::cardsCount[i] = 0;
				
			PlayerCards::score = 0;
			
			playersWins = 0;
			dealerWins = 0;
	
			cout << "\n New shuffle...";
		}				
	}
	while ( enoughCards );
	
	return 0;
}


bool displayPlayerCards( PlayerCards* players, PlayerCards dealer, bool open )
{
	int plIndex = 2;
	bool bj = false;
	
	bool blkJack = false;
	int plNextIdx;
	
	for ( int i = 0; i < PLAYERS; i++ )
	{
		cout << "\n Player " << i+1 << ": ";
		players[i].displayAllCards();
		
		plNextIdx = players[i].playerNextIdx();
		
		if ( plNextIdx > plIndex )
			plIndex = plNextIdx;
		
		if ( open )
			players[i].countGoneCards();
		
		blkJack = players[i].blackJack();
		
		bj = bj || blkJack;
	}
	
	cout << "\n Dealer  : ";
	
	blkJack = dealer.blackJack();
	
	if ( open )
	{
		dealer.countGoneCards();
		dealer.displayAllCards();
		
		bj = bj || blkJack;
	}
	else
	{
		plNextIdx = dealer.playerNextIdx();
			
		for ( int j = 0; j < plNextIdx; j++ )
			if ( plIndex == 2  &&  j == 0 )
				cout << "  ####";
			else
				dealer.displayCard(j);
		
		int dealerCount = dealer.playerCount();
				
		if ( plIndex > 2 )
			cout << "   Count " << dealerCount << " " << PlayerCards::blackJack( dealerCount );	
	}
	
	return bj;	
}


string winners( PlayerCards* players, PlayerCards dealer )
{
	string str = "",
		   ties = "";
	
	int dealerCount = dealer.playerCount();
	int playerCount;
	
	for ( int i = 0; i < PLAYERS; i++ )
	{
		playerCount = players[i].playerCount();
		
		if ( playerCount < 22  &&  ( playerCount > dealerCount || dealerCount > 21 ) )
			str += ( !str.empty() ? ", " : "" ) + to_string( i+1 );
		
		else if ( playerCount  ==  dealerCount && dealerCount < 22 )
			ties += ( !ties.empty() ? ", " : "" ) + to_string( i+1 );
	}
		
	if ( !str.empty() )
		playersWins++;
	
	if ( str.empty()  &&  ties.empty() )
		dealerWins++;
			
	return  !str.empty() ? "Player(s) " + str : ( dealerCount > 21 ? "Busted" : ( ties.empty() ? "Dealer" : "Ties with player(s) " + ties ) );
}


void displayGoneCards()
{
	cout << "\n ============================================================================================================== ";		
	cout << "\n Gone : ";
	
	for ( int i = 0; i < 13; i++ )
		if ( PlayerCards::cardsCount[i] > 0 )
			cout << setw(2) << PlayerCards::ranks[i] << ":-" << setw(2) << to_string( PlayerCards::cardsCount[i] ) << "  ";
		
	cout << "\n Left : ";
	
	for ( int i = 0; i < 13; i++ )
		if ( PlayerCards::cardsCount[i] < 4*DECKS )
			cout << setw(2) << PlayerCards::ranks[i] << ": " << setw(2) << to_string( 4*DECKS - PlayerCards::cardsCount[i] ) << "  ";
			
	cout << "\n Count:  " << PlayerCards::score << "   ";
	cout << "\tPlayers' wins: " << playersWins << "   Dealer's' wins: " << dealerWins << endl;	
}


int getPlayerNumber()
{
	string inp;
	int plNumber;
	bool wrongInput;
			
	do
	{
		inp = getInput( "\n\t\t\t\t\t\t\t\t\tAnother card for player # (0 - no): " );
		
		plNumber = stoi( inp );
		
		wrongInput = plNumber > PLAYERS  ||  plNumber < 0;
	
		if ( wrongInput )
			cout << "\n\t\t\t\t\t\t\t\t\tWrong input. Try again..." << endl;
	}
	while ( wrongInput );
	
	return 	plNumber;		
}


// Fills array with random numbers from 0 to n-1
void shuffleDeck( int* arr, int deckSize, int decks )
{
	int rnd, idx;
	bool sameCard;
	
	for ( int d = 0; d < decks; d++ )
		for ( int i = 0; i < deckSize; i++ )
			do
			{
				rnd = rand() % deckSize;
				
				idx = d*deckSize + i;
				
				sameCard = find( arr + d*deckSize, arr + idx, rnd ) != arr + idx;
			
				if ( !sameCard )
					arr[idx] = rnd;
			}
			while ( sameCard );
}


// Gets input from keyboard with prompt message
string getInput( string promptMsg )
{
	string inp = "";

	do
	{
		cout << promptMsg;

		cin >> inp;
	}
	while ( inp.empty() );

	return inp;
}
