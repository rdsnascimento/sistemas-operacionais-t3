#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raidfs.h"

typedef struct tabela{
	char nome[20];
	int seek;
	int seek1;
	int permissao;
	char * discoA;
	char * discoB;
	char * discoC;
	int discoAtual;
	int discoParidade;
	int indiceDisco1;
	int indiceDisco2;
	int indiceDisco3;
	int tamanho;
} tabela; 

tabela table [10];
int numArquivos = 0;

int initfs(char * arquivo){
	int pari_disco = 3;
	int tamBloco = 512;
	
	FILE *arq;
	char nome[20];
	strcpy(nome, arquivo);
	strcat(nome, ".bin");
	
	arq = fopen(nome, "r");
	if(arq == NULL){
		return FALHA;
	}else{
		char nome1[20];
		char nome2[20];
		char nome3[20];
		int tmp;
		strcpy(nome1, nome);
		strcat(nome1, "1"); 
		strcpy(nome2, nome);
		strcat(nome2, "2"); 
		strcpy(nome3, nome);
		strcat(nome3, "3"); 
		
		//abrindo arquivos como escrita
		FILE *disco1 = fopen(nome1, "wb");
		FILE *disco2 = fopen(nome2, "wb");
		FILE *disco3 = fopen(nome3, "wb");
		
		//alocando memória
		char * vetor1 = (char *) malloc(sizeof(char)*tamBloco);
		char * vetor2 = (char *) malloc(sizeof(char)*tamBloco);
		char * resultado = (char *) malloc(sizeof(char)*tamBloco);
		
		//contando quantidade de bytes que tem dentro do arquivo
		int contBytes = 0;
		while(!feof(arq)){
			fgetc(arq);
			contBytes++;
		}
		rewind(arq); //volta ponteiro para o comeco
		
		int quantidadeDeBlocos;
		quantidadeDeBlocos = contBytes / tamBloco; //descubro a quantidade de blocos que o arquivo vai gerar
		
		
		if(quantidadeDeBlocos == 0){ 
			
			if(contBytes > 0){ //tem uma quantidade de n de bytes mas nao chega a formar um bloco
				
				fgets (vetor1, contBytes, arq); //leio os bytes que tem
				
				for(int i=contBytes-1; i<512; i++){
					vetor1[i] = 48;
					//printf("%d - %c\n", i, vetor1[i]);
				}
				
				for(int i=0; i<tamBloco; i++){ //preencho o bloco complementar com 0 e faco o xor entre eles
					vetor2[i]=48;	
					tmp = vetor1[i] ^ vetor2[i];
					if(tmp == 0){
						resultado[i] = 48;
					}else{
						resultado[i] = vetor1[i] ^ vetor2[i];
					}
				}
				
				if(pari_disco==3){ //a paridade fica no disco 3
					fwrite(vetor1, 1, strlen(vetor1), disco1);
					fwrite(vetor2, 1, strlen(vetor2), disco2);
					fwrite(resultado, 1, strlen(resultado), disco3);
					pari_disco--;
				}else if(pari_disco==2){ //a paridade fica no disco 2
					fwrite(vetor1, 1, strlen(vetor1), disco1);
					fwrite(resultado, 1, strlen(vetor2), disco2);
					fwrite(vetor2, 1, strlen(resultado), disco3);
					pari_disco--;
				}else if(pari_disco==1){ //a paridade fica no disco 1
					fwrite(resultado, 1, strlen(vetor1), disco1);
					fwrite(vetor1, 1, strlen(vetor2), disco2);
					fwrite(vetor2, 1, strlen(resultado), disco3);
					pari_disco=3;
				}
				
			} else { // a quantidade de bytes lidas é nula
				return FALHA;
			}
			
		}else{
		
			if((quantidadeDeBlocos % 2) == 0){ //tem uma quantidade par de blocos

				for(int i=0; i < quantidadeDeBlocos; i++){ //vai até o último bloco par
					fgets (vetor1, tamBloco+1, arq); //leio um bloco
					fgets (vetor2, tamBloco+1, arq); //leio o outro bloco
					
					for(int i=0; i<tamBloco; i++){ //calculo a paridade
						tmp = vetor1[i] ^ vetor2[i];
						if(tmp == 0){
							resultado[i] = 48;
						}else{
							resultado[i] = vetor1[i] ^ vetor2[i];
						}
					}
				}
				if(pari_disco==3){ //a paridade fica no disco 3
					fwrite(vetor1, 1, strlen(vetor1), disco1);
					fwrite(vetor2, 1, strlen(vetor2), disco2);
					fwrite(resultado, 1, strlen(resultado), disco3);
					pari_disco--;
				}else if(pari_disco==2){ //a paridade fica no disco 2
					fwrite(vetor1, 1, strlen(vetor1), disco1);
					fwrite(resultado, 1, strlen(vetor2), disco2);
					fwrite(vetor2, 1, strlen(resultado), disco3);
					pari_disco--;
				}else if(pari_disco==1){ //a paridade fica no disco 1
					fwrite(resultado, 1, strlen(vetor1), disco1);
					fwrite(vetor1, 1, strlen(vetor2), disco2);
					fwrite(vetor2, 1, strlen(resultado), disco3);
					pari_disco=3;
				}
					
			}else{ //tem uma quantidade impar de blocos, tem bloco que ficou sozinho. Completar pra ficar com 1024 em dois arquivos
				printf("OIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII GENTEEEEEEEEEEEEEEEE\n");
				for(int i=0; i < (quantidadeDeBlocos - 1); i++){ //vai até o último bloco par
					fgets (vetor1, tamBloco, arq); //leio um bloco
					fgets (vetor2, tamBloco, arq); //leio o outro bloco
					for(int i=0; i<tamBloco; i++){ //calculo a paridade
						tmp = vetor1[i] ^ vetor2[i];
						if(tmp == 0){
							resultado[i] = 48;
						}else{
							resultado[i] = vetor1[i] ^ vetor2[i];
						}
					}
				}
				
				if(pari_disco==3){ //a paridade fica no disco 3
					fwrite(vetor1, 1, strlen(vetor1), disco1);
					fwrite(vetor2, 1, strlen(vetor2), disco2);
					fwrite(resultado, 1, strlen(resultado), disco3);
					pari_disco--;
				}else if(pari_disco==2){ //a paridade fica no disco 2
					fwrite(vetor1, 1, strlen(vetor1), disco1);
					fwrite(resultado, 1, strlen(vetor2), disco2);
					fwrite(vetor2, 1, strlen(resultado), disco3);
					pari_disco--;
				}else if(pari_disco==1){ //a paridade fica no disco 1
					fwrite(resultado, 1, strlen(vetor1), disco1);
					fwrite(vetor1, 1, strlen(vetor2), disco2);
					fwrite(vetor2, 1, strlen(resultado), disco3);
					pari_disco=3;
				}
					
				//agora faco isso para o bloco que sobrou
				fgets (vetor1, tamBloco, arq); //leio o bloco que sobrou
				for(int i=0; i<tamBloco; i++){ //preencho bloco complementar com zero e calculo a paridade
					vetor2[i]=48; //o bloco complementar é preenchido com zero
					tmp = vetor1[i] ^ vetor2[i];
					if(tmp == 0){
						resultado[i] = 48;
					}else{
						resultado[i] = vetor1[i] ^ vetor2[i];
					}
				}
				if(pari_disco==3){ //a paridade fica no disco 3
					fwrite(vetor1, 1, strlen(vetor1), disco1);
					fwrite(vetor2, 1, strlen(vetor2), disco2);
					fwrite(resultado, 1, strlen(resultado), disco3);
					pari_disco--;
				}else if(pari_disco==2){ //a paridade fica no disco 2
					fwrite(vetor1, 1, strlen(vetor1), disco1);
					fwrite(resultado, 1, strlen(vetor2), disco2);
					fwrite(vetor2, 1, strlen(resultado), disco3);
					pari_disco--;
				}else if(pari_disco==1){ //a paridade fica no disco 1
					fwrite(resultado, 1, strlen(vetor1), disco1);
					fwrite(vetor1, 1, strlen(vetor2), disco2);
					fwrite(vetor2, 1, strlen(resultado), disco3);
					pari_disco=3;
				}	
			}
				
			if(((contBytes-2) % tamBloco) != 0){ //significa sobraram bytes 
				fgets (vetor1, ((contBytes-2) % tamBloco), arq); //leio os bytes que sobraram, escrevendo no vetor 1
				
				for(int i=((contBytes-2) % tamBloco); i<tamBloco; i++){ //preenche o restante do vetor1 com zero
					vetor1[i]=48;
				}
				
				for(int i=0; i<tamBloco; i++){ //preenche o vetor 2 (complementar) com zero, para atingir 1024 (2 blocos de 512)
					vetor2[i]=48;
					tmp = vetor1[i] ^ vetor2[i];
					if(tmp == 0){
						resultado[i] = 48;
					}else{
						resultado[i] = vetor1[i] ^ vetor2[i];
					}
				}
				
				if(pari_disco==3){ //a paridade fica no disco 3
					fwrite(vetor1, 1, strlen(vetor1), disco1);
					fwrite(vetor2, 1, strlen(vetor2), disco2);
					fwrite(resultado, 1, strlen(resultado), disco3);
					pari_disco--;
				}else if(pari_disco==2){ //a paridade fica no disco 2
					fwrite(vetor1, 1, strlen(vetor1), disco1);
					fwrite(resultado, 1, strlen(vetor2), disco2);
					fwrite(vetor2, 1, strlen(resultado), disco3);
					pari_disco--;
				}else if(pari_disco==1){ //a paridade fica no disco 1
					fwrite(resultado, 1, strlen(vetor1), disco1);
					fwrite(vetor1, 1, strlen(vetor2), disco2);
					fwrite(vetor2, 1, strlen(resultado), disco3);
					pari_disco=3;
				}
			}
		}
			
		fclose(disco1);
		fclose(disco2);
		fclose(disco3);	
		disco1 = NULL;
		disco2 = NULL;
		disco3 = NULL;
		free(disco1);
		free(disco2);
		free(disco3);
	}	
	fclose(arq);
	arq = NULL;
	free(arq);
	
	return SUCESSO;
}

