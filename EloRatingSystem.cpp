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
const int MIN_GAMES = 12;

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
	int rating_change_history_time;
	string rating_change_history_oppo[50];
	double rating_change_history_rating[50];
	
}pi[200];

int games[1000][4];
int game_num=0;
int player_num=0;

bool sort_compare(player_info a, player_info b)
{
	return a.rating > b.rating;
}

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

void history_rating_mark(int self_id,string opponant)
{
	pi[self_id].rating_change_history_time++;
	pi[self_id].rating_change_history_oppo[pi[self_id].rating_change_history_time]= opponant;
	pi[self_id].rating_change_history_rating[pi[self_id].rating_change_history_time]=pi[self_id].rating;
	return;
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

void history_rating_output(int player_id)
{
	freopen("historyrating0.TXT","w",stdout);
	
	for(int p=1;p<=pi[player_id].rating_change_history_time;p++)
	cout << pi[player_id].rating_change_history_oppo[p] << " " << pi[player_id].rating_change_history_rating[p]<< endl;
	
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

double win_rating(int id)
{
	
	int game_played=pi[id].game_lost+pi[id].game_won;
	double x=100.0000*pi[id].game_won/game_played;
	
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
	history_rating_mark(winner_id,loser);
	history_rating_mark(loser_id,winner);
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
	freopen("16OctRatings.out","w",stdout);
	
	cout << "There are " << game_num << " games and " << player_num << " players in total." << endl;
	cout << "Minimum games played is " << MIN_GAMES << "." << endl;
	
	for(int p=1;p<=player_num;p++)
	{
		if(pi[p].game_lost+pi[p].game_won>=MIN_GAMES)
		cout << pi[p].name << " , " << pi[p].rating << " , " <<  win_rating(p)  << " , " << pi[p].game_lost+pi[p].game_won << endl;
	}
	fclose(stdout);
}

void rating_sort()
{
	sort(pi+1,pi+player_num+1,sort_compare);
	
	name_id.clear();
	for(int p=1;p<=player_num;p++)
	name_id.insert(make_pair<string,int>(pi[p].name,p));
}

int main()
{
	freopen("Games_16Oct.txt","r",stdin);
	int n;
	n=read();
	for(int p=1;p<=n;p++)
	input_file();
	rating_sort();
	history_rating_output(id_find("WFZ"));
//	output_file();
	fclose(stdout);
	return 0;
	
}




