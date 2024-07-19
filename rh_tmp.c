#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
/*
_ 함부로 사용 금지 
//TODO cell을 통해 car를 수정하는 건 좋지 않음 cell은 car를 통해 생성되는 것이므로 사용 X, 
// TODO car와 cell을 비교하지 말고 car와 car를 비교 / 각 함수와 변수의 개념을 확실하게 잡고 시작하는 게 좋음

*/
/* NOFIX --- */

typedef enum {
	start,
	left,
	right,
	up,
	down,
	quit,
	N_op 
} commands ;

typedef enum {
	vertical,
	horizontal 
} direction ;

typedef struct {
	int id ;
	int y1, y2 ;	// y1: the minimum of y, y2: the maximum of y
	int x1, x2 ;	// x1: the minimum of x, x2: the maximum of x
	int span ;		// the number of cells 
	direction dir ;	// the direction of the car
} car_t ;

char * op_str[N_op] = {
	"start",
	"left",
	"right",
	"up",
	"down",
	"quit"
} ;

int n_cars = 0 ;
car_t * cars = 0x0 ; //모든 차의 정보를 담고 있어야 함, 그 정보들을 가리키는 포인터 
int cells[6][6] ; // cells[Y][X] 안에 car_t의 id를 담고 있어야 함
// A6 -> cells[5][0]
// F4 -> cells[3][5]
// F1 -> cells[0][5]

/* --- NOFIX */

commands
get_op_code (char * s)
{
	// return the corresponding number for the command given as s.
	// FIXME
	for(int i=0;i<N_op;i++){
		if(strcmp(s,op_str[i]) == 0){
			return (commands)i;
		}
	}
	return N_op;
}

//FIXME
	// load_game returns 0 for a success, or return 1 for a failure.
	// Use fopen, getline, strtok, atoi, strcmp
	// Note that the last character of a line obtained by getline may be '\n'.
int
load_game (char * filename)
{
	FILE *inputF;
	char *line = NULL;
	size_t size = 0;
	ssize_t read = 0;
	int id = 1;
	

	if((inputF = fopen(filename, "r")) == NULL){
		return 1;
	} // 해당 파일이 없는 경우

	getline(&line, &size, inputF);
	
	int j;
	if(sscanf(line, "%d", &j) == 0){
		return 1;
	}

	n_cars = atoi(line);
	if( n_cars <2 || n_cars >37){
		return 1;
	} 

	cars = (car_t*)malloc(sizeof(car_t) * (n_cars+1));

	while((read = getline(&line, &size, inputF)) != -1){
		if(id > n_cars) 
			break;
		fprintf(stderr, "%s\n", line);
		char * ptr = strtok(line, ":");
		fprintf(stderr, "ptr : %s\n", ptr);
		
		if(ptr == NULL){
			fprintf(stderr, "ptr : %s\n", ptr);
			return 1;
		}

		if('A' <= ptr[0] && ptr[0] <='F' && ptr[1] >='1' && ptr[1] <='6'){
			if(id == 1){ // 시작 차는 4번째 줄
				if(ptr[1] != '4'){
					return 1;
				}
			}
			cars[id].x1 = ptr[0] - 'A';
			cars[id].x2 = cars[id].x1;
			cars[id].y1 = ptr[1] - '1';
			cars[id].y2 = cars[id].y1;
			//시작 위치값 설정 
		}
		else{
			return 1;
		}

		char * dir = strtok(NULL, ":");

		fprintf(stderr, "dir : %s\n", dir);

		if(dir == NULL){
			fprintf(stderr, "124");
			return 1;
		}

		if(strcmp(dir, "vertical") == 0)
			cars[id].dir = vertical;
		else if(strcmp(dir, "horizontal") == 0)
			cars[id].dir = horizontal;
		else{
			return 1;
		}

		char * span = strtok(NULL, ":");
		if(span == NULL){
			fprintf(stderr, "138");
			return 1;
		}

		fprintf(stderr, "span : %s\n", span);

		if(sscanf(span, "%d", &j) == 0){
			fprintf(stderr, "%d", 143);
			return 1;
		}
		if(atoi(span) != 0 && atoi(span) > 0 && atoi(span) <7){ //정수이고 1~6사이의 숫자라면
			cars[id].span = atoi(span);
		}
		else {
			return 1;
		}

		if(cars[id].dir == horizontal){// horizontal
			cars[id].x2 = cars[id].x1 + cars[id].span -1;
			if(cars[id].x2 >5){
				return 1;
			}
		}
		else {//vertical
			cars[id].y1 = cars[id].y2 - cars[id].span +1;
			if(cars[id].y1 <0){
				return 1;
			}
		}
		id++;
	}
    
	if(id-1!= n_cars){
		return 1;
	}

	free(line);
	fclose(inputF);

	return 0;
}