indice_arq_t f_open(char * nome,  int acesso){
	//concatena os nomes de entrada para ler o arquivo .bin
	char nome1[20];
	char nome2[20];
	char nome3[20];	
	strcpy(nome1, nome);
	strcpy(nome2, nome);
	strcpy(nome3, nome);
	strcat(nome1, ".bin1");
	strcat(nome2, ".bin2");
	strcat(nome3, ".bin3");
	
	FILE *a = fopen(nome1, "r");
	if(a==NULL){
		return FALHA;
	}
	
	FILE *b = fopen(nome2, "r");
	if(b==NULL){
		return FALHA;
	}
	
	FILE *c = fopen(nome3, "r");
	if(c==NULL){
		return FALHA;
	}
	
	if((acesso == 0)||(acesso == 1)||(acesso==2)){
		numArquivos++;
		strcpy(table[numArquivos].nome, nome);
		table[numArquivos].permissao = acesso;
		table[numArquivos].seek = 0;
		table[numArquivos].seek1 = 0;
		table[numArquivos].discoParidade = 3;
		table[numArquivos].discoAtual = 1;
		
		
		//conta quantos caracteres tem dentro de cada arquivo, para depois carregar para um char * em um único fgets
		int cont1, cont2, cont3;
		while(!feof(a)){
			fgetc(a);
			cont1++;
		}
		while(!feof(b)){
			fgetc(b);
			cont2++;
		}
		while(!feof(c)){
			fgetc(c);
			cont3++;
		}
		//volta para o inicio do disco
		rewind(a);
		rewind(b);
		rewind(c);
		
		//aloca três vetores para receber o conteudo do arquivo
		table[numArquivos].discoA = (char *) malloc(sizeof(char) * cont1);
		table[numArquivos].discoB = (char *) malloc(sizeof(char) * cont2);
		table[numArquivos].discoC = (char *) malloc(sizeof(char) * cont3);
		
		//lê o arquivo para um vetor
		fgets (table[numArquivos].discoA, cont1, a);
		fgets (table[numArquivos].discoB, cont2, b);
		fgets (table[numArquivos].discoC, cont3, c);
		
		table[numArquivos].tamanho = cont1;

		//fecha os arquivos
		fclose(a);
		fclose(b);
		fclose(c);
		a = NULL;
		b = NULL;
		c = NULL;
		free(a);
		free(b);
		free(c);
	}
	
	
	
	return numArquivos+1;
}

