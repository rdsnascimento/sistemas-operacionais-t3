#include "raidfs.h"
#include "simplegrade.h"
#include <stdio.h>

void inicializacao(){
	DESCRIBE("\tInicialização\t");
	
	int tmp = initfs("Sistemas");
	WHEN("Tento inicializar a partir de um arquivo inexistente");
    IF("Se eh um arquivo que nao existe");
    THEN("Nao deve conseguir criar o raid, retorna erro");
    isEqual(tmp, FALHA, 1);
    
    tmp = f_open("Sistemas", LEITURA);
	WHEN("Tenho um arquivo que nao existe");
    IF("Tento fazer abrir como LEITURA");
    THEN("Deve retornar falha");
    isEqual(tmp, FALHA, 1);
    
    tmp = f_open("Sistemas", ESCRITA);
	WHEN("Tenho um arquivo que nao existe");
    IF("Tento fazer abrir como ESCRITA");
    THEN("Deve retornar falha");
    isEqual(tmp, FALHA, 1);
    
    tmp = f_open("Sistemas", LEITURAESCRITA);
	WHEN("Tenho um arquivo que nao existe");
    IF("Tento fazer abrir como LEITURA/ESCRITA");
    THEN("Deve retornar falha");
    isEqual(tmp, FALHA, 1);
}

void raid(){
	DESCRIBE("\tTestes de RAID\t");
	
	int tmp = initfs("aluno1");
	WHEN("Crio o raid a partir de um arquivo");
    IF("Consegui criar o RAID");
    THEN("Deve retornar sucesso e fazer a leitura");
    if(tmp == SUCESSO){
		isEqual(tmp,SUCESSO,1);
		char tmp[1024];
		int indice = f_open("aluno1", LEITURA);
		if(indice!=0){
			if (f_read(indice, 1024, tmp)==1024){
				isEqual(tmp[0], 71, 1);
				isEqual(tmp[768], 71, 1);
				isEqual(tmp[1023], 0, 1);
			}
		}else{
			isEqual(indice, SUCESSO, 1);
		}
	}else{
		isEqual(tmp,SUCESSO,1);
	}
	
	tmp = initfs("aluno1");
	WHEN("Abro um arquivo para sobreescrita");
    IF("Consegui sobreescrever o arquivo");
    THEN("Deve ler os novos dados");
    if(tmp == SUCESSO){
		isEqual(tmp,SUCESSO,1);
		char texto[1024];
		for(int i=0; i<1024; i++){
			texto[i] = 'H';
		}
		int indice = f_open("aluno1", LEITURAESCRITA);
		
		if(indice!=0){
			f_seek(indice, 0);
			f_write(indice, 1024, texto);
			f_close(indice);
			indice = f_open("aluno1",LEITURA);
			if (f_read(indice, 1024, texto)==1024){
				int k=0;
				while(k<1024||texto[k]=='H'){
					k++;
				}
				isEqual(k,1024,1);
				isEqual(texto[1023], 'H', 1);
			} else isEqual(0,1,1);
			f_close(indice);
		}else{
			isEqual(indice, SUCESSO, 1);
		}
		
	}else{
		isEqual(tmp,SUCESSO,1);
	}
}

void recuperacao(){
	DESCRIBE("\tRecuperando disco\t");
	
	int tmp = initfs("aluno1");
	WHEN("Crio o raid e apago o Disco 1");
    IF("Consigo recuperar");
    THEN("Deve retornar sucesso");
    
    if(tmp == SUCESSO){
		isEqual(tmp, SUCESSO, 1);
		char texto[1024];
		remove((const char *)"aluno1.bin1");
		recover("aluno1");
		int indice = f_open("aluno1", LEITURA);
		if(indice!=0){
			if(f_read(indice, 1024, texto)==1024){
				int k=0;
				while(k<1024||texto[k]=='G'){
					k++;
				}
				isEqual(k,1024,1);
				isEqual(texto[1023], 0, 1);
				f_close(indice);
			} else{
				printf("TU");
				isEqual(indice, SUCESSO, 1);
			}
		}else{
			isEqual(tmp,SUCESSO,1);
		}
	}else{
		isEqual(tmp,SUCESSO,1);
	}
	
	tmp = initfs("aluno1");
	WHEN("Crio o raid e apago o Disco 2");
    IF("Consigo recuperar");
    THEN("Deve retornar sucesso");
    
    if(tmp == SUCESSO){
		isEqual(tmp, SUCESSO, 1);
		char texto[1024];
		remove((const char *)"aluno1.bin2");
		recover("aluno1");
		int indice = f_open("aluno1", LEITURA);
		if(indice!=0){
			if(f_read(indice, 1024, texto)==1024){
				int k=0;
				while(k<1024||texto[k]=='G'){
					k++;
				}
				isEqual(k,1024,1);
				isEqual(texto[1023], 0, 1);
				f_close(indice);
			} else{
				isEqual(indice, SUCESSO, 1);
			}
		}else{
			isEqual(tmp,SUCESSO,1);
		}
	}else{
		isEqual(tmp,SUCESSO,1);
	}
	
	tmp = initfs("aluno1");
	WHEN("Crio o raid e apago o Disco 3");
    IF("Consigo recuperar");
    THEN("Deve retornar sucesso");
    
    if(tmp == SUCESSO){
		isEqual(tmp, SUCESSO, 1);
		char texto[1024];
		remove((const char *)"aluno1.bin3");
		recover("aluno1");
		int indice = f_open("aluno1", LEITURA);
		if(indice!=0){
			if(f_read(indice, 1024, texto)==1024){
				int k=0;
				while(k<1024||texto[k]=='G'){
					k++;
				}
				isEqual(k,1024,1);
				isEqual(texto[1023], 0, 1);
				f_close(indice);
			} else{
				isEqual(indice, SUCESSO, 1);
			}
		}else{
			isEqual(tmp,SUCESSO,1);
		}
	}else{
		isEqual(tmp,SUCESSO,1);
	}
}

int main(){
	DESCRIBE("Testes do Aluno");
	inicializacao();
	raid();
	recuperacao();

	GRADEME();
	printf("\n\n");
	if (grade==maxgrade)
		return 0;
	else return grade;
}
