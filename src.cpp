#include <vector>
#include <iostream>
using namespace std;
int computer = 1;
int human = -1;
class Node
{
public:
	int board[3][3];
	int bestboard[3][3];
	int player;

	Node(void);
	Node(int[3][3], int);
	int getHeuristicValue(void);
	void makeMove(int, int, int);
	void drawBoard(void);

}; 
Node::Node(void)
{
	memcpy(board, new int[3][3], sizeof(board));
}
Node::Node(int inboard[3][3], int inplayer)
{
	memcpy(board, inboard, sizeof(board));
	player = inplayer;
}
void Node::drawBoard()
{
	for (int r = 0; r < 3; r++)
	{
		for (int c = 0; c < 3; c++)
		{
			if (board[r][c] == computer)
				cout << 'x' << " ";
			else if (board[r][c] == human)
				cout << 'o'<<" ";
			else
				cout << "-" << " ";
		}
		cout << '\n';
	}
}
void Node::makeMove(int r, int c, int player)
{
	board[r][c] = player;
}
int Node::getHeuristicValue(void)
{
	for (int r = 0; r < 3; r++)
	{
		for (int c = 0; c < 3; c++)
		{
			if (board[r][c] == board[r][(c + 1) % 3] && board[r][(c + 1) % 3] && board[r][(c + 1) % 3] == board[r][(c + 2) % 3])
				return board[r][c] * 10;
			if (board[r][c] == board[(r + 1) % 3][c] && board[(r + 1) % 3][c] && board[(r + 1) % 3][c] == board[(r + 2) % 3][c])
				return board[r][c] * 10;

			if (r==c&&board[r][c] == board[(r + 1) % 3][(c + 1) % 3] && board[(r + 1) % 3][(c + 1) % 3] && board[(r + 1) % 3][(c + 1) % 3] == board[(r + 2) % 3][(c + 2) % 3])
				return board[r][c] * 10;

			if ((r == 1 && c == 1) || (c == 2 && r == 0) || (c == 0 && r == 2))
			{
				if (board[r][c] == board[(r + 1) % 3][(c - 1 + 3) % 3] && board[(r + 1) % 3][(c - 1 + 3) % 3] && board[(r + 1) % 3][(c - 1 + 3) % 3] == board[(r + 2) % 3][(c - 2 + 3) % 3])
					return board[r][c] * 10;
			}
		}
	}
	return 0;
}


vector<Node> getChildren(Node node,int player)
{
	vector<int> moves;
	vector<Node> children;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (node.board[i][j] == 0)
			{
				moves.push_back(i);
				moves.push_back(j);
			}
		}
	}
	if (moves.size() > 0)
	{
		for (int i = 0; i < moves.size() - 1; i += 2)
		{
			int tmp[3][3];
			memcpy(tmp, node.board, sizeof(tmp));
			tmp[moves.at(i)][moves.at(i + 1)] = player;
			Node child = Node(tmp, player);
			children.push_back(child);
		}
	}
	return children;
}
int minimax(Node* nodeptr, int player, int depth, int maxdepth)
{
	Node node = *nodeptr;
	vector<Node> children = getChildren(node, player);
	int nodeValue = node.getHeuristicValue();
	if (depth == maxdepth || children.size() == 0 || nodeValue != 0)
		return nodeValue;
		if (player == computer)
		{
			int bestValue = -INFINITY;
			for each(Node child in children)
			{
				int val = minimax(&child, -player, depth + 1, maxdepth);
				if (val > bestValue)
				{
					bestValue = val;
					memcpy(node.bestboard, child.board, sizeof(node.bestboard));
				}
			}
			if (depth == 0)
				memcpy(nodeptr->board, node.bestboard, sizeof(nodeptr->board));
	
			return bestValue;
		}
		else
		{
			int bestValue = INFINITY;
			for each(Node child in children)
			{
				int val = minimax(&child, -player, depth + 1, maxdepth);
				if (val < bestValue)
				{
					bestValue = val;
				}
			}
			return bestValue;
		}
}
int negamax(Node* nodeptr,int player, int depth,int maxdepth)
{
	Node node = *nodeptr;
	vector<Node> children = getChildren(node, player);
	int nodeValue = node.getHeuristicValue();

	if (depth == maxdepth || children.size() == 0 || nodeValue != 0)
		return player * nodeValue;

	int bestValue = -INFINITY;
	for each(Node child in children)
	{
		int val = -negamax(&child,-player, depth + 1, maxdepth);
		if (val > bestValue)
		{
			bestValue = val;
			memcpy(node.bestboard, child.board, sizeof(node.bestboard));
		}


	}
	if (depth == 0)
		memcpy(nodeptr->board, node.bestboard, sizeof(nodeptr->board));

	return bestValue;
}

int abnegamax(Node* nodeptr,int alpha, int beta, int depth,int maxdepth, int player)
{
	Node node = *nodeptr;
	vector<Node> children = getChildren(node, player);
	int nodeValue = node.getHeuristicValue();

	if (depth == maxdepth || children.size() == 0 || nodeValue != 0)
		return player * nodeValue;

	int bestValue = -INFINITY;
	for each(Node child in children)
	{
		int val = -abnegamax(&child, -beta, -alpha, depth + 1, maxdepth,-player);
		if (val > bestValue)
		{
			bestValue = val;
			memcpy(node.bestboard, child.board, sizeof(node.bestboard));
			alpha = fmax(alpha, val);
			if (alpha >= beta)
				break;
		}
	}
	if (depth == 0)
		memcpy(nodeptr->board, node.bestboard, sizeof(nodeptr->board));

	return bestValue;
}

int main(void)
{
	int testboard[3][3] = { { 0, 0, 0 },
							{ 0, 0, 0 },
							{ 0, 0, 0 } };
	Node node = Node(testboard, computer);
	int r, c;
	while (true)
	{
		abnegamax(&node,-INFINITY,INFINITY, 0, 9, computer) << '\n';
		node.drawBoard();
		cout << "\n";
		cout << "pick row:";
		cin >> r;
		cout << "pick col:";
		cin >> c;
		cout << '\n';
		node.makeMove(r-1, c-1, human);
	}
	cin.get();
	return 0;
}