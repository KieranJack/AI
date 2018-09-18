#include<iostream>
#include<string>
using namespace std;

enum playType { Rock, Paper, Scissors , NoType};
enum result { Win, Lose, Tie };

playType Player, AI1, AI2;

const int RunThroughs = 1000;	//Number of games
playType PlayerAll [RunThroughs];	//Stores player plays for all games
int CurrentGame = 0;

struct AIInfo {
	result Record[RunThroughs];	//Stores result for each game for each AI
	int Wins = 0;	//Number of wins
	int Loses = 0;	//Number of loses
}m_AI1, m_AI2;


struct TotalPlayerPlays {
	//Total of each type played by player
	float Rock = 0;	
	float Paper = 0;
	float Scissors = 0;
}m_TotalPlayer;

struct PlayerProbables {
	//Average percentage that each type is played by a human
	float Rock = 35.4;
	float Paper = 29.6;
	float Scissors = 35.0;
	//Current percentage that each type has been played by player
	float RockPercentage;
	float PaperPercentage;
	float ScissorsPercentage;
	//Total of each type played by player that match W/L/T
	int RockCount;
	int PaperCount;
	int ScissorsCount;
}m_PlayerProbs;

playType GetPlayerPlay() {
	int PlayerInput;
	do {
		cin >> PlayerInput;
		if (PlayerInput < 0 || PlayerInput > 2) {
			cout << "Please input either 0, 1 or 2\n";
			cin.clear(); // reset failbit
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip bad input
		}
	} while (PlayerInput < 0 || PlayerInput > 2);
	switch (PlayerInput) {
	case 0: 
		m_TotalPlayer.Rock++;
		return Rock;
		break;
	case 1: 
		m_TotalPlayer.Paper++;
		return Paper;
		break;
	case 2: 
		m_TotalPlayer.Scissors++;
		return Scissors;
		break;
	};

	PlayerAll[CurrentGame] = Player;
}

void HeadingDisplay() {
	cout << "        Press 0 to pick Rock, 1 to pick Paper, 2 to pick Scissors\n";
	Player = GetPlayerPlay();
	cout << "Player       AI1          AI2          Total Games  AI1 Wins     AI1 Loses    AI2 Wins     AI2 Loses\n";
}

void AnswerDisplay() {
	switch (Player) {
	case Rock: cout << "Rock         ";
		break;
	case Paper: cout << "Paper        ";
		break;
	case Scissors: cout << "Scissors     ";
		break;
	}
	switch (AI1) {
	case Rock: cout << "Rock         ";
		break;
	case Paper: cout << "Paper        ";
		break;
	case Scissors: cout << "Scissors     ";
		break;
	}
	switch (AI2) {
	case Rock: cout << "Rock         ";
		break;
	case Paper: cout << "Paper        ";
		break;
	case Scissors: cout << "Scissors     ";
		break;
	}
	cout << CurrentGame + 1 << "            " << m_AI1.Wins << "            " << m_AI1.Loses << "            " << m_AI2.Wins << "            " << m_AI2.Loses << endl << endl;
}

