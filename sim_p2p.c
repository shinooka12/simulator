#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<math.h>

#define NODE_MAX 500
#define KNOWN_NODE 500
#define PARENT_MAX 2
#define CHILD_MAX 5
#define BUFSIZE 1024
#define KEY_SHARECOUNT 3
#define SEARCH_MAX 5000

#define S_WIFI 32.5 //11-54Mbps
#define S_BL 12.5 //1-24Mbps
#define S_ZIGBEE 0.25 //250Kbps

int flag;

typedef struct {
	int id;
	int id_known[KNOWN_NODE];
	int parent[PARENT_MAX];
	int child[CHILD_MAX];
	double speed;
	char data_label[BUFSIZE];
	//char data_value[BUFSIZE];
	char key[NODE_MAX][BUFSIZE];
	int key_flag;
	int zigbee_flag;
}node_t;

void data_input(node_t *);
void node_connect(node_t *);
void share_key(node_t *);
int search_key(node_t *,int );
void print_csv(node_t *,int );
void print_search(int );
double speed_get();

int main(){

	node_t *node;
	int hit_count;
	int ch;


	node = (node_t *)malloc(sizeof(node_t)*NODE_MAX);
	if(node == NULL){
		printf("node malloc error\n");
		exit(EXIT_FAILURE);
	}


	srand(time(NULL));

	//input data
	data_input(node);

	//conect
	node_connect(node);

	//send my key
	share_key(node);

	//search key
	ch = log2((double)NODE_MAX) - 0.5;
	hit_count = search_key(node,ch);

	//print for csv
	//print_csv(node,hit_count);
	print_search(hit_count);

	free(node);
	//printf(" ch = %d\n",ch);

	return 0;
}




void data_input(node_t *node){

	int i;
	int j;

	for (i=0;i<NODE_MAX;i++){
		flag = 0;
		node[i].id=i;

		for (j=0;j<KNOWN_NODE;j++){
			if(j!=i){
				node[i].id_known[j]=j;
			}else{
				node[i].id_known[j]=-1; //自分自身は-1
			}
		}

		//reset parent and child
		for (j=0;j<PARENT_MAX;j++){
			node[i].parent[j]=-1; //親も子もいない場合は -1
		}

		for (j=0;j<CHILD_MAX;j++){
			node[i].child[j]=-1;
		}

		//speed 0~9
		node[i].speed=speed_get();
		if(flag == -1){
			node[i].zigbee_flag = -1;
		}else{
			node[i].zigbee_flag = 0;
		}

		//key flag reset
		node[i].key_flag=0;

	}

	//data input
	for (i=0;i<NODE_MAX;i++){
		sprintf(node[i].data_label,"room%d/temperature",i);
		//sprintf(node[i].data_value,"%d",rand()%40);
	}


}