/** Fecha um arquivo.
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @return SUCESSO OU FALHA
 */
int f_close(indice_arq_t arquivo){
	arquivo--;
	table[arquivo].permissao = 0;
	table[arquivo].discoAtual = 0;
	table[arquivo].discoParidade = 0;
	table[arquivo].indiceDisco1 = 0;
	table[arquivo].indiceDisco2 = 0;
	table[arquivo].indiceDisco3 = 0;
	table[arquivo].seek = 0;
	table[arquivo].seek1 = 0;
	return SUCESSO;
}

uint32_t f_read(indice_arq_t arquivo, uint32_t tamanho, char *buffer){
	
	/*
	printf("ANTES\n");
	for(uint32_t i=0; i<tamanho; i++){
		printf("buffer %d - %d\n", i, buffer[i]);
	}*/

	arquivo=arquivo-1;
	int cont = 0;
	int saida = 0;
	
	
	for(uint32_t i=table[arquivo].seek1; i<tamanho; i++){
		if(table[arquivo].discoParidade == 3){
			if(table[arquivo].discoAtual == 1){ //escreve disco 1
				buffer[i] = table[arquivo].discoA[table[arquivo].indiceDisco1];
				table[arquivo].indiceDisco1++;
				cont++;
				if(cont==512){
					table[arquivo].discoAtual = 2;
					cont=0;
				}
			}
			else if(table[arquivo].discoAtual==2){ //escreve disco 2
				buffer[i] = table[arquivo].discoB[table[arquivo].indiceDisco2];
				table[arquivo].indiceDisco2++;
				cont++;
				
				if(cont==512){
					table[arquivo].discoAtual = 1;
					table[arquivo].discoParidade=2;
					cont=0;
				}
			}
		} else if (table[arquivo].discoParidade == 2){
			if(table[arquivo].discoAtual==1){ //escreve disco 1
				buffer[i] = table[arquivo].discoC[table[arquivo].indiceDisco1];
				table[arquivo].indiceDisco1++;
					
				cont++;
				if(cont==512){
					table[arquivo].discoAtual = 3;
					cont=0;
				}
			}
			else if(table[arquivo].discoAtual==3){ //escreve disco 3
				buffer[i] = table[arquivo].discoA[table[arquivo].indiceDisco3];
				table[arquivo].indiceDisco3++;
					
				cont++;
				if(cont==512){
					table[arquivo].discoAtual = 2;
					table[arquivo].discoParidade=1;
					cont=0;
				}
			}
		} else if (table[arquivo].discoParidade == 1){
			if(table[arquivo].discoAtual==2){ //escreve disco 2
				buffer[i] = table[arquivo].discoB[table[arquivo].indiceDisco2];
				table[arquivo].indiceDisco2++;
					
				cont++;
				if(cont==512){
					table[arquivo].discoAtual = 3;
					cont=0;
				}
			}
			else if(table[arquivo].discoAtual==3){ //escreve disco 2
				buffer[i] = table[arquivo].discoC[table[arquivo].indiceDisco3];
				table[arquivo].indiceDisco3++;
					
				cont++;
				if(cont==512){
					table[arquivo].discoAtual = 1;
					table[arquivo].discoParidade=3;
					cont=0;
				}
			}
			
		}
		saida++;
	} 
	table[arquivo].seek1 = tamanho;

	return saida;
}