playType GetAI1Play(){
	//Finite State Machine AI
	if (CurrentGame == 0) {
		//Majority of people will play Rock first
		return Paper;
	}
	else {
		switch (m_AI1.Record[CurrentGame - 1]) {
		case Win:	//Last game was a win
			switch (AI1) {
			case Rock:
				//Player lost last game with Scissors so should change to Rock
				return Paper;
				break;
			case Paper:
				//Player lost last game with Rock so should change to Paper
				return Scissors;
				break;
			case Scissors:
				//Player lost last game with Paper so should change to Scissors
				return Rock;
				break;
			}
			break;
		case Lose:	//Last game was a loss
			switch (AI1) {
			case Rock:
				if (PlayerAll[CurrentGame - 1] != PlayerAll[CurrentGame - 2]) {
					//Player won last game with Paper and has only played Paper once in a row so should play Paper again
					return Scissors;
				}
				else {
					//Player won last game with Paper and has played Paper twice in a row so should change
					if ((m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage) > (m_PlayerProbs.Scissors - m_PlayerProbs.ScissorsPercentage)) {	//Check whether player has played more Rock or Scissors with average play percentage for each in mind
					//Player is more likely to play Rock
						return Paper;
					}
					else {
						//Player is more liekly to play Scissors
						return Rock;			//Doesn't change value, just added for clarity
					}
				}
				break;
			case Paper:
				if (CurrentGame == 1) {
					//Player won last game with Scissors and has only played Scissors once in a row so should play Scissors again
					return Rock;
				}
				else if (PlayerAll[CurrentGame - 1] != PlayerAll[CurrentGame - 2]) {
					//Player won last game with Scissors and has only played Scissors once in a row so should play Scissors again
					return Rock;
				}
				else {
					//Player won last game with Scissors and has played Scissors twice in a row so should change
					if ((m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage) > (m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage)) {	//Check whether player has played more Paper or Rock with average play percentage for each in mind
					//Player is more likely to play Paper
						return Scissors;
					}
					else {
						//Player is more liekly to play Rock
						return Paper;			//Doesn't change value, just added for clarity
					}
				}
				break;
			case Scissors:
				if (PlayerAll[CurrentGame - 1] != PlayerAll[CurrentGame - 2]) {
					//Player won last game with Rock and has only played Rock once in a row so should play Rock again
					return Paper;
				}
				else {
					//Player won last game with Rock and has played Rock twice in a row so should change
					if ((m_PlayerProbs.Scissors - m_PlayerProbs.ScissorsPercentage) > (m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage)) {	//Check whether player has played more Scissors or Paper with average play percentage for each in mind
					//Player is more likely to play Scissors
						return Rock;
					}
					else {
						//Player is more liekly to play Paper
						return Scissors;			//Doesn't change value, just added for clarity
					}
				}
				break;
			}
			break;
		case Tie:	//Last game was a tie
			switch (AI1) {
			case Rock:
				if (PlayerAll[CurrentGame - 1] != PlayerAll[CurrentGame - 2]) {
					//Player has not played Rock twice in a row so may or may not change. Check whether player has played more Rock, Paper or Scissors with average play percentage for each in mind
					if (((m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage) > (m_PlayerProbs.Scissors - m_PlayerProbs.ScissorsPercentage)) && ((m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage) > (m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage))) {
						//Player is more likely to play Rock
						return Paper;
					}
					else if (((m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage) > (m_PlayerProbs.Scissors - m_PlayerProbs.ScissorsPercentage)) && ((m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage) > (m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage))) {
						//Player is more likely to play Paper
						return Scissors;
					}
					else {
						//Player is more likely to play Scissors
						return Rock;			//Doesn't change value, just added for clarity
					}
				}
				else {
					//Player has played Rock twice in a row so should change. Check whether player has played more Paper or Scissors with average play percentage for each in mind
					if ((m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage) > (m_PlayerProbs.Scissors - m_PlayerProbs.ScissorsPercentage)) {
						//Player is more likely to play Paper
						return Scissors;
					}
					else {
						//Player is more likely to play Scissors
						return Rock;			//Doesn't change value, just added for clarity
					}
				}
				break;
			case Paper:
				if (PlayerAll[CurrentGame - 1] != PlayerAll[CurrentGame - 2]) {
					//Player has not played Paper twice in a row so may or may not change. Check whether player has played more Paper, Scissors or Rock with average play percentage for each in mind
					if (((m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage) > (m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage)) && ((m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage) > (m_PlayerProbs.Scissors - m_PlayerProbs.ScissorsPercentage))) {
						//Player is more likely to play Paper
						return Scissors;
					}
					else if (((m_PlayerProbs.Scissors - m_PlayerProbs.ScissorsPercentage) > (m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage)) && ((m_PlayerProbs.Scissors - m_PlayerProbs.ScissorsPercentage) > (m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage))) {
						//Player is more likely to play Scissors
						return Rock;
					}
					else {
						//Player is more likely to play Rock
						return Paper;			//Doesn't change value, just added for clarity
					}
				}
				else {
					//Player has played Paper twice in a row so should change. Check whether player has played more Scissors or Rock with average play percentage for each in mind
					if ((m_PlayerProbs.Scissors - m_PlayerProbs.ScissorsPercentage) > (m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage)) {
						//Player is more likely to play Scissors
						return Rock;
					}
					else {
						//Player is more likely to play Rock
						return Paper;			//Doesn't change value, just added for clarity
					}
				}
				break;
			case Scissors:
				if (PlayerAll[CurrentGame - 1] != PlayerAll[CurrentGame - 2]) {
					//Player has not played Scissors twice in a row so may or may not change. Check whether player has played more Scissors, Rock or Paper with average play percentage for each in mind
					if (((m_PlayerProbs.Scissors - m_PlayerProbs.ScissorsPercentage) > (m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage)) && ((m_PlayerProbs.Scissors - m_PlayerProbs.ScissorsPercentage) > (m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage))) {
						//Player is more likely to play Scissors
						return Rock;
					}
					else if (((m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage) > (m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage)) && ((m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage) > (m_PlayerProbs.Scissors - m_PlayerProbs.ScissorsPercentage))) {
						//Player is more likely to play Rock
						return Paper;
					}
					else {
						//Player is more likely to play Paper
						return Scissors;			//Doesn't change value, just added for clarity
					}
				}
				else {
					//Player has played Scissors twice in a row so should change. Check whether player has played more Rock or Paper with average play percentage for each in mind
					if ((m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage) > (m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage)) {
						//Player is more likely to play Rock
						return Paper;
					}
					else {
						//Player is more likely to play Paper
						return Scissors;			//Doesn't change value, just added for clarity
					}
				}
				break;
			}
			break;
		}
	}
}

