#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>

// Modelo de Matriz>>
// 
//Requisitos:
//lidar com qualquer tipo de dados (bool, int, float, char, double,etc) 
//add/rem linhas e colunas
//eco memory
//incluir/retornar/modificar valores
//somar valores de linha ou coluna
//salvar/abrir matriz salva em arq. texto

using namespace std;

struct Pos {
	int linha;
	int coluna;
	Pos(){}
	Pos(int l, int c) { linha = l; coluna = c; }
};
struct Elemento {
	void * data;
	int size;
	char *tipo;
	Pos* pos;
	Elemento (){}
	Elemento(void * indata,int insize,char *intipo,Pos* inpos) : pos(inpos), tipo(intipo), size(insize), data(indata) {

	}
};


void Inserir(std::vector< std::vector <Elemento *> > *matrix);
void InserirLinha(std::vector< std::vector <Elemento *> >  *matrix);
void InserirColuna(std::vector< std::vector <Elemento *> >  *matrix);
void IncluirElemento(std::vector< std::vector <Elemento *> >  *matrix, Pos * pos);
void IncluirElemento(std::vector< std::vector <Elemento *> >  *matrix, Pos * pos, char* valor);
void ModificarElemento(std::vector< std::vector <Elemento *> >  *matrix, Pos * position);
void ModificarElemento(std::vector< std::vector <Elemento *> >  *matrix, Pos * position, char * valor);
void SomarLinha(std::vector< std::vector <Elemento *> >  *matrix);
void SomarColuna(std::vector< std::vector <Elemento *> >  *matrix);
Pos * Posicao();
Elemento * NovoElemento(void * dados, int tamanho, char* tipo, Pos * pos);
Elemento * NovoElementoPadrao(Pos * pos);

void openFile(std::vector< std::vector <Elemento *> > *matrix);
void saveFile(std::vector< std::vector <Elemento *> > *matrix);
void AbrirMatriz(std::vector< std::vector <Elemento *> > *matrix);
void ListarElementos(std::vector< std::vector <Elemento *> > *mat);

void Remover(std::vector< std::vector <Elemento *> > *matrix);
void RemoverLinha(std::vector< std::vector <Elemento *> >  *matrix);
void RemoverColuna(std::vector< std::vector <Elemento *> >  *matrix);

void FreeVecElemento(std::vector <Elemento *> vec);
void FreeElemento(Elemento *);

// principal
int main()
{
	std::vector< std::vector <Elemento *> >  lin3;
	int choice = 0;
	do {
		std::cout << "\n NEW Menu\n" << endl;
		std::cout << "0: Sair" << endl;
		std::cout << "1: Modificar" << endl;
		std::cout << "2: Listar" << endl;
		std::cout << "3: Inicializar matrix padrão" << endl;
		std::cout << "4: Remover" << endl;
		std::cout << "5: Somar Linha" << endl;
		std::cout << "6: Somar Coluna" << endl;
		std::cout << "7: Carregar Arquivo" << endl;
		std::cout << "8: Salvar Arquivo" << endl;

		std::cin >> choice;
		switch (choice) {

		case 1: Inserir(&lin3);
			break;
		case 2: ListarElementos(&lin3);
			break;
		case 3:
			AbrirMatriz(&lin3);
			break;
		case 4:
			Remover(&lin3);
			break;
		case 5:
			SomarLinha(&lin3);
			break;
		case 6:
			SomarColuna(&lin3);
			break; 
		case 7:
			openFile(&lin3);
			break;
		case 8:
			saveFile(&lin3);
			break;
		}

	} while (choice);

	return 0;
}

void openFile(std::vector< std::vector <Elemento *> > *matrix) {
	if (!matrix->empty()) { cout << " matriz ja carregada" << endl; return; }
	
	char *linha = (char*)malloc(sizeof(char*) * 30);
	char *dado = (char*)malloc(sizeof(char*) * 30);
	Pos *pos = new Pos();
	
	ifstream arquivoR("matrix.dat");
	if (arquivoR) {
	while (arquivoR>>linha>>pos->linha>>pos->coluna){
			if (strstr((const char*)linha, ";")) { break; }
			IncluirElemento(matrix, pos, linha);
			cout << "pos: " << pos->linha << "x" << pos->coluna << endl;
					
			}
		arquivoR.close();
	}
	std::cout << "arquivo carregado." << endl;
	return ;
}

