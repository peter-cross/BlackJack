#include "PlayerCards.h"

#include <iostream>
#include <ctime>
#include <iomanip>
#include <algorithm>
using namespace std;


const string PlayerCards::hearts[13]   = { "2 \3", "3 \3", "4 \3", "5 \3", "6 \3", "7 \3", "8 \3", "9 \3", "10 \3", "J \3", "Q \3", "K \3", "A \3" },
	   		 PlayerCards::diamonds[13] = { "2 \4", "3 \4", "4 \4", "5 \4", "6 \4", "7 \4", "8 \4", "9 \4", "10 \4", "J \4", "Q \4", "K \4", "A \4" },
	   		 PlayerCards::clubs[13]    = { "2 \5", "3 \5", "4 \5", "5 \5", "6 \5", "7 \5", "8 \5", "9 \5", "10 \5", "J \5", "Q \5", "K \5", "A \5" },
	   		 PlayerCards::spades[13]   = { "2 \6", "3 \6", "4 \6", "5 \6", "6 \6", "7 \6", "8 \6", "9 \6", "10 \6", "J \6", "Q \6", "K \6", "A \6" };

const string PlayerCards::ranks[13] = { "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };

const int PlayerCards::COUNT[13] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11 };

int PlayerCards::cardsCount[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	PlayerCards::score = 0;

	
PlayerCards::PlayerCards()
{
	clearCards();
}


void PlayerCards::clearCards()
{
	nextIdx = 0;
	
	for ( int i = 0; i < 10; i++ )
		cards[i] = 0;
}


void PlayerCards::getFirstCards( int* deck, int &deckIdx )
{
	cards[0] = deck[ deckIdx++ ];
	cards[1] = deck[ deckIdx++ ];
	
	nextIdx = 2;
}


void PlayerCards::getNextCard( int* deck, int &deckIdx )
{
	cards[ nextIdx++ ] = deck[ deckIdx++ ];
}


int PlayerCards::playerCount()
{
	int plCount = playerCount( nextIdx-1 );
	
	return plCount <= 21 ? plCount : adjustCount( plCount );
}


int PlayerCards::playerCount( int cardIdx )
{
	if ( cardIdx == 0 )
		return countCard( cards[cardIdx] );
	
	else if ( cardIdx < 0 )
		return 0;
	
	return countCard( cards[cardIdx] ) + playerCount( cardIdx - 1 );
}


int PlayerCards::countCard( int deckIdx )
{
	int crdIdx = deckIdx % 13;
	
	return COUNT[ crdIdx ];
}


int PlayerCards::adjustCount( int plCount )
{
	int aces = 0;
	
	for ( int i = 0; i < nextIdx  &&  cards[i] > 0; i++ )
		if ( cards[i] % 13 == 12 ) // Ace
			aces++;
	
	while ( plCount > 21  &&  aces > 0 )
	{
		plCount -= 10; 
		aces--;
	}
		
	return plCount;
}


int PlayerCards::playerNextIdx()
{
	return nextIdx;	
}


bool PlayerCards::blackJack()
{
	return playerCount() == 21;
}


void PlayerCards::displayLastCard()
{
	displayCard( nextIdx-1 );
}


void PlayerCards::displayCard( int cardIdx )
{
	const string* suit[] = { hearts, diamonds, clubs, spades };
	
	int deckIdx = cards[ cardIdx ];
	int suitIdx = deckIdx / 13;
	int rankIdx = deckIdx % 13;
	
	cout << setw(6) << suit[ suitIdx ][ rankIdx ];
}


void PlayerCards::displayAllCards()
{
	for ( int i = 0; i < nextIdx; i++ )
		displayCard(i);
	
	for ( int i = 0; i < 7 - nextIdx; i++ )
		cout << "      ";
	
	int plCount = playerCount();
	
	cout << "   Count " << plCount << " " << blackJack( plCount );
}


void PlayerCards::countGoneCards()
{
	countGoneCards( nextIdx-1 );
}


void PlayerCards::countGoneCards( int cardIdx )
{
	if ( cardIdx < 0 )
		return;
	
	int rankIdx = cards[ cardIdx ] % 13;
	
	cardsCount[ rankIdx ] += 1;
		
	if ( rankIdx < 6 )
		score++;
	else if ( rankIdx > 6 )
		score--;
		
	countGoneCards( cardIdx - 1 );
}


string PlayerCards::blackJack( int cnt )
{
	if ( cnt == 21 )
		return "BJ";
	else if ( cnt > 21 )
		return "Busted";
	else
		return "";
}
