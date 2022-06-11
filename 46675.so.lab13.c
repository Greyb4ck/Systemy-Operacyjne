// SO IS1 LAB13
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>
#include<stdbool.h>
#include <unistd.h>
#include <windows.h>
#include <conio.h>

#define M_SIZE 1000 
#define OX_SIZE 61
void changeStrEle(char* str, int element, char c); 
void change_data_IPC(char* ox_data_IPC, int digit, char c);

typedef struct ox_data_IPC{
	char ox[OX_SIZE];
	bool turn;
	char tab_picks[9];
} ox_data_IPC;

bool check_win(ox_data_IPC* ox_IPC);
bool check_digit(char* m, int size, int digit);
void* data_IPC;

int main(int argc , char **argv)
{
	if( argc != 2) {
		fprintf(stderr, "too many arguments %s \n", argv[0]);
		return 1;
	}

	int digit=100;
	int digit2=100;
	HANDLE MapFile = CreateFileMapping(
	INVALID_HANDLE_VALUE,
	NULL,
	PAGE_READWRITE,
	0,
	M_SIZE,
	argv[1]);
	int turn=0;
	
	if(GetLastError() == ERROR_ALREADY_EXISTS){
		data_IPC = MapViewOfFile(
		MapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		M_SIZE);
		if (data_IPC == NULL)
		 {
		 fprintf(stderr, "Could not map view of file\n");
		 CloseHandle(MapFile);
		 return 3;
		 }
		ox_data_IPC *ox_IPC = (ox_data_IPC*) data_IPC;
		
		printf("You are playing with O.\n%s", ox_IPC->ox); 
		do{	
			if(digit2==0) break;
			if( ox_IPC->turn==true){
				if(check_win(ox_IPC)){
					printf("Loser O\n%s", ox_IPC->ox);
					break;
				}else if(turn==4){
					printf("Draw for Losers O X\n%s", ox_IPC->ox);
					break;
				}
			
				printf("Your turn give digit please\n%s",ox_IPC->ox);
				scanf("%d", &digit);
				if(digit==0) break; 
				else if(1<=digit && digit<=9 && check_digit(ox_IPC->tab_picks,9,digit))
					change_data_IPC(ox_IPC->ox, digit, 'O');
				else {
					printf("Wrong digit, it's taken.\n");
					continue;
				}
				if(check_win(ox_IPC)){
					printf("Winner O\n%s", ox_IPC->ox);

					ox_IPC->turn=false;
					break;
				}
				printf("%s", ox_IPC->ox);
				ox_IPC->turn=false;
				turn++;
			}
			sleep(1);	
		} while(digit!=0);
		return 0;
	} else {
		
		data_IPC = MapViewOfFile(
		MapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		M_SIZE);
		if (data_IPC == NULL)
		 {
		 fprintf(stderr, "Could not map view of file\n");
		 CloseHandle(MapFile);
		 return 3;
		 }
		ox_data_IPC *ox_IPC = (ox_data_IPC*) data_IPC;
		if(ox_IPC == (void*)-1){
			fprintf(stderr, "Error attaching shared memory for IPC, shmat()\n");
			return 4;
		}		
		char tab[9]={1, 2,3, 4, 5, 6, 7, 8, 9};
		
		char ox_drawing[OX_SIZE]={ " 1 | 2 | 3 \n"   // (+2) 1,(+4) 5, (+4) 9, (+2)
						  "---+---+---\n"   //  (+12)
						  " 4 | 5 | 6 \n"   //  (+2) 25, 29, 33 (+2)
						  "---+---+---\n"   //  (+12)
						  " 7 | 8 | 9 \n"}; //  (+2) 49, 53, 57
		strcpy(ox_IPC->ox, ox_drawing);
		strcpy(ox_IPC->tab_picks, tab);
		
		ox_IPC->turn=false;
		printf("You are playing with X.\n");
		do{
			if(digit2==0) break;
			
			if( ox_IPC->turn==false){
				if(check_win(ox_IPC)){
					printf("Loser X\n%s", ox_IPC->ox);
					break;
				}
				printf("Your turn give digit please\n%s",ox_IPC->ox);
				scanf("%d", &digit);
				if(digit==0) break; 
				else if(1<=digit && digit<=9 && check_digit(ox_IPC->tab_picks,9,digit))
					change_data_IPC(ox_IPC->ox, digit, 'X');
				else {
					printf("Wrong digit, it's taken.\n");
					continue;
				}
				if(check_win(ox_IPC)){
					printf("Winner X\n%s", ox_IPC->ox);
					ox_IPC->turn=true;
					break;
				}
				printf("%s", ox_IPC->ox);
				ox_IPC->turn=true;
				turn++;
				if(turn==5){
					printf("Draw for Losers O X\n%s", ox_IPC->ox);
					break;
				}
			}
			sleep(1);	
		} while(digit!=0);
	
	}
	if(!UnmapViewOfFile(data_IPC)) {
 fprintf(stderr, "Could not unmap view of file\n");
 CloseHandle(MapFile);
 return 4;
 }
 CloseHandle(MapFile);
	return 0;
}