void saveFile(std::vector< std::vector <Elemento *> > *matrix) {
	if (matrix->empty()) { cout << " matriz nula" << endl; return; }
	
	ofstream arquivo;
	
	arquivo.open("matrix.dat");
	for (unsigned int j = 0; j < (matrix->size()-1); j++) {

		for (unsigned int i = 0; i < matrix->data()->size(); i++) {

			if (strstr(matrix->at(j).at(i)->tipo, "char") != NULL) {
				arquivo << (char*)matrix->at(j).at(i)->data	 << " " << matrix->at(j).at(i)->pos->linha << " "<< matrix->at(j).at(i)->pos->coluna <<endl;
			}
			if (strstr(matrix->at(j).at(i)->tipo, "int") != NULL) {
				arquivo << *(int*)matrix->at(j).at(i)->data << " " << matrix->at(j).at(i)->pos->linha << " " << matrix->at(j).at(i)->pos->coluna << endl;
			}
			if (strstr(matrix->at(j).at(i)->tipo, "bool") != NULL) {
				arquivo << std::boolalpha << *(bool*)matrix->at(j).at(i)->data << " " << matrix->at(j).at(i)->pos->linha << " " << matrix->at(j).at(i)->pos->coluna << endl;
			}
			if (strstr(matrix->at(j).at(i)->tipo, "float") != NULL && matrix->at(j).at(i)->size == 4) {
				std::cout.precision(17);
				arquivo << *(float*)matrix->at(j).at(i)->data << " " << matrix->at(j).at(i)->pos->linha << " " << matrix->at(j).at(i)->pos->coluna << endl;
			}
			if (strstr(matrix->at(j).at(i)->tipo, "double")) {
				std::cout.precision(17);
				arquivo << *(double*)matrix->at(j).at(i)->data << " " << matrix->at(j).at(i)->pos->linha << " " << matrix->at(j).at(i)->pos->coluna << endl;
			}
		}
		std::cout << endl;

	}
	
	arquivo.close();
	std::cout << "arquivo carregado." << endl;
}

void Inserir(std::vector< std::vector <Elemento *> > *matrix) {
	Pos * position = new Pos();
	int c;
	std::cout << "\nMenu Modificar Matriz \n" << endl;
	std::cout << "1 : inserir linha" << endl;
	std::cout << "2 : inserir coluna" << endl;
	std::cout << "3 : Modificar elemento" << endl;
	std::cout << "4 : Incluir elemento" << endl;
	std::cin >> c;
	//std::system("cls");

	switch (c) {
	case 1: InserirLinha(matrix);
		break;
	case 2: InserirColuna(matrix);
		break;
	case 3: 
		position = Posicao();
		ModificarElemento(matrix, position); 
	
		break;
	case 4: 
		position = Posicao(); 
		IncluirElemento(matrix,position);
		break;
	}
	return;
}
 
Pos * Posicao (){
	 Pos *position = new Pos();
	 std::cout << "Entre com a posição (linha e coluna):" << endl;
	 std::cin >> position->linha;
	 std::cin >> position->coluna;
	 return position;
 }

void InserirLinha(std::vector< std::vector <Elemento *> >  *matrix)
{
	int size_l = matrix->size();
	int size_c = (!size_l)? 0 : matrix->data()->size();
	std::vector <Elemento* > col;

	Pos *pos = new Pos(0,0);
	pos->linha = size_l + 1;
	for (int i = 0; i < size_c; i++) {
		pos->coluna = i;
		col.push_back(NovoElementoPadrao(pos));
	}
	if (!size_c) { col.push_back(NovoElementoPadrao(pos)); };
	(*matrix).push_back(col);

	std::cout << "novo size: " << matrix->size() << " " << matrix->data()->size() << endl;
}

void InserirColuna(std::vector< std::vector <Elemento *> >  *matrix) {
	//if (matrix->empty()) { cout << " matriz nula" << endl; return; }
	int size_l = matrix->size();
	int size_c = (!size_l) ? 0 : matrix->data()->size();
	//int size_c = matrix->at(0).size();
	std::cout << "antigo size: " << size_l << " x " << size_c << endl;

	Pos * pos = new Pos(0,0);
	pos->coluna = size_c + 1;
	for (int i = 0; i< size_l; i++) {
		pos->linha = i;
		matrix->at(i).push_back(NovoElementoPadrao(pos));
	}
	if (!size_l) {
		InserirLinha(matrix);
		matrix->at(0).push_back(NovoElementoPadrao(pos));
	}
	std::cout << "novo size: " << matrix->size() << " " << matrix->data()->size() << endl;
	
}

