#include<iostream>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<algorithm>
#include<string.h>
#include<queue>
#include<cmath>
#include<map>
#include<set>
using namespace std;

const double START_RATING = 1500.000;
const int MIN_GAMES = 15;

map<string,int>name_id;

inline int read()
{
    int x=0;char ch;
    ch=getchar();
    while(ch<'0'||ch>'9')ch=getchar();
    while(ch>='0'&&ch<='9')x=x*10+ch-'0',ch=getchar();
    return x;
}

struct player_info
{
	string name;
	int game_won;
	int game_lost;
	double rating;
//	bool operator<( const player_info& i ) { return rating > i.rating; }
	
}pi[200];

int games[1000][4];

int game_num=0;
int player_num=0;

double k_factor_calculate(int id)
{
	int game_played=pi[id].game_lost+pi[id].game_won;
	double x;
	if(game_played)
	x=(800/game_played*1.000);
	else
	x=48.00;
	x=max(x,48.000);
	x=min(x,16.000);
	return x;
}

void player_rating_calculate(int winner_id,int loser_id)
{
	
	double q_winner_rating=pow(10,pi[winner_id].rating/400.000);
	double q_loser_rating=pow(10,pi[loser_id].rating/400.000);
	
	
	double winner_expected_score = q_winner_rating/(q_winner_rating+q_loser_rating);
	double loser_expected_score = q_loser_rating/(q_winner_rating+q_loser_rating);
	double winner_new_score = pi[winner_id].rating + (k_factor_calculate(winner_id) * (1 - winner_expected_score));
	double loser_new_score =  pi[loser_id].rating + (k_factor_calculate(loser_id) * (0 - loser_expected_score));
	pi[winner_id].rating=winner_new_score;
	pi[loser_id].rating=loser_new_score;
}

int id_find(string name)
{
	int id;
	
	if(!name_id.count(name))
	{
		return -1;
	}
	
	map<string, int>::iterator iter;
	iter = name_id.find(name);
	/*
	if(iter == name_id.end())
	id=-1;
	else*/
	id=iter->second;
	
/*	
	int id=name_id.find(name);
	
	if(id==name_id.end())
	id=-1;
	*/
	return id;
}

void player_insert(string name)
{
	player_num++;
	
	name_id.insert(make_pair<string,int>(name,player_num));
	
	pi[player_num].rating=START_RATING;
	pi[player_num].name=name; 
	
}

double win_rating(string name)
{
	int id=id_find(name);
	
	int game_played=pi[id].game_lost+pi[id].game_won;
	
	double x=pi[id].game_won/game_played;
	
	return x;
}

void game_insert(string winner,string loser)
{
	int winner_id=id_find(winner);
	
	int loser_id=id_find(loser);
	
	if(winner_id==-1)
	{
		player_insert(winner);
		winner_id=id_find(winner);
	}
	if(loser_id==-1)
	{
		player_insert(loser);
		loser_id=id_find(loser);
	}
	game_num++;
	
	pi[loser_id].game_lost++;
	pi[winner_id].game_won++;
	games[game_num][1]=winner_id;
	games[game_num][2]=loser_id;
	player_rating_calculate(winner_id,loser_id);
}

void input_file()
{
	string winner;
	string loser;
	
	cin >> winner >> loser;
	game_insert(winner,loser);
}

void output_file()
{
//	freopen("16OctRatings.txt","w",stdout);
	
	cout << "There are " << game_num << " games and " << player_num << " players in total." << endl;
	cout << "Minimum games played is " << MIN_GAMES << "." << endl;
	
	for(int p=1;p<=player_num;p++)
	{
		if(pi[p].game_lost+pi[p].game_won>=15)
		cout << pi[p].name << "," << pi[p].rating << endl;
	}
//	fclose(stdout);
}
int main()
{
	freopen("Games_16Oct.txt","r",stdin);
	int n;
	n=read();
	for(int p=1;p<=n;p++)
	input_file();
	
	output_file();
	fclose(stdout);
	return 0;
	
}

/*
void rating_sort()
{
	sort(pi+1,pi+player_num+1);
	
	name_id.clear();
	for(int p=1;p<=player_num;p++)
	name_id.insert(make_pair<string,int>(pi[p].name,p));
}
*/