playType SearchCheck(playType m_Type, playType m_Type2, playType m_Type3, result m_Result) {
	//Check what player usually plays after losing with m_Type and returns the play to beat it
	for (int RecordCheck = 5; RecordCheck < (CurrentGame - 1); RecordCheck++) {	//Loop through all games to check for other times that player lost with m_Type
		if ((PlayerAll[RecordCheck] == m_Type) && (PlayerAll[RecordCheck - 1] == m_Type2) && (PlayerAll[RecordCheck - 2] == m_Type3) && (m_AI2.Record[RecordCheck] == m_Result)) {
			switch (PlayerAll[RecordCheck + 1]) {
			case Rock:
				m_PlayerProbs.RockCount++;
				break;
			case Paper:
				m_PlayerProbs.PaperCount++;
				break;
			case Scissors:
				m_PlayerProbs.ScissorsCount++;
				break;
			}
		}
	}
	if ((m_PlayerProbs.RockCount > m_PlayerProbs.PaperCount) && (m_PlayerProbs.RockCount > m_PlayerProbs.ScissorsCount)) {	//Player usually plays Rock
		return Paper;
	}
	else if ((m_PlayerProbs.PaperCount > m_PlayerProbs.RockCount) && (m_PlayerProbs.PaperCount > m_PlayerProbs.ScissorsCount)) {	//Player usually plays Paper
		return Scissors;
	}
	else if ((m_PlayerProbs.ScissorsCount > m_PlayerProbs.RockCount) && (m_PlayerProbs.ScissorsCount > m_PlayerProbs.PaperCount)) {	//Player usually plays Scissors
		return Rock;
	}
	else {//Usual count returned a tie
		return NoType;
	}
}

