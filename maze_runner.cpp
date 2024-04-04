#include <iostream>
#include <stdio.h>
#include <stack>
#include <fstream>
#include <thread>
#include <chrono>

// Matriz de char representnado o labirinto
char **maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;
bool exit_found = false;

// Representação de uma posição
struct pos_t
{
	int i;
	int j;
};

// Função que le o labirinto de um arquivo texto, carrega em memória e retorna a posição inicial
pos_t load_maze(const char *file_name)
{
	pos_t initial_pos;
	// Abre o arquivo para leitura (fopen)
	FILE *file = fopen(file_name, "r");

	// Le o numero de linhas e colunas (fscanf) e salva em num_rows e num_cols
	fscanf(file, "%d %d", &num_rows, &num_cols);

	// Aloca a matriz maze (malloc)
	maze = (char **)malloc(num_rows * sizeof(char *));

	for (int i = 0; i < num_rows; ++i)
	{
		// Aloca cada linha da matriz
		maze[i] = (char *)malloc(num_cols * sizeof(char));
	}

	for (int i = 0; i < num_rows; ++i)
	{
		for (int j = 0; j < num_cols; ++j)
		{
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			fscanf(file, " %c", &maze[i][j]);
			//  Se o valor for 'e' salvar o valor em initial_pos
			if (maze[i][j] == 'e')
			{
				initial_pos.i = i;
				initial_pos.j = j;
			}
		}
	}
	return initial_pos;
}

// Função que imprime o labirinto
void print_maze()
{
	for (int i = 0; i < num_rows; ++i)
	{
		for (int j = 0; j < num_cols; ++j)
		{
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}

// Função responsável pela navegação.
// Recebe como entrada a posição inicial e retorna um booleano indicando se a saída foi encontrada
void walk(pos_t pos)
{
	if (maze[pos.i][pos.j] != '.')
	{
		// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
		// Marcar a posição atual com o símbolo 'o'
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		maze[pos.i][pos.j] = 'o';
		/* Dado a posição atual, verifica quais sao as próximas posições válidas
			Checar se as posições abaixo são validas (i>0, i<num_rows, j>0, j <num_cols)
			e se são posições ainda não visitadas (ou seja, caracter 'x') e inserir
			cada uma delas no vetor valid_positions
				- pos.i, pos.j+1
				- pos.i, pos.j-1
				- pos.i+1, pos.j
				- pos.i-1, pos.j
			Caso alguma das posiçÕes validas seja igual a 's', retornar verdadeiro
		*/
		if (pos.j + 1 < num_cols)
		{
			if (maze[pos.i][pos.j + 1] == 'x')
			{
				std::thread t(walk, pos_t{pos.i, pos.j + 1});
				t.detach();
			}
			else if (maze[pos.i][pos.j + 1] == 's')
			{
				exit_found = true;
			}
		}
		if (pos.j - 1 >= 0)
		{
			if (maze[pos.i][pos.j - 1] == 'x')
			{
				std::thread t(walk, pos_t{pos.i, pos.j - 1});
				t.detach();
			}
			else if (maze[pos.i][pos.j - 1] == 's')
			{
				exit_found = true;
			}
		}
		if (pos.i + 1 < num_rows)
		{
			if (maze[pos.i + 1][pos.j] == 'x')
			{
				std::thread t(walk, pos_t{pos.i + 1, pos.j});
				t.detach();
			}
			else if (maze[pos.i + 1][pos.j] == 's')
			{
				exit_found = true;
			}
		}
		if (pos.i - 1 >= 0)
		{
			if (maze[pos.i - 1][pos.j] == 'x')
			{
				std::thread t(walk, pos_t{pos.i - 1, pos.j});
				t.detach();
			}
			else if (maze[pos.i - 1][pos.j] == 's')
			{
				exit_found = true;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		maze[pos.i][pos.j] = '.';
	}
}

int main(int argc, char *argv[])
{
	system("clear");
	pos_t initial_pos = load_maze("../data/maze6.txt");
	std::thread t(walk,initial_pos);
	while (!exit_found)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		system("clear");
		print_maze();
	}
	t.join();
	if (exit_found)
	{
		std::cout << "Saída encontrada" << std::endl;
	}
	return 0;
}