void node_connect(node_t *node){
	
	int i,j,k,l,m;
	int node_number;
	int flag;

	for(i=0;i<NODE_MAX;i++){
		for(j=0;j<KNOWN_NODE;j++){
			
			node_number = node[i].id_known[j];
			flag=-1;

			if(node_number != -1){
				
				if(node[i].speed > node[node_number].speed && node[i].zigbee_flag != -1){
					for(k=0;k<CHILD_MAX;k++){
						if (node[i].child[k] == -1){
							for(l=0;l<PARENT_MAX;l++){
								if(node[node_number].parent[l] == -1){
									for(m=0;m<CHILD_MAX;m++){
										if(node[i].child[m]==node[node_number].id){
											flag = 0;
										}
									}
									if(flag == -1){
										node[i].child[k]=node_number;
										node[node_number].parent[l]=node[i].id;
										flag=0;
									}
								}
							}
						}

					}
					if(flag == -1){
						//node osieru
						//tell_node();
					}

				}else if (node[i].speed < node[node_number].speed && node[node_number].zigbee_flag != -1){
					for(k=0;k<PARENT_MAX;k++){
						if (node[i].parent[k] == -1){
							for(l=0;l<CHILD_MAX;l++){
								if(node[node_number].child[l] == -1){
									for(m=0;m<PARENT_MAX;m++){
										if(node[i].parent[m]==node[node_number].id){
											flag = 0;
										}
									}
									if(flag == -1){
										node[i].parent[k]=node_number;
										node[node_number].child[l]=node[i].id;
										flag=0;
									}
								}
							}		
						}
					}
					if(flag == -1){
						//node osieru
						//tell_node();
					}

				}else if (node[i].speed == node[node_number].speed && node[i].zigbee_flag != -1){
					for(k=0;k<CHILD_MAX;k++){
						if (node[i].child[k] == -1){
							for(l=0;l<PARENT_MAX;l++){
								if(node[node_number].parent[l] == -1){
									for(m=0;m<PARENT_MAX;m++){
										if(node[i].parent[m]==node[node_number].id){
											flag = 0;
										}
									}
									for(m=0;m<CHILD_MAX;m++){
										if(node[i].child[m]==node[node_number].id){
										flag = 0;
										}
									}

									if(flag == -1){
										node[i].child[k]=node_number;
										node[node_number].parent[l]=node[i].id;
										flag = 0;
									}
								}
							}
						}
					}

					if(flag == -1){
						//node osieru
						//tell_node();
					}
				}
			}


		}


	}//for end
}



void share_key(node_t *node){

	int i,j,k,l,m;
	int node_number;
	int flag;
	int cmp_flag;
	char *tmp1,*tmp2,*tmp3;

	tmp1 = (char *)malloc(BUFSIZE);
	if(tmp1 == NULL){
		printf("tmp1 malloc error\n");
		exit(EXIT_FAILURE);
	}
	tmp2 = (char *)malloc(BUFSIZE);
	if(tmp2 == NULL){
		printf("tmp2 malloc error\n");
		exit(EXIT_FAILURE);
	}
	tmp3 = (char *)malloc(BUFSIZE);
	if(tmp3 == NULL){
		printf("tmp3 malloc error\n");
		exit(EXIT_FAILURE);
	}

	//send my key
	for(i=0;i<NODE_MAX;i++){
		for(j=0;j<PARENT_MAX;j++){
			node_number = node[i].parent[j];
			if(node_number != -1){
				flag = node[node_number].key_flag;
				strcpy(node[node_number].key[flag],node[i].data_label);
				node[node_number].key_flag++;
			}

		}
	}
	

	//send key
	for(m=0;m<KEY_SHARECOUNT;m++){	
		for(i=0;i<NODE_MAX;i++){
			for(j=0;j<PARENT_MAX;j++){
				node_number = node[i].parent[j];
				if(node_number != -1){
					for(k=0;k<node[i].key_flag;k++){
						flag = node[node_number].key_flag;
						strcpy(tmp1,node[node_number].key[flag]);
						strcpy(tmp2,node[i].key[k]);
						cmp_flag = 0;
						for(l=0;l<flag;l++){
							strcpy(tmp3,node[node_number].key[l]);
							if(strcmp(tmp3,tmp2)==0){
								cmp_flag = -1;
							}
						}
						if(cmp_flag != -1 && strcmp(tmp1,tmp2) != 0){
							strcpy(node[node_number].key[flag],tmp2);
							node[node_number].key_flag++;
						}
					}
				}

			}
		}
	}
	//input my key
	for(i=0;i<NODE_MAX;i++){
		sprintf(tmp2,"room%d/temperature",i);
		flag = node[i].key_flag;
		strcpy(node[i].key[flag],tmp2);
		node[i].key_flag++;
	}

	free(tmp1);
	free(tmp2);
	free(tmp3);

}