playType GetAI2Play() {
	//Decision Tree AI
	playType SearchResult = SearchCheck(PlayerAll[CurrentGame - 1], PlayerAll[CurrentGame - 2], PlayerAll[CurrentGame - 3], m_AI2.Record[CurrentGame - 1]);	
	if (CurrentGame == 0) {	//Is this the first game?
		//Yes
		return Paper;
	}
	else if (SearchResult == NoType) {
		switch (m_AI2.Record[CurrentGame - 1]) {	//What was the outcome of last game?
		case Win:	//AI won
			switch (PlayerAll[CurrentGame - 1]) {	//What did player play last game?
			case Rock:	//Rock
				return Scissors;	//Player should play Paper
				break;
			case Paper:	//Paper
				return Rock;	//Player should play Scissors
				break;
			case Scissors:	//Scissors
				return Paper;	//Player should play Rock
				break;
			}
			break;
		case Lose:	//AI lost
			if (CurrentGame == 1) {	//Is this game two?
			//Yes
			//Player won last game with Scissors and has only played Scissors once in a row so should play Scissors again
			return Rock;
			}
			else {
				//No
				if (PlayerAll[CurrentGame - 1] == PlayerAll[CurrentGame - 2]) {	//Did player play the same twice in a row?
				//Yes
					switch (PlayerAll[CurrentGame - 1]) {	//What did player play last game?
					case Rock:	//Rock
								//Player won last game with Rock and has played Rock twice in a row so should change
						if ((m_PlayerProbs.Scissors - m_PlayerProbs.ScissorsPercentage) > (m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage)) {	//Check whether player has played more Scissors or Paper with average play percentage for each in mind
							//Player is more likely to play Scissors
							return Rock;
						}
						else {
							//Player is more liekly to play Paper
							return Scissors;			//Doesn't change value, just added for clarity
						}
						break;
					case Paper:	//Paper
						//Player won last game with Paper and has played Paper twice in a row so should change
						if ((m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage) > (m_PlayerProbs.Scissors - m_PlayerProbs.ScissorsPercentage)) {	//Check whether player has played more Rock or Scissors with average play percentage for each in mind
							//Player is more likely to play Rock
							return Paper;
						}
						else {
							//Player is more liekly to play Scissors
							return Rock;			//Doesn't change value, just added for clarity
						}
						break;
					case Scissors:	//Scissors
						//Player won last game with Scissors and has played Scissors twice in a row so should change
						if ((m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage) > (m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage)) {	//Check whether player has played more Paper or Rock with average play percentage for each in mind
							//Player is more likely to play Paper
							return Scissors;
						}
						else {
							//Player is more liekly to play Rock
							return Paper;			//Doesn't change value, just added for clarity
						}
						break;
					}
				}
				else {
					//No
					switch (PlayerAll[CurrentGame - 1]) {	//What did player play last game?
					case Rock:	//Rock
						return Paper;	//Player should play Rock
						break;
					case Paper:	//Paper
						return Scissors;	//Player should play Paper
						break;
					case Scissors:	//Scissors
						return Rock;	//Player should play Scissors
						break;
					}
				}
			}
			break;
		case Tie:	//Tie
			if (PlayerAll[CurrentGame - 1] == PlayerAll[CurrentGame - 2]) {	//Did player play the same twice in a row?
				//Yes
				switch (PlayerAll[CurrentGame - 1]) {	//What did player play last game?
				case Rock:	//Rock
						//Player won last game with Rock and has played Rock twice in a row so should change
					if ((m_PlayerProbs.Scissors - m_PlayerProbs.ScissorsPercentage) > (m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage)) {	//Check whether player has played more Scissors or Paper with average play percentage for each in mind
						//Player is more likely to play Scissors
						return Rock;
					}
					else {
						//Player is more liekly to play Paper
						return Scissors;			//Doesn't change value, just added for clarity
					}
					break;
				case Paper:	//Paper
						//Player won last game with Paper and has played Paper twice in a row so should change
					if ((m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage) > (m_PlayerProbs.Scissors - m_PlayerProbs.ScissorsPercentage)) {	//Check whether player has played more Rock or Scissors with average play percentage for each in mind
						//Player is more likely to play Rock
						return Paper;
					}
					else {
						//Player is more liekly to play Scissors
						return Rock;			//Doesn't change value, just added for clarity
					}
					break;
				case Scissors:	//Scissors
						//Player won last game with Scissors and has played Scissors twice in a row so should change
					if ((m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage) > (m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage)) {	//Check whether player has played more Paper or Rock with average play percentage for each in mind
						//Player is more likely to play Paper
						return Scissors;
					}
					else {
						//Player is more liekly to play Rock
						return Paper;			//Doesn't change value, just added for clarity
					}
					break;
				}
			}
			else {
				//No
				//Check which play the player has played least
				if (((m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage) > (m_PlayerProbs.Scissors - m_PlayerProbs.ScissorsPercentage)) && ((m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage) > (m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage))) {
					//Player is more likely to play Rock
					return Paper;
				}
				else if (((m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage) > (m_PlayerProbs.Scissors - m_PlayerProbs.ScissorsPercentage)) && ((m_PlayerProbs.Paper - m_PlayerProbs.PaperPercentage) > (m_PlayerProbs.Rock - m_PlayerProbs.RockPercentage))) {
					//Player is more likely to play Paper
					return Scissors;
				}
				else {
					//Player is more likely to play Scissors
					return Rock;			//Doesn't change value, just added for clarity
				}
			}
			break;
		}
	}
	else {
		//Adapting behaviour
		return SearchResult;
	}
}

