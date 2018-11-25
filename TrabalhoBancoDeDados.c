#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include <stdio_ext.h>

static char *host = "localhost";
static char *user = "root";
static char *pass = "piterP@123";
static char *dbname = "Piter";

unsigned int port = 3306;
static char *unix_socket = NULL;
unsigned int flag = 0;

void Demitir_funcionario(MYSQL *conn){
	MYSQL_RES *res,*res2,*res3,*res4,*res6,*res7;
	MYSQL_ROW row,row2,row3,row4,row5,row6,row7;
	int i=0,op,primarykey,qtd,j=0,k=0,qt,venda=0,op2;
	float preco,preco2,preco3,salario,qt1,qt2,qt3,qt4,total=0;
	char query[600],query2[200],cpf[12],cpf2[12],cpf3[12];

	mysql_query(conn,"UPDATE pior_func set total = 0");
			mysql_query(conn,"select * from Cliente");
			res7 = mysql_store_result(conn); 			
			while(row7 = mysql_fetch_row(res7)){
				sprintf(query,"select Cpf_cliente,Qt from (select Cpf_cliente,sum(Preco_carro) as Qt from Vendas where Cpf_cliente = '%s' group by Cpf_cliente) as T1 order by Qt",row7[1]);			
				mysql_query(conn,query);
				res = mysql_store_result(conn); 
				row = mysql_fetch_row(res);
				if(row != NULL){
					i++;
				}
				sprintf(query," select T3.Cf_cliente,Qt1,Qt2 from (select Cf_cliente,sum(Preco_trocado) as Qt1 from Troca group by Troca.Cf_cliente) as T1, (select Cf_cliente,Qt2 from (select Cf_cliente,sum(Preco_Trocando) as Qt2 from Troca group by Troca.Cf_cliente) as T2) as T3 where T3.Cf_cliente = '%s' order by Qt2",row7[1]);
				mysql_query(conn,query);
 				res2 = mysql_store_result(conn); 
				row2 = mysql_fetch_row(res2);
				if(row2 != NULL){
	 				j++;
	 			}	
				sprintf(query," select cpf_cliente,Qt from (select cpf_cliente,sum(pagamento) as Qt from Revisao group by cpf_cliente) as T1 where cpf_cliente = '%s' order by Qt",row7[1]);
				mysql_query(conn,query);
	 			res3 = mysql_store_result(conn); 							
	  			row3 = mysql_fetch_row(res3);
	 			if(row3 != NULL){
					k++;
 				}
 				total = 0;
 				// Verifica as possibilidades, por exemplo, um cliente ter efetuado uma venda mas nenhuma troca nem Revisao
 				if(i == 1 && j == 1 && k == 1){
 					total = atof(row[1]) + atof(row2[1]) - atof(row2[2]) + atof(row3[1]);
 					}else  if(i == 1 && j == 1 && k == 0){
 							total = atof(row[1]) + atof(row2[1]) - atof(row2[2]);
 							}else if(i == 1 && j == 0 && k == 1){
 								total = atof(row[1]) + atof(row3[1]);
 								}else if(i == 1 && j == 0 && k == 0){
 									total = atof(row[1]);
 									}else if(i == 0 && j==1 && k == 1){
 										total = atof(row2[1]) + atof(row3[1]) - atof(row2[2]);
										}else if(i == 0 && j == 0 && k == 1){
	 										total = atof(row3[1]);
										}else if(i == 0 && j == 1 && k == 0){
											total = atof(row2[1]) - atof(row2[2]);
										}	
			
				sprintf(query,"SELECT * from pior_func where cpf_func = '%s'",row7[5]);
				mysql_query(conn,query);
				res4 = mysql_store_result(conn);
				row4 = mysql_fetch_row(res4);
				if(row4 != NULL){
					sprintf(query,"UPDATE pior_func set total = total + %f where cpf_func = '%s'",total,row7[5]);
					mysql_query(conn,query);
				}else{
					sprintf(query,"INSERT INTO pior_func values ('%s',null,%f)",row7[5],total);
					mysql_query(conn,query);
				}
				k = 0;
		 		i = 0;
		 		j = 0;
			}
			mysql_query(conn,"select CPF,Qt from (select min(total) as Qt from Pior) as T1,pior_func,Pior where Pior.total = Qt and pior_func.cpf_func = Pior.CPF");
 			res4 = mysql_store_result(conn);
 			sprintf(query,"create view Seundo_pior as select * from Pior where CPF <> '%s'",row4[0]);
 			mysql_query(conn,query);
 			mysql_query(conn,"select CPF,Qt from (select min(total) as Qt from Seundo_pior) as T1,pior_func,Seundo_pior where Seundo_pior.total = Qt and pior_func.cpf_func = Seundo_pior.CPF");
 			res4 = mysql_store_result(conn);
			row5 = mysql_fetch_row(res4);
			sprintf(query,"DELETE from funcionarios where CPF = '%s'",row4[0]);
			mysql_query(conn,query);

			if(row5 != NULL){
				sprintf(query,"select * from Cliente where CPF_funcionario = '%s'",row4[0]);
				mysql_query(conn,query);
				res6 = mysql_store_result(conn); 
					sprintf(query," UPDATE Cliente set Cpf_funcionario  = '%s' where Cpf_funcionario is NULL",row5[0]);
					mysql_query(conn,query);
			}else{
				puts("Nao existe um segundo funcionario");

			}
			mysql_query(conn,"drop view Seundo_pior");
}