void ModificarElemento(std::vector< std::vector <Elemento *> >  *matrix, Pos * position, char * valor) {
	std::vector<Elemento *>::iterator it;
	char * valor_in = (char*)malloc(sizeof(*valor));
	strcpy_s(valor_in, strlen(valor) + 1, valor);
	int inteiro = 0, booleano = 0, caracter = 0, ponto = 0, precision = 0;
	for (unsigned int i = 0; i < strlen(valor_in); i++)
	{
		valor_in[i] = toupper(valor_in[i]);

		if (isalpha(valor_in[i])) caracter = 1;
		if (isdigit(valor_in[i])) inteiro = 1;
		if (valor_in[i] == '.') { ponto += 1; precision = i; }
	}
	if (ponto > 1) { caracter = 1; }
	else {
		precision = strlen(valor_in) - precision;
	}


	if (!caracter) {
		if (!ponto) { //inteiro puro

			int *t = new int();
			*t = atoi(valor_in);
			void* dt;

			dt = (void *)malloc(sizeof(t));
			dt = (void*)t;

			FreeElemento(matrix->at(position->linha).at(position->coluna));
			matrix->at(position->linha).erase(matrix->at(position->linha).begin() + position->coluna);
			it = matrix->at(position->linha).begin();
			matrix->at(position->linha).insert
			(it + position->coluna, NovoElemento(dt, sizeof(t), (char*) typeid(t).name(), position)
			);

		}
		else { //com decimal
			if (precision <= 7) {
				float * f = new float((float)atof(valor_in));
				void * flu = (void *)(f);
				FreeElemento(matrix->at(position->linha).at(position->coluna));
				matrix->at(position->linha).erase(matrix->at(position->linha).begin() + position->coluna);
				it = matrix->at(position->linha).begin();
				matrix->at(position->linha).insert
				(it + position->coluna, NovoElemento(flu, sizeof(f), (char*) typeid(f).name(), position));
			}
			else {
				double * dou = new double((double)atof(valor_in));
				void * dbl = (void *)(dou);
				FreeElemento(matrix->at(position->linha).at(position->coluna));
				matrix->at(position->linha).erase(matrix->at(position->linha).begin() + position->coluna);
				it = matrix->at(position->linha).begin();
				matrix->at(position->linha).insert
				(it + position->coluna, NovoElemento(dbl, sizeof(dou), (char*) typeid(dou).name(), position));

			}

			//testar para double
		}
	}
	else {
		if (strcmp("FALSE", valor_in) == 0 || strcmp("TRUE", valor_in) == 0) { //boolean

			bool * b = new bool(false);
			if (strcmp("TRUE", valor_in) == 0) *b = true;
			void * bul = (void *)(b);
			FreeElemento(matrix->at(position->linha).at(position->coluna));
			matrix->at(position->linha).erase(matrix->at(position->linha).begin() + position->coluna);
			it = matrix->at(position->linha).begin();
			matrix->at(position->linha).insert
			(it + position->coluna, NovoElemento(bul, sizeof(b), (char*) typeid(b).name(), position));
		}
		else { //somente caracteres
			char * frase = (char *)malloc(sizeof(valor_in));
			std::cout << " " << strlen(valor_in);
			strcpy_s(frase, strlen(valor_in) + 1, valor_in);
			void *fras = (void *)malloc(sizeof(frase));
			fras = (void*)(frase);
			FreeElemento(matrix->at(position->linha).at(position->coluna));
			matrix->at(position->linha).erase(matrix->at(position->linha).begin() + position->coluna);
			it = matrix->at(position->linha).begin();
			matrix->at(position->linha).insert
			(it + position->coluna, NovoElemento(fras, sizeof(valor_in), (char*) typeid(valor_in).name(), position));

		}
	}
	return;
}

void ModificarElemento(std::vector< std::vector <Elemento *> >  *matrix, Pos * position) {
	if (matrix->empty()) { cout << " matriz nula" << endl; return; }
	if (matrix->at(0).size() <= (unsigned int)position->coluna) { cout << "coluna nula" << endl; return; }
	if (matrix->at(0).size() <= (unsigned int)position->linha) { cout << "linha nula" << endl; return; }
	
	char * valor_in = "";
	std::string line;
	std::getline(std::cin, line);
	valor_in = (char*)line.data();
	std::cout << "Digite o valor:" << endl;
	std::getline(std::cin, line);
	ModificarElemento(matrix, position, valor_in);

}
 