int search_key(node_t *node,int search_reach){

	int i,j,k,l,m,n,h,s;
	int search_first_node;
	int node_number;
	int hit_count;
	int match_node;
	int search_query;
	int target_id;
	int cmp_flag,break_flag;
	int *search_node;
	char *tmp1,*tmp2;

	hit_count = 0;
	
	search_node = (int *)malloc(sizeof(int)*NODE_MAX);
	if(search_node == NULL){
		printf("search_node malloc error\n");
		exit(EXIT_FAILURE);
	}

	tmp1 = (char *)malloc(BUFSIZE);
	if(tmp1 == NULL){
		printf("tmp1 malloc error\n");
		exit(EXIT_FAILURE);
	}
	tmp2 = (char *)malloc(BUFSIZE);
	if(tmp2 == NULL){
		printf("tmp2 malloc error\n");
		exit(EXIT_FAILURE);
	}

	for(n=0;n<SEARCH_MAX;n++){	
		
		//memset(search_node,0,sizeof(search_node));
		//memset(tmp1,0,sizeof(tmp1));
		//memset(tmp2,0,sizeof(tmp2));
		
		search_first_node = rand()%NODE_MAX;
		i = search_first_node;
		m=0;
		l=0;
		cmp_flag = -1;
		break_flag = -1;
		search_query = 0;
		target_id = rand()%NODE_MAX;
		sprintf(tmp2,"room%d/temperature",target_id);

		//myself
		for(j=0;j<node[i].key_flag;j++){
			strcpy(tmp1,node[i].key[j]);
			if(strcmp(tmp1,tmp2)==0){
				cmp_flag = 0;
				match_node = node_number;
			}
		}


		

		//other node
		for(j=0;j<PARENT_MAX;j++){
			s = node[i].parent[j];
			if(s != -1){
				break_flag =0;
			}
		}

		if(break_flag == -1){
			continue;
		}

		for(j=0;j<PARENT_MAX;j++){
			search_node[l] = node[i].parent[j];
			l++;
		}

		s=0;
		for(j=s;j<l;j++){
			break_flag = -1;
			node_number = search_node[j];
			if(node_number != -1){
				for(k=0;k<node[node_number].key_flag;k++){
					strcpy(tmp1,node[node_number].key[k]);
					if(strcmp(tmp1,tmp2)==0){
						cmp_flag = 0;
						match_node = node_number;
					}
				}
			}

			s++;

			if(cmp_flag != 0){
				for(h=0;h<PARENT_MAX;h++){
					
					if(node_number != -1){
						search_node[l] = node[node_number].parent[h];
						l++;
					}			
				
				search_query++;	

				}

			}else if(cmp_flag == 0){
				break;
			}


			if(log2((double)search_query) > search_reach ){
				break;
			}
		}

		if(cmp_flag == 0){
			hit_count++;

		}else{
			//printf("[%d] no hit\n",search_first_node);
		}

	}

	free(search_node);
	free(tmp1);
	free(tmp2);

	return hit_count;
}


void print_csv(node_t *node,int hit_count){

	int i,j;

	//for CSV (ノード番号,回線スピード,親ノード(PARENT_MAXの数に依存),検索回数,検索ヒット回数,キーの情報(ラベル))
	for(i=0;i<NODE_MAX;i++){
		printf("%d,%.3lf",node[i].id,node[i].speed);
		for(j=0;j<PARENT_MAX;j++){
			printf(",");
			printf("%d",node[i].parent[j]);

		}
		printf(",%d,%d",SEARCH_MAX,hit_count);
		for(j=0;j<node[i].key_flag;j++){
			printf(",");
			printf("%s",node[i].key[j]);
		}
		printf("\n");
	}
}


void print_search(int hit_count){

    printf("%d\n",hit_count);

}


double speed_get(){ //平均の0.8~1.2の範囲で回線速度を決定

	double kei;
	int choose;
	double speed;

	choose = rand()%100;


	if(choose > 60){
		if(choose < 90){ //BL

			kei = (rand()%18+1)/10.00;
			speed = S_BL * kei;

		}else{ //ZIGBEE

			kei = (rand()%7+8)/10.00;
			speed = S_ZIGBEE * kei;
			flag = -1;


		}

	}else{ //WIFI

		kei = (rand()%132+34)/100.00;
		speed = S_WIFI * kei;
	}

	return speed;

}
