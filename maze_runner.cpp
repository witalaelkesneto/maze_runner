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

// Representação de uma posição
struct pos_t
{
	int i;
	int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;
/* Inserir elemento:

	 pos_t pos;
	 pos.i = 1;
	 pos.j = 3;
	 valid_positions.push(pos)
 */
// Retornar o numero de elementos:
//    valid_positions.size();
//
// Retornar o elemento no topo:
//  valid_positions.top();
//
// Remover o primeiro elemento do vetor:
//    valid_positions.pop();

// Função que le o labirinto de um arquivo texto, carrega em
// memória e retorna a posição inicial
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
		maze[i] = (char *)malloc(num_cols * sizeof(char *));
	}

	for (int i = 0; i < num_rows; ++i)
	{
		for (int j = 0; j < num_cols; ++j)
		{
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			maze[i][j] = fgetc(file);
			// Se o valor for 'e' salvar o valor em initial_pos
			if (maze[i][j] == 'e')
			{
				initial_pos.i = i;
				initial_pos.j = j;
			}
		}
		fgetc(file);
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
	}
	printf("\n");
}

// Função responsável pela navegação.
// Recebe como entrada a posição inicial e retorna um booleano indicando se a saída foi encontrada
bool walk(pos_t pos)
{
	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas

	// Marcar a posição atual com o símbolo '.'
	maze[pos.i][pos.j] = '.';
	// Limpa a tela
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	system("clear");
	// Imprime o labirinto
	print_maze();

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
	if (pos.j + 1 <= num_rows)
	{
		if (maze[pos.i][pos.j + 1] == 'x')
		{
			valid_positions.push({pos.i, pos.j + 1});
		}
		else if (maze[pos.i][pos.j + 1] == 's')
		{
			return true;
		}
	}
	if (pos.j - 1 >= 0)
	{
		if (maze[pos.i][pos.j - 1] == 'x')
		{
			valid_positions.push({pos.i, pos.j - 1});
		}
		else if (maze[pos.i][pos.j - 1] == 's')
		{
			return true;
		}
	}
	if (pos.i + 1 <= num_cols)
	{
		if (maze[pos.i + 1][pos.j] == 'x')
		{
			valid_positions.push({pos.i + 1, pos.j});
		}
		else if (maze[pos.i + 1][pos.j] == 's')
		{
			return true;
		}
	}
	if (pos.i - 1 >= 0)
	{
		if (maze[pos.i - 1][pos.j] == 'x')
		{
			valid_positions.push({pos.i - 1, pos.j});
		}
		else if (maze[pos.i - 1][pos.j] == 's')
		{
			return true;
		}
	}

	// Verifica se a pilha de posições nao esta vazia
	if (!valid_positions.empty())
	{
		// Caso não esteja, pegar o primeiro valor de  valid_positions, remove-lo e chamar a funçao walk com esse valor
		pos_t next_position = valid_positions.top();
		valid_positions.pop();
		std::cout << "Position: (" << next_position.i << ", " << next_position.j << ")\n";
		walk(next_position);
	}
	else
	{
		// Caso contrario, retornar falso
		return false;
	}
}

int main(int argc, char *argv[])
{
	system("clear");
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze("../data/maze.txt");
	print_maze();
	// chamar a função de navegação
	bool exit_found = walk(initial_pos);
	// Tratar o retorno (imprimir mensagem)

	return 0;
}