double soma( int t, double u)
{
	return ((double )t + u);
}
double soma(double t, int u)
{
	return ((double)t + u);
}
double soma(double t, double u)
{
	return ((double)t + u);
}
int soma(int t, int u)
{
	return t + u;
}

void SomarLinha(std::vector< std::vector <Elemento *> >  *matrix){
	if (matrix->empty()) { cout << " matriz nula" << endl; return; }
	int linha;
	double somatorio = 0;
	std::cout << "linha para somar: " << endl;
	std::cin >> linha;
	for (unsigned int i = 0; i < matrix->data()->size(); i++) {

		if ((strstr(matrix->at(linha).at(i)->tipo, "char") != NULL) || (strstr(matrix->at(linha).at(i)->tipo, "bool") != NULL)) {
			cout << " existe valor nao somavel" << endl;
			return;
		}
		if (strstr(matrix->at(linha).at(i)->tipo, "int") != NULL) {
			somatorio = soma(somatorio, *(int*)matrix->at(linha).at(i)->data);
		}
		if (strstr(matrix->at(linha).at(i)->tipo, "float") != NULL) {
			somatorio = soma(somatorio, *(float*)matrix->at(linha).at(i)->data);
		}
		if (strstr(matrix->at(linha).at(i)->tipo, "double") != NULL) {
			somatorio = soma(somatorio, *(double*)matrix->at(linha).at(i)->data);
		}

	}
	std::cout << "resultado: " << somatorio << endl;
}

void  SomarColuna(std::vector< std::vector <Elemento *> >  *matrix){
	if (matrix->empty()) { cout << " matriz nula" << endl; return; }
	int coluna;
	double somatorio = 0;
	std::cout << "Coluna para somar: " << endl;
	std::cin >> coluna;
	for (unsigned int j = 0; j < matrix->size(); j++) {
		if ((strstr(matrix->at(j).at(coluna)->tipo, "char") != NULL) || (strstr(matrix->at(j).at(coluna)->tipo, "bool") != NULL)) {
			cout << " existe valor nao somavel" << endl;
			return;
		}
		if (strstr(matrix->at(j).at(coluna)->tipo, "int") != NULL) {
			somatorio = soma(somatorio, *(int*)matrix->at(j).at(coluna)->data);
		}
		if (strstr(matrix->at(j).at(coluna)->tipo, "float") != NULL) {
			somatorio = soma(somatorio, *(float*)matrix->at(j).at(coluna)->data);
		}
		if (strstr(matrix->at(j).at(coluna)->tipo, "double") != NULL) {
			somatorio = soma(somatorio, *(double*)matrix->at(j).at(coluna)->data);
		}
	}
	std::cout << "resultado: " << somatorio << endl;
}

Elemento * NovoElemento(void * dados, int tamanho, char* tipo, Pos * pos) {
	Pos * tpos = new Pos();
	*tpos = *pos;
	char * ttipo = (char*) malloc (sizeof(*tipo));
	strcpy_s(ttipo, strlen(tipo) + 1, tipo);
	Elemento * novo = new Elemento(dados, tamanho, ttipo, tpos);
	return novo;
}

Elemento * NovoElementoPadrao(Pos * pos) {
	int *tamanho = new int(); 
	*tamanho = 0;
	void* dt;

	dt = (void *)malloc(sizeof(tamanho));
	dt = (void*)tamanho;
	return 	NovoElemento(dt, sizeof(tamanho), (char*) typeid(*tamanho).name(), pos);
}

void AbrirMatriz(std::vector< std::vector <Elemento *> > *lin) {
	if (lin->size() > 0) { std::cout << "Matriz existe" << endl; return; }
	std::vector <Elemento* > col;
	std::cout << "Tamanho da nova matriz: linhas x colunas" << endl;

	int l = 1, c = 1;
	std::cin >> l; std::cin >> c;
	
	Pos *position = new Pos();
	for (int j = 0; j < l; j++) {
		col.clear();
		for (int i = 0; i < c; i++) {
			position->coluna = i;
			position->linha = j;
			col.push_back(NovoElementoPadrao(position));
		}
		(*lin).push_back(col);
	}
}