/** Escreve bytes em um arquivo aberto.
 * @param arquivo índice para a tabela de arquivos abertos
 * @param tamanho número de bytes a serem escritos
 * @param buffer ponteiro para conteúdo a ser escrito
 * @return SUCESSO ou FALHA
 */
int f_write(indice_arq_t arquivo, uint32_t tamanho, char *buffer){
	//printf("\n");
	arquivo--;
	char nome1[20];
	char nome2[20];
	char nome3[20];
	
	strcpy(nome1, table[arquivo].nome);
	strcpy(nome2, table[arquivo].nome);
	strcpy(nome3, table[arquivo].nome);
	strcat(nome1, ".bin1");
	strcat(nome2, ".bin2");
	strcat(nome3, ".bin3");
	
	FILE *disco1;
	FILE *disco2;
	FILE *disco3;
	
	disco1 = fopen(nome1, "r");
	if(disco1 == NULL){
		return FALHA;
	}
	
	uint32_t contBytes = 0;
	while(!feof(disco1)){
		fgetc(disco1);
		contBytes++;
	}
	contBytes--;
	fclose(disco1);
	
	//leitura
	disco1 = fopen(nome1, "r");
	disco2 = fopen(nome2, "r");
	disco3 = fopen(nome3, "r");
	
	//vendo se da o tamanho
	int totalBytes = contBytes;
	//printf("Antes: %d\n", totalBytes);
	//printf("Seek: %d\n", table[arquivo].seek);
	//printf("Tamanho: %d\n", tamanho);
	
	if(contBytes > tamanho){
		totalBytes = contBytes + tamanho;
	}
	if(table[arquivo].seek >= (int)contBytes){
		totalBytes = contBytes + tamanho;
	}
	
	char * disk1 = (char *) malloc(sizeof(char) * totalBytes);
	char * disk2 = (char *) malloc(sizeof(char) * totalBytes);
	char * disk3 = (char *) malloc(sizeof(char) * totalBytes);
	
	//copiando para os buffers do disco
	fgets (disk1, contBytes, disco1);
	fgets (disk2, contBytes, disco2);
	fgets (disk3, contBytes, disco3);
	disk1[contBytes-1]=disk1[contBytes-2];
	disk2[contBytes-1]=disk2[contBytes-2];
	disk3[contBytes-1]=disk3[contBytes-2];
	
	//fechando arquivos
	fclose(disco1);
	fclose(disco2);
	fclose(disco3);
	
	//printf("Total bytes: %d\n", totalBytes);
	int tmp = totalBytes / 1024;
	int discoParidade = 1;
	for(int i=0; i<tmp; i++){
		discoParidade--;
		if(discoParidade == 0){
			discoParidade = 3;
		}
	}
	//printf("DiscoParidade: %d\n", discoParidade);
	table[arquivo].discoParidade=discoParidade;

	int discoAtual=1;
	if(discoParidade == 3){
		discoAtual = 1;
	} else if(discoParidade == 2){
		discoAtual = 1;
	} else if(discoParidade == 1){
		discoAtual = 2;
	}

	int cont = 0;
	int k = 0;
	int tmpParidade = 0;
	//printf("Seek: %d Seek+tamanho %d\n", table[arquivo].seek, table[arquivo].seek+tamanho);
	
	for(uint32_t i=table[arquivo].seek; i < (table[arquivo].seek+tamanho); i++){
		if(discoParidade == 3){
			if(discoAtual == 1){ //escreve disco 1
				disk1[i] = buffer[k];
				cont++;
				k++;
				if(cont==512){
					discoAtual = 2;
					cont=0;
				}
			}
			else if(discoAtual == 2){ //escreve disco 2
				//printf("Oi");
				disk2[i-512] = buffer[k];
				cont++;
				k++;
				if(cont==512){
					for(int j=i-512; j<512; j++){
						tmpParidade = disk1[j] ^ disk2[j];
						if(tmpParidade == 0){
							disk3[j] = '0';
						} else { 
							disk3[j] = tmpParidade;
						}
					}
					discoAtual = 1;
					cont = 0;
					discoParidade = 2;
				}
			}
		}
		else if(discoParidade == 2){
			//printf("aqui eh o i: %d\n", i);
			if(discoAtual == 1){ //escreve disco 1
				disk1[i] = buffer[k];
				cont++;
				k++;
				if(cont==512){
					discoAtual = 3;
					cont=0;
					//disk1[1023] = buffer[k]; 
				}
			}
			else if(discoAtual == 3){ //escreve disco 2
				//printf("Oi");
				disk3[i-512] = buffer[k];
				cont++;
				k++;
				if(cont==512){
					for(int j=i-512; j<512; j++){
						tmpParidade = disk1[j] ^ disk3[j];
						if(tmpParidade == 0){
							disk2[j] = '0';
						} else { 
							disk2[j] = tmpParidade;
						}
					}
					discoAtual = 2;
					cont = 0;
					discoParidade = 1;
				}
			}
		}
		else if(discoParidade == 1){
			if(discoAtual == 2){ //escreve disco 1
				disk2[i] = buffer[k];
				cont++;
				k++;
				if(cont==512){
					//discoAtual = 3;
					cont=0;
				}
			}
			else if(discoAtual == 3){ //escreve disco 2
				//printf("Oi");
				disk3[i-512] = buffer[k];
				cont++;
				k++;
				if(cont==512){
					for(int j=i-512; j<512; j++){
						tmpParidade = disk2[j] ^ disk3[j];
						if(tmpParidade == 0){
							disk1[j] = '0';
						} else { 
							disk1[j] = tmpParidade;
						}
					}
					discoAtual = 1;
					cont = 0;
					discoParidade = 3;
				}
			}
		}
	}
	
	/*
	for(int i=0; i<1024; i++){
		printf("%d - %c\n", i, disk1[i]);
	}*/
	
	/*
	for(int i=0; i<1026; i++){
		printf("%d %c\n", i, disk3[i]);
	}
	printf("Total: %d", totalBytes);*/
	
	disco1 = fopen(nome1, "w+");
	disco2 = fopen(nome2, "w+");
	disco3 = fopen(nome3, "w+");
	
	fwrite(disk1, 1, strlen(disk1), disco1);
	fwrite(disk2, 1, strlen(disk2), disco2);
	fwrite(disk3, 1, strlen(disk3), disco3);
	
	fclose(disco1);
	fclose(disco2);
	fclose(disco3);
	
	free(disk1);
	free(disk2);
	free(disk3);
	
	return SUCESSO;
}