/* The beginning state of board1.txt must be shown as follows: 
 	 + + 2 + + +
 	 + + 2 + + +
	 1 1 2 + + +
	 3 3 3 + + 4
	 + + + + + 4
	 + + + + + 4
*/
void
display ()
{
	for(int i=5; i>=0; i--){
		for (int j=0; j<=5; j++){
			if(cells[i][j] == 0){
				printf("+ ");
			}
			else{
				printf("%d ", cells[i][j]);
			}
		}
		printf("\n");
	}
}

int 
update_cells ()
{
	memset(cells, 0, sizeof(int) * 36) ; // clear cells before the write.

	//FIXME
	// return 0 for sucess
	// return 1 if the given car information (cars) has a problem

	for(int i=1; i<=n_cars; i++){
		for(int a = cars[i].y1;a<=cars[i].y2;a++){
			for (int b= cars[i].x1; b<=cars[i].x2; b++){
				if(cells[a][b] == 0){
					cells[a][b] = i;
				}
				else {
					return 1;
				}
			}
		}
	}
	return 0;
}
int is_overlap(int id, int op){
	int target_x;
	int target_y;
	switch (op)
	{
	case left:
		target_x = cars[id].x1-1;
		target_y = cars[id].y1;
		break;
	case right:
		target_x = cars[id].x2+1;
		target_y = cars[id].y1;
		break;
	case up:
		target_x = cars[id].x1;
		target_y = cars[id].y2+1;
		break;
	case down:
		target_x = cars[id].x1;
		target_y = cars[id].y1-1;
		break;
	default:
		break;
	}
	for(int i=1; i<=n_cars; i++){
		if(id == i) {
			continue;
		}
		for (int c = cars[i].x1; c <= cars[i].x2; c++){
			for (int d = cars[i].y1; d <= cars[i].y2; d++){
				if(target_x == c && target_y == d){
					return 1;
				}
			}
		}
	}

	return 0;
}

//FIXME
	// move returns 1 when the given input is invalid.
	// or return 0 for a success.

	// 주어진 input이 valid한지 판단


	// Update cars[id].x1, cars[id].x2, cars[id].y1 and cars[id].y2
	//   according to the given command (op) if it is possible.

	// The condition that car_id can move left is when 
	//  (1) car_id is horizontally placed, and
	//  (2) the minimum x value of car_id is greather than 0, and
	//  (3) no car is placed at cells[cars[id].y1][cars[id].x1-1].
	// You can find the condition for moving right, up, down as
	//   a similar fashion.
int move (int id, int op) 
{
	if(id <0 || id > n_cars){
		return 1; // error
	}

	if(cars[id].dir == horizontal){
		if(op == left){
			if(cars[id].x1 - 1 >= 0 && is_overlap(id, op) == 0){//board를 벗어나지 않고 이미 차가 존재하지 않는 경우
				cars[id].x1 --;
				cars[id].x2 --;
			}
			else{
				return 1;
			}
		}
		else if(op == right){
			if(cars[id].x2 + 1 <= 5 && is_overlap(id, op) == 0){//board를 벗어나지 않고 이미 차가 존재하지 않는 경우
				cars[id].x2 ++;
				cars[id].x1 ++;
			}
			else{
				return 1;
			}
		}
		else {
			return 1;
		}
	}
	else if(cars[id].dir == vertical){
		if(op == up){
			if(cars[id].y2 + 1 <= 5 && is_overlap(id, op) == 0){//board를 벗어나지 않고 이미 차가 존재하지 않는 경우
				cars[id].y2 ++;
				cars[id].y1 ++;
			}
			else {
				return 1;
			}
		}
		else if(op == down){
			if(cars[id].y1 - 1 >= 0 && is_overlap(id, op) == 0){//board를 벗어나지 않고 이미 차가 존재하지 않는 경우
				cars[id].y1 --;
				cars[id].y2 --;
			}
			else{
				return 1;
			}
		}
		else {
			return 1;
		}
	}


	return 0;
}

int
main ()
{
	char buf[128] ;
	int op ;
	int id ;

	while (1) {
		scanf("%s", buf) ;
		
		switch (op = get_op_code(buf)) {
			case start:
				scanf("%s", buf) ;
				if(load_game(buf) == 1){
					free(cars);
					fprintf(stderr, "Invalid data!\n") ;
					break;
				}
				update_cells() ;
				display() ;
				break;

			case left:
			case right:
			case up:
			case down:
				scanf("%d", &id) ;
				if(cars == 0x0) {
					fprintf(stderr, "Invalid Command\n");
					break;
				}
				if(move(id, op) == 1) {
					fprintf(stderr, "impossible\n");
					break;
				}
				update_cells() ;
				display() ;
				break;
			case quit:
				exit(EXIT_SUCCESS);
			default:
				fprintf(stderr, "Invalid Command\n");
		}
		while (getchar() != '\n') ;

		if(cells[3][5] == 1){
			printf("Game Over!\n");
			exit(EXIT_SUCCESS);
		}
	}
}