void Gasto_cliente(MYSQL *conn){

	MYSQL_RES *res,*res2,*res3,*res4,*res6,*res7;
	MYSQL_ROW row,row2,row3,row4;
	int i=0,op,primarykey,qtd,j=0,k=0,qt,venda=0,op2;
	float preco,preco2,preco3,salario,qt1,qt2,qt3,qt4,total=0;
	char query[600],query2[200],cpf[12],cpf2[12],cpf3[12];

	mysql_query(conn,"select * from Cliente");
			res4 = mysql_store_result(conn); //mysql_store_results recupera um resultado completo para o cliente				
			while(row4 = mysql_fetch_row(res4)){ // mysql_row pega a proxima linha do resultado da tabela.
				sprintf(query,"select Cpf_cliente,Qt from (select Cpf_cliente,sum(Preco_carro) as Qt from Vendas where Cpf_cliente = '%s' group by Cpf_cliente) as T1 order by Qt",row4[1]);			
				mysql_query(conn,query);
				res = mysql_store_result(conn); 
				row = mysql_fetch_row(res);
				if(row != NULL){
					i++;
				}
				sprintf(query," select T3.Cf_cliente,Qt1,Qt2 from (select Cf_cliente,sum(Preco_trocado) as Qt1 from Troca group by Troca.Cf_cliente) as T1, (select Cf_cliente,Qt2 from (select Cf_cliente,sum(Preco_Trocando) as Qt2 from Troca group by Troca.Cf_cliente) as T2) as T3 where T3.Cf_cliente = '%s' order by Qt2",row4[1]);
				mysql_query(conn,query);
 				res2 = mysql_store_result(conn); 	
				row2 = mysql_fetch_row(res2);
				if(row2 != NULL){
	 				j++;
	 			}	
				sprintf(query," select cpf_cliente,Qt from (select cpf_cliente,sum(pagamento) as Qt from Revisao group by cpf_cliente) as T1 where cpf_cliente = '%s' order by Qt",row4[1]);
				mysql_query(conn,query);
	 			res3 = mysql_store_result(conn); 							
	  			row3 = mysql_fetch_row(res3);
	 			if(row3 != NULL){
					k++;
 				}	
 				total = 0;
 				// Verifica as possibilidades, por exemplo, um cliente ter efetuado uma venda mas nenhuma troca nem Revisao
 				if(i == 1 && j == 1 && k == 1){
 					total = atof(row[1]) + atof(row2[1]) - atof(row2[2]) + atof(row3[1]);
 					}else  if(i == 1 && j == 1 && k == 0){
 							total = atof(row[1]) + atof(row2[1]) - atof(row2[2]);
 							}else if(i == 1 && j == 0 && k == 1){
 								total = atof(row[1]) + atof(row3[1]);
 								}else if(i == 1 && j == 0 && k == 0){
 									total = atof(row[1]);
 									}else if(i == 0 && j==1 && k == 1){
 										total = atof(row2[1]) + atof(row3[1]) - atof(row2[2]);
										}else if(i == 0 && j == 0 && k == 1){
	 										total = atof(row3[1]);
										}else if(i == 0 && j == 1 && k == 0){
											total = atof(row2[1]) - atof(row2[2]);
										}
				printf("Total gasto pelo cliente %s foi %f\n",row4[0],total);
	 			k = 0;
	 			i = 0;
	 			j = 0;	
	 		}
}


