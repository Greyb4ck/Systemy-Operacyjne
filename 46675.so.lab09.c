# SO IS1 LAB09
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <windows.h>


int main(int argc, char **argv)
{
	 //printf("argc:%d\n", argc);
	 //printf("argv[0]:%s\n", argv[0]);
	if(argc >= 3 || argc<=1)
	{
		fprintf(stderr, "nie poprawna dana!\n");
		return 1;
	}
	int size=0;
	while(argv[argc-1][size] != '\0')
	{
		// printf("%c\n", argv[1][size]);
		if(isdigit(argv[argc-1][size]) == 0)
		{
			fprintf(stderr, "nie liczba\n");
			return 2;
		}
		size += 1;
	}
	
	int number = atoi(argv[argc-1]);
	// printf("nubmer %d\n", number);
	if(number>13 || number <=0)
	{
		fprintf(stderr, "nie w zakresie\n");
		return 3;
	}
	if(number<=2)
	{
		return 1;
	}
	int status2 = 0;
    int a = number - 1;
    int b = number - 2;
    char liczba1[5];
    char liczba2[5];
    snprintf(liczba1, sizeof(liczba1), "%d", a);
    snprintf(liczba2, sizeof(liczba2), "%d", b);
	//fprintf(stdout, "%s\n", liczba1);
	//fprintf(stdout, "%s\n", liczba2);
	char p1[34];
	char p2[34];
	snprintf(p1, sizeof(p1),"%s %d", argv[0], a); //C:\\Users\\adamt\\comp\\
	
	snprintf(p2, sizeof(p2), "%s %d", argv[0], b);
	STARTUPINFO info1={sizeof(info1)};
	STARTUPINFO info2={sizeof(info2)};
	DWORD IDcurrent;
	IDcurrent= GetCurrentProcessId();

	//printf("ID: %d number: %d" , IDcurrent, number);
	PROCESS_INFORMATION processInfo1;
	PROCESS_INFORMATION processInfo2;
	DWORD ec1;
	DWORD ec2;
	DWORD PIDch1;
	DWORD PIDch2;
	if(number>2){
		if (CreateProcessA(argv[0], p1, NULL, NULL, 0, 0, NULL, NULL, &info1, &processInfo1)){
			PIDch1=processInfo1.dwProcessId;
			WaitForSingleObject(processInfo1.hProcess, INFINITE);
			GetExitCodeProcess(processInfo1.hProcess, &ec1);

			CloseHandle(processInfo1.hProcess);
			CloseHandle(processInfo1.hThread);
		}
	
		if (CreateProcessA(argv[0], p2, NULL, NULL, 0, 0, NULL, NULL, &info2, &processInfo2)){
			PIDch2=processInfo2.dwProcessId;
			WaitForSingleObject(processInfo2.hProcess, INFINITE);
			GetExitCodeProcess(processInfo2.hProcess, &ec2);
			
			CloseHandle(processInfo2.hProcess);
			CloseHandle(processInfo2.hThread);
		}
		printf("%d \t%d %d %d\n",IDcurrent,PIDch1, a ,ec1);
		printf("%d \t%d %d %d\n",IDcurrent,PIDch2, b ,ec2);
		status2 = ec1+ec2;
		printf("%d \t \t%d \n \n",IDcurrent, status2);
		//fprintf(stdout, "status sum children: %d \n", status2);
	}

    //fprintf(stdout,"Return: %d \n",status2);
    return status2;
}