// O objetivo desse programa é calcular a média dos dados do coeficiente de arrasto (Cd) calculado pelo OpenFoam
// JACM

#include <iostream>
#include <fstream>
#include <cstdio>

#define NARQ "teste" //"/home/meloo/Documents/CFD_OpenFOAM/Cilindro-termico/Re-200/Re-200_9k/postProcessing/forceCoef/0/forceCoeffs.dat" // é o nome do arquivo
#define NCOL 6 // é o n° de colunas no arquivo acima
#define NTXT 50 // é o n° máximo de caracteres em uma linha

#define DIST 14	// distância em caracteres da coluna de tempo para a primeira coluna de coeficientes

using std::cout, std::cin, std::endl, std::cerr, std::ifstream;

int contaLinhas();
void separaStr(char *, double **, int);
double proxNum(char *, int);

int main(int argc, char **argv)
{
	FILE *arquivo = NULL;
	int nLinha = 0;
	char txt[NTXT];
	
	// Conta o total de linhas antes de abrir o arquivo (isso é importante)
	int nLinhas = contaLinhas();
	
	// Lendo o arquivo APÓS contar as linhas (IMPORTANTE)
	arquivo = fopen(NARQ, "r");
	if(arquivo == NULL)
	{
		cerr << "Erro: O arquivo \"" << NARQ << "\" Não pode ser lido ou encontrado" << endl;
		
		exit(-1);
	}
	
	// Criando uma matriz com alocação dinâmica em baixo nível
	double **mat = (double**) calloc(sizeof(double*), nLinhas);
	
	for(int i = 0 ; i < nLinhas ; i++)	
	{
		mat[i] = (double*) calloc(sizeof(double), NCOL);
	}
	
	// Inicio da leitura do arquivo
	while( ( fscanf(arquivo, "%[^\n] ", txt) ) != EOF )
	{
		 separaStr(txt, mat, nLinha);
		 
		 nLinha++;
	}

	for(int i = 0 ; i < nLinhas  ; i++)
	{
		for(int j = 0 ; j < NCOL ; j++)
			cout << mat[i][j] << "\t";
		
		cout << endl;
	}
	
	// Liberando memória alocada
	for(int i = 0 ; i < nLinhas ; i++)	
	{
		free( mat[i] );
	}
	
	free(mat);
	
	// Fechando arquivo
	fclose(arquivo);
	
	return 0;
}

int contaLinhas()
{
	FILE *arquivo = NULL;
	int nLinhas = 0;
	char caractere;
	
	arquivo = fopen(NARQ, "r");
	if(arquivo == NULL)
	{
		cerr << "Erro: O arquivo \"" << NARQ << "\" Não pode ser lido ou encontrado" << endl;
	
		exit(-1);
	}
	while( fscanf(arquivo, "%*[^\n] ") != EOF )
		nLinhas++;
	
	fclose(arquivo);
	
	return nLinhas;
}

void separaStr(char* txt, double **mat, int nLinha)
{
	for(int i = 0 ; i < NCOL ; i++)
	{
		mat[nLinha][i] = proxNum(txt, i);
	}
		
	return;
}

double proxNum(char* text, int num)
{
	char *var = NULL, *txt = text;
	int nIndice = 0;
	
	if(num > 0)
		txt = &txt[DIST + 1];
	
	for(int i = 0 ; i < NTXT ; i++ )
	{
		if((txt[i] != ' ' || txt[i] != '\t')  )
			var = &txt[i];
		
		if( txt[i] == '\t' || txt[i] == ' ' )
			nIndice++;

		if(nIndice == num)
			return atof(var);
	}
	
	return 0; //O compilador tá me enchendo pra botar isso
}