void menuConsulta(MYSQL *conn){
	MYSQL_RES *res,*res2,*res3,*res4,*res6,*res7;
	MYSQL_ROW row,row2,row3,row4,row5,row6,row7;
	int i=0,op,primarykey,qtd,estrangeira,estrangeira2,anovendido,j=0,k=0,result,qt,venda=0,op2;
	float preco,preco2,preco3,salario,qt1,qt2,qt3,qt4,total=0;
	char nome[100],nome3[100],endereco[100],c,nome4[100],nome2[100],query[600],query2[200],data[12],data2[12],placa[10],telefone[18],cpf[12],cpf2[12],cpf3[12];

	printf("=============================== MENU DE CONSULTA ============================\n");
	printf("\t1. Ver o carro mais vendido no ultimo ano(ou em um intervalo de tempo)\n\t2. Ver o carro mais vendido de todos os tempos\n\t3. Lista de clientes que trocaram de carro ha mais de 3 anos\n");
	printf("\t4. Ver funcionarios mais eficientes e funcionarios menos eficientes\n\t5. Quais cliente trocaram de carros mais de uma vez em menos de 1 ano\n");
	printf("\t6. Mostrar a folha de pagamento\n\t7. Quantos funcionarios ha em cada setor e o custo para mante-lo\n");
	printf("\t8. Quantos carros ha na agencia	 (por modelo), e o valor total destes\n\t9. Aumentar o salario do funcionario do mes\n");
	printf("\t10.Gasto total por cliente, com compra/troca de carro ou manutencoes\n\t11.Lucro da agencia com venda/troca em um intervalo de tempo\n");
	printf("\t12.Deletar clientes que nao fazem troca OU revisao ha mais de 10 anos\n");
	printf("\t13.Demitir o funcionario com pior rendimento ate um determinado momento.\n\t14. Obter os clientes que estao com revisao atrasadas\n");
	scanf("%d",&op);
	__fpurge(stdin);
	switch(op){
		case 1:
			printf("Deseja colocar um intervalo especifico?(1 para sim e 2 para nao)\n");
			scanf("%d", &op2);
			__fpurge(stdin);
			if(op2 == 2){
				mysql_query(conn,"select modelo from (Select modelo,Qt from (select modelo,count(modelo) as Qt from Vendas where  ano >= '2015-01-01' and ano <= '2015-12-31' group by modelo) as T4) as T5 WHere Qt in (select Max(Qt) from (Select Qt from (select modelo,count(modelo) as Qt from Vendas where ano >= '2015-01-01' and ano <= '2015-12-31' group by modelo) as T1) as T2)");
				res = mysql_store_result(conn); 
				row = mysql_fetch_row(res); // mysql_row pega a proxima linha do resultado da tabela.
				if(row != NULL){
					printf("O carro mais vendido no ultimo ano eh %s\n",row[0]); // Printa linha por linha ate chegar ao final
					mysql_free_result(res);
				}else{
					puts("Nao ha nenhum carro vendido no ultimo ano");
				}
			}else{
				printf("Digite o inicio do intervalo de tempo que deseja saber (AAAA-MM-DD)\n");
				scanf(" %[^\n]s",data);
				printf("Digite o final do intervalo de tempo que deseja (AAAA-MM-DD)\n");
				scanf(" %[^\n]s", data2);
				sprintf(query,"select modelo from (Select modelo,Qt from (select modelo,count(modelo) as Qt from Vendas where ano > '%s' and ano < '%s' group by modelo) as T4) as T5 WHere Qt in (select Max(Qt) from (Select Qt from (select modelo,count(modelo) as Qt from Vendas where ano > '%s' and ano < '%s' group by modelo) as T1) as T2)",data,data2,data,data2);			
				mysql_query(conn,query);
				res = mysql_store_result(conn);
				row = mysql_fetch_row(res);
				if(row != NULL){
					puts("O carro mais vendido entre o intervalo solicitado foi:");
					printf("%s\n",row[0]);
				}else{
					puts("Nenhum carro foi vendido neste intervalo de tempo");
				}
			}
		break;
		

		case 2:
   			mysql_query(conn, "SELECT nome  from Quantidade_carro where Qt in (select MAX(Qt) from Quantidade_carro)"); // Acha o carro mais vendido
			res = mysql_store_result(conn); 
			row = mysql_fetch_row(res); // mysql_row pega a proxima linha do resultado da tabela.
			if(row != NULL){
				printf("O carro mais vendido de todos os tempos eh %s\n",row[0]); // Printa linha por linha ate chegar ao final
			}else{
				puts("Nao ha nenhum carro vendido");
			}
		break;
		

		case 3:
			mysql_query(conn,"select nome from Troca,Cliente where ano < '2013-01-01' and cpf = Cf_cliente group by nome");
			res = mysql_store_result(conn); 
			puts("Lista de clintes:");
			while(row = mysql_fetch_row(res)){ // mysql_row pega a proxima linha do resultado da tabela.
				printf("%s",row[0]);  // Printa o row[1] primeiro, pois o row 0 vem com um \n
				printf("\n");
				i++;
			}		
			printf("\n\n\n");
			mysql_free_result(res);
		break;
		

		case 4:
			// Utiliza a visao Eficiencia_funcionario
		 	mysql_query(conn,"SELECT nome  from Eficiencia_funcionario where Qt in (select MAX(Qt) from Eficiencia_funcionario)");
			res = mysql_store_result(conn); 
			if(res != NULL){
				puts("Lista de funcionario mais eficientes:");
				while(row = mysql_fetch_row(res)){ // mysql_row pega a proxima linha do resultado da tabela.
					printf("%s",row[0]);  // Printa o row[1] primeiro, pois o row 0 vem com um \n
					printf("\n");
				}		
				mysql_query(conn,"SELECT nome  from Eficiencia_funcionario where Qt in (select MIN(Qt) from Eficiencia_funcionario)");
				res = mysql_store_result(conn); //mysql_store_results recupera um resultado completo para o cliente
				puts("Lista de funcionario menos eficientes:");
				while(row = mysql_fetch_row(res)){ // mysql_row pega a proxima linha do resultado da tabela.
					printf("%s",row[0]);  // Printa o row[1] primeiro, pois o row 0 vem com um \n
					printf("\n");
				}	
			}else{
				printf("Nao ha nenhum funcionario reistrado\n");
			}	
			mysql_free_result(res);
		break;
		case 5:
			mysql_query(conn,"select Cf_cliente,Qt from (select Cf_cliente,count(Cf_cliente) as Qt from Troca where ano >= '2015-11-10'  group by Cf_cliente) as T1");
			res = mysql_store_result(conn); 
			row = mysql_fetch_row(res); // Pega o cpf do fucionario do cliente
			if(row != NULL){
				mysql_query(conn,"select Cf_cliente,Qt from (select Cf_cliente,count(Cf_cliente) as Qt from Troca where ano >= '2015-11-10'  group by Cf_cliente) as T1");
				res = mysql_store_result(conn); 
				row = mysql_fetch_row(res); // Pega o cpf do fucionario do cliente
				while(row = mysql_fetch_row(res)){ // mysql_row pega a proxima linha do resultado da tabela.
					printf("%s",row[0]);  // Printa o row[1] primeiro, pois o row 0 vem com um \n
					printf("\n");
				}	
			}else{
				printf("Nao ha nenhum cliente que trocou o carro mais de uma vez no ultimo ano\n");
			}
			mysql_free_result(res);
		break;
		case 6:
			
			mysql_query(conn,"select nome,salario from funcionarios");
			res = mysql_store_result(conn); 
			if(res != NULL){
				while(row = mysql_fetch_row(res)){ // mysql_row pega a proxima linha do resultado da tabela.
					printf("%s   :  %s",row[0],row[1]);  // Printa o row[1] primeiro, pois o row 0 vem com um \n
					printf("\n");
				}	 				
			}
			mysql_free_result(res);
		break;
		

		case 7:
				
			mysql_query(conn,"select cod_setor,Qt,Soma from (select cod_setor,Qt,sum(salario) as Soma from Quantidade_setor2 group by cod_setor) as T1");
 			res = mysql_store_result(conn); 
			if(res != NULL){
				while(row = mysql_fetch_row(res)){ // mysql_row pega a proxima linha do resultado da tabela.
					printf("O setor %s tem %s funcionarios e o custo para mante-lo eh %s",row[0],row[1],row[2]);  // Printa o row[1] primeiro, pois o row 0 vem com um \n
					printf("\n");
				}	 				
			}
			mysql_free_result(res);
		break;
		case 8:			// Quantos carros ha na agencia	 (por modelo), e o valor total destes
  			mysql_query(conn,"select Quantidade_modelo.modelo,Qt,valor from Quantidade_modelo, Cadastro_carros where Cadastro_carros.modelo = Quantidade_modelo.modelo");
  			res = mysql_store_result(conn); 
			if(res != NULL){
				while(row = mysql_fetch_row(res)){ // mysql_row pega a proxima linha do resultado da tabela.
					printf("A agencia tem %s carros do modelo %s no valor de %sR$",row[1],row[0],row[2]);  
					printf("\n");
				}	 				
			}
			mysql_free_result(res);
		break;
  		case 9:
  			printf("Digite o cpf do funcionario do mes cujo salario sera acrescido em 6 por cento\n");
  			scanf(" %[^\n]s",cpf);
			sprintf(query,"UPDATE funcionarios set salario = salario * 1.06 WHERE CPF = '%s'",cpf);
			mysql_query(conn,query);
			if((long) mysql_affected_rows(conn) == 1){
				puts("Salario atualizado com sucesso\n");
			}else{
				puts("funcionario nao registrado");
			}
		break;
		case 10:
			
			Gasto_cliente(conn);	

		break;

		case 11:
			preco = 0;
			preco2 = 0;
			preco3 = 0;
			printf("Digite o inicio do intervalo de tempo que deseja saber o lucro (AAAA-MM-DD)\n");
			scanf(" %[^\n]s",data);
			printf("Digite o final do intervalo de tempo que deseja saber o lucro (AAAA-MM-DD)\n");
			scanf(" %[^\n]s", data2);
			sprintf(query," select sum(Preco_carro) from Vendas where Vendas.ano >= '%s' and Vendas.ano <= '%s'",data,data2);	
			mysql_query(conn,query);
			res = mysql_store_result(conn); 
			row = mysql_fetch_row(res); // Pega o cpf do fucionario do cliente
			if(row != NULL){
				printf("%s\n",row[0]);
				preco = atof(row[0]);
			}else{

				puts("Nao ha carros vendidos nesse intervalo de tempo\n");
			}
			sprintf(query,"select * from Vendas where ano >= '%s' and ano <= '%s'",data,data2);
			mysql_query(conn,query);
			res = mysql_store_result(conn); 
			row = mysql_fetch_row(res); // Pega o cpf do fucionario do cliente
			if(row != NULL){
				preco2 = atof(row[0]);
				preco3 = atof(row[1]);
			}else{
				puts("Nao ha carros trocados nesse intervalo de tempo");
			}	
			preco2 = preco2 - preco3 + preco;
			printf("O lucro total da empresa entre %s e %s foi %f$\n",data,data2,preco2);
			printf("\n");
		break;

		case 12:
			mysql_query(conn,"select cpf_cliente, count(*) from  (Select * from (select cpf_cliente,max(data_revisao) as data from Revisao group by cpf_cliente) as T1 where data <='2006-11-26' union Select * from (select Cf_cliente,max(ano) as data from Troca group by Cf_cliente) as T2 where data <='2006-11-26') as T5 group by cpf_cliente"); // Retorna a maior data de cada cliente em Troca e Revisao
			res = mysql_store_result(conn); 
			while(row = mysql_fetch_row(res)){ 
				qtd = atof(row[1]);
				if(qtd >= 2){
					sprintf(query,"Delete from Troca where Cf_cliente = '%s'",row[0]);
					mysql_query(conn,query);
					sprintf(query,"Delete from Vendas where Cpf_cliente = '%s'",row[0]);
					mysql_query(conn,query);
					sprintf(query,"Delete from Revisao where cpf_cliente = '%s'",row[0]);
					mysql_query(conn,query);
					sprintf(query,"Delete from Cliente where CPF = '%s'",row[0]);
					mysql_query(conn,query);
					printf("\n");
				}
			}	
			res = mysql_store_result(conn); 
			puts("Clientes deletados");	
		break;
	
		case 13:
			 
			 Demitir_funcionario(conn);
		
		break;
		case 14:
			mysql_query(conn,"select nome from Cliente where CPF not in (select cpf_cliente from Revisao where data_revisao > '2015-11-25')");			
			res = mysql_store_result(conn);
			puts("Os clientes que estao com a revisao atrasada sao:");
			while(row = mysql_fetch_row(res)){
				printf("%s\n",row[0] );
			}

	}
			
}


