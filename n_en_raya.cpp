#include <iostream>
#include <vector>


using map = std::vector<std::vector<char>>;

class game
{
public:
	game(const size_t& in_n) :
		n(in_n), table()
	{ }

private:
	map table;
	size_t n;

	int count_options_to_win(const char& game_piece)
	{
		size_t options = 0;

		for (size_t i = 0; i < n; i++)
		{
			if (check_column(i, game_piece))
				options++;

			if (check_row(i, game_piece))
				options++;
		}

		check_diagonal(game_piece, options);
	}

	bool check_column(const size_t& index, const char& game_piece)
	{

		for (int i = 0; i < n; i++)
		{
			if (table[i][index] != ' ' && table[i][index] != game_piece)
				return false;
		}

		return true;
	}

	bool check_row(const size_t& index, const char& game_piece)
	{

		for (int i = 0; i < n; i++)
		{
			if (table[index][i] != ' ' && table[index][i] != game_piece)
				return false;
		}

		return true;
	}

	void check_diagonal(const char& game_piece, size_t&counter)
	{
		counter += 2;

		for (int i = 0; i < n; i++)
		{
			if (table[i][i] != ' ' && table[i][i] != game_piece)
			{
				counter--;
				break;
			}
		}

		for (int i = 0; i < n; i++)
		{
			if (table[i][n - i - 1] != ' ' && table[i][n - i - 1] != game_piece)
			{
				counter--;
				break;
			}
		}
	}
};

int main()
{

}