void ListarElementos(std::vector< std::vector <Elemento *> > *mat) {
	if (mat->empty()) { cout << " matriz nula" << endl; return; }
	std::cout << "Listar:  " << endl;
	for (unsigned int j = 0; j < mat->size(); j++) {
		for (unsigned int i = 0; i < mat->data()->size(); i++) {
			if (strstr(mat->at(j).at(i)->tipo, "char") != NULL) {
				std::cout << "  " << (char*)mat->at(j).at(i)->data;
			}
			if (strstr(mat->at(j).at(i)->tipo, "int") != NULL) {
				std::cout << "  " << *(int*)mat->at(j).at(i)->data;
			}
			if (strstr(mat->at(j).at(i)->tipo, "bool") != NULL) {
				std::cout << "  " << std::boolalpha << *(bool*)mat->at(j).at(i)->data;
			}
			if (strstr(mat->at(j).at(i)->tipo, "float") != NULL && mat->at(j).at(i)->size == 4) {
				std::cout.precision(17);
				std::cout << "  " << *(float*)mat->at(j).at(i)->data; 
			}
			if (strstr(mat->at(j).at(i)->tipo, "double")) {
				std::cout.precision(17);
				std::cout << "  " << *(double*)mat->at(j).at(i)->data;
			}
		}
		std::cout << endl;
	}
	return;
}

void Remover(std::vector< std::vector <Elemento *> > *matrix) {
	std::cout << "\nMenu Remover \n" << endl;
	std::cout << "1 : Remover linha" << endl;
	std::cout << "2 : Remover coluna" << endl;

	int c;
	std::cin >> c;
	//std::system("cls");

	switch (c) {
	case 1: RemoverLinha(matrix);
		break;
	case 2: RemoverColuna(matrix);
		break;
	}
	return;
}

void RemoverLinha(std::vector< std::vector <Elemento *> >  *matrix) {
	int l;
	std::cout << "linha: " << endl;
	std::cin >> l;

	std::cout << "Tamanho antigo: " << matrix->size() << " x " << matrix->at(0).size() << endl;
	if (matrix->size() <= (unsigned int)l) { std::cout << "linha não existe" << endl; return; }
	std::vector <Elemento* > col;

	//free(matrix->at(l).data());
	FreeVecElemento(matrix->at(l));
	matrix->at(l).clear(); 

	matrix->erase(matrix->begin() + l);

	std::cout << "Novo Tamanho: " << matrix->size() << " x " << matrix->data()->size() << endl;
}

void RemoverColuna(std::vector< std::vector <Elemento *> >  *matrix) {
	int c;
	std::cout << "coluna: " << endl;
	std::cin >> c;
	if (matrix->at(0).size() <= (unsigned int)c) { std::cout << "coluna nao existe" << endl; return; }
	for (unsigned int i = 0; i< matrix->size(); i++) {

		matrix->at(i).erase((matrix->at(i)).begin() + c);
	}
}

void IncluirElemento(std::vector< std::vector <Elemento *> >  *matrix, Pos * pos, char *valor) {
	if (matrix->size() <= (unsigned int)pos->linha) {
		for (unsigned int i = 0; i <= (unsigned int)pos->linha - (matrix->size() - 1); i++)
		{
			InserirLinha(matrix);
		}
	}
	if (matrix->data()->size() <= (unsigned int)pos->coluna) {
		for (unsigned int i = 0; i <= (unsigned int)pos->coluna - (matrix->data()->size() - 1); i++)
		{
			InserirColuna(matrix);
		}
	}
	ModificarElemento(matrix, pos, valor);
}

void IncluirElemento(std::vector< std::vector <Elemento *> >  *matrix, Pos * pos) {
	if (matrix->size() <= (unsigned int)pos->linha) {
		for (unsigned int i = 0; i <= (unsigned int)pos->linha - (matrix->size() - 1); i++)
		{
			InserirLinha(matrix);
		}
	}
	if (matrix->data()->size() <= (unsigned int)pos->coluna) { 
		for (unsigned int i = 0; i <= (unsigned int)pos->coluna - (matrix->data()->size() - 1) ; i++)
		{
			InserirColuna(matrix);
		}
	}
	ModificarElemento(matrix, pos);
}

void FreeVecElemento(std::vector <Elemento *> vec){
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		FreeElemento(vec.at(i));
	}
	return;
}

void FreeElemento(Elemento * e) {
	free(e->data);
	free(e->pos);
	free(e);
}