void menuCadastro(MYSQL *conn){
	MYSQL_RES *res;
	MYSQL_ROW row;
	int i,op,primarykey,qtd,estrangeira,estrangeira2,anovendido,result,qt,troca=0,venda=0;
	float preco,preco2,salario;
	char nome[50],nome3[50],endereco[50],c,nome4[50],nome2[50],query[300],query2[200],telefone[18],data[12],placa[10],cpf[12],cpf2[12];

	printf("=============================== MENU DE REGISTRO ============================\n");
	printf("\t1. Cadastrar setor\n\t2. Cadastrar Carros\n\t3. Cadastrar Pecas\n\t4. Cadastrar funcionario\n\t5. Cadastrar Cliente\n");
	printf("\t6. Registrar Venda\n\t7. Registrar troca\n\t8. Registrar revisao\n");
	scanf("%d",&op);
	__fpurge(stdin);
    	switch(op){
			case 1:  //  Cadastrar setor
				
				printf("Digite o Codigo do setor\n");
				scanf("%d",&primarykey);
				__fpurge(stdin); // PARA LIMPAR O BUFFER DEPOIS DE LER A CHAVE
				printf("Digite o nome do setor\n");
				scanf(" %[^\n]s", nome);			
				sprintf(query,"Insert into Cadastro_setor values ('%s',%d)",nome,primarykey); //FAZ A STRING COM O NOME DO SETOR 
				mysql_query(conn,query);

			break;
			case 2:   // Cadastrar Carros
			
				printf("Digite o nome do carro\n");
				scanf(" %[^\n]s", nome);			
				printf("Digite o modelo do carro\n");
				scanf(" %[^\n]s", nome2);			
				printf("Digite o Ano do carro (Ano-Mes-Dia)\n");
				scanf(" %[^\n]s", data);			
				printf("Digite o Codigo do carro\n");
				scanf("%d",&primarykey);
				__fpurge(stdin);
				printf("Digite o Codigo do setor\n");
				scanf("%d",&estrangeira);
				__fpurge(stdin);
				printf("Digite o valor do carro\n");
				scanf("%f",&preco);
				__fpurge(stdin);
				printf("Quantidade no estoque\n");
				scanf("%d",&qtd);
				__fpurge(stdin);
				sprintf(query,"Insert into Cadastro_carros values ('%s','%s','%s',%d,%d,%f,%d)",nome,nome2,data,primarykey,estrangeira,preco,qtd);
				mysql_query(conn,query);

			break;
			case 3:   // Cadastrar Pecas
				
				printf("Digite o nome da peca\n");
				scanf(" %[^\n]s", nome);			
				printf("Digite o Codigo da Peca\n");
				scanf("%d",&primarykey);
				__fpurge(stdin);
				printf("Digite o Codigo do setor\n");
				scanf("%d",&estrangeira);
				__fpurge(stdin);
				printf("Digite o valor da peca\n");
				scanf("%f",&preco);
			    __fpurge(stdin);
				printf("Quantidade no estoque\n");
				scanf("%d",&qtd);
				__fpurge(stdin);			
				sprintf(query,"Insert into Pecas values ('%s',%d,%d,%f,%d)",nome,primarykey,estrangeira,preco,qtd);
				mysql_query(conn,query);
			break;

			case 4: //  Cadastrar funcionario
				
				printf("Digite o nome do funcionario\n");
				scanf(" %[^\n]s", nome);			
				printf("Digite o CPF do funcionario\n");
				scanf(" %[^\n]s", cpf);			
				__fpurge(stdin); // PARA LIMPAR O BUFFER DEPOIS DE LER A CHAVE
				printf("Digite o endereco do funcionario\n");
				scanf(" %[^\n]s", endereco);			
				printf("Digite o codigo do setor\n");
				scanf("%d",&estrangeira);
				printf("Digite o salario do funcionario\n");
				scanf("%f",&salario);
				sprintf(query,"INSERT INTO funcionarios values('%s','%s','%s',%d,%f)",nome,cpf,endereco,estrangeira,salario);
			    mysql_query(conn,query);
			break;

			case 5:   // Cadastrar Cliente
				
				printf("Digite o nome do cliente\n");
				scanf(" %[^\n]s", nome);			
				printf("Digite o CPF do cliente\n");
				scanf(" %[^\n]s", cpf);			
				__fpurge(stdin); // PARA LIMPAR O BUFFER DEPOIS DE LER A CHAVE
				printf("Digite o endereco do cliente\n");
				scanf(" %[^\n]s", endereco);			
				printf("Placa do carro\n");
				scanf(" %[^\n]s", placa);			
				printf("Digite o CPF do funcionario\n");
				scanf(" %[^\n]s", cpf2);			
				__fpurge(stdin); // PARA LIMPAR O BUFFER DEPOIS DE LER A CHAVE
				printf("Digite o telefone do cliente\n");
				scanf(" %[^\n]s", telefone);			
				__fpurge(stdin); // PARA LIMPAR O BUFFER DEPOIS DE LER A CHAVE

				sprintf(query,"INSERT INTO Cliente values('%s','%s','%s','%s','%s','%s')",nome,cpf,telefone,endereco,placa,cpf2);
				mysql_query(conn,query);
			break;
			case 6:  // Registrar Venda
				printf("Digite o nome do carro vendido\n");
				scanf(" %[^\n]s", nome);			
				printf("Digite o modelo do carro vendido\n");
				scanf(" %[^\n]s", nome2);	
				printf("Digite o CPF do cliente que esta comprando\n");
				scanf(" %[^\n]s",cpf);
				printf("Digite o ano em que o carro foi vendido(AAAA-MM-DD)\n");
				scanf(" %[^\n]s", data);	
				__fpurge(stdin); // PARA LIMPAR O BUFFER DEPOIS DE LER A CHAVE
				sprintf(query,"SELECT * from Cliente where Cliente.cpf = '%s'",cpf);
				mysql_query(conn,query);
				res = mysql_store_result(conn); //mysql_store_results recupera um resultado completo para o cliente
				row = mysql_fetch_row(res); // Pega o cpf do fucionario do cliente
				if(row != NULL){
					strcpy(cpf2,row[5]);
					sprintf(query,"SELECT * from Cadastro_carros where nome = '%s' and modelo = '%s'",nome,nome2);
					i = 0;
					mysql_query(conn,query);
					res = mysql_store_result(conn); //mysql_store_results recupera um resultado completo para o cliente
					row = mysql_fetch_row(res); // mysql_row pega a proxima linha do resultado da tabela.
					if(row != NULL){
						preco = atof(row[5]);
						sprintf(query,"UPDATE Cadastro_carros set qtd_estoque = qtd_estoque - 1 WHERE cod_carro = %s",row[3]);
						mysql_query(conn,query);
						sprintf(query,"INSERT INTO Vendas values('%s','%s','%s',null,%f,'%s','%s')",nome,nome2,data,preco,cpf,cpf2);
						mysql_query(conn,query);
					}else{
						printf("O carro nao esta disponivel para venda\n");
					}

				}else{
					printf("Este cliente nao esta registrado\n");
				}
				mysql_free_result(res);

				printf("\n");
				break;

			case 7:   // Registrar troca
				printf("Digite o nome do carro a ser trocado:\n");
				scanf(" %[^\n]s", nome);			
				printf("Digite o modelo do carro a ser trocado:\n");
				scanf(" %[^\n]s", nome2);	
				printf("Digite o nome do carro que deseja:\n");
				scanf(" %[^\n]s", nome3);			
				printf("Digite o modelo do carro que deseja:\n");
				scanf(" %[^\n]s", nome4);	
				printf("Digite o dia que esta realizando a troca(AAAA-MM-DD):\n");
				scanf(" %[^\n]s", data);	
				printf("Digite o CPF do cliente que esta trocando\n");
				scanf(" %[^\n]s",cpf);	
				__fpurge(stdin); // PARA LIMPAR O BUFFER DEPOIS DE LER A CHAVE

				sprintf(query,"SELECT * from Cliente where Cliente.cpf = '%s'",cpf);
				mysql_query(conn,query);
				res = mysql_store_result(conn); //mysql_store_results recupera um resultado completo para o cliente
				row = mysql_fetch_row(res);
				if(row != NULL){
					sprintf(query,"SELECT * from Cadastro_carros where nome = '%s' and modelo = '%s'",nome,nome2);
					mysql_query(conn,query);
					res = mysql_store_result(conn); //mysql_store_results recupera um resultado completo para o cliente
					row = mysql_fetch_row(res); // mysql_row pega a proxima linha do resultado da tabela.
					if(row != NULL){
						preco = atof(row[5]);
						sprintf(query,"UPDATE Cadastro_carros set qtd_estoque = qtd_estoque + 1 WHERE cod_carro = %s",row[3]);
						mysql_query(conn,query);


					}else{
						printf("O carro nao esta disponivel para troca\n");

					}
				
					mysql_free_result(res);
					sprintf(query,"SELECT * from Cadastro_carros where nome = '%s' and modelo = '%s'",nome3,nome4);
					mysql_query(conn,query);
					res = mysql_store_result(conn); //mysql_store_results recupera um resultado completo para o cliente
					row = mysql_fetch_row(res); // mysql_row pega a proxima linha do resultado da tabela.
					if(row != NULL){
						preco2 = atof(row[5]);
						sprintf(query,"UPDATE Cadastro_carros set qtd_estoque = qtd_estoque - 1 WHERE cod_carro = %s",row[3]);
						mysql_query(conn,query);
						sprintf(query,"INSERT INTO Troca values('%s','%s','%s','%s','%s',null,%f,%f,'%s')",nome,nome2,nome3,nome4,data,preco,preco2,cpf);
						mysql_query(conn,query);
					}else{
						printf("O carro nao esta disponivel para troca\n");
					}
					printf("\n");
					mysql_free_result(res);
				}else{
					printf("Este cliente nao esta registado\n");
				}
			break;
			case 8:  // Registrar revisao
				printf("Digite o cpf do cliente que realizou a revisao\n");
				scanf(" %[^\n]s",cpf);
				printf("Digite a data em que revisao foi realizada\n");
				scanf(" %[^\n]s",data);
				printf("Digite o preco da revisao\n");
				scanf("%f",&preco);
				sprintf(query,"INSERT INTO Revisao values('%s',null,'%s',%f)",cpf,data,preco);
				mysql_query(conn,query);
				if((long) mysql_affected_rows(conn) == 1){
					puts("Revisao registada com sucesso");
				}else{
					puts("Este cliente nao eh registadd");
				}
			break;
	}

}