/** Apaga um arquivo e o fecha.
 * @param arquivo índice para a tabela de arquivos abertos
 * @return SUCESSO ou FALHA
 */
int f_delete(indice_arq_t arquivo){
	return SUCESSO;
}

/** Modifica a posição atual de leitura ou escrita do arquivo
 * @param arquivo índice para a tabela de arquivos abertos
 * @param seek deslocamento em relação ao início do arquivo
 * @return SUCESSO ou FALHA
 */
int f_seek(indice_arq_t arquivo, uint32_t seek){
	arquivo=arquivo-1;
	table[arquivo].seek = seek;
	table[arquivo].indiceDisco1 = seek;
	table[arquivo].indiceDisco2 = seek;
	table[arquivo].indiceDisco3 = seek;
	return SUCESSO;
}

/** Recupera arquivo RAID que perdeu um dos seus arquivos componentes
 * @param nome nome do arquivo sem terminação .bin
 * @return SUCESSO ou FALHA
 */
int recover(char * nome){
	char nome1[20];
	char nome2[20];
	char nome3[20];
	char nomeTmp[20];
	
	strcpy(nome1, nome);
	strcpy(nome2, nome);
	strcpy(nome3, nome);
	strcat(nome1, ".bin1");
	strcat(nome2, ".bin2");
	strcat(nome3, ".bin3");
	
	FILE *discoA1 = fopen(nome1, "r");
	FILE *discoB1 = fopen(nome2, "r");
	
	if(discoA1 == NULL){
		strcpy(nomeTmp, nome1);
		strcpy(nome1, nome2);
		strcpy(nome2, nome3);
		strcpy(nome3, nomeTmp);
		fclose(discoB1);
		discoB1 = NULL;
		free(discoB1);
		
	} else if (discoB1 == NULL){
		
		strcpy(nomeTmp, nome2);
		strcpy(nome2, nome3);
		strcpy(nome3, nomeTmp);
		
		fclose(discoA1);
		discoA1 = NULL;
		free(discoA1);
	
	} else {
		fclose(discoA1);
		fclose(discoB1);
		
		discoA1 = NULL;
		discoB1 = NULL;
		free(discoA1);
		free(discoB1);
	}
	
	FILE *discoA = fopen(nome1, "r");
	FILE *discoB = fopen(nome2, "r");
	
	int cont1, cont2;
	while(!feof(discoA)){
		fgetc(discoA);
		cont1++;
	}
	while(!feof(discoB)){
		fgetc(discoB);
		cont2++;
	}
	rewind(discoA);
	rewind(discoB);
	
	

	char * vetorA = (char *) malloc(sizeof(char) * cont1);
	char * vetorB = (char *) malloc(sizeof(char) * cont2);
	char * vetorC = (char *) malloc(sizeof(char) * cont1);
	
	fgets (vetorA, cont1, discoA);
	fgets (vetorB, cont1, discoB);
	
	for(int i=0; i<cont1; i++){
		if(vetorA[i]=='0'){
			vetorA[i]=0;
		}
		if(vetorB[i]=='0'){
			vetorB[i]=0;
		}
		vetorC[i] = vetorA[i] ^ vetorB[i];
	}
	
	FILE *discoC = fopen(nome3, "w+");
	fwrite(vetorC, 1, strlen(vetorC), discoC);
	
	fclose(discoA);
	fclose(discoB);
	fclose(discoC);
	discoA = NULL;
	discoB = NULL;
	discoC = NULL;
	free(discoA);
	free(discoB);
	free(discoC);
	
	return SUCESSO;
}