void change_data_IPC(char* data_IPC, int digit, char c)
{
	
		switch(digit){
			case 1:
				changeStrEle(data_IPC, 1, c);
				break;
			case 2: 
				changeStrEle(data_IPC, 5, c);
				break;
			case 3: 
				changeStrEle(data_IPC, 9, c);
				break;
			case 4: 
				changeStrEle(data_IPC, 25, c);
				break;
			case 5: 
				changeStrEle(data_IPC, 29, c);
				break;
			case 6: 
				changeStrEle(data_IPC, 33, c);
				break;
			case 7: 
				changeStrEle(data_IPC, 49, c);
				break;
			case 8: 
				changeStrEle(data_IPC, 53, c);
				break;
			case 9: 
				changeStrEle(data_IPC, 57, c);
				break;
			default:
				fprintf(stderr, "Error wrong digit: %d\n" ,digit);
		}
}

void changeStrEle(char* str, int element, char c)
{
	if(OX_SIZE > element  && 0 < element)
		str[element] = c;
	else
		fprintf(stderr, "Error element size out of bound");
	return;
}

bool check_win(ox_data_IPC* ox_IPC)
{
	if(ox_IPC->ox[1] == ox_IPC->ox[5] && ox_IPC->ox[5] == ox_IPC->ox[9]){
		return true;
	}
	else if((ox_IPC->ox[25] == ox_IPC->ox[29]) && (ox_IPC->ox[29] == ox_IPC->ox[33])){
		return true;
	}
	else if((ox_IPC->ox[49] == ox_IPC->ox[53]) && (ox_IPC->ox[53] == ox_IPC->ox[57])){
		return true;
	}
	else if((ox_IPC->ox[1] == ox_IPC->ox[25]) && (ox_IPC->ox[25] == ox_IPC->ox[49])){
		return true;
	}
	else if((ox_IPC->ox[5] == ox_IPC->ox[29]) && (ox_IPC->ox[29] == ox_IPC->ox[53])){
		return true;
	}
	else if((ox_IPC->ox[9] == ox_IPC->ox[33]) && (ox_IPC->ox[33] == ox_IPC->ox[57])){
		return true;
	}
	else if((ox_IPC->ox[1] == ox_IPC->ox[29]) && (ox_IPC->ox[29] == ox_IPC->ox[57])){
		return true;
	}
	else if((ox_IPC->ox[49] == ox_IPC->ox[29]) && (ox_IPC->ox[29] == ox_IPC->ox[9])){
		return true;
	}
	return false;
}

bool check_digit(char* m, int size, int digit)
{
	for(int i=0; i<size; i++){
		if(m[i]==digit){
			m[i]=0;
			return true;
		}
	}	
	return false;
}