int main(){

	int i=0,op;
	char nome_setor[30],querry[200];
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	conn = mysql_init(NULL);
	if((mysql_real_connect(conn,host,user,pass,dbname,port,unix_socket,flag)) == NULL){
		printf("\nMysql error %s [%d]\n",mysql_error(conn),mysql_errno(conn));	

		exit(1);
	}
	//printf("banco aberto com sucesso\n");

	for(;;){
		printf("============================Menu Principal=====================\n");
		printf("\t1.Cadastro/registro\n\t2.Consultar\n");
		scanf("%d",&op);
		system("clear");

		__fpurge(stdin);

		switch(op){
			case 1:
				menuCadastro(conn);
				break;
			case 2:
				menuConsulta(conn);	
				break;
		}
	}
}

	// PARA COMPILAR O COGICO :   gcc -o a.e $(mysql_config --cflags) TrabalhoBdPronto.c $(mysql_config --libs)


//gcc mysql_config --cflags --include --libs teste-mysql.c -o teste-mysql.c
//============================= DATA DEFINITION ======================
/*
create table Cadastro_carros( 
	nome varchar(30) NOT NULL, 
	modelo varchar(30) NOT NULL,
	ano date NOT NULL,
	cod_carro int(11) NOT NULL PRIMARY KEY, 
	cod_setor int(11) NOT NULL, 
	valor decimal(10,2) NOT NULL,
	qtd_estoque int(11) NOT NULL,
	foreign key(cod_setor) references Cadastro_setor(cod_setor));	

INSERT INTO Cadastro_carros values ('Chevrolet','modelo2',' 1993-01-01', 3, 1, 4000, 50);
INSERT INTO Cadastro_carros values ('Del rei','modelo1',' 1990-01-01', 123, 1, 6000, 50);


 create table pior_func(
   cpf_func varchar(16) NOT NULL,
   cod_pior int(11) NOT NULL PRIMARY KEY AUTO_INCREMENT,
   total decimal(10,2) NOT NULL);


create table Pecas( 
   nome varchar(100) NOT NULL,
   cod_peca int(11) NOT NULL PRIMARY KEY,
   cod_setor int(11) NOT NULL,
   valor decimal(10,2) NOT NULL,
   qtd_estoque int(11) not null ,
   foreign key(cod_setor) references Cadastro_setor(cod_setor));

create table Cadastro_setor(
	nome varchar(100) NOT NULL,
	cod_setor int(11) NOT NULL PRIMARY KEY AUTO_INCREMENT);

	INSERT INTO Cadastro_setor values('Setor de Vendas1',1);
	INSERT INTO Cadastro_setor values('Setor de Vendas2',2);



create table funcionarios( 
	nome varchar(100) NOT NULL,
	CPF char(16) NOT NULL PRIMARY KEY,
	endereco varchar(150) NOT NULL,
	cod_setor int(11) NOT NULL,
	salario decimal(10,2) NOT NULL, 
	foreign key(cod_setor) references Cadastro_setor(cod_setor));

	INSERT INTO funcionarios values('Marcelo','12341234123','Rua Porto Alegre',1,5999);
	INSERT INTO funcionarios values('Sonia','12341234234','Rua Uruana',2,8000);
	INSERT INTO funcionarios values('Luiz','12341234345','Rua Porto Alegre',1,5999);




create table Cliente(
 nome varchar(100) NOT NULL,
 CPF char(16) NOT NULL PRIMARY KEY,
 telefone varchar(18) NOT NULL,
 endereco varchar(150) NOT NULL,
 placa_do_carro char (8) NOT NULL,
 CPF_funcionario char(16) NULL,
 foreign key(CPF_funcionario) references funcionarios(CPF) ON DELETE CASCADE);

	INSERT INTO Cliente values('Rejane','12345678912','3661-0656','Cascatinha','ABC-1234','12341234123');
	INSERT INTO Cliente values('Sandra','12345678923','3661-0655','Imbarie','ABC-1233','12341234234');
	INSERT INTO Cliente values('Rogerio','12345678916','3661-0656','Cascatinha','ABC-1264','12341234123');
	INSERT INTO Cliente values('Bianca','12345678918','3661-0656','Cascatinha','ABC-1264','12341234345');



create table Revisao(
cpf_cliente varchar(15) NOT NULL,
cod_revisao int(11) NOT NULL PRIMARY KEY AUTO_INCREMENT, 
data_revisao date NOT NULL, 
pagamento decimal(10,5) NOT NULL,
foreign key (cpf_cliente) references Cliente (cpf) ON DELETE CASCADE);


CREATE TABLE Data_Venda(
	cod_venda int(11) NOT NULL,
	dia date NOT NULL,
	codigo_venda int(11),
	foreign key (cod_venda) references Vendas(cod_venda)	
	);

CREATE TABLE Vendas(				
	nome varchar(50) NOT NULL,
	modelo varchar(50) NOT NULL,
	ano date NOT NULL,
	cod_venda int(11) NOT NULL PRIMARY KEY AUTO_INCREMENT,
	Preco_carro decimal(10,2) NOT NULL,
	Cpf_cliente varchar(15) NOT NULL,
	Cpf_funcionario varchar(15) NOT NULL,
	foreign key(Cpf_funcionario) references funcionarios(CPF) ON DELETE CASCADE,
	foreign key (cpf_cliente) references Cliente (cpf) ON DELETE CASCADE);


CREATE TABLE Troca(				
	nomeTrocado varchar(50) NOT NULL,
	modeloTrocado varchar(50) NOT NULL,
	nomeTrocando varchar (50) NOT NULL,
	modeloTrocando varchar(50) NOT NULL,
	ano date NOT NULL,
	cod_troca int(11) NOT NULL PRIMARY KEY AUTO_INCREMENT,
	Preco_Trocado decimal(10,2) NOT NULL,
	Preco_Trocando decimal(10,2) NOT NULL,
	Cf_cliente varchar(15) NOT NULL,
	foreign key (Cf_cliente) references Cliente (cpf) ON DELETE CASCADE);


	

	DELETE from Cadastro_carros where cod_carro = 123;


	DELIMITER $$			
	CREATE TRIGGER Venda_Carro AFTER INSERT
	ON Vendas
	FOR EACH ROW 
	BEGIN
	DELETE Cadastro_carros values  where (OLD.nome, OLD.modelo,OLD.ano,OLD.cod_carro);
	END$$
	DELIMITER ;



	CREATE VIEW Quantidade_carro(nome,Qt) as select nome,Qt from  (select nome,count(nome) as Qt from Vendas group by nome,modelo) as T1 WHERE T1.Qt;


create view Quantidade_ano(nome,modelo,ano,cod_venda,Preco_carro,Cpf_cliente) as SELECT * FROM Vendas where ano >= '2015-01-01' and ano <= '2015-12-31'; // Esta visao 
// retorna uma tabela igual a Vendas, mas apenas dentro do tempo desejado

create view
CREATE VIEW Quantidade_ano2(nome,Qt) as select nome,Qt from  (select nome,count(nome) as Qt from Quantidade_ano group by nome,modelo) as T1 WHERE T1.Qt;
// Esta visao eh uma visao igual a Quantidade_carros, so que apenas em um determinado tempo

CREATE VIEW Eficiencia_funcionario (nome,Qt) as select nome,Qt from (select funcionarios.nome,count(Vendas.Cpf_cliente) as Qt from funcionarios left outer join Vendas on funcionarios.cpf = Vendas.Cpf_funcionario group by nome) as T1;

SELECT nome  from Quantidade_ano2 where Qt in (select MAX(Qt) from Quantidade_ano2); // Selecao que retorna o nome do carro mais vendido no ultima ano

 create view Atualizar_cliente as  select CPF,IFNULL(CPF_funcionario,0) as Cpf_func from Cliente

 CREATE VIEW Pior as select CPF,IFNULL(total,0) as select total from funcionarios left outer join pior_func on funcionarios.CPF = pior_func.cpf_func;
 //create view Quantidade_setor (Cod_setor,Qt) as select cod_setor,Qt from (select cod_setor, count(cod_setor) as Qt from funcionarios group by cod_setor) as T1
//create view Quantidade_setor2 (cod_setor,Qt,salario) as  select cod_setor,Qt,salario from (select funcionarios.cod_setor,Qt,salario from funcionarios,Quantidade_setor where funcionarios.cod_setor= Quantidade_setor.Cod_setor) as T1
*/