void GetResult() {
	switch (Player) {
	case Rock:
		switch (AI1) {
		case Rock:
			m_AI1.Record[CurrentGame] = Tie;
			break;
		case Paper:
			m_AI1.Record[CurrentGame] = Win;
			m_AI1.Wins++;
			break;
		case Scissors:
			m_AI1.Record[CurrentGame] = Lose;
			m_AI1.Loses++;
			break;
		}
		switch (AI2) {
		case Rock:
			m_AI2.Record[CurrentGame] = Tie;
			break;
		case Paper:
			m_AI2.Record[CurrentGame] = Win;
			m_AI2.Wins++;
			break;
		case Scissors:
			m_AI2.Record[CurrentGame] = Lose;
			m_AI2.Loses++;
			break;
		}
		break;
	case Paper:
		switch (AI1) {
		case Rock:
			m_AI1.Record[CurrentGame] = Lose;
			m_AI1.Loses++;
			break;
		case Paper:
			m_AI1.Record[CurrentGame] = Tie;
			break;
		case Scissors:
			m_AI1.Record[CurrentGame] = Win;
			m_AI1.Wins++;
			break;
		}
		switch (AI2) {
		case Rock:
			m_AI2.Record[CurrentGame] = Lose;
			m_AI2.Loses++;
			break;
		case Paper:
			m_AI2.Record[CurrentGame] = Tie;
			break;
		case Scissors:
			m_AI2.Record[CurrentGame] = Win;
			m_AI2.Wins++;
			break;
		}
		break;
	case Scissors:
		switch (AI1) {
		case Rock:
			m_AI1.Record[CurrentGame] = Win;
			m_AI1.Wins++;
			break;
		case Paper:
			m_AI1.Record[CurrentGame] = Lose;
			m_AI1.Loses++;
			break;
		case Scissors:
			m_AI1.Record[CurrentGame] = Tie;
			break;
		}
		switch (AI2) {
		case Rock:
			m_AI2.Record[CurrentGame] = Win;
			m_AI2.Wins++;
			break;
		case Paper:
			m_AI2.Record[CurrentGame] = Lose;
			m_AI2.Loses++;
			break;
		case Scissors:
			m_AI2.Record[CurrentGame] = Tie;
			break;
		}
		break;
	}
}

void main()
{
	for (CurrentGame;  CurrentGame < RunThroughs; CurrentGame++) {
		//Calculate Percentage that player has played each shape
		m_PlayerProbs.RockPercentage = (CurrentGame - 1) / 100 * m_TotalPlayer.Rock;
		m_PlayerProbs.PaperPercentage = (CurrentGame - 1) / 100 * m_TotalPlayer.Paper;
		m_PlayerProbs.ScissorsPercentage = (CurrentGame - 1) / 100 * m_TotalPlayer.Scissors;

		//Reset AI2 search results
		m_PlayerProbs.RockCount = 0;
		m_PlayerProbs.PaperCount = 0;
		m_PlayerProbs.ScissorsCount = 0;

		//Display instructions, get Player and AI plays
		HeadingDisplay();	//Contains GetPlayerPlay

		AI1 = GetAI1Play();

		AI2 = GetAI2Play();

		//Keep track of what plays the Player and AI have made
		PlayerAll[CurrentGame] = Player;

		//Calculate whether AI won or lost
		GetResult();

		//Display Player and AI plays and total AI wins and loses
		AnswerDisplay();
	}
}
